#include "gamemanager.h"

s16 GM::init()
{
  window_width_ = 960;
  window_height_ = 704;
  exit = false;
  time_step = (u32)((1.0f/kLogicFreqSec) * 1000.0f); // * 1000 because SDL_Tick return miliseconds
  window = SDL_CreateWindow("Animo Lucas, tu puedes!",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               window_width_, window_height_, 0);
  if (!window)
    return -1;

  renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer)
    return -1;

  return 0;
}

u32 GM::window_height() const
{
  return window_height_;
}

u32 GM::window_width() const
{
  return window_width_;
}