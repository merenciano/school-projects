#ifndef __RAYCASTED_TUNNEL_STRUCTS_H__
#define __RAYCASTED_TUNNEL_STRUCTS_H__ 1
#define PI 3.14159265f
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define TILE_SIZE 16
#define HORIZ_TILES ((WINDOW_WIDTH / TILE_SIZE) + 1)
#define VERT_TILES ((WINDOW_HEIGHT / TILE_SIZE) + 1)

#undef MAX
#undef MIN 
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

struct Vector2
{
  float u;
  float v;
};

struct Vector3
{
  float x;
  float y;
  float z;
};

struct UVLight
{
  struct Vector2 uv;
  float light;
};

struct InnerParams
{
  unsigned int* screen_pixels;
  unsigned int* texture;
  struct UVLight top_left;
  struct UVLight top_right;
  struct UVLight bottom_left;
  struct UVLight bottom_right;
  float step_value;
  int grid_x;
  int grid_y;
};

#endif // __RAYCASTED_TUNNEL_STRUCTS_H__
