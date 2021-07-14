// adt_dllist.h

#ifndef __ADT_DLLIST_H__
#define __ADT_DLLIST_H__ 1

#include "ABGS_MemoryManager/abgs_platform_types.h"
#include "adt_memory_node.h"

//  List type
typedef struct dllist_s
{
  MemoryNode* first_;
  MemoryNode* last_;
  u16 capacity_;
  u16 length_;
  struct dllist_ops_s* ops_;
} DLList;

struct dllist_ops_s
{
  /** @brief Reset list data
   *
   * Remove all the data stored but not the list
   *
   * @return Error code
   * @param list List to reset
   */
  s16(*reset) (DLList* list);

  /** @brief Destroy the list
   *
   * Destroy the list and all its data
   *
   * @return Error code
   * @param list List to destroy
   */
  s16(*destroy) (DLList* list);

  /** @brief Resize list capacity
   *
   * Change the capacity of the list.
   * If the new value is smaller than the current,
   * you can lose data
   *
   * @return Error code
   * @param list List to resize
   */
  s16(*resize) (DLList *list, u16 new_size);
  
  // Getters  

  /** @brief Capacity getter
   *
   * Returns the maximum number of elements the list can store
   *
   * @return List capacity
   * @param list List to check
   */
  u16(*capacity) (DLList* list);

  /** @brief Length getter
   *
   * Returns the current number of elements stored.
   * If it returns 0xFFFF is an error return.
   *
   * @return List length
   * @param list List to check
   */
  u16(*length) (DLList* list);

  /** @brief Checks if the list is empty
   *
   * Returns true if the list is empty
   *
   * @return If list is empty
   * @param list List to check
   */
  bool(*isEmpty) (DLList* list);

  /** @brief Checks if the list is full
   *
   * Returns true if the list is full
   *
   * @return If list is full
   * @param list List to check
   */
  bool(*isFull) (DLList* list);

  /** @brief First element getter
   *
   * Returns a reference to the first element of the list
   *
   * @return A reference to the first element
   * @param list List to check
   */
  void*(*first) (DLList* list);

  /** @brief Last element getter
   *
   * Returns a reference to the last element of the list
   *
   * @return A reference to the last element
   * @param list List to check
   */
  void*(*last) (DLList* list);

  /** @brief Any element getter
   *
   * Returns a reference to the element of the list
   * stored at a given position
   *
   * @return A reference to the element
   * @param list List to check
   * @param position Position of the element to get
   */
  void*(*at) (DLList* list, u16 position);

  // Insertion

  /** @brief Insert at first position
   *
   * Store data in the first position of the list
   *
   * @return Error code
   * @param list List
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*insertFirst) (DLList* list, void* data, u16 bytes);

  /** @brief Insert at last position
   *
   * Store data in the last position of the list.
   *
   * @return Error code
   * @param list List
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*insertLast) (DLList* list, void* data, u16 bytes);

  /** @brief Insert at position
   *
   * Store data in a specific position.
   * If the position is greater than the length of the list
   * but it has enough capacity, the data will be stored
   * at last position
   *
   * @return Error code
   * @param list List
   * @param data Data to insert
   * @param bytes Size of the data
   * @param position Position
   */
  s16(*insertAt) (DLList* list, void* data, u16 bytes, u16 position);

  // Extraction

  /** @brief Extract first element
   *
   * Extracts the data stored in the first position of the list
   *
   * @return Pointer to data extracted
   * @param vector Vector
   */
  void*(*extractFirst) (DLList* list);

  /** @brief Extract last element
   *
   * Extracts the data stored in the last position of the list
   *
   * @return Pointer to data extracted
   * @param list List
   */
  void*(*extractLast) (DLList* list);

  /** @brief Extract the element at the position
   *
   * @return Pointer to data extracted
   * @param list List
   * @param position Position
   */
  void*(*extractAt) (DLList* list, u16 position);

  /** @brief Concatenate two lists
   *
   * Attach a copy of the elements of the source list to a list
   *
   * @return Error code
   * @param list List to concatenate
   * @param list_src List to copy
   */
  s16(*concat) (DLList* list, DLList* list_src);

  /** @brief Execute a function from all elements of the list
   *
   * Iterate through the list calling a function from each element
   *
   * @return Error code
   * @param list List to traverse
   * @param callback Function to be called
   */
  s16(*traverse) (DLList* list, void(*callback) (MemoryNode *));

  /** @brief Print the information and content of the list
   *
   * The data saved in the list is printed as hexadecimal
   *
   * @param list List to print
   */
  void(*print) (DLList* list);

  /** @brief Print the information and content of the list
   *
   * Each byte of the data saved in the list is printed as char
   *
   * @param list List to print
   */
  void(*printAsChar) (DLList* list);

  /** @brief Print the information and content of the list
   *
   * The data saved in the list is printed as string
   *
   * @param list List to print
   */
  void(*printAsString) (DLList* list);
};

/** @brief Creates a new list
  *
  * Allocate and initialize an empty new list
  *
  * @param capacity Capacity of the new list
  */
DLList* LIST_create(u16 capacity);

/** @brief Create a MemoryNode for operations
  *
  * You need to call this in order to use the list
  */
void DLLIST_prepareDLList();

/** @brief Destroy the MemoryNode for operations
  */
void DLLIST_releaseDLList();

#endif // __ADT_DLLIST_H__