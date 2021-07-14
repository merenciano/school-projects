#ifndef __LMM_LEVEL_MAP_H__
#define __LMM_LEVEL_MAP_H__ 1

#include "entity.h"
#include <SDL2/SDL.h>
#include <string>

/**
 * @file levelmap.h
 * @brief Map class
 * 
 * Represents a playable map with information
 * needed for the pathfinding
 */
class LevelMap : public Entity
{
public:
  LevelMap();
  ~LevelMap();

  /**
   * @file levelmap.h
   * @brief Loads a map using two images for layout and information
   * @param map_layout_path Image to display the map
   * @param map_info_path Image with the information needed for the A*
   */
  bool loadMap(std::string map_layout_path, std::string map_info_path);

  /**
   * @file levelmap.h
   * @brief Checks if the position is valid for the path
   * 
   * Reads the information from the info image
   * 
   * @param position Position of the node to be checked
   */
  bool isNavigable(Vector2 position) const;

  /**
   * @file levelmap.h
   * @brief Level of granularity getter
   * 
   * If the info map is half of the layout, this method will return 2
   * 
   * @param position Position of the node to be checked
   */
  s32 info_ratio() const;

  /**
   * @file levelmap.h
   * @brief Prints the map info in the console
   * 
   * Used for debugging.
   * Spaces means navigable position and 'O' are obstacles.
   */
  void DEBUG_PrintStdout();

private:
  bool loadMapInfo(std::string map_info_path);

  s32 *map_info_;
  s32 map_info_w_;
  s32 map_info_h_;
  s32 info_ratio_;
};

#endif // __LMM_LEVEL_MAP_H__