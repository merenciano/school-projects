// test_dllist.c
// Toni Barella
// Algoritmos & Inteligencia Artificial
// ESAT 2016-2019
//
// Test battery for double linked list ADT

#if 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_dllist.h"
#include "ABGS_MemoryManager\abgs_memory_manager.h"

#include ".\..\tests\test_base.c"

const u16 kCapacityDLList1 = 30;
const u16 kCapacityDLList2 = 5;
const u16 kCapacityDLList3 = 2;
int main()
{
	s16 error_type = 0;

  DLLIST_prepareDLList(); // Initialize mn_ops
	
	TESTBASE_generateDataForTest();
	
	// dllist created just to have a reference to the operations
	DLList *dlls = NULL;
	dlls = DLLIST_create(1);
	if (NULL == dlls) {
		printf("\n create returned a null node in list for ops");
		return -1;
	}
	// vectors to work with
	DLList *dllist_1 = NULL;
	DLList *dllist_2 = NULL;
	DLList *dllist_3 = NULL;	
	dllist_1 = DLLIST_create(kCapacityDLList1);
	if (NULL == dllist_1) {
		printf("\n create returned a null node in dllist_1\n");
		return -1;
	}
	dllist_2 = DLLIST_create(kCapacityDLList2);
	if (NULL == dllist_2) {
		printf("\n create returned a null node in dllist_2\n");
		return -1;
	}
	dllist_3 = DLLIST_create(kCapacityDLList3);
	if (NULL == dllist_3) {
		printf("\n create returned a null node in dllist_3\n");
		return -1;
	}

	printf("Size of:\n");
	printf("  + Mem Node: %zu\n", sizeof(MemoryNode));
	printf("  + dllist_1: %zu\n", sizeof(*dllist_1));
	printf("  + dllist_2: %zu\n", sizeof(*dllist_2));
	printf("  + dllist_3: %zu\n", sizeof(*dllist_3));

	
	//FIRST BATTERY
	printf("---------------- FIRST BATTERY ----------------\n\n");
	// reset
	printf("\n\n# Test Reset\n");
	error_type = dlls->ops_->reset(dllist_1);
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"reset dllist_1", error_type);
	
	error_type = dlls->ops_->insertFirst(dllist_1, TestData.single_ptr_data_4, kSingleSizeData4);
	TestData.single_ptr_data_4 = NULL;
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"insertFirst dllist_1", error_type);

	error_type = dlls->ops_->reset(dllist_1);
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"reset dllist_1", error_type);
	
	if (false == dlls->ops_->isEmpty(dllist_1))
	{
		printf("ERROR: isEmpty doesn't work correctly (dllist_1)\n");
	}

	printf("\n\n# Test Insert\n");
	printf("\t insertFirst dllist_1\n");
	for (u16 i = 0; i < (kNumberOfStoragePtrTest_A / 2); ++i)
	{
		error_type = dlls->ops_->insertFirst(dllist_1, TestData.storage_ptr_test_A[i], (strlen(TestData.storage_ptr_test_A[i]) + 1));
		TestData.storage_ptr_test_A[i] = NULL;
		TESTBASE_printFunctionResult(dllist_1, (u8 *)"insertFirst dllist_1", error_type);
	}
	printf("dllist_1:\n");
	dlls->ops_->print(dllist_1);

	printf("\t insertLast dllist_1\n");
	for (u16 i = (kNumberOfStoragePtrTest_A / 2); i < kNumberOfStoragePtrTest_A; ++i)
	{
		error_type = dlls->ops_->insertLast(dllist_1, TestData.storage_ptr_test_A[i], (strlen(TestData.storage_ptr_test_A[i]) + 1));
		TestData.storage_ptr_test_A[i] = NULL;
		TESTBASE_printFunctionResult(dllist_1, (u8 *)"insertLast dllist_1", error_type);
	}
	printf("dllist_1:\n");
	dlls->ops_->print(dllist_1);

	printf("\t insertAt dllist_1\n");
	u16 position = (kNumberOfStoragePtrTest_A / 2);
	for (u16 i = 0; i < kNumberOfStoragePtrTest_B; ++i)
	{
		error_type = dlls->ops_->insertAt(dllist_1, TestData.storage_ptr_test_B[i], (strlen(TestData.storage_ptr_test_B[i]) + 1), position);
		TestData.storage_ptr_test_B[i] = NULL;
		TESTBASE_printFunctionResult(dllist_1, (u8 *)"insertAt dllist_1", error_type);
		position++;
	}
	printf("dllist_1:\n");
	dlls->ops_->print(dllist_1);

	printf("\t dllist_2 insertAt\n");
	error_type = dlls->ops_->insertAt(dllist_2, TestData.single_ptr_data_1, kSingleSizeData1, 0);
	TestData.single_ptr_data_1 = NULL;
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertAt dllist_2", error_type);
	
	error_type = dlls->ops_->insertAt(dllist_2, TestData.single_ptr_data_1, kSingleSizeData1, 0);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertAt dllist_2 with NULL data", error_type);
		
	error_type = dlls->ops_->insertAt(dllist_2, TestData.single_ptr_data_2, kSingleSizeData2, 0);
	TestData.single_ptr_data_2 = NULL;
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertAt dllist_2", error_type);
	
	error_type = dlls->ops_->insertAt(dllist_2, TestData.single_ptr_data_3, kSingleSizeData3, 4);
	TestData.single_ptr_data_3 = NULL;
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertAt dllist_2", error_type);
	
	printf("dllist_2:\n");
	dlls->ops_->print(dllist_2);

	printf("\n\n# Test Insert\n");
	printf("\t insertLast dllist_3\n");
	u16 insert_errors = 0;
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		if (false == dlls->ops_->isFull(dllist_3))
		{
			error_type = dlls->ops_->insertLast(dllist_3, TestData.storage_ptr_test_C[i], (strlen(TestData.storage_ptr_test_C[i]) + 1));
			TestData.storage_ptr_test_C[i] = NULL;
			TESTBASE_printFunctionResult(dllist_3, (u8 *)"insertLast dllist_3", error_type);
		}
		else
		{
			printf("ERROR: Trying to insert an element in a full dllist (dllist_3 capacity = %d)\n", dlls->ops_->capacity(dllist_3));
			insert_errors++;
		}
	}
	if ((kNumberOfStoragePtrTest_C - kCapacityDLList3) != insert_errors)
	{
		printf("  ==> ERROR: isFull doesn't work correctly (dllist_3)\n");
	}
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\n\n# Test Resize\n");
	printf("\t resize dllist_3\n");
	error_type = dlls->ops_->resize(dllist_3, 10);
	TESTBASE_printFunctionResult(dllist_3, (u8 *)"resize dllist_3", error_type);
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	for (u16 i = 2; i < kNumberOfStoragePtrTest_C; ++i)
	{
		error_type = dlls->ops_->insertLast(dllist_3, TestData.storage_ptr_test_C[i], (strlen(TestData.storage_ptr_test_C[i]) + 1));
		TestData.storage_ptr_test_C[i] = NULL;
		TESTBASE_printFunctionResult(dllist_3, (u8 *)"insertLast dllist_3", error_type);
	}
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\n\n# Test Extract\n");
	printf("\t extractAt dllist_3\n");
	position = 2;
	TestData.storage_ptr_test_C[position] = dlls->ops_->extractAt(dllist_3, position);
	if (NULL == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at position %d in dllist_3\n", position);
	else
		printf("extracted \"%s\" at position %d in dllist_3\n", TestData.storage_ptr_test_C[position], position);
	position++;
	TestData.storage_ptr_test_C[position] = dlls->ops_->extractAt(dllist_3, 2);
	if (NULL == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at position %d in dllist_3\n", 2);
	else
		printf("extracted \"%s\" at position %d in dllist_3\n", TestData.storage_ptr_test_C[position], 2);
	position++;
	TestData.storage_ptr_test_C[position] = dlls->ops_->extractAt(dllist_3, 2);
	if (NULL == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at position %d in dllist_3\n", 2);
	else
		printf("extracted \"%s\" at position %d in dllist_3\n", TestData.storage_ptr_test_C[position], 2);

	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\t extractFirst dllist_3\n");
	for (u16 i = 0; i < 2; ++i)
	{
		TestData.storage_ptr_test_C[i] = dlls->ops_->extractFirst(dllist_3);
		if (NULL == TestData.storage_ptr_test_C[i])
			printf("ERROR: NULL pointer extracted at first position\n");
		else
			printf("extracted \"%s\" at first position in dllist_3\n", TestData.storage_ptr_test_C[i]);
	}
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);
	printf("\t extractLast dllist_3\n");
	for (u16 i = 6; i > 4; --i)
	{
		TestData.storage_ptr_test_C[i] = dlls->ops_->extractLast(dllist_3);
		if (NULL == TestData.storage_ptr_test_C[i])
			printf("ERROR: NULL pointer extracted at Last position\n");
		else
			printf("extracted \"%s\" at last position in dllist_3\n", TestData.storage_ptr_test_C[i]);
	}
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	void *data = NULL;
	u16 extract_errors = 0;
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		if (false == dlls->ops_->isEmpty(dllist_3))
		{
			data = dlls->ops_->extractLast(dllist_3);
			MM->free(data);
			printf("ERROR: Trying to extract an element in an empty list (dllist_3 length = %d)\n", dlls->ops_->length(dllist_3));
			extract_errors++;
		}
	}
	if (extract_errors > 0)
	{
		printf("  ==> ERROR: isEmpty doesn't work correctly (list_3)\n");
	}

	printf("\n\n# Test Insert\n");
	printf("\t insertLast dllist_3\n");
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		error_type = dlls->ops_->insertLast(dllist_3, TestData.storage_ptr_test_C[i], (strlen(TestData.storage_ptr_test_C[i]) + 1));
		TestData.storage_ptr_test_C[i] = NULL;
		TESTBASE_printFunctionResult(dllist_3, (u8 *)"insertLast dllist_3", error_type);
	}
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\n\n# Test First\n");
	printf("\t first dllist_1\n");
	data = dlls->ops_->first(dllist_1);
	if (NULL == data)
		printf("First in dllist_1: NULL\n");
	else
		printf("First in dllist_1: \"%s\"\n", data);

	printf("\n\n# Test At\n");
	position = 1;
	printf("\t at dllist_3 (position %d)\n", position);
	data = dlls->ops_->at(dllist_3, 1);
	if (NULL == data)
		printf("AT in dllist_3: NULL\n");
	else
		printf("AT in dllist_3 (position %d): \"%s\"\n", position, data);

	printf("\n\n# Test Last\n");
	printf("\t last dllist_1\n");
	data = dlls->ops_->last(dllist_1);
	if (NULL == data)
		printf("LAST in dllist_1: NULL\n");
	else
		printf("LAST in dllist_1: \"%s\"\n", data);
	
	printf("\n\n# Test Length\n");
	printf("\t dllist_1: [Capacity = %d] - [Length  = %d]\n", dlls->ops_->capacity(dllist_1), dlls->ops_->length(dllist_1));
	printf("\t dllist_2: [Capacity = %d] - [Length  = %d]\n", dlls->ops_->capacity(dllist_2), dlls->ops_->length(dllist_2));
	printf("\t dllist_3: [Capacity = %d] - [Length  = %d]\n", dlls->ops_->capacity(dllist_3), dlls->ops_->length(dllist_3));
	
	printf("\n\n# Test Concat\n");
	printf("\t concat dllist_1 + dllist_3\n");
	error_type = dlls->ops_->concat(dllist_1, dllist_3);
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"concat dllist_1 + dllist_3", error_type);
	printf("dllist_1:\n");
	dlls->ops_->print(dllist_1);
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\n\n# Test Length\n");
	printf("\t Length dllist_1 = %d \n", dlls->ops_->length(dllist_1));
	printf("\t Length dllist_2 = %d \n", dlls->ops_->length(dllist_2));
	printf("\t Length dllist_3 = %d \n", dlls->ops_->length(dllist_3));

	printf("\n\n# Test Reset\n");
	error_type = dlls->ops_->reset(dllist_3);
	TESTBASE_printFunctionResult(dllist_3, (u8 *)"reset dllist_3", error_type);

	printf("\n\n# Test Length\n");
	printf("\t Length dllist_1 = %d \n", dlls->ops_->length(dllist_1));
	printf("\t Length dllist_2 = %d \n", dlls->ops_->length(dllist_2));
	printf("\t Length dllist_3 = %d \n", dlls->ops_->length(dllist_3));
	
	
	printf("\n\n# Test First\n");
	printf("\t first dllist_3\n");
	data = dlls->ops_->first(dllist_3);
	if (NULL == data)
		printf("First in dllist_3: NULL\n");
	else
		printf("ERROR: First in dllist_3: \"%s\"\n", data);

	printf("\n\n# Test At\n");
	position = 1;
	printf("\t at dllist_3 (position %d)\n", position);
	data = dlls->ops_->at(dllist_3, 1);
	if (NULL == data)
		printf("At in dllist_3: NULL\n");
	else
		printf("ERROR: AT in dllist_3 (position %d): \"%s\"\n", position, data);

	printf("\n\n# Test Last\n");
	printf("\t last dllist_3\n");
	data = dlls->ops_->last(dllist_3);
	if (NULL == data)
		printf("Last in dllist_3: NULL\n");
	else
		printf("ERROR: Last in dllist_3: \"%s\"\n", data);

	
	printf("\n\n# Test Concat\n");
	printf("\t concat dllist_3 + dllist_2\n");
	error_type = dlls->ops_->concat(dllist_3, dllist_2);
	TESTBASE_printFunctionResult(dllist_3, (u8 *)"concat dllist_3 + dllist_2", error_type);
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\n\n# Test Destroy\n");
	printf("\t dllist_2 Destroy\n");
	error_type = dlls->ops_->destroy(dllist_2);
	dllist_2 = NULL;
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"destroy dllist_2", error_type);
	printf("dllist_2:\n");
	dlls->ops_->print(dllist_2);
	
	printf("\t dllist_3 Destroy\n");
	error_type = dlls->ops_->destroy(dllist_3);
	dllist_3 = NULL;
	TESTBASE_printFunctionResult(dllist_3, (u8 *)"destroy dllist_3", error_type);
	printf("dllist_3:\n");
	dlls->ops_->print(dllist_3);

	printf("\n\n# Test Resize\n");
	printf("\t dllist_1 resize\n");
	error_type = dlls->ops_->resize(dllist_1, 3);
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"resize dllist_1", error_type);
	printf("dllist_1:\n");
	dlls->ops_->print(dllist_1);

	MM->status();

	printf("\n\n---------------- NULL BATTERY ----------------\n\n");
	printf("\n\n# Test Insert\n");
	data = TestData.single_ptr_big_data;
	error_type = dlls->ops_->insertFirst(dllist_2, data, kSingleSizeBigData);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertFirst dllist_2 (NOT VALID)", error_type);
	error_type = dlls->ops_->insertLast(dllist_2, data, kSingleSizeBigData);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertLast dllist_2 (NOT VALID)", error_type);
	error_type = dlls->ops_->insertAt(dllist_2, data, kSingleSizeBigData, 1);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"insertAt dllist_2 (NOT VALID)", error_type);

	printf("\n\n# Test First\n");
	printf("\t first dllist_2\n");
	data = dlls->ops_->first(dllist_2);
	if (NULL != data)
		printf("ERROR: First in dllist_2 is not NULL\n");

	printf("\n\n# Test At\n");
	printf("\t at dllist_2 (position 1)\n");
	data = dlls->ops_->at(dllist_2, 1);
	if (NULL != data)
		printf("ERROR: AT in dllist_2 is not NULL\n");

	printf("\n\n# Test Last\n");
	printf("\t last dllist_2\n");
	data = dlls->ops_->last(dllist_2);
	if (NULL != data)
		printf("ERROR: LAST in dllist_2 is not NULL\n");


	printf("\n\n# Test Extract\n");
	data = dlls->ops_->extractFirst(dllist_2);
	if (NULL != data)
	{
		printf("ERROR: trying to extractFirst from an empty dllist\n");
	}
	data = dlls->ops_->extractLast(dllist_2);
	if (NULL != data)
	{
		printf("ERROR: trying to extractLast from an empty dllist\n");
	}
	data = dlls->ops_->extractAt(dllist_2, 1);
	if (NULL != data)
	{
		printf("ERROR: trying to extractAt from an empty dllist\n");
	}
		
	printf("\n\n# Test Concat\n");
	printf("\t concat dllist_1 + dllist_2\n");
	error_type = dlls->ops_->concat(dllist_1, dllist_2);
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"concat dllist_1 + dllist_2 (NOT VALID)", error_type);

	printf("\n\n# Test Resize\n");
	error_type = dlls->ops_->resize(dllist_2, 5);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"resize dllist_2 (NOT VALID)", error_type);
	
	printf("\n\n# Test Reset\n");
	error_type = dlls->ops_->reset(dllist_2);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"reset dllist_2 (NOT VALID)", error_type);

	
	printf("\n\n# Test Destroy\n");
	error_type = dlls->ops_->destroy(dllist_2);
	TESTBASE_printFunctionResult(dllist_2, (u8 *)"destroy dllist_2 (NOT VALID)", error_type);
	error_type = dlls->ops_->destroy(dllist_3);
	TESTBASE_printFunctionResult(dllist_3, (u8 *)"destroy dllist_3 (NOT VALID)", error_type);


	// Work is done, clean the system
	error_type = dlls->ops_->destroy(dllist_1);
	TESTBASE_printFunctionResult(dllist_1, (u8 *)"destroy dllist_1", error_type);
	error_type = dlls->ops_->destroy(dlls);
	TESTBASE_printFunctionResult(dlls, (u8 *)"destroy DLList Operations", error_type);

	printf("\nAt this point, it must be only one block of %d bytes waiting to be freed\n\n", kSingleSizeBigData);
  DLLIST_releaseDLList(); // destroy mn->ops
	MM->status();
	TESTBASE_freeDataForTest();
	MM->status();
	MM->destroy();
	printf("Press ENTER to continue\n");
	getchar();
	return 0;
}

#endif
