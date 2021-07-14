#include "adt_dllist.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"
#include "common_def.h"

//u8 *name = "DLList ";

void printFunctionResult(void* adt, u8 *msg, s16 error_type) {
  u8 *error_msg = (u8 *)MM->malloc(sizeof(u8) * 100);
  if (NULL == error_msg)
  {
    printf(" [TEST] Error allocating error mesage\n");
    return;
  }
  printf(" [TEST] Function %s returns ", msg);
  switch (error_type)
  {
  case kErrorCode_Ok:
    printf("OK");
    break;
    
  case kErrorCode_Memory:
    strcpy((char *)error_msg, "Default memory error");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_Memory_NULL:
    strcpy((char *)error_msg, "NULL memory");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_Param:
    strcpy((char *)error_msg, "Invalid parameter value");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_Param_NULL:
    strcpy((char *)error_msg, "NULL parameter");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_Param_NULL_Src:
    strcpy((char *)error_msg, "NULL source parameter");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_Param_Zero_Size:
    strcpy((char *)error_msg, "Invalid parameter: size is zero");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_Node:
    strcpy((char *)error_msg, "Default node error");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Node_NULL:
    strcpy((char *)error_msg, "NULL node");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Node_NULL_Data:
    strcpy((char *)error_msg, "NULL node data");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Vector:
    strcpy((char *)error_msg, "Default vector error");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Vector_NULL:
    strcpy((char *)error_msg, "NULL vector");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Vector_NULL_Storage:
    strcpy((char *)error_msg, "NULL vector storage");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Vector_Full:
    strcpy((char *)error_msg, "Full vector");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Vector_Empty:
    strcpy((char *)error_msg, "Empty vector");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_List_NULL:
    strcpy((char*)error_msg, "NULL list");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_List_Empty:
    strcpy((char*)error_msg, "Empty list");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_List_Not_Enough_Capacity:
    strcpy((char*)error_msg, "Not enough capacity in the list");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  default:
    strcpy((char *)error_msg, "Default error, needs to be implemented");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
  }
  printf(" in address = %p", adt);
  printf("\n");
  MM->free(error_msg);
}

typedef struct data_s {
  u16 length;
  u8 *text;
} Data;

Data* CreateData(s16 value, u16 length)
{
  Data *data = MM->malloc(sizeof(Data));
  data->text = MM->malloc(length);
  data->length = length;
  for (s16 i = 0; i < length; i++)
  {
    data->text[i] = '0' + value;//atoi(value);
  }

  return data;
}

int main() {
  s16 i = 0;
  s16 error = 0;

  DLLIST_prepareDLList();

  // DLList for ops
  DLList *list = DLLIST_create(10);
  if (NULL == list) {
    printf("\n create returned a null list un list for ops");
    return -1;
  }

  DLList *list_1 = DLLIST_create(10);
  if (NULL == list_1) {
    printf("\n create returned a null list un list_1");
    return -1;
  }

  DLList *list_2 = DLLIST_create(5);
  if (NULL == list_2) {
    printf("\n create returned a null list un list_2");
    return -1;
  }

  DLList *list_3 = DLLIST_create(10);
  if (NULL == list_3) {
    printf("\n create returned a null list un list_3");
    return -1;
  }

  printf("DLList size: %zu\n", sizeof(DLList));
  printf("MemoryNode size: %zu\n", sizeof(MemoryNode));

  printf("---------------------- FIRST BATTERY ----------------------\n\n");

  for (i = 0; i < 5; i++)
  {
    Data *data = CreateData(i, 7);
    error = list->ops_->insertLast(list_1, data->text, data->length);
    printFunctionResult(list_1, (u8 *)"insertLast list_1", error);
    if (kErrorCode_Ok != error) MM->free(data->text);
    MM->free(data);
  }
  list->ops_->printAsChar(list_1);

  for (i = 0; i < 5; i++)
  {
    Data *data = CreateData(i, 7);
    error = list->ops_->insertFirst(list_2, data->text, data->length);
    printFunctionResult(list_2, (u8 *)"insertFirst list_2", error);
    if (kErrorCode_Ok != error) MM->free(data->text);
    MM->free(data);
  }
  list->ops_->printAsChar(list_2);

  Data *data = CreateData(1, 3);
  error = list->ops_->insertAt(list_1, data->text, 3, data->length);
  printFunctionResult(list_1, (u8 *)"insertAt 3 list_1", error);
  if (kErrorCode_Ok != error) MM->free(data->text);
  MM->free(data);

  data = CreateData(2, 3);
  error = list->ops_->insertAt(list_1, data->text, 1, data->length);
  printFunctionResult(list_1, (u8 *)"insertAt 1 list_1", error);
  if (kErrorCode_Ok != error) MM->free(data->text);
  MM->free(data);

  data = CreateData(3, 3);
  error = list->ops_->insertAt(list_1, data->text, 17, data->length);
  printFunctionResult(list_1, (u8 *)"insertAt 17 list_1", error);
  if (kErrorCode_Ok != error) MM->free(data->text);
  MM->free(data);
  list->ops_->printAsChar(list_1);

  data = CreateData(3, 3);
  error = list->ops_->insertAt(list_2, data->text, 3, data->length);
  printFunctionResult(list_1, (u8 *)"insertAt list_2", error);
  if (kErrorCode_Ok != error) MM->free(data->text);
  MM->free(data);
  list->ops_->printAsChar(list_1);


  printf("\nExtractLast test\n");
  void *extracted_data = list->ops_->extractLast(list_1);
  if (NULL == extracted_data) printf("Extraction failed\n");
  else printf("Data extracted: %s\n\n", extracted_data);
  list->ops_->printAsChar(list_1);
  if (NULL != extracted_data) MM->free(extracted_data);

  printf("\nExtractFirst test\n");
  extracted_data = list->ops_->extractFirst(list_1);
  if (NULL == extracted_data) printf("Extraction failed\n");
  else printf("Data extracted: %s\n\n", extracted_data);
  list->ops_->printAsChar(list_1);
  if (NULL != extracted_data) MM->free(extracted_data);

  printf("\nExtractAt position 3 test\n");
  extracted_data = list->ops_->extractAt(list_1, 3);
  if (NULL == extracted_data) printf("Extraction failed\n");
  else printf("Data extracted: %s\n\n", extracted_data);
  list->ops_->printAsChar(list_1);
  if (NULL != extracted_data) MM->free(extracted_data);

  printf("\nExtractAt position 7 test (outside length)\n");
  extracted_data = list->ops_->extractAt(list_1, 7);
  if (NULL == extracted_data) printf("Extraction failed\n");
  else printf("Data extracted: %s\n\n", extracted_data);
  list->ops_->printAsChar(list_1);
  if (NULL != extracted_data) MM->free(extracted_data);

  printf("\nExtract on empty list test\n");
  extracted_data = list->ops_->extractFirst(list_3);
  if (NULL == extracted_data) printf("Extraction failed\n");
  else printf("Data extracted: %s\n\n", extracted_data);
  list->ops_->printAsChar(list_1);
  if (NULL != extracted_data) MM->free(extracted_data);

  printf("\nGetters:\n\n");
  void *data_ref = list->ops_->first(list_1);
  if (NULL == data_ref) printf(" NULL data\n");
  else printf("First: %s\n", data_ref);
  data_ref = list->ops_->last(list_1);
  if (NULL == data_ref) printf(" NULL data\n");
  else printf("Last: %s\n", data_ref);
  data_ref = list->ops_->at(list_1, 1);
  if (NULL == data_ref) printf(" NULL data\n");
  else printf("At 1: %s\n", data_ref);
  printf("At 1 on empty list\n");
  data_ref = list->ops_->at(list_3, 1);
  if (NULL == data_ref) printf(" NULL data\n");
  else printf("At 1: %s\n", data_ref);

  u16 result = list->ops_->capacity(list_1);
  printf("The capacity of list_1 is: %d (should be 10)\n", result);
  result = list->ops_->length(list_1);
  printf("The length of list_1 is: %d (should be 4)\n", result);

  printf("isEmpty on list_1 (not empty): ");
  bool bresult = list->ops_->isEmpty(list_1);
  if (bresult) printf(" Is empty\n");
  else printf(" Not empty\n");

  printf("isEmpty on list_3 (empty): ");
  bresult = list->ops_->isEmpty(list_3);
  if (bresult) printf(" Is empty\n");
  else printf(" Not empty\n");

  printf("isFull on list_1 (not full): ");
  bresult = list->ops_->isFull(list_1);
  if (bresult) printf(" Is full\n");
  else printf(" Not full\n");

  printf("isFull on list_2 (full): ");
  bresult = list->ops_->isFull(list_2);
  if (bresult) printf(" Is full\n");
  else printf(" Not full\n");

  printf("\nConcat test:\n\n");
  error = list->ops_->concat(list_1, list_2);
  printFunctionResult(list_1, (u8 *)"Concat list_1 and list_2", error);
  list->ops_->printAsChar(list_2);
  list->ops_->printAsChar(list_1);

  printf("\n\n ---------------------- NULL BATTERY ----------------------\n\n");
  printf("Creating list with 0 capacity:\n");
  DLList *tmp = DLLIST_create(0);
  if (NULL == tmp) printf("Error creating new list\n");
  else printf("DLList created");
  error = list->ops_->insertLast(NULL, data->text, data->length);
  printFunctionResult(list_1, (u8 *)"insertLast NULL list", error);
  error = list->ops_->insertLast(list_1, NULL, 0);
  printFunctionResult(list_1, (u8 *)"insertLast NULL data", error);
  printf("extractLast(NULL):\n");
  data = list->ops_->extractLast(NULL);
  if (NULL == data) printf("Extraction failed\n");
  error = list->ops_->concat(NULL, list_2);
  printFunctionResult(list_1, (u8 *)"concat(NULL, list_2)", error);
  error = list->ops_->concat(list_1, NULL);
  printFunctionResult(list_1, (u8 *)"concat(list_1, NULL)", error);
  error = list->ops_->traverse(NULL, list->ops_->print);
  printFunctionResult(list_1, (u8 *)"traverse(NULL, list->ops_->print)", error);
  error = list->ops_->traverse(list_1, NULL);
  printFunctionResult(list_1, (u8 *)"traverse(list_1, NULL)", error);

  printf("Destroys:\n");
  error = list->ops_->destroy(list_1);
  printFunctionResult(list_1, (u8 *)"destroy list_1", error);
  error = list->ops_->destroy(list_2);
  printFunctionResult(list_2, (u8 *)"destroy list_2", error);
  error = list->ops_->destroy(list_3);
  printFunctionResult(list_3, (u8 *)"destroy list_3", error);
  error = list->ops_->destroy(list);
  printFunctionResult(list, (u8 *)"destroy list", error);


  DLLIST_releaseDLList();
  MM->status();

  return 0;
}