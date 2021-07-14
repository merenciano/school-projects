// test_vector.c
// Lucas Merenciano
//
// Test battery for vector

#include <stdio.h>
#include <string.h>

#include "adt_vector.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"
#include "common_def.h"

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

  case kErrorCode_Node:
    strcpy((char *)error_msg, "Default node error");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Param_NULL:
    strcpy((char *)error_msg, "NULL node");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_NULL_Data:
    strcpy((char *)error_msg, "NULL node data");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Vector:
    strcpy((char *)error_msg, "Default vector error");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Param_NULL:
    strcpy((char *)error_msg, "NULL vector");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;
    
  case kErrorCode_NULL_Storage:
    strcpy((char *)error_msg, "NULL vector storage");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Full:
    strcpy((char *)error_msg, "Full vector");
    printf("FAIL with error %d (%s)", error_type, error_msg);
    break;

  case kErrorCode_Empty:
    strcpy((char *)error_msg, "Empty vector");
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

u8 *big_data_msg = (u8 *)"\n   ABGS - Artificial Bytes Game Studio\n  **  Tap Pop Balloon - Combo Party  **\n      Available for Android and iOS\n\n";
#define kIterationsBigData 10

const u16 kSizeData1 = (sizeof(u8) * 5);
const u16 kSizeData2 = (sizeof(u8) * 15);
const u16 kSizeData3 = (sizeof(u8) * 10);
const u16 kSizeData4 = (sizeof(u8) * 117);
const u16 kSizeBigData = (sizeof(u8) * (117 * kIterationsBigData));

int main() {
  s16 error_type;

  // Creating data for the nodes
  u8 *ptr_data_1 = (u8*)MM->malloc(sizeof(u8) * kSizeData1); // abcde
  if (NULL == ptr_data_1) {
    printf("\n create returned a null node in ptr_data_1");
    return -1;
  }
  u8 *ptr_data_2 = (u8*)MM->malloc(sizeof(u8) * kSizeData2); // fghijklmnopqrst
  if (NULL == ptr_data_2) {
    printf("\n create returned a null node in ptr_data_2");
    return -1;
  }
  u8 *ptr_data_3 = (u8*)MM->malloc(sizeof(u8) * kSizeData3); // 0123456789
  if (NULL == ptr_data_3) {
    printf("\n create returned a null node in ptr_data_3");
    return -1;
  }
  u8 *ptr_data_4 = (u8*)MM->malloc(sizeof(u8) * kSizeData4); // ABGS
  if (NULL == ptr_data_4) {
    printf("\n create returned a null node in ptr_data_4");
    return -1;
  }
  u8 *ptr_big_data = (u8*)MM->malloc(sizeof(u8) * kSizeBigData);
  if (NULL == ptr_big_data) {
    printf("\n create returned a null node in ptr_big_data");
    return -1;
  }
  for (u8 i = 0; i < kSizeData1; ++i)
  {
    *(ptr_data_1 + i) = 'a' + i;
  }
  for (u8 i = 0; i < kSizeData2; ++i)
  {
    *(ptr_data_2 + i) = 'f' + i;
  }
  for (u8 i = 0; i < kSizeData3; ++i)
  {
    *(ptr_data_3 + i) = '0' + i;
  }
  for (u8 i = 0; i < kSizeData4; ++i)
  {
    *(ptr_data_4 + i) = '0' + i;
  }
  strncpy((char *)ptr_data_4, (char *)big_data_msg, kSizeData4);

  printf("\n");
  printf("ptr_data_1 = ");
  for (u8 i = 0; i < kSizeData1; ++i)
  {
    printf("%c", *(ptr_data_1 + i));
  }
  printf("\n");
  printf("ptr_data_2 = ");
  for (u8 i = 0; i < kSizeData2; ++i)
  {
    printf("%c", *(ptr_data_2 + i));
  }
  printf("\n");
  printf("ptr_data_3 = ");
  for (u8 i = 0; i < kSizeData3; ++i)
  {
    printf("%c", *(ptr_data_3 + i));
  }
  printf("\n");
  printf("ptr_data_4 = ");
  for (u8 i = 0; i < kSizeData4; ++i)
  {
    printf("%c", *(ptr_data_4 + i));
  }
  printf("\n");

  // Creating vectors to work with
  Vector* vec = VECTOR_create(1);
  Vector* vector_1 = VECTOR_create(4);
  if (NULL == vector_1) {
    printf("vector create returned a null vector in vector_1");
    return -1;
  } else {
    printf("vector_1 created with a capacity of %d\n", vec->ops_->capacity(vector_1));
  }

  // FIRST BATTERY
  printf("---------- First battery ----------\n\n");
  // Inserts
  printf("\n\n##### TEST INSERTS #####\n");
  printf("\t insertLast\n");
  error_type = vec->ops_->insertLast(vector_1, ptr_data_2, kSizeData2);
  printFunctionResult(vector_1, (u8*)"insertLast data 2", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\t insertFirst\n");
  error_type = vec->ops_->insertFirst(vector_1, ptr_data_1, kSizeData1);
  printFunctionResult(vector_1, (u8*)"insertFirst data 1", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\t insertLast\n");
  error_type = vec->ops_->insertLast(vector_1, ptr_data_3, kSizeData3);
  printFunctionResult(vector_1, (u8*)"insertLast data 3", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\t insertAt 2\n");
  error_type = vec->ops_->insertAt(vector_1, ptr_data_4, 2, kSizeData4);
  printFunctionResult(vector_1, (u8*)"insertAt pos:2 data:4", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsString(vector_1);

  printf("\t insertAt 2 (full vector check)\n");
  error_type = vec->ops_->insertAt(vector_1, ptr_data_4, 2, kSizeData4);
  printFunctionResult(vector_1, (u8*)"insertAt pos:2 data:4", error_type);

  // Resize test
  printf("\n\n##### Test resize #####\n");
  printf("\tLess size\n");
  error_type = vec->ops_->resize(vector_1, 2);
  printFunctionResult(vector_1, (u8*)"resize to 2 capacity", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\n\tSame size\n");
  error_type = vec->ops_->resize(vector_1, 2);
  printFunctionResult(vector_1, (u8*)"resize to 2 capacity", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\n\tMore size\n");
  error_type = vec->ops_->resize(vector_1, 4);
  printFunctionResult(vector_1, (u8*)"resize to 4 capacity", error_type);
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  // Refill vector
  vec->ops_->insertLast(vector_1, ptr_data_3, kSizeData3);
  vec->ops_->insertAt(vector_1, ptr_data_4, 2, kSizeData4);


  // Extracts
  printf("\n\n##### TEST EXTRACTS #####\n");
  printf("\textractAt 2\n");
  ptr_data_4 = vec->ops_->extractAt(vector_1, 2);
  if (NULL != ptr_data_4) {
    printf("Data extracted\n");
  } else {
    printf("ERROR: data not extracted\n");
  }
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\t extractLast\n");
  ptr_data_3 = vec->ops_->extractLast(vector_1);
  if (NULL != ptr_data_3)
  {
    printf("Data extracted\n");
  } else {
    printf("ERROR: data not extracted\n");
  }
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\t extractFirst\n");
  ptr_data_1 = vec->ops_->extractFirst(vector_1);
  if (NULL != ptr_data_1) {
    printf("Data extracted\n");
  } else {
    printf("ERROR: data not extracted\n");
  }
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  printf("\t extractFirst\n");
  ptr_data_2 = vec->ops_->extractFirst(vector_1);
  if (NULL != ptr_data_2) {
    printf("Data extracted\n");
  } else {
    printf("ERROR: data not extracted\n");
  }
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);

  //check extract from empty vec
  printf("\t extractFirst (with empty vec)\n");
  u8* ptr_temp = vec->ops_->extractFirst(vector_1);
  if (NULL != ptr_temp) {
    printf("Data extracted\n");
  } else {
    printf("ERROR: data not extracted\n");
  }
  printf("Vector 1:\n");
  //vec->ops_->print(vector_1);
  vec->ops_->printAsChar(vector_1);
  
  
}