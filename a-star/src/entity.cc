#include "entity.h"
#include "gamemanager.h"
#include <SDL2/SDL_image.h>

Entity::Entity(Entity &&other)
{
  position_ = other.position_;
  rect_ = other.rect_;
  sprite_ = other.sprite_;
  for (s32 i = 0; i < 4; i++)
    color_[i] = other.color_[i];

  other.position_ = Vector2(0.0f, 0.0f);
  other.rect_ = SDL_Rect();
  other.sprite_ = nullptr;
  for (s32 i = 0; i < 4; i++)
    other.color_[i] = 0U;
}

void Entity::draw()
{
  if (sprite_ != nullptr)
  {
    SDL_RenderCopy(GM::ins().renderer, sprite_, NULL, &rect_);
  }
  else
  {
    SDL_SetRenderDrawColor(GM::ins().renderer, 
                          color_[0],
                          color_[1],
                          color_[2],
                          color_[3]);
    SDL_RenderFillRect(GM::ins().renderer, &rect_);
  }
}

void Entity::set_color(u8 r, u8 g, u8 b, u8 a)
{
  color_[0] = r;
  color_[1] = g;
  color_[2] = b;
  color_[3] = a;
}

bool Entity::set_sprite(std::string img_path, bool resize_rect)
{
  s32 w;
  s32 h;
  sprite_ = IMG_LoadTexture(GM::ins().renderer, img_path.c_str());
  if (!sprite_)
    return false;

  if (resize_rect)
  {
    if (SDL_QueryTexture(sprite_, NULL, NULL, &w, &h) == 0)
    {
      set_size(w, h);
    }
  }
  return true;
}

void Entity::set_size(s32 w, s32 h)
{
  rect_.w = w;
  rect_.h = h;
}

Vector2 Entity::position() const
{
  return position_;
}
