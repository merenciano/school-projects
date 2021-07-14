//  adt_stack.c

#include <stdio.h>
#include <string.h>

#include "common_def.h"
#include "adt_stack.h"
#include "ABGS_MemoryManager\abgs_memory_manager.h"

// Stack declarations
static s16 STACK_init(Stack *stack, u16 capacity);
static s16 STACK_destroy(Stack *stack);
static s16 STACK_reset(Stack *stack);
static s16 STACK_resize(Stack *stack, u16 new_size);
static u16 STACK_capacity(Stack *stack);
static u16 STACK_length(Stack *stack);
static bool STACK_isEmpty(Stack *stack);
static bool STACK_isFull(Stack *stack);
static void* STACK_top(Stack *stack);
static s16 STACK_push(Stack *stack, void *data, u16 bytes);
static void* STACK_pop(Stack *stack);
static s16 STACK_concat(Stack *stack, Stack *stack_src);
static s16 STACK_traverse(Stack *stack, void(*callback) (MemoryNode *mn));
static void STACK_print(Stack *stack);
static void STACK_printAsChar(Stack *stack);
static void STACK_printAsString(Stack *stack);

// Stack's API definitions
struct stack_ops_s stack_ops =
{
  .destroy = STACK_destroy,
  .reset = STACK_reset,
  .resize = STACK_resize,
  .capacity = STACK_capacity,
  .length = STACK_length,
  .isEmpty = STACK_isEmpty,
  .isFull = STACK_isFull,
  .top = STACK_top,
  .push = STACK_push,
  .pop = STACK_pop,
  .concat = STACK_concat,
  .traverse = STACK_traverse,
  .print = STACK_print,
  .printAsChar = STACK_printAsChar,
  .printAsString = STACK_printAsString,
};

// Stack definitions
Stack* STACK_create(u16 capacity)
{
  Stack *stack = MM->malloc(sizeof(Stack));
  if (NULL == stack)
  {
#ifdef VERBOSE_
    printf("Error: [%s] Memory allocation failed\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (kErrorCode_Ok != STACK_init(stack, capacity))
  {
#ifdef VERBOSE_
    printf("Error: [%s] Storage allocation failed\n", __FUNCTION__);
#endif
    MM->free(stack);
    return NULL;
  }
  
  return stack;
}

s16 STACK_init(Stack *stack, u16 capacity)
{
  stack->storage_ = VECTOR_create(capacity);
  if (NULL == stack->storage_)
  {
    return kErrorCode_Memory_Allocation;
  }
  stack->ops_ = &stack_ops;

  return kErrorCode_Ok;
}

s16 STACK_destroy(Stack *stack)
{
  s16 error = kErrorCode_NONE;
  if (NULL == stack) return kErrorCode_NULL;
  if (NULL != stack->storage_)
  {
    error = stack->storage_->ops_->destroy(stack->storage_);
    if (kErrorCode_Ok != error) return error;
  }

  memset(stack, 0, sizeof(Stack));
  MM->free(stack);
  return kErrorCode_Ok;
}

s16 STACK_reset(Stack *stack)
{
  if (NULL == stack) return kErrorCode_NULL;
  if (NULL == stack->storage_) return kErrorCode_NULL_Storage;
  return stack->storage_->ops_->reset(stack->storage_);
}

s16 STACK_resize(Stack *stack, u16 new_size)
{
  if (NULL == stack) return kErrorCode_NULL;
  if (NULL == stack->storage_) return kErrorCode_NULL_Storage;
  return stack->storage_->ops_->resize(stack->storage_, new_size);
}

u16 STACK_capacity(Stack *stack)
{
  if (NULL == stack) return 0; // Error
  if (NULL == stack->storage_) return 0; // Error 
  return stack->storage_->ops_->capacity(stack->storage_);
}

u16 STACK_length(Stack *stack)
{
  if (NULL == stack) return 0; // Error
  if (NULL == stack->storage_) return 0; // Error
  return stack->storage_->ops_->length(stack->storage_);
}

bool STACK_isEmpty(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return true; 
  }

  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return true; 
  }
  return stack->storage_->ops_->isEmpty(stack->storage_);
}

bool STACK_isFull(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return true;
  }

  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return true;
  }

  return stack->storage_->ops_->isFull(stack->storage_);
}

void* STACK_top(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }
  return stack->storage_->ops_->last(stack->storage_);
}

s16 STACK_push(Stack *stack, void *data, u16 bytes)
{
  if (NULL == stack) return kErrorCode_NULL;
  if (NULL == stack->storage_) return kErrorCode_NULL_Storage;
  if (NULL == data) return kErrorCode_NULL_Data;
  return stack->storage_->ops_->insertLast(stack->storage_, data, bytes);
}

void* STACK_pop(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return NULL;
  }
  return stack->storage_->ops_->extractLast(stack->storage_);
}

s16 STACK_concat(Stack *stack, Stack *stack_src)
{
  if (NULL == stack) return kErrorCode_NULL;
  if (NULL == stack_src) return kErrorCode_NULL;
  if (NULL == stack->storage_) return kErrorCode_NULL_Storage;
  
  return stack->storage_->ops_->concat(stack->storage_, stack_src->storage_);
}

s16 STACK_traverse(Stack *stack, void(*callback) (MemoryNode *mn))
{
  if (NULL == stack) return kErrorCode_NULL;
  if (NULL == stack->storage_) return kErrorCode_NULL_Storage;
  return stack->storage_->ops_->traverse(stack->storage_, callback); 
}

void STACK_print(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return;
  }
  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }
  stack->storage_->ops_->print(stack->storage_);
}

void STACK_printAsChar(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return;
  }

  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }

  stack->storage_->ops_->printAsChar(stack->storage_);
}

void STACK_printAsString(Stack *stack)
{
  if (NULL == stack) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL parameter\n", __FUNCTION__);
#endif
    return;
  }

  if (NULL == stack->storage_) 
  {
#ifdef VERBOSE_
    printf("Error: [%s] NULL storage\n", __FUNCTION__);
#endif
    return;
  }

  stack->storage_->ops_->printAsString(stack->storage_);
}
