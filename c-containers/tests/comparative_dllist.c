// comparative.c
// Toni Barella
// Algoritmos & Inteligencia Artificial
// ESAT 2016-2019
//
// File for comparative of ADTs

#if 0

#include <windows.h>
#include <adt_dllist.h>
#include <common_def.h>
#include <ABGS_MemoryManager/abgs_memory_manager.h>
#include <ABGS_MemoryManager/abgs_platform_types.h>

void *data;
DLList *list;

void TESTBASE_generateDataForComparative()
{
  // I only need one piece of data
  // That all tests are done with references to the same data does not affect the time it takes
  data = NULL;
  data = MM->malloc(128);
  if (NULL == data)
  {
    printf("Generate data failed\n");
    return NULL;
  }

  u8 *temp = (u8*)data;
  for (u16 i = 0; i < 128; ++i)
  {
    *temp = 'a';
    temp++;
  }
}
void calculateTimeForInsertFirst()
{
	LARGE_INTEGER frequency;				// ticks per second
	LARGE_INTEGER  time_start, time_end;    // ticks in interval
	double elapsed_time = 0.0f;
	u32 repetitions = 127;

	///////////////////////////////////////////////////////////////////////
	// Frequency: ticks per second
	QueryPerformanceFrequency(&frequency);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Meassurement time
	// start timer
	QueryPerformanceCounter(&time_start);
	// execute function to meassure 'repetitions' times
	for (u32 rep = 0; rep < repetitions; ++rep)
	{
		// execute function
    if (kErrorCode_Ok != list->ops_->insertFirst(list, data, 128))
    {
      printf("Error with insertFirst\n");
      return;
    }
	}
	// stop timer
	QueryPerformanceCounter(&time_end);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// compute the elapsed time in microseconds
	elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("127 DLList insertFirst total time: %lf.\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	elapsed_time /= repetitions;
  printf("DLList insertFirst average time: %lf.\n\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
}

void calculateTimeForInsertLast()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 127;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep)
  {
    // execute function
    if (kErrorCode_Ok != list->ops_->insertLast(list, data, 128))
    {
      printf("Error with insertLast\n");
      return;
    }
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("127 DLList insertLast total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("DLList insertLast average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
}

void calculateTimeForInsertAt()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 127;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep)
  {
    // execute function
    if (kErrorCode_Ok != list->ops_->insertAt(list, data, 128, (rep * 3)/4))
    {
      printf("Error With insertAt\n");
      return;
    }
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("127 DLList insertAt total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("DLList insertAt average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
}

void calculateTimeForExtractFirst()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 127;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep)
  {
    // execute function
    if (NULL == list->ops_->extractFirst(list))
    {
      printf("Error With extractFirst\n");
      return;
    }
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("127 DLList extractFirst total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("DLList extractFirst average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
}

void calculateTimeForExtractLast()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 127;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep)
  {
    // execute function
    if (NULL == list->ops_->extractLast(list))
    {
      printf("Error With extractLast\n");
      return;
    }
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("127 DLList extractLast total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("DLList extractLast average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
}

void calculateTimeForExtractAt()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 127;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (s32 rep = repetitions - 1; rep >= 0; --rep)
  {
    // execute function
    if (NULL == list->ops_->extractAt(list, (rep/3)*2))
    {
      printf("Error With extractAt\n");
      return;
    }
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("127 DLList extractAt total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("DLList extractAt average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
}

void calculateTimeForConcat()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10;
  DLList *list2 = DLLIST_create(10);
  for (u16 i=0; i<10; ++i)
    list->ops_->insertLast(list2, data, 128);

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep)
  {
    // execute function
    if (kErrorCode_Ok != list->ops_->concat(list, list2))
    {
      printf("Error With concat\n");
      return;
    }
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("10 DLList concat total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("DLList concat average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////

  // Soft reset and destroy (all the vectors have references to the same piece of data)
  list->ops_->traverse(list2, list2->first_->ops_->softFree);
  MM->free(list2);
}

int main()
{
  DLLIST_prepareDLList();
	TESTBASE_generateDataForComparative();

  list = DLLIST_create(127);
	calculateTimeForInsertFirst();
  calculateTimeForExtractFirst();
  calculateTimeForInsertLast();
  calculateTimeForExtractLast();
  calculateTimeForInsertAt();
  calculateTimeForExtractAt();
  calculateTimeForConcat();

  list->ops_->destroy(list); // I can destroy without errors because concat makes copies
  MM->free(data);   // Unique piece of data for the comparative
  DLLIST_releaseDLList();
	MM->status();
	MM->destroy();
	printf("Press ENTER to continue\n");
	getchar();
	return 0;
}

#endif