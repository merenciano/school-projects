// adt_memory_node.h
// Toni Barella
// Algoritmos & Inteligencia Artificial
// ESAT 2016-2019
//
#ifndef __ADT_MEMORY_NODE_H__
#define __ADT_MEMORY_NODE_H__

#include "ABGS_MemoryManager\abgs_platform_types.h"

// Memory Node type
typedef struct memory_node_s
{
	void* data_;
	u16 size_;
  struct memory_node_s* next_;
  struct memory_node_s* prev_;
	struct memory_node_ops_s* ops_;
} MemoryNode;

// Memory Node's API Declarations
struct memory_node_ops_s
{
	/** @brief Data getter
	 *
	 * Returns a reference to the data_ of the Memory Node
	 *
	 * @return A reference to data
	 * @param node Memory node
	 */
	void*(*data) (MemoryNode *node);

	/** @brief Data setter
	 *
	 * Assigns src to the Memory Node data
	 *
	 * @return Error code
	 * @param node Memory node
	 * @param src Pointer to new data
	 * @param bytes Size of the new data
	 */
	s16(*setData) (MemoryNode *node, void *src, u16 bytes);

  /** @brief Next setter
   *
   * Set the next memory node
   *
   * @return Error code
   * @param node Memory node
   * @param next pointer to next node
   */
  s16(*setNext) (MemoryNode *node, MemoryNode *next);

  /** @brief Prev setter
   *
   * Set the previous memory node
   *
   * @return Error code
   * @param node Memory node
   * @param next pointer to previous node
   */
  s16(*setPrev) (MemoryNode *node, MemoryNode *prev);

	/** @brief Size getter
	 *
	 * Returns the size of the data of the Memory Node in bytes.
	 * If it returns 0 if Memory Node is NULL
	 *
	 * @return Size of the data of the Memory Node
	 * @param node Memory node
	 */
	u16(*size) (MemoryNode *node);

  /** @brief Next node getter
   *
   * Returns a pointer to the next Memory Node
   *
   * @return Pointer to next Memory Node
   * @param node Memory node
   */
  MemoryNode* (*next) (MemoryNode *node);

  /** @brief Prev node getter
   *
   * Returns a pointer to the previous Memory Node
   *
   * @return Pointer to previous Memory Node
   * @param node Memory node
   */
  MemoryNode* (*prev) (MemoryNode *node);

	/** @brief Reset content of Memory Node
	 *
	 * Frees data and sets data to NULL and size to 0
	 *
	 * @return Error code
	 * @param node Memory node
	 */
	s16(*reset) (MemoryNode *node);

  /** @brief Reset content of Memory Node
   *
   * Sets data to NULL and size to 0, but leave the data reserved
   *
   * @return Error code
   * @param node Memory node
   */
  s16(*softReset) (MemoryNode *node);

	/** @brief Free memory, both data and node
	 *
	 * Frees the data and the Memory Node and sets Memory Node to NULL
	 *
	 * @return Error code
	 * @param node Memory node
	 */
	s16(*free) (MemoryNode *node);

	/** @brief Free the Memory Node but not its data
	 *
	 * Free the Memory Node and sets it to NULL, but leave the data reserved
	 *
	 * @return Error code
	 * @param node Memory node
	 */
	s16(*softFree) (MemoryNode *node);

	/** @brief Memory setter
	 *
	 * Sets all the data memory with an u8 value
	 *
	 * @return Error code
	 * @param node Memory node
	 * @param value Set value
	 */
	s16(*memSet) (MemoryNode *node, u8 value);

	/** @brief Copy data
	 *
	 * Makes a copy of src and assigns the copy to Memory Node data
	 *
	 * @return Error code
	 * @param node Memory node
	 * @param src Pointer to the data you want to copy
	 * @param bytes Size of the src data
	 */
	s16(*memCopy) (MemoryNode *node, void *src, u16 bytes);

	/** @brief Add data to the current data
	 *
	 * Makes a copy of the src data and adds to the current data.
	 * data A + src B = data AB
	 *
	 * @return Error code
	 * @param node Memory node
	 * @param src Pointer to the data you want to add
	 * @param bytes Size of the data you want to add
	 */
	s16(*memConcat) (MemoryNode *node, void *src, u16 bytes);

	/** @brief Applies a mask to the Memory Node
	 *
	 * Aplies an u8 AND mask to the data of the Memory Node
	 *
	 * @return Error code
	 * @param node Memory node
	 * @param mask The AND mask you want to apply
	 */
	s16(*memMask) (MemoryNode *node, u8 mask);

	/** @brief Prints data
	 *
	 * Prints the data of a MemoryNode in hexadecimal
	 *
	 * @param node Memory node
	 */
	void(*print) (MemoryNode *node);

  /** @brief Prints data
   *
   * Prints each byte of data as a character
   *
   * @param node Memory node
   */
  void(*printAsChar) (MemoryNode *node);

  /** @brief Prints data
   *
   * Prints the value of data as a string
   *
   * @param node Memory node
   */
  void(*printAsString) (MemoryNode *node);
};

MemoryNode* MEMNODE_create();
s16 MEMNODE_createFromRef(MemoryNode **node);
s16 MEMNODE_createLite(MemoryNode *node); // Creates a memory node without memory allocation
s16 MEMNODE_printMemoryNodeInfo(MemoryNode* node); // Prints all the info of a MemoryNode, this function is called by the other print functions

#endif // __ADT_MEMORY_NODE_H__