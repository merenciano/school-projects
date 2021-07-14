#include "adt_list.h"

#include <stdio.h>
#include <string.h>

#include "common_def.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

#define MNO mn_ops->ops_
MemoryNode *mn_ops = NULL;  // Only for operations

// List declarations
static s16 LIST_prepareNodeToInsert(List* list, void* data, u16 bytes, MemoryNode **out);
static void LIST_init(List* list);
static s16 LIST_reset(List* list);
static s16 LIST_destroy(List* list);
static s16 LIST_resize(List *list, u16 new_size);
static u16 LIST_capacity(List* list);
static u16 LIST_length(List* list);
static bool LIST_isEmpty(List* list);
static bool LIST_isFull(List* list);
static void* LIST_first(List* list);
static void* LIST_last(List* list);
static void* LIST_at(List* list, u16 position);
static s16 LIST_insertFirst(List* list, void* data, u16 bytes);
static s16 LIST_insertLast(List* list, void* data, u16 bytes);
static s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position);
static void* LIST_extractFirst(List* list);
static void* LIST_extractLast(List* list);
static void* LIST_extractAt(List* list, u16 position);
static s16 LIST_copy(List **list, List *list_src);
static s16 LIST_concat(List* list, List* list_src);
static s16 LIST_traverse(List* list, void(*callback)(MemoryNode* mn));
static void LIST_print(List *list);
static void LIST_printAsChar(List *list);
static void LIST_printAsString(List *list);
static void LIST_printAsString(List *list);

// List API definitions
struct list_ops_s list_ops = 
{
  .reset = LIST_reset,
  .destroy = LIST_destroy,
  .resize = LIST_resize,
  .capacity = LIST_capacity,
  .length = LIST_length,
  .isEmpty = LIST_isEmpty,
  .isFull = LIST_isFull,
  .first = LIST_first,
  .last = LIST_last,
  .at = LIST_at,
  .insertFirst = LIST_insertFirst,
  .insertLast = LIST_insertLast,
  .insertAt = LIST_insertAt,
  .extractFirst = LIST_extractFirst,
  .extractLast = LIST_extractLast,
  .extractAt = LIST_extractAt,
  .traverse = LIST_traverse,
  .concat = LIST_concat,
  .print = LIST_print,
  .printAsChar = LIST_printAsChar,
  .printAsString = LIST_printAsString,
};

// List definitions
List* LIST_create(u16 capacity)
{
  List* list = NULL;

  if (0 == capacity)
  {
#ifdef VERBOSE_
    printf("Error: [%s] Capacity has to be greater than 0.\n", __FUNCTION__);
#endif
    return NULL;
  }


  list = (List*)MM->malloc(sizeof(List));
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] Memory allocation error\n", __FUNCTION__);
#endif
    return NULL;
  }

  list->capacity_ = capacity;
  LIST_init(list);

  return list;
}

void LIST_init(List *list)
{
  list->length_ = 0;
  list->first_ = NULL;
  list->last_ = NULL;
  list->ops_ = &list_ops;
}

s16 LIST_destroy(List* list)
{
  s16 error;

  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  error = LIST_reset(list);
  if (kErrorCode_Ok != error)
  {
    return error;
  }

  memset(list, 0, sizeof(List));
  MM->free(list);

  return kErrorCode_Ok;
}

s16 LIST_resize(List *list, u16 new_size)
{
  u16 i = 0;
  MemoryNode *curr;
  MemoryNode *next;

  if (NULL == list) return kErrorCode_Param_NULL;
  if (0 == new_size) return kErrorCode_Param_Zero_Size;

  if (new_size >= list->length_)
  {
    list->capacity_ = new_size;
    return kErrorCode_Ok;
  }
  
  i = 0;
  curr = list->first_;
  next = MNO->next(curr);
  while (NULL != next)
  {
    if (i == new_size - 1)
      list->last_ = curr;

    if (i >= new_size)
      MNO->free(curr);

    curr = next;
    next = MNO->next(curr);
    i++;
  }
  MNO->free(curr);

  list->capacity_ = new_size;
  list->length_ = new_size;
  MNO->setNext(list->last_, NULL);
  return kErrorCode_Ok;
}

s16 LIST_reset(List* list)
{
  s16 error = kErrorCode_NONE;
  MemoryNode* current = NULL;
  MemoryNode* next = NULL;

  if (NULL == list)         return kErrorCode_Param_NULL;
  if (NULL == list->first_) return kErrorCode_Ok;

  error = LIST_traverse(list, MNO->free);
  if (kErrorCode_Ok != error)
  {
    return error;
  }

  list->length_ = 0;
  list->first_ = NULL;
  list->last_ = NULL;

  return kErrorCode_Ok;
}

u16 LIST_capacity(List* list)
{
  if (NULL == list)
  {
    return 0;
  }

  return list->capacity_;
}

u16 LIST_length(List* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] List is NULL, the value returned is not valid\n", __FUNCTION__);
#endif
    return 0xFFFF; // Largest u16 number, -1 if signed. This is an error return
  }
  return list->length_;
}

bool LIST_isEmpty(List* list)
{
  if (NULL == list)
  {
#ifdef VERBOSE_
    printf("Error: [%s] List is NULL, , the value returned is not valid\n", __FUNCTION__);
#endif
    return true;
  }

  return !list->length_;
}

bool LIST_isFull(List* list)
{
  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  return list->length_ == list->capacity_;
}

void* LIST_first(List* list)
{
  if (NULL == list)
  {
    return NULL;
  }

  return MNO->data(list->first_);
}

void* LIST_last(List* list)
{
  if (NULL == list)
  {
    return NULL;
  }

  return MNO->data(list->last_);
}

void* LIST_at(List* list, u16 position)
{
  s16 i;
  MemoryNode* at;

  if (NULL == list)
  {
    return NULL;
  }

  if (0 == position)
  {
    return LIST_first(list);
  }

  if (list->length_ == position + 1)
  {
    return LIST_last(list);
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

s16 LIST_prepareNodeToInsert(List* list, void* data, u16 bytes, MemoryNode **out)
{
  s16 error = kErrorCode_NONE;

  if (NULL == list)
  {
    return kErrorCode_Param_NULL;
  }

  if (LIST_isFull(list))
  {
    return kErrorCode_Not_Enough_Capacity;
  }

  if (NULL == data)
  {
#ifdef VERBOSE_
    printf("Error: [%s] You should not insert NULL data\n", __FUNCTION__);
#endif
    return kErrorCode_Memory_NULL;
  }

  *out = MEMNODE_create();
  if (NULL == *out) return kErrorCode_Memory_Allocation;

  error = MNO->setData(*out, data, bytes);
  if (error != kErrorCode_Ok)
  {
    MNO->free(*out);
    return error;
  }

  return kErrorCode_Ok;
}

s16 LIST_insertFirst(List* list, void* data, u16 bytes)
{
  MemoryNode* mn;
  s16 error = kErrorCode_NONE;

  error = LIST_prepareNodeToInsert(list, data, bytes, &mn);
  if (kErrorCode_Ok != error) return error;

  error = MNO->setNext(mn, list->first_);
  if (kErrorCode_Ok != error)
  {
    MNO->free(mn);
    return error;
  }

  list->first_ = mn;
  if (NULL == list->last_) list->last_ = mn;
  list->length_++;

  return kErrorCode_Ok;
}

s16 LIST_insertLast(List* list, void* data, u16 bytes)
{
  s16 error;
  MemoryNode* mn = NULL;

  error = LIST_prepareNodeToInsert(list, data, bytes, &mn);
  if (kErrorCode_Ok != error) return error;

  if (LIST_isEmpty(list))
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
  }
  list->last_ = mn;
  list->length_++;

  return kErrorCode_Ok;
}

s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position)
{
  s16 error = kErrorCode_NONE;
  MemoryNode* mn = NULL;
  MemoryNode* at = NULL;

  if (NULL == list) return kErrorCode_Param_NULL;

  if (0 == position) return LIST_insertFirst(list, data, bytes);
  if (list->length_ == position) return LIST_insertLast(list, data, bytes);
  if (position > list->length_)
  {
#ifdef VERBOSE_
    printf("Warning: [%s] Position out of the range, inserting last\n", __FUNCTION__);
#endif
    return LIST_insertLast(list, data, bytes);
  }

  error = LIST_prepareNodeToInsert(list, data, bytes, &mn);
  if (kErrorCode_Ok != error) return error;

  at = list->first_;
  for (u16 i = 0; i < position - 1; i++) // position-1 never will be negative at this point
  {
    at = MNO->next(at);
  }
  error = MNO->setNext(mn, MNO->next(at));
  if (kErrorCode_Ok != error) return error;

  error = MNO->setNext(at, mn);
  if (kErrorCode_Ok != error) return error;

  list->length_++;

  return kErrorCode_Ok;
}

void* LIST_extractFirst(List* list)
{
  MemoryNode* mn = NULL;
  void* data = NULL;

  if (NULL == list)
  {
    return NULL;
  }

  if (LIST_isEmpty(list))
  {
    return NULL;
  }

  mn = list->first_;
  list->first_ = MNO->next(list->first_);
  data = MNO->data(mn);
  MNO->softFree(mn);
  list->length_--;
  return data;
}

void* LIST_extractLast(List* list)
{
  MemoryNode* mn;
  void* data;

  if (NULL == list)
  {
    return NULL;
  }

  if (LIST_isEmpty(list))
  {
    return NULL;
  }

  if (1 == list->length_)
  {
    return LIST_extractFirst(list);
  }

  mn = list->first_;
  while (MNO->next(mn) != list->last_)
  {
    mn = MNO->next(mn);
  }
  list->last_ = mn;
  data = MNO->data(MNO->next(mn));
  MNO->softFree(MNO->next(mn));
  MNO->setNext(mn, NULL);
  list->length_--;

  return data;
}

void* LIST_extractAt(List* list, u16 position)
{
  MemoryNode* mn;
  MemoryNode* temp_mn;
  void* data;

  if (NULL == list)
  {
    return NULL;
  }

  if (LIST_isEmpty(list))
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

  if (position == list->length_ - 1)
  {
    return LIST_extractLast(list);
  }

  if (0 == position)
  {
    return LIST_extractFirst(list);
  }

  mn = list->first_;
  for (u16 i = 0; i < position - 1; i++)
  {
    mn = MNO->next(mn);
  }

  data = MNO->data(MNO->next(mn));
  temp_mn = MNO->next(mn);
  MNO->setNext(mn, MNO->next(temp_mn));
  MNO->softFree(temp_mn);
  list->length_--;
  return data;
}

s16 LIST_copy(List **list, List *list_src)
{
  List* new_list = NULL;
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

  if (LIST_isEmpty(list_src))
  {
    return kErrorCode_Ok;
  }

  new_list = LIST_create(LIST_capacity(list_src));
  if (NULL == new_list)
  {
    return kErrorCode_Memory_Allocation;
  }
  new_list->capacity_ = LIST_capacity(list_src);
  new_list->length_ = LIST_length(list_src);

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
    LIST_destroy(new_list);
    return error;
  }

  if (1 == new_list->length_) {
    new_list->last_ = new_list->first_;
    LIST_destroy(*list);
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
      LIST_destroy(new_list);
      return kErrorCode_Memory_Allocation;
    }

    error = MNO->memCopy(i->next_, MNO->data(j), MNO->size(j));
    if (kErrorCode_Ok != error)
    {
      LIST_destroy(new_list);
      return error;
    }
    i = MNO->next(i);
    j = MNO->next(j);
  }
  new_list->last_ = i;
  LIST_destroy(*list);
  *list = new_list;
  return kErrorCode_Ok;
}

s16 LIST_concat(List *list, List *list_src)
{
  List* src_list_copy = NULL;
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

  if (LIST_isEmpty(list_src))
  {
    return kErrorCode_Ok;
  }

  src_list_copy = LIST_create(LIST_capacity(list_src));
  if (NULL == src_list_copy) return kErrorCode_Memory_Allocation;
  error = LIST_copy(&src_list_copy, list_src);
  if (kErrorCode_Ok != error)
  {
    LIST_destroy(src_list_copy);
    return error;
  }

  if (LIST_isEmpty(list))
  {
    list->first_ = src_list_copy->first_;
  }
  else
  {
    MNO->setNext(list->last_, src_list_copy->first_);
  }
  list->length_ += LIST_length(src_list_copy);
  list->last_ = src_list_copy->last_;
  list->capacity_ = list->capacity_ + list_src->capacity_;

  MM->free(src_list_copy); // SoftDestroy

  return kErrorCode_Ok;
}

s16 LIST_traverse(List* list, void(*callback)(MemoryNode* mn))
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

s16 LIST_printListInfo(List *list) {
  printf("List Info:\n");
  if (NULL == list)
  {
    printf("List = NULL \n");
    return -1;
  }

  if (LIST_isEmpty(list))
  {
    printf("Empty List\n");
    return -1;
  }

  printf("List address = %p\n", list);
  printf("List capacity: %zu\n", LIST_capacity(list));
  printf("List length: %zu\n", LIST_length(list));
  return 1;
}

void LIST_print(List *list)
{
  if (-1 == LIST_printListInfo(list))
  {
    return;
  }
  printf("Elements:\n");
  LIST_traverse(list, MNO->print);
}

void LIST_printAsChar(List *list)
{
  if (-1 == LIST_printListInfo(list))
  {
    return;
  }
  printf("Elements as char:\n");
  LIST_traverse(list, MNO->printAsChar);
}

void LIST_printAsString(List *list)
{
  if (-1 == LIST_printListInfo(list))
  {
    return;
  }
  printf("Elements as string:\n");
  LIST_traverse(list, MNO->printAsString);
}

void LIST_prepareList()
{
  //  MEMORY NODE PARA ACCESO A LAS OPERACIONES!!!
  mn_ops = MEMNODE_create();
}

void LIST_releaseList()
{
  //  MEMORY NODE PARA ACCESO A LAS OPERACIONES!!!
  MM->free(mn_ops);
}
