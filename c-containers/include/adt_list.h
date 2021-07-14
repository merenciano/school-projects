// adt_list.h

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__ 1

#include "ABGS_MemoryManager/abgs_platform_types.h"
#include "adt_memory_node.h"

//  List type
typedef struct list_s
{
  MemoryNode* first_;
  MemoryNode* last_;
  u16 capacity_;
  u16 length_;
  struct list_ops_s* ops_;
} List;

//  List API Declarations
struct list_ops_s
{
  /** @brief Reset list data
   *
   * Remove all the data stored but not the list
   *
   * @return Error code
   * @param list List to reset
   */
  s16(*reset) (List* list);

  /** @brief Destroy the list
   *
   * Destroy the list and all its data
   *
   * @return Error code
   * @param list List to destroy
   */
  s16(*destroy) (List* list);

  /** @brief Resize list capacity
   *
   * Change the capacity of the list. 
   * If the new value is smaller than the current,
   * you can lose data
   *
   * @return Error code
   * @param list List to resize
   */
  s16(*resize) (List *list, u16 new_size);
  
  /** @brief Capacity getter
   *
   * Returns the maximum number of elements the list can store
   *
   * @return List capacity
   * @param list List to check
   */
  u16(*capacity) (List* list);

  /** @brief Length getter
   *
   * Returns the current number of elements stored.
   * If it returns 0xFFFF is an error return.
   *
   * @return List length
   * @param list List to check
   */
  u16(*length) (List* list);

  /** @brief Checks if the list is empty
   *
   * Returns true if the list is empty
   *
   * @return If list is empty
   * @param list List to check
   */
  bool(*isEmpty) (List* list);

  /** @brief Checks if the list is full
   *
   * Returns true if the list is full
   *
   * @return If list is full
   * @param list List to check
   */
  bool(*isFull) (List* list);

  /** @brief First element getter
   *
   * Returns a reference of the first element of the list
   *
   * @return A reference of the first element
   * @param list List to check
   */
  void*(*first) (List* list);

  /** @brief Last element getter
   *
   * Returns a reference of the last element of the list
   *
   * @return A reference of the last element
   * @param list List to check
   */
  void*(*last) (List* list);

  /** @brief Any element getter
   *
   * Returns a reference of the element of the list
   * stored at a given position
   *
   * @return A reference of the element
   * @param list List to check
   * @param position Position of the element to get
   */
  void*(*at) (List* list, u16 position);

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
  s16(*insertFirst) (List* list, void* data, u16 bytes);

  /** @brief Insert at last position
   *
   * Store data in the last position of the list.
   *
   * @return Error code
   * @param list List
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*insertLast) (List* list, void* data, u16 bytes);

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
  s16(*insertAt) (List* list, void* data, u16 bytes, u16 position); // Inserts a new element on a given position

  // Extraction

  /** @brief Extract first element
   *
   * Extracts the data stored in the first position of the list
   *
   * @return Pointer to data extracted
   * @param list List
   */
  void*(*extractFirst) (List* list);

  /** @brief Extract last element
   *
   * Extracts the data stored in the last position of the list
   *
   * @return Pointer to data extracted
   * @param list List
   */
  void*(*extractLast) (List* list);

  /** @brief Extract the element at the position
   *
   * @return Pointer to data extracted
   * @param list List
   * @param position Position
   */
  void*(*extractAt) (List* list, u16 position);

  /** @brief Concatenate two lists
   *
   * Attach a copy of the elements of the source list to a list
   *
   * @return Error code
   * @param list List to concatenate
   * @param list_src List to copy
   */
  s16(*concat) (List* list, List* list_src);

  /** @brief Execute a function from all elements of the list
   *
   * Iterate through the list calling a function from each element
   *
   * @return Error code
   * @param list List to traverse
   * @param callback Function to be called
   */
  s16(*traverse) (List* list, void(*callback) (MemoryNode *));

  /** @brief Print the information and content of the list
   *
   * The data saved in the list is printed as hexadecimal
   *
   * @param list List to print
   */
  void(*print) (List* list);

  /** @brief Print the information and content of the list
   *
   * Each byte of the data saved in the list is printed as char
   *
   * @param list List to print
   */
  void(*printAsChar) (List* list);

  /** @brief Print the information and content of the list
   *
   * The data saved in the list is printed as string
   *
   * @param list List to print
   */
  void(*printAsString) (List* list);
};

/** @brief Creates a new list
  *
  * Allocate and initialize an empty new list
  *
  * @param capacity Capacity of the new list
  */
List* LIST_create(u16 capacity);

/** @brief Create a MemoryNode for operations
  *
  * You need to call this in order to use the list
  */
void LIST_prepareList();

/** @brief Destroy the MemoryNode for operations
  */
void LIST_releaseList();

#endif // __ADT_LIST_H__