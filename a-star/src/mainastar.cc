#include "platform_types.h"
#include "vector_2.h"
#include "astarnode.h"
#include "gamemanager.h"
#include "levelmap.h"
#include "path.h"
#include "agent.h"
#include <stdio.h>
#include <list>
#include <vector>

Agent g_agent;
Path g_path;
LevelMap g_map;

s32 AStar(Vector2 start_pos, Vector2 goal_pos, const LevelMap &level_map, Path &out_path)
{
  start_pos.x_ = std::floor(start_pos.x_ / level_map.info_ratio());
  start_pos.y_ = std::floor(start_pos.y_ / level_map.info_ratio());
  goal_pos.x_ = std::floor(goal_pos.x_ / level_map.info_ratio());
  goal_pos.y_ = std::floor(goal_pos.y_ / level_map.info_ratio());
  if (level_map.isNavigable(start_pos) == false ||
      level_map.isNavigable(goal_pos) == false)
  {
    printf("Start or goal is not a navigable position\n");
    return kErrorCode_GenericError;
  }
  bool completed = false;
  std::list<AStarNode*> open_list;
  std::list<AStarNode*> closed_list;

  //Create a node containing the goal state: node_goal
  AStarNode *goal = new AStarNode(goal_pos);
  goal->pos_ = goal_pos;

  //Create a node containing the start state: node_start
  AStarNode *start = new AStarNode(start_pos);
  start->g_ = 0.0f;
  start->h_ = start->heuristicOctile(*goal);
  start->f_ = start->h_;
  start->pos_ = start_pos;

  //Put node_start on the OPEN list
  open_list.push_back(start);

  //while the OPEN list is not empty
  AStarNode *current = nullptr;
  while (!open_list.empty())
  {
    //Get the node off the OPEN list with the lowest f and call it node_current
    //open_list.sort(); // See operator< in AStarNode struct
    open_list.sort([](AStarNode* n1, AStarNode* n2)
    {
      return n1->f_ < n2->f_;
    });
    current = open_list.front();
    open_list.pop_front();

    //If node_current is the same state as node_goal: break from the while loop
    if (current->pos_ == goal->pos_)
    {
      completed = true;
      break;
    }

    //Generate each state node_successor that can come after node_current
    std::vector<AStarNode*> successor;

    // Top left succesor
    Vector2 new_pos = Vector2(current->pos_.x_ - 1, current->pos_.y_ - 1);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 14.0f;
      successor.push_back(n);
    }

    // Top succesor
    new_pos = Vector2(current->pos_.x_, current->pos_.y_ - 1);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 10.0f;
      successor.push_back(n);
    }

    // Top right succesor
    new_pos = Vector2(current->pos_.x_ + 1, current->pos_.y_ - 1);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 14.0f;
      successor.push_back(n);
    }

    // Right succesor
    new_pos = Vector2(current->pos_.x_ + 1, current->pos_.y_);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 10.0f;
      successor.push_back(n);
    }

    // Bottom right succesor
    new_pos = Vector2(current->pos_.x_ + 1, current->pos_.y_ + 1);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 14.0f;
      successor.push_back(n);
    }
    
    // Bottom succesor
    new_pos = Vector2(current->pos_.x_, current->pos_.y_ + 1);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 10.0f;
      successor.push_back(n);
    }

    // Bottom left succesor
    new_pos = Vector2(current->pos_.x_ - 1, current->pos_.y_ + 1);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 14.0f;
      successor.push_back(n);
    }

    // Left succesor
    new_pos = Vector2(current->pos_.x_ - 1, current->pos_.y_);
    if (level_map.isNavigable(new_pos))
    {
      AStarNode *n = new AStarNode();
      n->pos_ = new_pos;
      n->g_ = 10.0f;
      successor.push_back(n);
    }

    //For each node_successor of node_current
    for (AStarNode *n : successor)
    {
      bool discarded = false;
      // Set the cost of node_successor to be the cost of node_current
      // plus the cost to get to node_successor from node_current
      n->g_ = current->g_ + n->g_;
      
      // Find node_successor on the OPEN list
      for (auto it = open_list.begin(); it != open_list.end(); )
      {
        if ((*it)->pos_ == n->pos_)
        {
          // If node_successor is on the OPEN list but the existing one is as good
          // or better then discard this successor and continue with next successor
          if ((*it)->g_ <= n->g_)
            discarded = true;
          else
            open_list.erase(it); // Its safe because next instruction is break
            
          break; // There are no more elements in the open list with the same position
        }
        ++it;
      }

      if (discarded)
      {
        delete n;
        continue;
      }

      for (auto it = closed_list.begin(); it != closed_list.end(); )
      {
        if ((*it)->pos_ == n->pos_)
        {
          // If node_successor is on the CLOSED list but the existing one is as good
          // or better then discard this successor and continue with next successor
          if ((*it)->g_ <= n->g_)
            discarded = true;
          else
            closed_list.erase(it); // Its safe because next instruction is break

          break; // There are no more elements in the closed list with the same position
        }
        ++it;
      }

      if (discarded)
      {
        delete n;
        continue;
      }

      // Remove occurences of node_successor from OPEN and CLOSED
      // Lucas: DONE in the loop

      // Set the parent of node_successor to node_current
      n->parent_ = current;

      // Set h to be the estimated distance to node_goal (using the heuristic function)
      n->h_ = n->heuristicOctile(*goal); // The content of the ptr for recerence or pointer directly? Which is better?
      n->f_ = n->h_ + n->g_;

      // Add node_successor to the OPEN list
      open_list.push_back(n);
    }
    successor.clear();

    // Add node_current to the CLOSED list
    closed_list.push_back(current);
  }
  if (completed)
  {
    Vector2 *path_points;
    std::list<Vector2> temp_list;
    AStarNode *i = current;
    
    while (i != nullptr)
    {
      temp_list.push_front(i->pos_);
      i = i->parent_;
    }

    path_points = new Vector2[temp_list.size()];
    s32 count = 0;
    for (auto p : temp_list)
    {
      path_points[count++] = p * (float)level_map.info_ratio();
    }
    out_path.set_points(path_points, count, Path::kPathOnce);
    
  }

  // Remove all nodes
  for (auto n : open_list)
    delete n;
  open_list.clear();

  for (auto n : closed_list)
    delete n;
  closed_list.clear();

  delete current;
  
  if (completed)
    return kErrorCode_Ok;
  return kErrorCode_GenericError;
}

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

  if (!g_map.loadMap("../../data/map_03_layoutABGS.bmp",
      "../../data/map_03_60x44_cost.bmp"))
  {
    printf("Error loading the level map\n");
    return kErrorCode_GenericError;
  }
  g_agent.init(Vector2(144.0f, 256.0f), AgentSize::kNormal);
  g_agent.set_size(8, 8);
  g_agent.set_color(255, 0, 255, 255);
  if (kErrorCode_Ok != AStar(g_agent.position(),
      Vector2(944.0f, 272.0f), g_map, g_path))
  {
    return kErrorCode_GenericError;
  }

  g_agent.path_ = &g_path;

  //g_agent.path_.DEBUG_PrintStdout();
  //g_map.DEBUG_PrintStdout();
  return kErrorCode_Ok;
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
  g_agent.update(delta_time);
}

void Render()
{
  // Clear to black before drawing the frame
  SDL_SetRenderDrawColor(GM::ins().renderer, 0, 0, 0, 255);
  SDL_RenderClear(GM::ins().renderer);

  g_map.draw();
  g_agent.draw();

  SDL_RenderPresent(GM::ins().renderer);
}

s32 main() {
  if (kErrorCode_Ok != Init())
  {
    return 1;
  }

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
  return(0);
}
