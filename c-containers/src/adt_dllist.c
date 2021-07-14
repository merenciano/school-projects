#include "adt_dllist.h"

#include <stdio.h>
#include <string.h>

#include "common_def.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"
#include "adt_memory_node.h"

#define MNO mn_ops->ops_
MemoryNode *mn_ops = NULL;  // Only for operations

// List declarations
static s16 DLLIST_prepareNodeToInsert(DLList* list, void* data, u16 bytes, MemoryNode **out);
static void DLLIST_init(DLList* list);
static s16 DLLIST_reset(DLList* list);
static s16 DLLIST_destroy(DLList* list);
static s16 DLLIST_resize(DLList *list, u16 new_size);
static u16 DLLIST_capacity(DLList* list);
static u16 DLLIST_length(DLList* list);
static bool DLLIST_isEmpty(DLList* list);
static bool DLLIST_isFull(DLList* list);
static void* DLLIST_first(DLList* list);
static void* DLLIST_last(DLList* list);
static void* DLLIST_at(DLList* list, u16 position);
static s16 DLLIST_insertFirst(DLList* list, void* data, u16 bytes);
static s16 DLLIST_insertLast(DLList* list, void* data, u16 bytes);
static s16 DLLIST_insertAt(DLList* list, void* data, u16 bytes, u16 position);
static void* DLLIST_extractFirst(DLList* list);
static void* DLLIST_extractLast(DLList* list);
static void* DLLIST_extractAt(DLList* list, u16 position);
static s16 DLLIST_copy(DLList **list, DLList *list_src);
static s16 DLLIST_concat(DLList* list, DLList* list_src);
static s16 DLLIST_traverse(DLList* list, void(*callback)(MemoryNode* mn));
static void DLLIST_print(DLList *list);
static void DLLIST_printAsChar(DLList *list);
static void DLLIST_printAsString(DLList *list);
static void DLLIST_printAsString(DLList *list);

// List API definitions
struct dllist_ops_s dllist_ops = 
{
  .reset = DLLIST_reset,
  .destroy = DLLIST_destroy,
  .resize = DLLIST_resize,
  .capacity = DLLIST_capacity,
  .length = DLLIST_length,
  .isEmpty = DLLIST_isEmpty,
  .isFull = DLLIST_isFull,
  .first = DLLIST_first,
  .last = DLLIST_last,
  .at = DLLIST_at,
  .insertFirst = DLLIST_insertFirst,
  .insertLast = DLLIST_insertLast,
  .insertAt = DLLIST_insertAt,
  .extractFirst = DLLIST_extractFirst,
  .extractLast = DLLIST_extractLast,
  .extractAt = DLLIST_extractAt,
  .traverse = DLLIST_traverse,
  .concat = DLLIST_concat,
  .print = DLLIST_print,
  .printAsChar = DLLIST_printAsChar,
  .printAsString = DLLIST_printAsString,
};

// List definitions
DLList* DLLIST_create(u16 capacity)
{
  DLList* list = NULL;

  if (0 == capacity)
  {
#ifdef VERBOSE_
    printf("Error: [%s] You are trying to create a list with 0 capacity\n", __FUNCTION__);
#endif
    return NULL;
  }

  list = (DLList*)MM->malloc(sizeof(DLList));
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] Memory allocation error\n", __FUNCTION__);
#endif
    return NULL;
  }

  list->capacity_ = capacity;
  DLLIST_init(list);

  return list;
}

void DLLIST_init(DLList *list)
{
  list->length_ = 0;
  list->first_ = NULL;
  list->last_ = NULL;
  list->ops_ = &dllist_ops;
}

s16 DLLIST_destroy(DLList* list)
{
  s16 error;

  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  error = DLLIST_reset(list);
  if (kErrorCode_Ok != error)
  {
    return error;
  }

  memset(list, 0, sizeof(DLList));
  MM->free(list);

  return kErrorCode_Ok;
}

s16 DLLIST_resize(DLList *list, u16 new_size)
{
  u16 i = 0;
  MemoryNode *curr;
  MemoryNode *prev;

  if (NULL == list) return kErrorCode_Param_NULL;
  if (0 == new_size) return kErrorCode_Param_Zero_Size;

  if (new_size >= list->length_)
  {
    list->capacity_ = new_size;
    return kErrorCode_Ok;
  }

  i = list->length_ - 1;
  curr = list->last_;
  prev = MNO->prev(curr);
  while (i >= new_size)
  {
    MNO->free(curr);

    curr = prev;
    prev = MNO->prev(curr);
    i--;
  }

  list->capacity_ = new_size;
  list->length_ = new_size;
  list->last_ = curr;
  MNO->setNext(list->last_, NULL);
  return kErrorCode_Ok;
}

s16 DLLIST_reset(DLList* list)
{
  MemoryNode* i = NULL;
  MemoryNode* next = NULL;

  if (NULL == list)         return kErrorCode_Param_NULL;
  if (NULL == list->first_) return kErrorCode_Ok;

  s16 error = DLLIST_traverse(list, MNO->free);
  if (kErrorCode_Ok != error)
  {
    return error;
  }

  list->length_ = 0;
  list->first_ = NULL;
  list->last_ = NULL;

  return kErrorCode_Ok;
}

u16 DLLIST_capacity(DLList* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL\n", __FUNCTION__);
#endif
    return 0;
  }

  return list->capacity_;
}

u16 DLLIST_length(DLList* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL, the value returned is not valid\n", __FUNCTION__);
#endif
    return 0xFFFF; // Largest u16 number, -1 if signed. This is an error return
  }
  return list->length_;
}

bool DLLIST_isEmpty(DLList* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL, the value returned is not valid\n", __FUNCTION__);
#endif
    return true;
  }

  return !list->length_;
}

bool DLLIST_isFull(DLList* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL, the value returned is not valid\n", __FUNCTION__);
#endif
    return true;
  }

  return list->length_ == list->capacity_;
}

void* DLLIST_first(DLList* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL\n", __FUNCTION__);
#endif
    return NULL;
  }

  return MNO->data(list->first_);
}

void* DLLIST_last(DLList* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL\n", __FUNCTION__);
#endif
    return NULL;
  }

  return MNO->data(list->last_);
}

void* DLLIST_at(DLList* list, u16 position)
{
  s16 i;
  MemoryNode* at;

  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] DLList is NULL\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (0 == position)
  {
    return DLLIST_first(list);
  }

  if (list->length_ == position + 1)
  {
    return DLLIST_last(list);
  }

  if (list->length_ <= position)
  {
#ifdef VERBOSE_
    printf("Error [%s] Position out of the list", __FUNCTION__);
#endif
    return NULL;
  }

  at = list->first_;
  for (i = 0; i < position; i++)
  {
    at = MNO->next(at);
  }
  
  return MNO->data(at);
}

s16 DLLIST_prepareNodeToInsert(DLList *list, void *data, u16 bytes, MemoryNode **out)
{
  s16 error = kErrorCode_NONE;

  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  if (DLLIST_isFull(list))
  {
    return kErrorCode_Not_Enough_Capacity;
  }

  // NULL insert is not allowed
  if (NULL == data)
  {
#ifdef VERBOSE_
    printf("Error: [%s] You should not insert NULL data\n", __FUNCTION__);
#endif
    return kErrorCode_Memory_NULL;
  }

  *out = MEMNODE_create();
  error = MNO->setData(*out, data, bytes);
  if (error != kErrorCode_Ok) // This error handles MEMNODE_create() because setData checks for null mn
  {
    MNO->free(*out);
    return error;
  }

  return kErrorCode_Ok;
}

s16 DLLIST_insertFirst(DLList* list, void* data, u16 bytes)
{
  MemoryNode* mn;
  s16 error = kErrorCode_NONE;

  error = DLLIST_prepareNodeToInsert(list, data, bytes, &mn);
  if (kErrorCode_Ok != error) return error;

  error = MNO->setNext(mn, list->first_);
  if (kErrorCode_Ok != error)
  {
    MNO->free(mn);
    return error;
  }

  error = MNO->setPrev(mn, NULL);
  if (kErrorCode_Ok != error)
  {
    MNO->free(mn);
    return error;
  }

  if (NULL != list->first_)
  {
    error = MNO->setPrev(list->first_, mn);
    if (kErrorCode_Ok != error)
    {
      MNO->free(mn);
      return error;
    }
  }
  
  list->first_ = mn;
  if (NULL == list->last_) list->last_ = mn;
  list->length_++;

  return kErrorCode_Ok;
}

s16 DLLIST_insertLast(DLList *list, void *data, u16 bytes)
{
  s16 error;
  MemoryNode* mn = NULL;

  error = DLLIST_prepareNodeToInsert(list, data, bytes, &mn);
  if (kErrorCode_Ok != error) return error;

  if (DLLIST_isEmpty(list))
  {
    list->first_ = mn;
  }
  else
  {
    error = MNO->setNext(list->last_, mn);
    if (error != kErrorCode_Ok)
    {
      MNO->free(mn);
      return error;
    }
    error = MNO->setPrev(mn, list->last_);
    if (error != kErrorCode_Ok)
    {
      MNO->setNext(list->last_, NULL);
      MNO->free(mn);
      return error;
    }
  }
  list->last_ = mn;
  list->length_++;

  return kErrorCode_Ok;
}

s16 DLLIST_insertAt(DLList *list, void *data, u16 bytes, u16 position)
{
  s16 i;
  s16 error;
  MemoryNode *mn = kErrorCode_NONE;
  MemoryNode *at = NULL;

  if (NULL == list) return kErrorCode_Param_NULL;

  if (0 == position) return DLLIST_insertFirst(list, data, bytes);
  if (list->length_ == position) return DLLIST_insertLast(list, data, bytes);

  if (position > list->length_)
  {
#ifdef VERBOSE_
    printf("Warning: [%s] Position out of the range, inserting last\n", __FUNCTION__);
#endif
    return DLLIST_insertLast(list, data, bytes);
  }

  error = DLLIST_prepareNodeToInsert(list, data, bytes, &mn);
  if (kErrorCode_Ok != error) return error;

  if (position < list->length_ / 2)
  {
    at = list->first_;
    for (i = 0; i < position - 1; i++) // position-1 will never be negative at this point
    {
      at = MNO->next(at);
    }
    
    error = MNO->setPrev(MNO->next(at), mn);
    if (kErrorCode_Ok != error) return error;
    
    error = MNO->setNext(mn, MNO->next(at));
    if (kErrorCode_Ok != error) return error;

    error = MNO->setPrev(mn, at);
    if (kErrorCode_Ok != error) return error;  

    error = MNO->setNext(at, mn);
    if (kErrorCode_Ok != error) return error;
  }
  else
  {
    at = list->last_;
    for (i = list->length_ - 1; i > position; i--)
    {
      at = MNO->prev(at);
    }
        
    error = MNO->setPrev(mn, MNO->prev(at));
    if (kErrorCode_Ok != error) return error;  

    error = MNO->setNext(MNO->prev(at), mn);
    if (kErrorCode_Ok != error) return error;
        
    error = MNO->setPrev(at, mn);
    if (kErrorCode_Ok != error) return error;
    
    error = MNO->setNext(mn, at);
    if (kErrorCode_Ok != error) return error;
  }

  list->length_++;
  return kErrorCode_Ok;
}

void* DLLIST_extractFirst(DLList *list)
{
  MemoryNode* mn = NULL;
  void* data = NULL;

  if (NULL == list)
  {
    return NULL;
  }

  if (DLLIST_isEmpty(list))
  {
    return NULL;
  }

  mn = list->first_;
  list->first_ = MNO->next(list->first_);
  MNO->setPrev(list->first_, NULL);
  data = MNO->data(mn);
  MNO->softFree(mn);
  list->length_--;
  if (list->length_ == 0)
  {
    list->first_ = NULL;
    list->last_ = NULL;
  }
  return data;
}

void* DLLIST_extractLast(DLList *list)
{
  MemoryNode* mn;
  void* data;
  s16 error;

  if (NULL == list)
  {
    return NULL;
  }

  if (DLLIST_isEmpty(list))
  {
    return NULL;
  }

  if (1 == list->length_)
  {
    return DLLIST_extractFirst(list);
  }
  
  mn = list->last_;
  list->last_ = MNO->prev(mn);
  error = MNO->setNext(list->last_, NULL);
  if (kErrorCode_Ok != error) return NULL;
  data = MNO->data(mn);
  error = MNO->softFree(mn);
  if (kErrorCode_Ok != error) return NULL;
  
  list->length_--;
  if (list->length_ == 0)
  {
    list->first_ = NULL;
    list->last_ = NULL;
  }
  return data;
}

void* DLLIST_extractAt(DLList *list, u16 position)
{
  s16 i;
  MemoryNode* mn;
  void* data;
  s16 error;

  if (NULL == list)
  {
    return NULL;
  }

  if (DLLIST_isEmpty(list))
  {
    return NULL;
  }

  if (position >= list->length_)
  {
#ifdef VERBOSE_
    printf("Error: [%s] Position out of the list\n", __FUNCTION__);
#endif
    return NULL;
  }

  if (position == list->length_ -1)
  {
    return DLLIST_extractLast(list);
  }

  if (0 == position)
  {
    return DLLIST_extractFirst(list);
  }

  if (position < list->length_ / 2)
  {
    mn = list->first_;
    for (i = 0; i < position; i++)
    {
      mn = MNO->next(mn);
    }
  }
  else
  {
    mn = list->last_;
    for (i = list->length_ - 1; i > position; i--)
    {
      mn = MNO->prev(mn);
    }
  }  

  data = MNO->data(mn);
  error = MNO->setNext(MNO->prev(mn), MNO->next(mn));
  if (kErrorCode_Ok != error) return NULL;
  error = MNO->setPrev(MNO->next(mn), MNO->prev(mn));
  if (kErrorCode_Ok != error) return NULL;
  error = MNO->softFree(mn);  
  if (kErrorCode_Ok != error) return NULL;
  list->length_--;
  if (list->length_ == 0)
  {
    list->first_ = NULL;
    list->last_ = NULL;
  }
  return data;
}

s16 DLLIST_copy(DLList **list, DLList *list_src)
{
  DLList* new_list = NULL;
  MemoryNode* i = NULL;
  MemoryNode* j = NULL;
  s16 error;

  if (NULL == *list)
  {
    return kErrorCode_Param_NULL;
  }

  if (NULL == list_src)
  {
    return kErrorCode_Param_NULL;
  }

  if (DLLIST_isEmpty(list_src))
  {
    return kErrorCode_Ok;
  }

  new_list = DLLIST_create(DLLIST_capacity(list_src));
  if (NULL == new_list)
  {
    return kErrorCode_Memory_Allocation;
  }
  new_list->length_ = DLLIST_length(list_src);

  j = list_src->first_;

  new_list->first_ = MEMNODE_create();
  if (NULL == new_list->first_)
  {
    return kErrorCode_Memory_Allocation;
  }
  error = MNO->memCopy(new_list->first_, 
                       MNO->data(j),
                       MNO->size(j));
  if (kErrorCode_Ok != error)
  {
    DLLIST_destroy(new_list);
    return error;
  }

  if (1 == new_list->length_)
  {
    new_list->last_ = new_list->first_;
    DLLIST_destroy(*list);
    *list = new_list;
    return kErrorCode_Ok;
  }

  j = MNO->next(j);
  i = new_list->first_;
  while (NULL != j)
  {
    i->next_ = MEMNODE_create();
    if (NULL == i->next_)
    {
      DLLIST_destroy(new_list);
      return kErrorCode_Memory_Allocation;
    }

    error = MNO->memCopy(i->next_, MNO->data(j), MNO->size(j));
    if (kErrorCode_Ok != error)
    {
      DLLIST_destroy(new_list);
      return error;
    }
    MNO->setPrev(i->next_,i);
    i = MNO->next(i);
    j = MNO->next(j);
  }
  new_list->last_ = i;
  DLLIST_destroy(*list);
  *list = new_list;
  return kErrorCode_Ok;
}

s16 DLLIST_concat(DLList *list, DLList *list_src)
{
  DLList* src_list_copy = NULL;
  s16 error = kErrorCode_NONE;
  u16 i = 0;

  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  if (NULL == list_src)
  {
    return kErrorCode_Param_NULL;
  }

  if (DLLIST_isEmpty(list_src))
  {
    return kErrorCode_Ok;
  }

  src_list_copy = DLLIST_create(DLLIST_capacity(list_src));
  if (NULL == src_list_copy) return kErrorCode_Memory_Allocation;
  error = DLLIST_copy(&src_list_copy, list_src);
  if (kErrorCode_Ok != error)
  {
    DLLIST_destroy(src_list_copy);
    return error;
  }

  if (DLLIST_isEmpty(list))
  {
    list->first_ = src_list_copy->first_;
  }
  else
  {
    error = MNO->setNext(list->last_, src_list_copy->first_);
    if (kErrorCode_Ok != error) 
    {
      DLLIST_destroy(src_list_copy);
      return error;
    }
    error = MNO->setPrev(src_list_copy->first_, list->last_);
    if (kErrorCode_Ok != error)
    {
      DLLIST_destroy(src_list_copy);
      return error;
    }
  }
  list->capacity_ += DLLIST_capacity(list_src);
  list->length_ += DLLIST_length(src_list_copy);
  list->last_ = src_list_copy->last_;

  MM->free(src_list_copy); // SoftDestroy

  return kErrorCode_Ok;
}

s16 DLLIST_traverse(DLList *list, void(*callback)(MemoryNode *mn))
{
  MemoryNode *current;
  MemoryNode *next;

  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  if (NULL == list->first_)
  {
    return kErrorCode_Ok;
  }

  if (NULL == callback)
  {
    return kErrorCode_Param_NULL;
  }

  current = list->first_;
  while (current != NULL)
  {
    next = MNO->next(current);
    callback(current);
    current = next;
  }

  return kErrorCode_Ok;
}

s16 DLLIST_printListInfo(DLList *list) {
  printf("List Info:\n");
  if (NULL == list)
  {
    printf("List = NULL \n");
    return -1;
  }

  if (DLLIST_isEmpty(list))
  {
    printf("Empty List\n");
    return -1;
  }

  printf("List address = %p\n", list);
  printf("List capacity: %zu\n", DLLIST_capacity(list));
  printf("List length: %zu\n", DLLIST_length(list));
  return 1;
}

void DLLIST_print(DLList *list)
{
  if (-1 == DLLIST_printListInfo(list))
  {
    return;
  }
  printf("Elements:\n");
  DLLIST_traverse(list, MNO->print);
}

void DLLIST_printAsChar(DLList *list)
{
  if (-1 == DLLIST_printListInfo(list))
  {
    return;
  }
  printf("Elements as char:\n");
  DLLIST_traverse(list, MNO->printAsChar);
}

void DLLIST_printAsString(DLList *list)
{
  if (-1 == DLLIST_printListInfo(list))
  {
    return;
  }
  printf("Elements as string:\n");
  DLLIST_traverse(list, MNO->printAsString);
}

void DLLIST_prepareDLList()
{
  //  MEMORY NODE PARA ACCESO A LAS OPERACIONES!!!
  mn_ops = MEMNODE_create();
}

void DLLIST_releaseDLList()
{
  //  MEMORY NODE PARA ACCESO A LAS OPERACIONES!!!
  MM->free(mn_ops);
}