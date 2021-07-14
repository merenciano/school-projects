//	adt_vector.h
//  Lucas Merenciano Martinez
//
#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__ 1

#include "ABGS_MemoryManager\abgs_platform_types.h"
#include "adt_memory_node.h"

//	Vector type
typedef struct vector_s
{
	u16 head_;
	u16 tail_;
	u16 capacity_;
	MemoryNode *storage_;
	struct vector_ops_s *ops_;
} Vector;

// Vector API Declarations
struct vector_ops_s
{
  /** @brief Reset vector's data
   *
   * Remove all the data stored but not the vector
   *
   * @return Error code
   * @param vector Vector to reset
   */
	s16(*reset) (Vector* vector);

  /** @brief Destroys the vector
   *
   * Remove all the data stored and the vector itself
   *
   * @return Error code
   * @param vector Vector to destroy
   */
  s16(*destroy) (Vector* vector);

  /** @brief Resize vector's capacity
   *
   * Change the capacity of the vector. 
   * If the new value is smaller than the current,
   * you can lose data
   *
   * @return Error code
   * @param vector Vector to resize
   * @param new_size A new size for the vector
   */
  s16(*resize)(Vector* vector, u16 new_size);

  // State queries

  /** @brief Capacity getter
   *
   * Returns the maximum number of elements the vector can store
   *
   * @return Vector capacity
   * @param vector Vector to check
   */
  u16(*capacity)(Vector* vector);

  /** @brief Length getter
   *
   * Returns the current number of elements stored
   *
   * @return Vector length
   * @param vector Vector to check
   */
  u16(*length)(Vector* vector);

  /** @brief Checks if the vector is empty
   *
   * Returns true if the vector is empty
   *
   * @return If vector is empty
   * @param vector Vector to check
   */
  bool(*isEmpty) (Vector* vector);

  /** @brief Checks if the vector is full
   *
   * Returns true if the vector is full
   *
   * @return If vector is full
   * @param vector Vector to check
   */
  bool(*isFull) (Vector* vector);

  // Data queries

  /** @brief First element getter
   *
   * Returns a reference to the first element of the vector
   *
   * @return A reference to the first element
   * @param vector Vector to check
   */
  void* (*first)(Vector* vector);

  /** @brief Last element getter
   *
   * Returns a reference to the last element of the vector
   *
   * @return A reference to the last element
   * @param vector Vector to check
   */
  void* (*last)(Vector* vector);

  /** @brief Any element getter
   *
   * Returns a reference to the element of the vector
   * stored at a given position
   *
   * @return A reference to the element
   * @param vector Vector to check
   * @param position Position of the element to get
   */
  void* (*at)(Vector* vector, u16 position);

  // Insertion

  /** @brief Insert at first position
   *
   * Store data in the first position of the vector,
   * displacing other elements if necessary.
   *
   * @return Error code
   * @param vector Vector
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*insertFirst) (Vector* vector, void* data, u16 bytes);

  /** @brief Insert at last position
   *
   * Store data in the last position of the vector.
   *
   * @return Error code
   * @param vector Vector
   * @param data Data to insert
   * @param bytes Size of the data
   */
  s16(*insertLast) (Vector* vector, void* data, u16 bytes);

  /** @brief Insert at position
   *
   * Store data in a specific position.
   * If the position is greater than the length of the vector
   * but it has enough capacity, the data will be stored
   * at last position
   *
   * @return Error code
   * @param vector Vector
   * @param data Data to insert
   * @param bytes Size of the data
   * @param position Position
   */
  s16(*insertAt) (Vector* vector, void* data, u16 bytes, u16 position);

  // Extraction

  /** @brief Extract first element
   *
   * Extracts the data stored in the first position of the vector
   *
   * @return Pointer to data extracted
   * @param vector Vector
   */
  void*(*extractFirst) (Vector* vector);

  /** @brief Extract last element
   *
   * Extracts the data stored in the last position of the vector
   *
   * @return Pointer to data extracted
   * @param vector Vector
   */
  void*(*extractLast) (Vector* vector);

  /** @brief Extract the element at the position
   *
   * @return Pointer to data extracted
   * @param vector Vector
   * @param position Position
   */
  void*(*extractAt) (Vector* vector, u16 position); // Extracts the element of the vector at the given position

  // Miscellaneous

  /** @brief Concatenate two vectors
   *
   * Attach a copy of the storage of the source vector to a vector
   *
   * @return Error code
   * @param vector Vector to concatenate
   * @param vector_src Vector to copy
   */
  s16(*concat) (Vector* vector, Vector* vector_src);

  /** @brief Execute a function from all elements of the vector
   *
   * Iterate through the vector calling a function from each element
   *
   * @return Error code
   * @param vector Vector to traverse
   * @param callback Function to be called
   */
  s16(*traverse)(Vector* vector, void(*callback) (MemoryNode *));

  /** @brief Print the information and content of the vector
   *
   * The data saved in the vector is printed as hexadecimal
   *
   * @param vector Vector to print
   */
  void(*print)(Vector* vector);

  /** @brief Print the information and content of the vector
   *
   * Each byte of the data saved in the vector is printed as char
   *
   * @param vector Vector to print
   */
  void(*printAsChar)(Vector* vector);

  /** @brief Print the information and content of the vector
   *
   * The data saved in the vector is printed as string
   *
   * @param vector Vector to print
   */
  void(*printAsString)(Vector* vector);
};

/** @brief Creates a new vector
  *
  * Allocate and initialize an empty new vector
  *
  * @param capacity Capacity of the new vector
  */
Vector* VECTOR_create(u16 capacity);

#endif  //	ADT_VECTOR_H__
