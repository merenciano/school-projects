#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include "chrono.h"
#include "structs.h"

#define ASSEMBLY  1

#if ASSEMBLY
  void TileLightedSpecialization(struct InnerParams* params);
#else
  #include "inner_specialization.h"
#endif

static struct Vector3 v0, v1, v2, v3;
static int screen_w = 6;
static int screen_h = 6;
static int screen_dst = 10;
static float radius = 22.0f;

struct UVLight grid[HORIZ_TILES * VERT_TILES];
static unsigned int* texture;
static int cpu_mhz = 0;

static void ChronoShow ( char* name, int computations)
{
  float ms = ChronoWatchReset();
  float cycles = ms * (1000000.0f/1000.0f) * (float)cpu_mhz;
  float cyc_per_comp = cycles / (float)computations;
  fprintf ( stdout, "%s: %f ms, %d cycles, %f cycles/iteration\n", name, ms, (int)cycles, cyc_per_comp);
}

static void SetScreenEdges()
{
  v0.x = -screen_w * 0.5f;
  v0.y = screen_h * 0.5f;
  v0.z = screen_dst;

  v1.x = screen_w * 0.5f;
  v1.y = screen_h * 0.5f;
  v1.z = screen_dst;

  v2.x = screen_w * 0.5f;
  v2.y = -screen_h * 0.5f;
  v2.z = screen_dst;

  v3.x = -screen_w * 0.5f;
  v3.y = -screen_h * 0.5f;
  v3.z = screen_dst;
}

static inline struct Vector3 Lerp(struct Vector3 v0, struct Vector3 v1, float step)
{
  struct Vector3 v_result;
  v_result.x = (1.0f - step) * v0.x + step * v1.x;
  v_result.y = (1.0f - step) * v0.y + step * v1.y;
  v_result.z = v0.z;

  return v_result;
}

static inline struct UVLight LerpUVLight(struct UVLight a, struct UVLight b, float step)
{
  struct UVLight v_result;
  v_result.uv.u = (1.0f - step) * a.uv.u + step * b.uv.u;
  v_result.uv.v = (1.0f - step) * a.uv.v + step * b.uv.v;
  v_result.light = (1.0f - step) * a.light + step * b.light;

  return v_result;
}

static inline void Cut3DToCilynderCoordenates(struct Vector3* cut, struct UVLight* result)
{
  float u;
  u = atan2f(cut->y, cut->x);
  u += PI;
  u /= 2.0f * PI;
  u *= 512.0f;
  result->uv.v = fabs(cut->z);

  if (result->uv.v > 511.0f) result->uv.v = 511.0f;

  result->uv.u = u;
  result->light = 2.0f - (result->uv.v * (2.0f / 512.0f));
  result->light = MIN(result->light, 1.0f);
  if (result->light < 0.0f)
    result->light = 0.0f;
}

static void CutWithCylinder(struct Vector3 direction, struct Vector3* cut)
{
  if (direction.x == 0.0f && direction.y == 0.0f)
    return;

  float alpha = sqrt((radius * radius) / ((direction.x * direction.x) + (direction.y * direction.y)));
  cut->x = alpha * direction.x;
  cut->y = alpha * direction.y;
  cut->z = alpha * direction.z;
}

static void TileUVLight(struct UVLight* grid)
{
  float step_x_value = TILE_SIZE / (float)WINDOW_WIDTH;
  float step_y_value = TILE_SIZE / (float)WINDOW_HEIGHT;
  float step_y = 0.0f;
  for (int y = 0; y < VERT_TILES; ++y)
  {
    step_y += step_y_value;
    struct Vector3 v_left = Lerp(v0, v3, step_y);
    struct Vector3 v_right = Lerp(v1, v2, step_y);
    float step_x = 0.0f;

    for (int x = 0; x < HORIZ_TILES; ++x)
    {
      struct Vector3 cut;
      struct Vector3 ray = Lerp(v_left, v_right, step_x);
      step_x += step_x_value;
      CutWithCylinder(ray, &cut);
      Cut3DToCilynderCoordenates(&cut, grid + (HORIZ_TILES * y + x));
    }
  }
}


static void RaycastedTunnel(unsigned int* screen_pixels)
{
  struct InnerParams params;
  params.screen_pixels = screen_pixels;
  params.texture = texture;
  params.step_value = 1.0f / (float)TILE_SIZE;

  for (int grid_y = 0; grid_y < VERT_TILES - 1; ++grid_y)
  {
    for (int grid_x = 0; grid_x < HORIZ_TILES - 1; ++grid_x)
    {
      params.top_left     = grid[HORIZ_TILES * grid_y + grid_x];
      params.top_right    = grid[HORIZ_TILES * grid_y + (grid_x + 1)];
      params.bottom_left  = grid[HORIZ_TILES * (grid_y + 1) + grid_x];
      params.bottom_right = grid[HORIZ_TILES * (grid_y + 1) + (grid_x + 1)];
      float uv_offset = MAX(fabs(params.bottom_left.uv.u - params.top_left.uv.u),
                            fabs(params.bottom_right.uv.u - params.top_left.uv.u));
      uv_offset = MAX(uv_offset, fabs(params.top_right.uv.u - params.top_left.uv.u));
      if (uv_offset > 100.0f)
      {
        if (params.top_left.uv.u     < 100.0f) params.top_left.uv.u     += 512;
        if (params.top_right.uv.u    < 100.0f) params.top_right.uv.u    += 512;
        if (params.bottom_left.uv.u  < 100.0f) params.bottom_left.uv.u  += 512;
        if (params.bottom_right.uv.u < 100.0f) params.bottom_right.uv.u += 512;
      }
      if (params.top_left.light    < 1.0f || params.top_right.light    < 1.0f ||
          params.bottom_left.light < 1.0f || params.bottom_right.light < 1.0f)
      {
        float step_y = 0.0f;
        for (int screen_y = 0; screen_y < TILE_SIZE; ++screen_y)
        {
          struct UVLight left  = LerpUVLight(params.top_left,  params.bottom_left,  step_y);
          struct UVLight right = LerpUVLight(params.top_right, params.bottom_right, step_y);

          int u_step_value_fx16 = (int)(((right.uv.u - left.uv.u) / TILE_SIZE) * (float)(1 << 16));
          int v_step_value_fx16 = (int)(((right.uv.v - left.uv.v) / TILE_SIZE) * (float)(1 << 16));
          int u_fx16 = (int)(left.uv.u * (float)(1 << 16));
          int v_fx16 = (int)(left.uv.v * (float)(1 << 16));

          unsigned int* current_pixel = params.screen_pixels + (WINDOW_HEIGHT * (grid_y * TILE_SIZE + screen_y) + (grid_x * TILE_SIZE));

          int light_step_value_fx24 = (int)(((right.light - left.light) / TILE_SIZE) * (float)(1 << 24));
          unsigned int light_fx24 = (unsigned int)(left.light * (float)(1 << 24));
          for (int screen_x = 0; screen_x < TILE_SIZE; ++screen_x)
          {
            int u = u_fx16;
            int v = v_fx16;
            u = u >> 16;
            v = v >> 16;
            light_fx24 += light_step_value_fx24;

            u &= 511;
            v &= 511;
            unsigned int color = texture[(v << 9) + u];
            unsigned int red = color >> 16;
            red &= 0xFF;
            red *= light_fx24;
            red = red >> 24;
            unsigned int green = color >> 8;
            green &= 0xFF;
            green *= light_fx24;
            green = green >> 24;
            unsigned int blue = color & 0xFF;
            blue *= light_fx24;
            blue = blue >> 24;
            color = (red << 16) | (green << 8) | blue;
            *current_pixel++ = color;

            u_fx16 += u_step_value_fx16;
            v_fx16 += v_step_value_fx16;
          }
          step_y += params.step_value;
        }
      }
      else
      {
        params.grid_x = grid_x;
        params.grid_y = grid_y;
        TileLightedSpecialization(&params);
      }
    }
  }
}

int main ( int argc, char **argv)
{
  SDL_Surface *g_SDLSrf;
  int req_w = WINDOW_WIDTH;
  int req_h = WINDOW_HEIGHT;

  SetScreenEdges();

  if ( argc < 2)
  { 
    fprintf(stderr, "I need the cpu speed in Mhz!\n");
    exit(0);
  }

  cpu_mhz = atoi( argv[1]);
  assert(cpu_mhz > 0);
  fprintf ( stdout, "Cycle times for a %d Mhz cpu\n", cpu_mhz);

  // Init SDL and screen
  if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
  {
    fprintf(stderr, "Can't Initialise SDL: %s\n", SDL_GetError());
    exit(1);
  }

  if (0 == SDL_SetVideoMode( req_w, req_h, 32,  SDL_HWSURFACE | SDL_DOUBLEBUF))
  {
    printf("Couldn't set %dx%dx32 video mode: %s\n", req_w, req_h, SDL_GetError());
    return 0;
  }

  g_SDLSrf = SDL_GetVideoSurface();
  SDL_Surface *tex_srf = SDL_LoadBMP("./pink_pattern_pow.bmp");
  texture = (unsigned int*)(tex_srf->pixels);

  // Screen buffer data
  unsigned int* screen_pixels = (unsigned int *) g_SDLSrf->pixels;

  int end = 0;
  while ( !end) { 
    SDL_Event event;
    // Lock screen to get access to the memory array
    SDL_LockSurface( g_SDLSrf);
    // Borrar pantalla
    SDL_FillRect(g_SDLSrf, NULL, SDL_MapRGB(g_SDLSrf->format, 0, 0, 0));

    // RAYCASTED TUNNEL CALL
    ChronoWatchReset();
    TileUVLight(grid);
    ChronoShow ("TileUVLightCalculation", HORIZ_TILES * VERT_TILES);

    ChronoWatchReset();
    RaycastedTunnel(screen_pixels);
    ChronoShow ("InterpolateAndDrawTiles", WINDOW_WIDTH * WINDOW_HEIGHT);
    
    SDL_UnlockSurface( g_SDLSrf);
    SDL_Flip( g_SDLSrf);

    // Check input events
    while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_QUIT:
          end = 1;
          break;
      }
    }
  }
  SDL_FreeSurface(tex_srf);
  SDL_FreeSurface(g_SDLSrf);
  SDL_Quit();
  return 0;
}
