#ifndef __LMM_GAMEMANAGER_H__
#define __LMM_GAMEMANAGER_H__ 1

#include "platform_types.h"
#include "agent.h"
#include "SDL.h"
#include <vector>

const float kLogicFreqSec = 60.0f;

/**
 * @file gamemanager.h
 * @brief Game manager singleton
 */
struct GM
{
public:
  /**
   * @file gamemanager.h
   * @brief Singleton's instance getter
   * 
   * @return Game manager reference
   */
  static GM& ins() { static GM i; return i; }

  /**
   * @file gamemanager.h
   * @brief Game manager initializer
   *
   * This method has to be called at the start of the application
   * @return Error
   */
  s16 init();

  /**
   * @file gamemanager.h
   * Application window
   */
  SDL_Window *window;

  /**
   * @file gamemanager.h
   * SDL2 Renderer
   */
  SDL_Renderer *renderer;

  /**
   * @file gamemanager.h
   * Logic loop simulation time
   */
  u32 time_step;

  /**
   * @file gamemanager.h
   * True means the game will close before starting the next loop
   */
  bool exit;

  /**
   * @file gamemanager.h
   * Window width getter
   */
  u32 window_width() const;

  /**
    * @file gamemanager.h
    * Window height getter
    */
  u32 window_height() const;

  /**
   * @file gamemanager.h
   * Vector containing all agents in the scene
   */
  std::vector<Agent> agents;

private:
  GM() {};
  u32 window_width_;
  u32 window_height_;

public:
  GM(GM const&) = delete;
  void operator=(GM const&) = delete;
};

#endif // __LMM_GAMEMANAGER_H__
