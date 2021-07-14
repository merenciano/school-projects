#include "inner_specialization.h"

void TileLightedSpecialization(struct InnerParams* params)
{
  float step_y = 0.0f;
  for (int screen_y = 0; screen_y < TILE_SIZE; ++screen_y)
  {
    struct UVLight left;
    struct UVLight right;

    left.uv.u = (1.0f - step_y) * params->top_left.uv.u  + step_y * params->bottom_left.uv.u;
    left.uv.v = (1.0f - step_y) * params->top_left.uv.v  + step_y * params->bottom_left.uv.v;

    right.uv.u = (1.0f - step_y) * params->top_right.uv.u  + step_y * params->bottom_right.uv.u;
    right.uv.v = (1.0f - step_y) * params->top_right.uv.v  + step_y * params->bottom_right.uv.v;

    int u_step_value_fx16 = (int)(((right.uv.u - left.uv.u) / TILE_SIZE) * (float)(1 << 16));
    int v_step_value_fx16 = (int)(((right.uv.v - left.uv.v) / TILE_SIZE) * (float)(1 << 16));
    int u_fx16 = (int)(left.uv.u * (float)(1 << 16));
    int v_fx16 = (int)(left.uv.v * (float)(1 << 16));

    unsigned int* current_pixel = params->screen_pixels + (WINDOW_HEIGHT *
                                  (params->grid_y * TILE_SIZE + screen_y) +
                                  (params->grid_x * TILE_SIZE));

    for (int screen_x = 0; screen_x < TILE_SIZE; ++screen_x)
    {
      int u = u_fx16;
      int v = v_fx16;
      u = u >> 16;
      v = v >> 16;

      u &= 511;
      v &= 511;
      unsigned int color = params->texture[(v << 9) + u];
      *current_pixel++ = color;

      u_fx16 += u_step_value_fx16;
      v_fx16 += v_step_value_fx16;
    }
    step_y += params->step_value;
  }
}

