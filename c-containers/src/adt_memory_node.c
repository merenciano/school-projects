// adt_memory_node.c : 
// Toni Barella
// Algoritmos & Inteligencia Artificial
// ESAT 2016-2019
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_def.h"
#include "adt_memory_node.h"
#include "ABGS_MemoryManager\abgs_memory_manager.h"

// Memory Node Declarations
static s16 MEMNODE_initWithoutCheck(MemoryNode *node);
static void* MEMNODE_data(MemoryNode *node);
static s16 MEMNODE_setData(MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_setNext(MemoryNode *node, MemoryNode *next);
static s16 MEMNODE_setPrev(MemoryNode *node, MemoryNode *prev);
static u16 MEMNODE_size(MemoryNode *node);
static MemoryNode* MEMNODE_next(MemoryNode *node);
static MemoryNode* MEMNODE_prev(MemoryNode *prev);
static s16 MEMNODE_reset(MemoryNode *node);
static s16 MEMNODE_softReset(MemoryNode* node);
static s16 MEMNODE_free(MemoryNode *node);
static s16 MEMNODE_softFree(MemoryNode *node);
static s16 MEMNODE_memSet(MemoryNode *node, u8 value);
static s16 MEMNODE_memCopy(MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_memConcat(MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_memMask(MemoryNode *node, u8 mask);
static void MEMNODE_print(MemoryNode *node);
static void MEMNODE_printAsChar(MemoryNode *node);
static void MEMNODE_printAsString(MemoryNode *node);

// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops =
{
  .data = MEMNODE_data,
  .setData = MEMNODE_setData,
  .setNext = MEMNODE_setNext,
  .setPrev = MEMNODE_setPrev,
	.size = MEMNODE_size,
  .next = MEMNODE_next,
  .prev = MEMNODE_prev,
	.reset = MEMNODE_reset,
  .softReset = MEMNODE_softReset,
	.free = MEMNODE_free,
	.softFree = MEMNODE_softFree,
	.memSet = MEMNODE_memSet,
	.memCopy = MEMNODE_memCopy,
	.memConcat = MEMNODE_memConcat,
	.memMask = MEMNODE_memMask,
	.print = MEMNODE_print,
  .printAsChar = MEMNODE_printAsChar,
  .printAsString = MEMNODE_printAsString,
};

// Memory Node Definitions
MemoryNode* MEMNODE_create()
{
	MemoryNode* node = (MemoryNode*)MM->malloc(sizeof(MemoryNode));
	if (NULL == node)
	{
#ifdef VERBOSE_
		printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return NULL;
	}
	MEMNODE_initWithoutCheck(node);
	return node;
}

s16 MEMNODE_createFromRef(MemoryNode **node)
{
	if (NULL == node)
	{
		return kErrorCode_Param_NULL;
	}
	*node = MEMNODE_create();
	if (NULL == *node)
	{
#ifdef VERBOSE_
		printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
		return kErrorCode_Param_NULL;
	}
	return kErrorCode_Ok;
}

s16 MEMNODE_createLite(MemoryNode *node)
{
	if (NULL == node)
	{
		return kErrorCode_Param_NULL;
	}

  MEMNODE_initWithoutCheck(node); // No posible error to check

	return kErrorCode_Ok;
}

s16 MEMNODE_initWithoutCheck(MemoryNode *node)
{
	node->data_ = NULL;
  node->next_ = NULL;
  node->prev_ = NULL;
	node->size_ = 0;
	node->ops_ = &memory_node_ops;
	return kErrorCode_Ok;
}

void* MEMNODE_data(MemoryNode *node)
{
	if (NULL == node) {
		return NULL;
	}
	
	return node->data_;
}

u16	MEMNODE_size(MemoryNode *node)
{
	if (NULL == node) {
		return 0;
	}

	return node->size_;
}

s16 MEMNODE_setData(MemoryNode *node, void *src, u16 bytes)
{
  if (NULL == node) {
    return kErrorCode_Param_NULL;
  }

  if (NULL == src) {
    return kErrorCode_Param_NULL_Src;
  }

  if (0 == bytes) {
    return kErrorCode_Param_Zero_Size;
  }

  if (NULL != node->data_) {
    MM->free(node->data_);
  }
  node->data_ = src;
  node->size_ = bytes;

  return kErrorCode_Ok;
}

s16 MEMNODE_setNext(MemoryNode* node, MemoryNode* next)
{
  if (NULL == node)
  {
    return kErrorCode_Param_NULL;
  }

  node->next_ = next;

  return kErrorCode_Ok;
}

s16 MEMNODE_setPrev(MemoryNode *node, MemoryNode *prev)
{
  if (NULL == node)
  {
    return kErrorCode_Param_NULL;
  }

  node->prev_ = prev;

  return kErrorCode_Ok;
}

MemoryNode* MEMNODE_next(MemoryNode *node)
{
  if (NULL == node)
  {
    return kErrorCode_Param_NULL;
  }

  return node->next_;
}

MemoryNode* MEMNODE_prev(MemoryNode *node)
{
  if (NULL == node)
  {
    return kErrorCode_Param_NULL;
  }

  return node->prev_;
}

s16 MEMNODE_reset(MemoryNode *node) {
	if (NULL == node) {
		return kErrorCode_Param_NULL;
	}

	if (NULL != node->data_) {
		MM->free(node->data_);
		node->data_ = NULL;
    node->size_ = 0;
	}

	return kErrorCode_Ok;
}

s16 MEMNODE_softReset(MemoryNode* node) {
  if (NULL == node) {
    return kErrorCode_Param_NULL;
  }

  node->data_ = NULL;
  node->size_ = 0;
  node->prev_ = NULL;
  node->next_ = NULL;

  return kErrorCode_Ok;
}

s16 MEMNODE_free(MemoryNode *node) {
	if (NULL != node) {
		if (NULL != node->data_) {
			MM->free(node->data_);
		}
		MM->free(node);
	}
	return kErrorCode_Ok;
}

s16 MEMNODE_softFree(MemoryNode *node) {
	if (NULL != node) {
		MM->free(node);
	}
	return kErrorCode_Ok;
}

s16 MEMNODE_memSet(MemoryNode *node, u8 value) {
	if (NULL == node) {
		return kErrorCode_Param_NULL;
	}

	if (NULL == node->data_) {
		return kErrorCode_NULL_Data;
	}

	if (0 == node->size_) {
		return kErrorCode_Param_Zero_Size;
	}

	memset(node->data_, value, node->size_);

	return kErrorCode_Ok;
}

s16 MEMNODE_memCopy(MemoryNode *node, void *src, u16 bytes) {
	if (NULL == node) {
		return kErrorCode_Param_NULL;
	}

	if (NULL == src) {
		return kErrorCode_Param_NULL_Src;
	}

	if (0 == bytes) {
		return kErrorCode_Param_Zero_Size;
	}

	node->size_ = bytes;
	void *temp = MM->malloc(node->size_);

	if (NULL == temp) {
		return kErrorCode_Memory_Allocation;
	}

	memcpy(temp, src, bytes);

	if (node->data_ != NULL) {
		MM->free(node->data_);
	}

	node->data_ = temp;
	return kErrorCode_Ok;
}

s16 MEMNODE_memConcat(MemoryNode *node, void *src, u16 bytes) {
	if (NULL == node) {
		return kErrorCode_Param_NULL;
	}

	if (0 == node->size_ + bytes) {
		return kErrorCode_NULL_Data;
	}

	u8* temp = (u8*)MM->malloc(node->size_ + bytes);
	if (NULL == temp) {
		return kErrorCode_Memory_Allocation;
	}

	if (NULL != node->data_) {
		memcpy(temp, node->data_, node->size_);
		MM->free(node->data_);
	}

	if (NULL != src) {
		memcpy((temp + node->size_), src, bytes);
	}

	node->data_ = (void*)temp;
	node->size_ = node->size_ + bytes;
	return kErrorCode_Ok;
}

s16 MEMNODE_memMask(MemoryNode *node, u8 mask) {
	if (NULL == node) {
		return kErrorCode_Param_NULL;
	}

	if (NULL == node->data_) {
		return kErrorCode_NULL_Data;
	}

	u8 *data = (u8*)node->data_;

	for (u16 i = 0; i < node->size_; ++i) {
		*(data + i) &= mask;
	}

	return kErrorCode_Ok;
}

void MEMNODE_print(MemoryNode *node) {
  if (-1 == MEMNODE_printMemoryNodeInfo(node))
  {
    return;
  }
  printf("Data = ");
  u8* ptr_iterator = (u8*)node->ops_->data(node);
  for (u16 i = 0; i < node->ops_->size(node); ++i)
  {
    if (i % 4 == 0)
      printf(" 0x");
    printf("%X", *(ptr_iterator++));
  }
  printf("\n");
}

s16 MEMNODE_printMemoryNodeInfo(MemoryNode* node) {
  printf("Node Info:\n");
  if (NULL == node)
  {
    printf("Node = NULL \n");
    return -1;
  }
  if (NULL == node->ops_->data(node))
  {
    printf("Data = NULL\n");
    printf("Node size: %zu\n", node->ops_->size(node));
    return -1;
  }
  if (0 == node->ops_->size(node))
  {
    printf("Size = 0\n");
    return -1;
  }
  printf("Node address = %p\n", node);
  printf("Data address = %p\n", node->ops_->data(node));
  printf("Node size: %zu\n", node->ops_->size(node));
  return 1;
}

void MEMNODE_printAsChar(MemoryNode *node) {
  if (-1 == MEMNODE_printMemoryNodeInfo(node))
  {
    return;
  }
  printf("Data = ");
  u8* ptr_iterator = (u8*)node->ops_->data(node);
  for (u16 i = 0; i < node->ops_->size(node); ++i)
  {
    printf("%c", *(ptr_iterator++));
  }
  printf("\n");
}

void MEMNODE_printAsString(MemoryNode* node) {
  if (-1 == MEMNODE_printMemoryNodeInfo(node))
  {
    return;
  }
  printf("Data = ");
  printf("%s\n", (u8*)node->ops_->data(node));
}