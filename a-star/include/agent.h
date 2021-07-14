// Lucas Merenciano Martinez  <merencianolu@esat-alumni.com>
#ifndef __LMM_AGENT_H__
#define __LMM_AGENT_H__ 1

#include "entity.h"
#include "common_def.h"
#include "vector_2.h"
#include "path.h"

/**
 * The Agent's speed attribute is a mutiplier for this base value.
 */
const float kBaseSpeed = 5.0f;

/**
 * The Agent's rest duration in seconds.
 */
const float kRestDuration = 3.0f;

/**
 * @file agent.h
 * @brief Agent size 
 * 
 * Agent size
 */
enum AgentSize {
  kNone = 0,
  kSmall = 1,
  kNormal = 2,
  kHuge = 3,
};

/**
 * @file agent.h
 * @brief Movement Types
 * 
 * Agent's movement behaviours
 */
enum MovType {
  kMovNone = 0,
  kMovDeterministic,
  kMovRandom,
  kMovTracking,
  kMovPattern,
  kMovStopped,
};

/**
 * @file agent.h
 * @brief Directions for patterns 
 * 
 * Directions used in patters. I'm using up, down, right and left
 * because the agens will move in 2D.
 */
enum Direction {
  kDirNone = 0,
  kDirUp,
  kDirDown,
  kDirRight,
  kDirLeft,
  // TODO: diagonals
};

/**
 * @file agent.h
 * @brief Agent states
 * 
 * Different states that will determine the agent's behavior.
 */
enum AgentState {
  kStateNone = 0,
  kStateWorking,
  kStateChasing,
  kStateFleeing,
  kStateResting,
};

/**
 * @file agent.h
 * @brief Pattern information 
 * 
 * In order to properly execute a pattern, other data must be
 * known. The two most important for the user are the pattern
 * itself (an array of Directions) and the Segment Distance,
 * that are the meters an agent has to move before switching
 * to the next direction of the pattern.
 */
struct PatternData {
  Direction *pattern;
  u32 num_elements;
  u32 current;
  float segment_distance;
  float current_distance;
};

/**
 * @file agent.h
 * @brief Agent class
 * 
 * Agents are Entities which have the capacity of moving across the
 * level according to a given behaviour.
 */
class Agent : public Entity {
 public:
  Agent(); 
  Agent(Agent &&other);
  ~Agent();

  /**
   * @file agent.h
   * @brief Initializer method
   * @param pos The initial position of the Agent
   * @param size Agent size
   * @param type Movement behaviour
   */
  void init(Vector2 pos, AgentSize size);

  /**
   * @file agent.h
   * @brief Deterministic movement behaviour
   *
   * The Agent follows a given path step by step
   *
   * @param delta_time Delta time
   */
  void MOV_deterministic(u32 delta_time);

  /**
   * @file agent.h
   * @brief Random movement behaviour
   *
   * The Agent moves to a random point
   *
   * @param delta_time Delta time
   */
  void MOV_random(u32 delta_time);

  /**
   * @file agent.h
   * @brief Tracking movement behaviour
   *
   * The Agent chases a given Entity
   *
   * @param delta_time Delta time
   * @param inverse If true, the agent will flee instead of chase the target
   */
  void MOV_tracking(u32 delta_time, bool inverse = false);

  /**
   * @file agent.h
   * @brief Tracking movement behaviour
   *
   * The Agent chases a given Entity
   *
   * @param delta_time Delta time
   */
  void MOV_pattern(u32 delta_time);

  /**
   * @file agent.h
   * @brief The Agent not moves
   *
   * It is used when the Agent needs to stay quiet.
   * For example when the Agent ends a Path with the
   * behaviour of doing it once.
   *
   * @param delta_time Delta time
   */
  void MOV_stopped(u32 delta_time);

  /**
   * @file agent.h
   * @brief Random point setter 
   *
   * Sets a random point to be used.
   * by the Random behaviour.
   * The point is randomly calculated inside a given square.
   * x_start and y_start indicates the top-left corner.
   *
   * @param x_start Square left limit 
   * @param x_len Square width
   * @param y_start Square top limit
   * @param x_start Square height 
   */
  void set_rand_target(int x_start, int x_len,
                       int y_start, int y_len);

  /**
   * @file agent.h
   * @brief Pattern setter 
   *
   * Sets a Direction array.
   * The pattern array is deleted by this class.
   * Forget about it when passed to this method.
   *
   * @param pattern Pointer to dynamic array
   * @param num_elements Direction count
   * @param seg_distance Segment (each element) distance in meters
   */
  void set_pattern(Direction *pattern,
                   u32 num_elements,
                   float seg_distance);

  /**
   * @file agent.h
   * @brief Target Entity setter
   *
   * Sets target for the Tracking movement.
   *
   * @param e Entity pointer. This class do not handle this pointer, it's used as reference.
   * @param reaction Value between 0.0 and 1.0 used for lerp between current direction and direct direction to the target.
   */
  void set_target_entity(Entity *e,
                         float reaction = 0.5f);

  /**
   * @file agent.h
   * @brief Applies the velocity vector to the Agent
   *
   * Velocity is calculated in meters per second.
   *
   * @param delta_time Delta time
   */
  void move(u32 delta_time);

  /**
   * @file agent.h
   * @brief Agent update method
   *
   * Calls to a movement method depending of his movement type
   *
   * @param delta_time Delta time
   */
  void update(u32 delta_time);

  /**
   * @file agent.h
   * @brief Agent state updater
   *
   * Check if it has to change its state
   *
   */
  void updateState();

  /**
   * @file agent.h
   *
   * Path followed in the deterministic movement.
   */
  Path *path_;

 private:

  void workingState(u32 delta_time);
  void crossWindowEdges();

  AgentState state_;
  MovType type_;
  Vector2 velocity_;
  float speed_;
  Vector2 target_position_;
  Vector2 return_position_;
  bool target_reached_;
  PatternData pattern_data_;
  AgentSize size_;
  Entity *target_entity_;
  /* 1.0f means direct tracking (not reallistic)
     and 0.0f means not tracking at all         */
  float tracking_reaction_;
  float start_rest_time_;
};

#endif // __LMM_AGENT_H__
