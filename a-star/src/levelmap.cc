#include <levelmap.h>
#include <gamemanager.h>

LevelMap::LevelMap()
{
  map_info_ = nullptr;
}

LevelMap::~LevelMap()
{
  delete map_info_;
}

bool LevelMap::loadMap(std::string map_layout_path, std::string map_info_path)
{
  bool ret = false;
  ret = set_sprite(map_layout_path, true);
  ret &= loadMapInfo(map_info_path);


  info_ratio_ = rect_.w / map_info_w_;
  if (info_ratio_ != rect_.h / map_info_h_)
  {
    printf("The image of map info must match the aspect ratio of the \
            image layout.\n");
    ret = false;
  }

  return ret;
}

bool LevelMap::loadMapInfo(std::string map_info_path)
{
  SDL_Surface *srf = SDL_LoadBMP(map_info_path.c_str());
  if (!srf)
    return false;

  if (map_info_)
    delete map_info_;

  map_info_ = new s32[srf->w * srf->h];
  u32 *pix = (u32*)srf->pixels;
  for (s32 y = 0; y < srf->h; ++y)
  {
    for (s32 x = 0; x < srf->w; ++x)
    {
      if ((pix[srf->w * y + x] & 0xF) > 0)
      {
        map_info_[srf->w * y + x] = 1;
      }
      else
      {
        map_info_[srf->w * y + x] = 0;
      }
    }
  }
  map_info_w_ = srf->w;
  map_info_h_ = srf->h;

  return true;
}

bool LevelMap::isNavigable(Vector2 position) const
{
  if (!map_info_)
  {
    printf("You should load a map info file first\n");
    return false;
  }

  if (position.x_ < 0.0f ||
      position.y_ < 0.0f ||
      position.x_ >= (float)map_info_w_ ||
      position.y_ >= (float)map_info_h_)
  {
    return false;
  }

  return (bool)map_info_[map_info_w_ * (s32)position.y_ + (s32)position.x_];
}

s32 LevelMap::info_ratio() const
{
  return info_ratio_;
}

void LevelMap::DEBUG_PrintStdout()
{
  if (!map_info_)
    return;

  printf("\n");
  for (s32 i = 0; i < map_info_h_; i++)
  {
    for (s32 j = 0; j < map_info_w_; j++)
    {
      if (map_info_[map_info_w_ * i + j] == 1)
      {
        printf(" ");
      }
      else
      {
        printf("O");
      }
    }
    printf("\n");
  }
}