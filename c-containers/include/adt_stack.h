//  adt_stack.h

#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__ 1

#include "ABGS_MemoryManager\abgs_platform_types.h"
#include "adt_memory_node.h"
#include "adt_vector.h"

// Stack type
typedef struct stack_s
{
  Vector *storage_;
  struct stack_ops_s *ops_;
} Stack;

struct stack_ops_s
{

  /** @brief Destroy the stack
   *
   * Destroy the stack and all its data
   *
   * @return Error code
   * @param stack Stack to destroy
   */
  s16(*destroy) (Stack *stack);

  /** @brief Reset stack data
   *
   * Remove all the data stored but not the stack
   *
   * @return Error code
   * @param stack Stack to reset
   */
  s16(*reset) (Stack *stack);

  /** @brief Resize the stack capacity
   *
   * Change the capacity of the stack.
   * If the new value is smaller than the current,
   * you can lose data
   *
   * @return Error code
   * @param list List to resize
   */
  s16(*resize) (Stack *stack, u16 new_size);

  /** @brief Capacity getter
   *
   * Returns the maximum number of elements the stack can store
   *
   * @return Stack capacity
   * @param stack Stack to check
   */
  u16(*capacity) (Stack *stack);

  /** @brief Length getter
   *
   * Returns the current number of elements stored.
   * If it returns 0xFFFF is an error return.
   *
   * @return Stack length
   * @param stack Stack to check
   */
  u16(*length) (Stack *stack);

  /** @brief Checks if the stack is empty
   *
   * Returns true if the stack is empty
   *
   * @return If stack is empty
   * @param stack Stack to check
   */
  bool(*isEmpty) (Stack *stack);

  /** @brief Checks if the stack is full
   *
   * Returns true if the stack is full
   *
   * @return If stack is full
   * @param stack Stack to check
   */
  bool(*isFull) (Stack *stack);

  /** @brief Top element getter
   *
   * Returns a reference to the element of the top
   *
   * @return A reference to the top element
   * @param stack Stack to check
   */
  void*(*top) (Stack *stack);

  /** @brief Insert at the top
   *
   * Store data in the top position of the stack
   *
   * @return Error code
   * @param stack Stack
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*push) (Stack *stack, void *data, u16 bytes);

  /** @brief Extract the top element
   *
   * Extracts the data stored in the top position of the stack
   *
   * @return Pointer to data extracted
   * @param stack Stack
   */
  void*(*pop) (Stack *stack);

  /** @brief Concatenate two stacks
   *
   * Attach a copy of the elements of the source stack to a stack
   *
   * @return Error code
   * @param stack Stack to concatenate
   * @param stack_src Stack to copy
   */
  s16(*concat) (Stack *stack, Stack *stack_src);

  /** @brief Execute a function from all elements of the stack
   *
   * Iterate through the stack calling a function from each element
   *
   * @return Error code
   * @param stack Stack to traverse
   * @param callback Function to be called
   */
  s16(*traverse) (Stack *stack, void(*callback) (MemoryNode *));

  /** @brief Print the information and content of the stack
   *
   * The data saved in the stack is printed as hexadecimal
   *
   * @param stack Stack to print
   */
  void(*print) (Stack *stack);

  /** @brief Print the information and content of the stack
   *
   * Each byte of the data saved in the stack is printed as char
   *
   * @param stack Stack to print
   */
  void(*printAsChar) (Stack *stack);

  /** @brief Print the information and content of the stack
   *
   * The data saved in the stack is printed as string
   *
   * @param stack Stack to print
   */
  void(*printAsString) (Stack *stack);
};

/** @brief Creates a new stack
  *
  * Allocate and initialize an empty new stack
  *
  * @param capacity Capacity of the new stack
  */
Stack* STACK_create(u16 capacity);

#endif // __ADT_STACK_H__
