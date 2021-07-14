//  adt_queue.h

#ifndef __ADT_QUEUE_H__
#define __ADT_QUEUE_H__ 1

#include "adt_list.h"
#include "ABGS_MemoryManager/abgs_platform_types.h"

// Queue type
typedef struct queue_s
{
  List *storage_;
  struct queue_ops_s *ops_;
} Queue;

struct queue_ops_s
{
  /** @brief Reset queue data
   *
   * Remove all the data stored but not the queue
   *
   * @return Error code
   * @param queue Queue to reset
   */
  s16(*reset) (Queue *queue);

  /** @brief Destroy the queue
   *
   * Destroy the queue and all its data
   *
   * @return Error code
   * @param queue Queue to destroy
   */
  s16(*destroy) (Queue *queue);

  /** @brief Resize the queue capacity
   *
   * Change the capacity of the queue.
   * If the new value is smaller than the current,
   * you can lose data
   *
   * @return Error code
   * @param queue Queue to resize
   */
  s16(*resize) (Queue *queue, u16 new_size);

  /** @brief Capacity getter
   *
   * Returns the maximum number of elements the queue can store
   *
   * @return Queue capacity
   * @param queue Queue to check
   */
  u16(*capacity) (Queue *queue);

  /** @brief Length getter
   *
   * Returns the current number of elements stored.
   * If it returns 0xFFFF is an error return.
   *
   * @return Queue length
   * @param queue Queue to check
   */
  u16(*length) (Queue *queue);

  /** @brief Checks if the queue is empty
   *
   * Returns true if the queue is empty
   *
   * @return If queue is empty
   * @param queue Queue to check
   */
  bool(*isEmpty) (Queue *queue);

  /** @brief Checks if the queue is full
   *
   * Returns true if the queue is full
   *
   * @return If queue is full
   * @param queue Queue to check
   */
  bool(*isFull) (Queue *queue);

  /** @brief Front element getter
   *
   * Returns a reference of the front element of the queue
   *
   * @return A reference of the front element
   * @param queue Queue to check
   */
  void*(*front) (Queue *queue);

  /** @brief Back element getter
   *
   * Returns a reference of the queue element of the back
   *
   * @return A reference of the element of the back
   * @param queue Queue to check
   */
  void*(*back) (Queue *queue);

  /** @brief Insert at the back
   *
   * Store data in the back of the queue
   *
   * @return Error code
   * @param queue Queue
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*enqueue) (Queue *queue, void *data, u16 bytes);

  /** @brief Extract front element
   *
   * Extracts the data stored in the front position of the queue
   *
   * @return Pointer to data extracted
   * @param queue Queue
   */
  void*(*dequeue) (Queue *queue);

  /** @brief Concatenate two queues
   *
   * Attach a copy of the elements of the source queue to a queue
   *
   * @return Error code
   * @param queue Queue to concatenate
   * @param queue_src Queue to copy
   */
  s16(*concat) (Queue *queue, Queue *queue_src);

  /** @brief Execute a function from all elements of the queue
   *
   * Iterate through the queue calling a function from each element
   *
   * @return Error code
   * @param queue Queue to traverse
   * @param callback Function to be called
   */
  s16(*traverse) (Queue *queue, void(*callback) (MemoryNode *));

  /** @brief Print the information and content of the queue
   *
   * The data saved in the queue is printed as hexadecimal
   *
   * @param queue Queue to print
   */
  void(*print) (Queue *queue);

  /** @brief Print the information and content of the queue
   *
   * Each byte of the data saved in the queue is printed as char
   *
   * @param queue Queue to print
   */
  void(*printAsChar) (Queue *queue);

  /** @brief Print the information and content of the queue
   *
   * The data saved in the queue is printed as string
   *
   * @param queue Queue to print
   */
  void(*printAsString) (Queue *queue);
};

/** @brief Creates a new queue
  *
  * Allocate and initialize an empty new queue
  *
  * @param capacity Capacity of the new queue
  */
Queue* QUEUE_create(u16 capacity);

#endif // __ADT_QUEUE_H__
