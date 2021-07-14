#include "astarnode.h"

float AStarNode::heuristicOctile(const AStarNode &goal) const
{
  float dx = abs(pos_.x_ - goal.pos_.x_);
  float dy = abs(pos_.y_ - goal.pos_.y_);
  return 10.0f * (dx + dy) - 6.0f * std::min(dx, dy);
}

float AStarNode::heuristicEuclidean(const AStarNode &goal) const
{
  float dx = abs(pos_.x_ - goal.pos_.x_);
  float dy = abs(pos_.y_ - goal.pos_.y_);
  return sqrtf(dx*dx + dy*dy);
}