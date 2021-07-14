//	adt_vector.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_def.h"
#include "adt_vector.h"
#include "ABGS_MemoryManager\abgs_memory_manager.h"

// Vector Declarations
static s16 VECTOR_init(Vector* vector);
static s16 VECTOR_destroy(Vector* vector);
static s16 VECTOR_reset(Vector* vector);
static s16 VECTOR_resize(Vector *vector, u16 new_size);
static u16 VECTOR_capacity(Vector *vector);
static u16 VECTOR_length(Vector *vector);
static bool VECTOR_isEmpty(Vector *vector);
static bool VECTOR_isFull(Vector *vector);
static void* VECTOR_first(Vector *vector);
static void* VECTOR_last(Vector *vector);
static void* VECTOR_at(Vector *vector, u16 position);
static s16 VECTOR_insertFirst(Vector* vector, void* data, u16 bytes);
static s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes);
static s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position);
static void* VECTOR_extractFirst(Vector* vector);
static void* VECTOR_extractLast(Vector* vector);
static void* VECTOR_extractAt(Vector* vector, u16 position);
static s16 VECTOR_concat(Vector* vector, Vector* vector_src);
static s16 VECTOR_traverse(Vector* vector, void(*callback) (MemoryNode *mn));
static void VECTOR_print(Vector* vector);
static void VECTOR_printAsChar(Vector* vector);
static void VECTOR_printAsString(Vector* vector);

// Vector's API Definitions
struct vector_ops_s vector_ops =
{
  .reset = VECTOR_reset,
  .destroy = VECTOR_destroy,
  .resize = VECTOR_resize,
  .capacity = VECTOR_capacity,
  .length = VECTOR_length,
  .isEmpty = VECTOR_isEmpty,
  .isFull = VECTOR_isFull,
  .first = VECTOR_first,
  .last = VECTOR_last,
  .at = VECTOR_at,
  .insertFirst = VECTOR_insertFirst,
  .insertLast = VECTOR_insertLast,
  .insertAt = VECTOR_insertAt,
  .extractFirst = VECTOR_extractFirst,
  .extractLast = VECTOR_extractLast,
  .extractAt = VECTOR_extractAt,
  .concat = VECTOR_concat,
  .traverse = VECTOR_traverse,
  .print = VECTOR_print,
  .printAsChar = VECTOR_printAsChar,
  .printAsString = VECTOR_printAsString
};

// Vector Definitions
Vector* VECTOR_create(u16 capacity)
{
  if (0 == capacity)
  {
#ifdef VERBOSE_
    printf("Error: [%s] You can not create a vector with 0 capacity\n", __FUNCTION__);
#endif
    return NULL;
  }
	Vector *vec = (Vector*)MM->malloc(sizeof(Vector));
	if (NULL == vec)
	{
#ifdef VERBOSE_
    printf("Error: [%s] Memory allocation failed\n", __FUNCTION__);
#endif
		return NULL;
	}

	vec->capacity_ = capacity;
	if (kErrorCode_Ok != VECTOR_init(vec))
	{
#ifdef VERBOSE_
    printf("Error: [%s] Error initializing\n", __FUNCTION__);
#endif
		MM->free(vec);
		vec = NULL;
	}

	return vec;
}

s16 VECTOR_init(Vector *vector)
{
  u16 i = 0;
  vector->head_ = 0;
  vector->tail_ = 0;
  vector->ops_ = &vector_ops;

  vector->storage_ = (MemoryNode*)MM->malloc(vector->capacity_ * sizeof(MemoryNode));
  if (NULL == vector->storage_)
  {
	  return kErrorCode_Memory_Allocation;
  }

  for (i = 0; i < vector->capacity_; i++)
  {
	  if (kErrorCode_Ok != MEMNODE_createLite(vector->storage_ + i))
	  {
		  MM->free(vector->storage_);
		  return kErrorCode_Param_NULL;
	  }
  }

  return kErrorCode_Ok;
}

s16 VECTOR_destroy(Vector *vector)
{
  u16 i = 0;
	if (NULL == vector)
	{
		return kErrorCode_Param_NULL;
	}
	
  if (NULL != vector->storage_)
  {
    VECTOR_traverse(vector, vector->storage_->ops_->reset);
    MM->free(vector->storage_);
  }
  MM->free(vector);

  return kErrorCode_Ok;
}

s16 VECTOR_reset(Vector *vector)
{
  u16 i = 0;
  s16 error;

	if (NULL == vector)
	{
		return kErrorCode_Param_NULL;
	}

	if (NULL == vector->storage_)
	{
		return kErrorCode_NULL_Storage;
	}

  for (i=0; i < vector->capacity_; i++) {
    error = (vector->storage_)->ops_->reset(vector->storage_ + i);
    if (kErrorCode_Ok != error) {
      return error;
    }
  }
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

s16 VECTOR_resize(Vector *vector, u16 new_size)
{
  u16 i = 0;
  s16 error;

  if (NULL == vector)
  {
    return kErrorCode_Param_NULL;
  }

  if (0 == new_size)
  {
    return kErrorCode_Param_Zero_Size;
  }

  if (new_size == vector->capacity_) {
    // Do nothing
    return kErrorCode_Ok;
  }

  if (NULL == vector->storage_)
  {
    return kErrorCode_NULL_Storage;
  }

  // Create new buffer and copy data from old buffer
  MemoryNode* temp_ptr = (MemoryNode*)MM->malloc(new_size * sizeof(MemoryNode));
  if (NULL == temp_ptr)
  {
    return kErrorCode_Memory_Allocation;
  }

  u16 bytes_to_cpy;
  if (new_size < vector->capacity_) bytes_to_cpy = new_size * sizeof(MemoryNode);
  else bytes_to_cpy = vector->capacity_ * sizeof(MemoryNode);
  
  if (0 != memcpy_s(temp_ptr, new_size * sizeof(MemoryNode), vector->storage_, bytes_to_cpy))
  {
    MM->free(temp_ptr);
    return kErrorCode_Memory;
  }

  // Initialize remaining memory
  if (new_size > vector->capacity_)
  {
    for (i = vector->capacity_; i < new_size; i++)
    {
      error = MEMNODE_createLite(temp_ptr + i);
      if (kErrorCode_Ok != error)
      {
        MM->free(temp_ptr);
        return error;
      }
    }
  } else { // Free data from deleted nodes
    for (i = new_size; i < vector->capacity_; i++) {
      vector->storage_->ops_->reset(vector->storage_ + i);
    }
  }

  // Erase old data
  memset(vector->storage_, 0, vector->capacity_);

  // Free old storage
  MM->free(vector->storage_);

  // Assign new values
  vector->storage_ = temp_ptr;
  vector->capacity_ = new_size;
  if (vector->tail_ > vector->capacity_)
  {
    vector->tail_ = vector->capacity_;
  }

  return kErrorCode_Ok;
}

u16 VECTOR_capacity(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return 0;
  }

  return vector->capacity_;
}

u16 VECTOR_length(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return 0xFFFF; // Largest u16 number, -1 if signed. This is an error return
  }
  return vector->tail_;
}

bool VECTOR_isEmpty(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return true;
  }

  return vector->head_ == vector->tail_;
}

bool VECTOR_isFull(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return true;
  }

  return vector->capacity_ == vector->tail_;
}

void* VECTOR_first(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == vector->storage_)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_);
}

void* VECTOR_last(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == vector->storage_)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (true == VECTOR_isEmpty(vector))
  {
#ifdef VERBOSE_
    printf("Error: [%s] Is empty\n", __FUNCTION__);
#endif
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_ + (vector->tail_ - 1));
}

void* VECTOR_at(Vector* vector, u16 position)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL Parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == vector->storage_)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (position >= vector->ops_->capacity(vector))
  {
#ifdef VERBOSE_
    printf("Error: [%s] Position out of range\n", __FUNCTION__);
#endif
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_ + position);
}

s16 VECTOR_insertFirst(Vector* vector, void* data, u16 bytes)
{
  if (NULL == vector) return kErrorCode_Param_NULL;

  return VECTOR_insertAt(vector, data, bytes, vector->head_);
}

s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes)
{
  if (NULL == vector)
  {
    return kErrorCode_Param_NULL;
  }

  if (NULL == vector->storage_)
  {
    return kErrorCode_NULL_Storage;
  }

  if (VECTOR_isFull(vector))
  {
    return kErrorCode_Full;
  }

  vector->storage_->ops_->setData(vector->storage_ + vector->tail_, data, bytes);
  vector->tail_++;

  return kErrorCode_Ok;
}

s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position)
{
  u16 i = 0;
  s16 error = kErrorCode_NONE;

  if (NULL == data)
  {
    return kErrorCode_NULL_Data;
  }

  if (NULL == vector)
  {
    return kErrorCode_Param_NULL;
  }

  if (position > vector->capacity_)
  {
    return kErrorCode_Not_Enough_Capacity;
  }

  if (position > vector->tail_)
  {
    return VECTOR_insertLast(vector, data, bytes);
  }

  if (NULL == vector->storage_)
  {
    return kErrorCode_NULL_Storage;
  }

  if (VECTOR_isFull(vector))
  {
    return kErrorCode_Full;
  }

  for (i = vector->tail_; i > position; i--)
  {
    error = vector->storage_->ops_->setData(
        vector->storage_ + i, 
        vector->storage_->ops_->data(vector->storage_ + (i - 1)),
        vector->storage_->ops_->size(vector->storage_ + (i - 1)));
#ifdef VERBOSE_
    if (kErrorCode_Ok != error)
      printf("Error: [%s] error displacing memory\n", __FUNCTION__);
#endif
    error = vector->storage_->ops_->softReset(vector->storage_ + i - 1);
#ifdef VERBOSE_
    if (kErrorCode_Ok != error)
      printf("Error: [%s] error displacing memory\n", __FUNCTION__);
#endif
  }

  error = vector->storage_->ops_->setData(vector->storage_ + position, data, bytes);
  if (kErrorCode_Ok != error) return error;

  vector->tail_++;

  return kErrorCode_Ok;
}

void* VECTOR_extractFirst(Vector* vector)
{
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  return VECTOR_extractAt(vector, 0);
}

void* VECTOR_extractLast(Vector* vector)
{
  void* data;
  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (VECTOR_isEmpty(vector))
  {
#ifdef VERBOSE_
    printf("Error: [%s] Is empty\n", __FUNCTION__);
#endif
    return NULL;
  }

  data = vector->storage_->ops_->data(vector->storage_ + vector->tail_ - 1);
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_ - 1);

  vector->tail_--;
  return data;
}

void* VECTOR_extractAt(Vector* vector, u16 position)
{
  void* data;
  u16 i = 0;

  if (NULL == vector)
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (VECTOR_isEmpty(vector))
  {
#ifdef VERBOSE_
    printf("Error: [%s] Is empty\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (position > vector->capacity_)
  {
#ifdef VERBOSE_
    printf("Warning: [%s] Out of range, extracting last\n", __FUNCTION__);
#endif
    return VECTOR_extractLast(vector);
  }

  data = vector->storage_->ops_->data(vector->storage_ + position);
  vector->storage_->ops_->softReset(vector->storage_ + position);
  for (i = position; i < vector->tail_-1; i++)
  {
    vector->storage_->ops_->setData(vector->storage_ + i, vector->storage_->ops_->data(vector->storage_ + i + 1), vector->storage_->ops_->size(vector->storage_ + i + 1));
    vector->storage_->ops_->softReset(vector->storage_ + i + 1);
  }

  vector->tail_--;
  return data;
}

s16 VECTOR_concat(Vector* vector, Vector* vector_src)
{
  s16 i = 0;

  if (NULL == vector_src)
  {
    return kErrorCode_Param_NULL;
  }

  if (NULL == vector)
  {
    return kErrorCode_Param_NULL;
  }

  if (NULL == vector->storage_)
  {
    return kErrorCode_NULL_Storage;
  }

  if (NULL == vector_src->storage_)
  {
    return kErrorCode_NULL_Storage;
  }

  s16 error = VECTOR_resize(vector, vector->capacity_ + vector_src->capacity_);
  if (kErrorCode_Ok != error)
  {
    return error;
  }

  for (i = 0; i < VECTOR_length(vector_src); i++)
  {
    error = vector->storage_->ops_->memCopy(vector->storage_ + vector->tail_ + i, vector->storage_->ops_->data(vector_src->storage_+i), vector_src->storage_->ops_->size(vector_src->storage_ + i));
    if (kErrorCode_Ok != error)
      return error;
  }

  vector->tail_ = vector->tail_ + vector_src->tail_;
  
  return kErrorCode_Ok;
}

s16 VECTOR_traverse(Vector* vector, void(*callback) (MemoryNode *mn))
{
  if (NULL == vector)
  {
    return kErrorCode_Param_NULL;
  }

  u16 i;
  for (i = vector->head_; i < vector->tail_; i++)
  {
    callback(vector->storage_ + i);
  }
  return kErrorCode_Ok;
}

s16 VECTOR_printVectorInfo(Vector* vector) {
  printf("Info:\n");
  if (NULL == vector)
  {
    printf("Is NULL \n");
    return -1;
  }
  if (NULL == vector->storage_)
  {
    printf("Storage = NULL\n");
    return -1;
  }
  if (0 == vector->tail_)
  {
    printf("Empty storage\n");
    return -1;
  }
  printf("Address = %p\n", vector);
  printf("Storage address = %p\n", vector->storage_);
  printf("Storage capacity: %zu\n", VECTOR_capacity(vector));
  printf("Storage used: %zu\n", VECTOR_length(vector));
  return 1;
}

void VECTOR_print(Vector* vector)
{
  if (-1 == VECTOR_printVectorInfo(vector))
  {
    return;
  }

  if (NULL == vector->storage_->ops_) return;

  printf("Storage:\n");
  VECTOR_traverse(vector, vector->storage_->ops_->print);
}

void VECTOR_printAsChar(Vector* vector)
{
  if (-1 == VECTOR_printVectorInfo(vector))
  {
    return;
  }
  printf("Storage:\n");
  VECTOR_traverse(vector, vector->storage_->ops_->printAsChar);
}

void VECTOR_printAsString(Vector* vector)
{
  if (-1 == VECTOR_printVectorInfo(vector))
  {
    return;
  }
  printf("Storage:\n");
  VECTOR_traverse(vector, vector->storage_->ops_->printAsString);
}