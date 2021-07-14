//  adt_queue.c

#include <stdio.h>
#include <string.h>

#include "adt_queue.h"
#include "common_def.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

// Queue declarations
static s16 QUEUE_init(Queue *queue, u16 capacity);
static s16 QUEUE_destroy(Queue *queue);
static s16 QUEUE_resize(Queue *queue, u16 new_size);
static s16 QUEUE_reset(Queue *queue);
static u16 QUEUE_capacity(Queue *queue);
static u16 QUEUE_length(Queue *queue);
static bool QUEUE_isEmpty(Queue *queue);
static bool QUEUE_isFull(Queue *queue);
static void* QUEUE_front(Queue *queue);
static void* QUEUE_back(Queue *queue);
static s16 QUEUE_enqueue(Queue *queue, void* data, u16 bytes);
static void* QUEUE_dequeue(Queue *queue);
static s16 QUEUE_concat(Queue *queue, Queue *queue_src);
static s16 QUEUE_traverse(Queue *queue, void(*callback)(MemoryNode *mn));
static void QUEUE_print(Queue *queue);
static void QUEUE_printAsChar(Queue *queue);
static void QUEUE_printAsString(Queue *queue);

// Queue API definitions
struct queue_ops_s queue_ops =
{
  .destroy = QUEUE_destroy,
  .resize = QUEUE_resize,
  .reset = QUEUE_reset,
  .capacity = QUEUE_capacity,
  .length = QUEUE_length,
  .isEmpty = QUEUE_isEmpty,
  .isFull = QUEUE_isFull,
  .front = QUEUE_front, 
  .back = QUEUE_back,
  .enqueue = QUEUE_enqueue,
  .dequeue = QUEUE_dequeue,
  .concat = QUEUE_concat,
  .traverse = QUEUE_traverse,
  .print = QUEUE_print,
  .printAsChar = QUEUE_printAsChar,
  .printAsString = QUEUE_printAsString,
};

// Queue definitions
Queue* QUEUE_create(u16 capacity)
{
  Queue *queue = MM->malloc(sizeof(Queue));
  if (NULL == queue) {
#ifdef VERBOSE_
    printf("Error: [%s] Memory allocation error\n", __FUNCTION__);
#endif
    return NULL;
  }
  
  if (kErrorCode_Ok != QUEUE_init(queue, capacity))
  {
#ifdef VERBOSE_
    printf("Error: [%s] Error creating storage\n", __FUNCTION__);
#endif
    MM->free(queue);
    return NULL;
  }
  
  return queue;
}

s16 QUEUE_init(Queue *queue, u16 capacity)
{
  queue->storage_ = LIST_create(capacity);
  if (NULL == queue->storage_) return kErrorCode_Memory_Allocation;
  queue->ops_ = &queue_ops; 

  return kErrorCode_Ok;
}

s16 QUEUE_destroy(Queue *queue)
{
  s16 error = kErrorCode_NONE;
  if (NULL == queue) return kErrorCode_NULL;
  if (NULL != queue->storage_)
  {
    error = queue->storage_->ops_->destroy(queue->storage_);
    if (kErrorCode_Ok != error) return error;
  }

  memset(queue, 0, sizeof(Queue));
  MM->free(queue); 
  return kErrorCode_Ok;
}

s16 QUEUE_resize(Queue *queue, u16 new_size)
{
  if (NULL == queue) return kErrorCode_NULL;
  if (0 == new_size) return kErrorCode_Param_Zero_Size;
  return queue->storage_->ops_->resize(queue->storage_, new_size);
}

s16 QUEUE_reset(Queue *queue)
{
  if (NULL == queue) return kErrorCode_NULL;
  if (NULL == queue->storage_) return kErrorCode_NULL_Storage;
  return queue->storage_->ops_->reset(queue->storage_);
}

u16 QUEUE_capacity(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return 0;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return 0;
  }

  return queue->storage_->ops_->capacity(queue->storage_);
}

u16 QUEUE_length(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return 0;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return 0;
  }

  return queue->storage_->ops_->length(queue->storage_);
}

bool QUEUE_isEmpty(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return true;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return true;
  }

  return queue->storage_->ops_->isEmpty(queue->storage_);
}

bool QUEUE_isFull(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return true;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return true;
  }

  return queue->storage_->ops_->isFull(queue->storage_);
}

void* QUEUE_front(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }

  return queue->storage_->ops_->first(queue->storage_);
}

void* QUEUE_back(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }

  return queue->storage_->ops_->last(queue->storage_);
}

s16 QUEUE_enqueue(Queue *queue, void* data, u16 bytes)
{
  if (NULL == queue) return kErrorCode_NULL;
  if (NULL == queue->storage_) return kErrorCode_NULL_Storage;
  return queue->storage_->ops_->insertLast(queue->storage_, data, bytes);
}

void* QUEUE_dequeue(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }

  return queue->storage_->ops_->extractFirst(queue->storage_);
}

s16 QUEUE_concat(Queue *queue, Queue *queue_src)
{
  if (NULL == queue) return kErrorCode_NULL;
  if (NULL == queue->storage_) return kErrorCode_NULL_Storage;
  if (NULL == queue_src) return kErrorCode_Param_NULL_Src;
  return queue->storage_->ops_->concat(queue->storage_, queue_src->storage_);
}

s16 QUEUE_traverse(Queue *queue, void(*callback)(MemoryNode *mn))
{
  if (NULL == queue) return kErrorCode_NULL;
  if (NULL == queue->storage_) return kErrorCode_NULL_Storage;
  return queue->storage_->ops_->traverse(queue->storage_, callback);
}

void QUEUE_print(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }

  queue->storage_->ops_->print(queue->storage_);
}

void QUEUE_printAsChar(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }

  queue->storage_->ops_->printAsChar(queue->storage_);
}

void QUEUE_printAsString(Queue *queue)
{
  if (NULL == queue) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }

  if (NULL == queue->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }

  queue->storage_->ops_->printAsString(queue->storage_);
}
