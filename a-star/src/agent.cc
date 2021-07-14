#include "agent.h"
#include "gamemanager.h"
#include <cassert>
#include <stdio.h>

static Agent* CheckDistances(const Agent& agent, float reaction_distance);

Agent::Agent()
{
  state_ = AgentState::kStateNone;
  speed_ = 0.0f;
  type_ = MovType::kMovNone;
  position_ = Vector2(0.0f, 0.0f);
  velocity_ = Vector2(0.0f, 0.0f);
  target_reached_ = true;
  target_entity_ = nullptr;
  tracking_reaction_ = 0.75f;
  path_ = nullptr;
  srand(SDL_GetTicks());
}

Agent::Agent(Agent &&other) : Entity(std::move(other))
{
  state_ = other.state_;
  type_ = other.type_;
  velocity_ = other.velocity_;
  speed_ = other.speed_;
  target_position_ = other.target_position_;
  pattern_data_ = other.pattern_data_;
  size_ = other.size_;
  target_entity_ = other.target_entity_;
  tracking_reaction_ = other.tracking_reaction_;
  path_ = other.path_;

  other.state_ = AgentState::kStateNone;
  other.type_ = MovType::kMovNone;
  other.velocity_ = Vector2(0.0f, 0.0f);
  other.speed_ = 0.0f;
  other.target_position_ = Vector2(0.0f, 0.0f);
  other.pattern_data_ = PatternData();
  other.size_ = AgentSize::kNone;
  other.target_entity_ = nullptr;
  other.tracking_reaction_ = 0.0f;
  other.path_ = nullptr;
}

Agent::~Agent()
{
  
}

void Agent::init(Vector2 pos, AgentSize size)
{
  position_ = pos;
  size_ = size;
  velocity_ = Vector2(0.0f, 0.0f);
  state_ = AgentState::kStateWorking;

  switch (size_)
  {
    case kHuge:
      speed_ = 0.75f;
      set_size(100, 100);
      type_ = kMovPattern;
      break;

    case kNormal:
      speed_ = 1.0f;
      set_size(60, 60);
      type_ = kMovDeterministic;
      break;

    case kSmall:
      speed_ = 1.25f;
      set_size(30, 30);
      type_ = kMovRandom;
      break;
    
    default:
      type_ = kMovNone;
      assert(false && "Default case in Agent init method.");
      break;
  }
}

void Agent::MOV_deterministic(u32 delta_time)
{
  target_position_ = path_->currentPos();
  velocity_ = target_position_ - position_;
  if (velocity_.magnitude() <= 1.0f + kBaseSpeed * speed_ * (delta_time / 1000)) {
    target_position_ = path_->getNextPos();
    if (target_position_.x_ == -1)
    {
      type_ = kMovStopped;
      velocity_ = Vector2(0, 0);
      return;
    }
    velocity_ = target_position_ - position_;
  }
  velocity_.normalize();
}

void Agent::MOV_random(u32 delta_time)
{
  velocity_ = target_position_ - position_;
  if (velocity_.magnitude() <= 1.0f + kBaseSpeed * speed_ * (delta_time / 1000)) {
    target_reached_ = true;
    velocity_ = Vector2(0,0);
  }
  velocity_.normalize();
}

void Agent::MOV_tracking(u32 delta_time, bool inverse)
{
  assert(target_entity_ && "Entity target not setted");
  target_position_ =  target_entity_->position();
  Vector2 direct_path= target_position_ - position_;
  direct_path.normalize();
  velocity_ = Vector2::Lerp(velocity_, direct_path, tracking_reaction_);
  velocity_.normalize(); // NOTE: necessary?
  if (inverse)
    velocity_ *= -1;
}

void Agent::MOV_pattern(u32 delta_time)
{
  assert(pattern_data_.pattern && "No pattern");
  if (pattern_data_.current_distance >= pattern_data_.segment_distance)
  {
    ++pattern_data_.current %= pattern_data_.num_elements;
    pattern_data_.current_distance = 0.0f;
  }

  // NOTE: Should I make pattern check time instead of distance in order to
  // move to the next direction? That way I could use kDirNone as a valid direciton
  assert(pattern_data_.pattern[pattern_data_.current] != kDirNone && "Direction None");
  switch(pattern_data_.pattern[pattern_data_.current])
  {
    case kDirUp:
      velocity_ = Vector2(0,-1);
      break;

    case kDirDown:
      velocity_ = Vector2(0,1);
      break;

    case kDirRight:
      velocity_ = Vector2(1,0);
      break;

    case kDirLeft:
      velocity_ = Vector2(-1,0);
      break;

    default:
      assert(false && "Default case");
      break;
  }
}

void Agent::MOV_stopped(u32 delta_time)
{
  velocity_ = Vector2(0, 0);
}

void Agent::crossWindowEdges()
{
  if (position_.x_ < 0)
  {
    position_.x_ += GM::ins().window_width();
  }
  else if (position_.x_ >= GM::ins().window_width())
  {
    position_.x_ -= GM::ins().window_width();
  }

  if (position_.y_ < 0)
  {
    position_.y_ += GM::ins().window_height();
  }
  else if (position_.y_ >= GM::ins().window_height())
  {
    position_.y_ -= GM::ins().window_height();
  }
}

void Agent::move(u32 delta_time)
{
  float delta_seconds = delta_time / 1000.0f;
  Vector2 step = velocity_ * kBaseSpeed * speed_ * delta_seconds;
  position_ = position_ + step;
  crossWindowEdges();
  pattern_data_.current_distance += step.magnitude();
  rect_.x = (s32)position_.x_;
  rect_.y = (s32)position_.y_;
}

void Agent::set_rand_target(int x_start, int x_len,
                       int y_start, int y_len)
{
  target_position_.x_ = (float)(rand() % x_len + x_start);
  target_position_.y_ = (float)(rand() % y_len + y_start);
  target_reached_ = false;
}

void Agent::set_pattern(Direction *pattern, u32 num_elements, float seg_distance)
{
  pattern_data_.pattern = pattern; 
  pattern_data_.num_elements = num_elements;
  pattern_data_.segment_distance = seg_distance;
  pattern_data_.current = 0;
  pattern_data_.current_distance = 0.0f;
}

void Agent::set_target_entity(Entity *e, float reaction)
{
  target_entity_ = e;
  tracking_reaction_ = reaction;
}

void Agent::updateState()
{
  float reaction_distance = 0.0f;
  switch(size_)
  {
    case kSmall: reaction_distance = 250.0f; break;
    case kNormal: reaction_distance = 225.0f; break;
    case kHuge: reaction_distance = 200.0f; break;
    default: assert(false && "Default case sertting reaction distance."); break;
  }
  // closest_agent will be nullptr if there is no agent closer than reaction_distance
  Agent *closest_agent = CheckDistances(*this, reaction_distance);
  switch (state_)
  {
    case kStateWorking:
      if (closest_agent != nullptr)
      {
        if (closest_agent->size_ > size_)
        {
          if (type_ == MovType::kMovPattern)
            target_reached_ = false;
          state_ = AgentState::kStateFleeing;
          target_entity_ = closest_agent;
        }
        else if (closest_agent->size_ < size_)
        {
          if (type_ == MovType::kMovPattern)
            target_reached_ = false;
          state_ = AgentState::kStateChasing;
          target_entity_ = closest_agent;
        }
      }
      break;

    case kStateChasing:
      if (closest_agent == nullptr)
      {
        state_ = AgentState::kStateWorking;
        target_entity_ = nullptr;
      }
      else
      {
        if (closest_agent->size_ > size_)
        {
          state_ = AgentState::kStateFleeing;
          target_entity_ = closest_agent;
        }
        else if (closest_agent->size_ < size_)
        {
          target_entity_ = closest_agent;
        }
      }
      break;

    case kStateFleeing:
      if (closest_agent == nullptr)
      {
        state_ = AgentState::kStateResting;
        target_entity_ = nullptr;
        start_rest_time_ = (float)SDL_GetTicks();
      }
      else
      {
        if (closest_agent->size_ > size_)
        {
          target_entity_ = closest_agent;
        }
        else if (closest_agent->size_ < size_)
        {
          state_ = AgentState::kStateChasing;
          target_entity_ = closest_agent;
        }
      }
      break;
    
    case kStateResting:
      if (closest_agent != nullptr && closest_agent->size_ > size_)
      {
        state_ = AgentState::kStateFleeing;
        target_entity_ = closest_agent;
      }
      else
      {
        if ((SDL_GetTicks() - start_rest_time_) > kRestDuration * 1000.0f)
        {
          state_ = AgentState::kStateWorking;
        }
      }
      break;

    default:
      assert(false && "The agent must have a valid state.");
      break;
  }
}

void Agent::update(u32 delta_time)
{
  switch (state_)
  {
    case kStateWorking:
      workingState(delta_time);
      break;

    case kStateChasing:
      MOV_tracking(delta_time);
      break;
    
    case kStateFleeing:
      MOV_tracking(delta_time, true);
      break;

    case kStateResting:
      MOV_stopped(delta_time);
      break;

    default:
      assert(false && "Agent update default case.");
      break;
  }

  move(delta_time);
}

void Agent::workingState(u32 delta_time)
{
  switch(type_)
  {
    case kMovNone:
      velocity_ = Vector2(0.0f, 0.0f); 
      break;

    case kMovStopped:
      MOV_stopped(delta_time);
      break;

    case kMovDeterministic:
      MOV_deterministic(delta_time);
      break;
    
    case kMovRandom:
      if (target_reached_)
        set_rand_target(50, 680, 50, 480);
      MOV_random(delta_time);
      break;

    case kMovPattern:
      if (target_reached_ == false)
      {
        target_position_ = return_position_;
        MOV_random(delta_time);
      }
      else
      {
        return_position_ = position_;
        MOV_pattern(delta_time);
      }
      break;
    
    default:
      assert(false && "Default case on agent workingState method.");
      break;
  }
}

static Agent* CheckDistances(const Agent& agent, float reaction_distance)
{
  Agent *closest = nullptr;
  float closest_distance = 0.0f; // Save value in order to reduce computation
  for (Agent &i : GM::ins().agents)
  {
    float sqrdst = Vector2::SqrDistance(agent.position(), i.position());
    if (sqrdst == 0.0f)
      continue; // Itself

    if (closest == nullptr)
    {
      closest = &i;
      closest_distance = sqrdst;
    }
    else
    {
      if (sqrdst < closest_distance)
      {
        closest = &i;
        closest_distance = sqrdst;
      }
    }
  }

  if (closest_distance <= (reaction_distance*reaction_distance))
  {
    return closest;
  }
  return nullptr;
}
