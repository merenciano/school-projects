#ifndef __LMM_A_STAR_NODE_H__
#define __LMM_A_STAR_NODE_H__ 1

#include <math.h>
#include <algorithm>
#include "vector_2.h"
#include "platform_types.h"

/**
 * @file astarnode.h
 * @brief A* node class
 * 
 * Represents a navigable position of the map
 * and stores the necessary information
 * for the A* algorithm
 */
class AStarNode
{
public:
  AStarNode() { parent_ = nullptr; }
  AStarNode(Vector2 pos) : pos_(pos) { parent_ = nullptr; }

  /**
   * @file astarnode.h
   * @brief Heuristic value calculation for octile movement
   * @param goal Node representing the goal position
   */
  float heuristicOctile(const AStarNode &goal) const;

  /**
   * @file astarnode.h
   * @brief Heuristic value calculation for direct distance
   * @param goal Node representing the goal position
   */
  float heuristicEuclidean(const AStarNode &goal) const;

  /**
   * @file astarnode.h
   * The cost up to this node plus the heuristic cost to goal
   */
  float f_;

  /**
   * @file astarnode.h
   * The cost up to this node
   */
  float g_;

  /**
   * @file astarnode.h
   * Heuristic cost to goal
   */
  float h_;

  /**
   * @file astarnode.h
   * Position of the node, this can be different
   * to the position on the map due to granularity
   */
  Vector2 pos_;

  /**
   * @file astarnode.h
   * Predecesor node
   */
  AStarNode *parent_;
};

#endif // __LMM_A_STAR_NODE_H__
