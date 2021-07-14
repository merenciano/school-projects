#include "path.h"
#include <stdio.h>
#include <cassert>

Path::Path()
{
  current_ = 0;
  points_ = nullptr;
  length_ = 0;
  type_ = kPathNone;
}

Path::~Path()
{
  if (points_) delete[] points_;
  points_ = nullptr;
}

void Path::set_points(Vector2 p[], u32 count, Type t)
{
  if (points_) delete[] points_;
  points_ = p;
  length_ = (s32)count;
  current_ = 0;
  type_ = t;
}

Vector2 Path::getNextPos()
{
  assert(points_ && "Points not setted");
  
  switch (type_)
  {
  case kPathNone:
    return Vector2(-1, -1);
    break;

  case kPathOnce:
    // ARREGLAR PARPADEO AL ACABAR (MOV_NONE o P_STOPPED)
    if (!inLast())
      current_++;
    else
      type_ = kPathNone;
    break;

  case kPathForward:
    if (!inLast())
      current_++;
    else
    {
      type_ = kPathReverse;
      current_--;
    }
    break;

  case kPathReverse:
    if (current_ != 0)
      current_--;
    else
    {
      type_ = kPathForward;
      current_++;
    }
    break;

  case kPathCircular:
    if (!inLast())
      current_++;
    else
      current_ = 0;
    break;
  }

  assert(current_ < length_ && "current pos out of array");
  return points_[current_];
}

Vector2 Path::currentPos() const
{
  assert(current_ < length_ && "current pos out of array");
  return points_[current_];
}

bool Path::inLast() const
{
  return current_ == (length_ - 1);
}

void Path::DEBUG_PrintStdout() const
{
  for (u32 i = 0; i < length_; ++i)
  {
    printf("X: %d, Y: %d \n", (int)points_[i].x_, (int)points_[i].y_);
  }
}