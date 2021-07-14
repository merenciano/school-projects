#ifndef __LMM_PATH_H__
#define __LMM_PATH_H__ 1

#include "platform_types.h"
#include "common_def.h"
#include "vector_2.h"

/**
 * @file path.h
 * @brief Path class
 *
 * This class represents a path that can be followed by any agent.
 * The path is basically an array of 2D screen points and some
 * functionality in order to be used properly.
 * The agents need a path for the deterministic movement.
 */
struct Path {
public:
  /**
   * @file path.h
   * @brief Way to move across the path.
   *
   * Once means the agent will stop when reaching the last point.
   * Forward and reverse are the same thing,
   * when the agent reach the end of the path
   * if the path is set to kPathForward, it automatically
   * changes to kPathReverse and go back, and then when
   * the agent reach the start of the path it changes to kPathForward
   * again and repeat until a change of type.
   * If the path is Circular, when reaching the end, the path
   * return to the first point and start again.
   */
  enum Type {
    kPathNone,
    kPathOnce,
    kPathForward,
    kPathReverse,
    kPathCircular,
  };

  Path();
  ~Path();

  /**
   * @file path.h
   * @brief Path points setter
   *
   * You need to create the vector but then forget about it,
   * this class will handle it from now on 
   * @param p Array of points previously created with new[]
   * @param count Number of points in the array
   * @param t Type of path (Path::Type enum)
   */
  void set_points(Vector2 p[], u32 count, Type t);

  /**
   * @file path.h
   * @brief Check if the agent is currently in the last point
   * of the path
   *
   * @return true if current_ is the last point of the array
   */
  bool inLast() const;

  /**
   * @file path.h
   * @brief Update current_ to the next point and returns it.
   *
   * @return The current_ point after being updated.
   */
  Vector2 getNextPos(); // This is not a getter of an attribute

  /**
   * @file path.h
   * @brief Return the current point 
   *
   * @return points_[current_]
   */
  Vector2 currentPos() const;

  void DEBUG_PrintStdout() const;

private:
  u32 current_;
  Vector2 *points_;
  u32 length_;
  Type type_;
};

#endif // __LMM_PATH_H__
