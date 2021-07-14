#include <stdio.h>
#include <platform_types.h>
#include <SDL2/SDL.h>
#include "gamemanager.h"
#include "path.h"
#include "agent.h"

Path g_path1;
Vector2 *g_path_pts;
Direction *g_pattern;

s16 Init()
{
  SDL_SetMainReady();
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("Failed to initialize the SDL2 library\n");
    return -1;
  }
  if (0 != GM::ins().init())
    return -1;

  Agent normal_agent;
  Agent small_agent;
  Agent huge_agent;

  normal_agent.init(Vector2(0, 0), AgentSize::kNormal);
  normal_agent.set_color(255, 255, 0, 255);

  g_path_pts = new Vector2[4];
  g_path_pts[0] = Vector2(30.0f, 30.0f);
  g_path_pts[1] = Vector2(720.0f, 30.0f);
  g_path_pts[2] = Vector2(720.0f, 520.0f);
  g_path_pts[3] = Vector2(30.0f, 520.0f);
  g_path1.set_points(g_path_pts, 4, Path::Type::kPathCircular);
  normal_agent.path_ = &g_path1;

  small_agent.init(Vector2(100,100), AgentSize::kSmall);
  small_agent.set_color(0, 255, 100, 255);

  huge_agent.init(Vector2(200,350), AgentSize::kHuge);
  huge_agent.set_color(50, 0, 255, 255);

  g_pattern = new Direction[4];
  g_pattern[0] = kDirUp;
  g_pattern[1] = kDirDown;
  g_pattern[2] = kDirRight;
  g_pattern[3] = kDirLeft;
  huge_agent.set_pattern(g_pattern, 4, 200.0f);

  // NOTE: Change this method with a safer thing
  //g_track.set_target_entity(&g_normal_agent);

  GM::ins().agents.push_back(std::move(normal_agent));
  GM::ins().agents.push_back(std::move(small_agent));
  GM::ins().agents.push_back(std::move(huge_agent));

  return 0;
}

void Input(SDL_Event &e)
{
  while (SDL_PollEvent(&e) > 0)
  {
    switch (e.type)
    {
      case SDL_QUIT:
        GM::ins().exit = true;
        break;

      case SDL_KEYDOWN:
      {
        switch (e.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            GM::ins().exit = true;
            break;
        }
        break;
      }
    }
  }
}

void Logic(u32 delta_time)
{
  for (auto& a : GM::ins().agents)
  {
    a.updateState();
  }
  for (auto& a : GM::ins().agents)
  {
    a.update(delta_time);
  }
}

void Render()
{
  // Clear to black before drawing the frame
  SDL_SetRenderDrawColor(GM::ins().renderer, 0, 0, 0, 255);
  SDL_RenderClear(GM::ins().renderer);

  for (auto& a : GM::ins().agents)
  {
    a.draw();
  }

  SDL_RenderPresent(GM::ins().renderer);
}

int main()
{
  Init();
  u32 current_time = SDL_GetTicks();

  SDL_Event e;
  while (!GM::ins().exit)
  {
    u32 accum_time = SDL_GetTicks() - current_time;
    Input(e);

    // Only do the simulation when we have enough time accumulated
    // to simulate. We do not want to predict the future (I want, but not here).
    while (accum_time >= GM::ins().time_step)
    {
      Logic(GM::ins().time_step);
      current_time += GM::ins().time_step;
      accum_time = SDL_GetTicks() - current_time;
    }

    Render();
  }

  SDL_Quit();
  return 0;
}
