#ifndef __LMM_ENTITY_H__
#define __LMM_ENTITY_H__ 1

#include "platform_types.h"
#include "SDL2/SDL.h"
#include "vector_2.h"
#include <string>

/**
 * @file entity.h
 * @brief Entity class
 * 
 * Represents any drawable entity of the game represented as a square.
 * Is the parent class of the agents.
 */
class Entity {
public:
  Entity() { sprite_ = nullptr; }
  Entity(Entity &&other);
  ~Entity() { SDL_DestroyTexture(sprite_); }

  /**
   * @file entity.h
   * @brief Mark this entity to be rendered at the end of the frame
   */
  void draw();

  /**
   * @file entity.h
   * @brief Rect color setter
   * @param r Red
   * @param g Green 
   * @param b Blue 
   * @param a Alpha 
   */
  void set_color(u8 r, u8 g, u8 b, u8 a);

  /**
   * @file entity.h
   * @brief Sprite image setter
   * @param img_path Relative path of the image
   * @param resize_rect Set it to true if you want to resize entity to native image size
   * @return false if any error ocurred.
   */
  bool set_sprite(std::string img_path, bool resize_rect = false);

  /**
   * @file entity.h
   * @brief Rect size setter
   * @param w Entity width
   * @param h Entity height
   */
  void set_size(s32 w, s32 h);

  /**
   * @file entity.h
   * @brief Entity position getter
   */
  Vector2 position() const;

protected:
  Vector2 position_;
  SDL_Rect rect_;
  SDL_Texture *sprite_;
  u8 color_[4];
};

#endif // __LMM_ENTITY_H__
