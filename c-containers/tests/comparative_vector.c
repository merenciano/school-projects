// comparative.c
// Toni Barella
// Algoritmos & Inteligencia Artificial
// ESAT 2016-2019
//
// File for comparative of ADTs
#if 0

#include <windows.h>
#include <adt_vector.h>
#include <common_def.h>
#include <ABGS_MemoryManager/abgs_memory_manager.h>
#include <ABGS_MemoryManager/abgs_platform_types.h>

void *data;
Vector *vec;

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
    if (kErrorCode_Ok != vec->ops_->insertFirst(vec, data, 128))
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
  printf("127 Vector insertFirst total time: %lf.\n", elapsed_time);
	///////////////////////////////////////////////////////////////////////
	// compute the average time
	elapsed_time /= repetitions;
  printf("Vector insertFirst average time: %lf.\n\n", elapsed_time);
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
    if (kErrorCode_Ok != vec->ops_->insertLast(vec, data, 128))
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
  printf("127 Vector insertLast total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("Vector insertLast average time: %lf.\n\n", elapsed_time);
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
    if (kErrorCode_Ok != vec->ops_->insertAt(vec, data, 128, (rep * 3)/4))
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
  printf("127 Vector insertAt total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("Vector insertAt average time: %lf.\n\n", elapsed_time);
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
    if (NULL == vec->ops_->extractFirst(vec))
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
  printf("127 Vector extractFirst total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("Vector extractFirst average time: %lf.\n\n", elapsed_time);
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
    if (NULL == vec->ops_->extractLast(vec))
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
  printf("127 Vector extractLast total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("Vector extractLast average time: %lf.\n\n", elapsed_time);
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
    if (NULL == vec->ops_->extractAt(vec, (rep/3)*2))
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
  printf("127 Vector extractAt total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("Vector extractAt average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
}

calculateTimeForConcat()
{
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = 10;
  Vector *vec2 = VECTOR_create(10);
  for (u16 i=0; i<10; ++i)
    vec->ops_->insertLast(vec2, data, 128);

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
    if (kErrorCode_Ok != vec->ops_->concat(vec, vec2))
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
  printf("10 Vector concat total time: %lf.\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  elapsed_time /= repetitions;
  printf("Vector concat average time: %lf.\n\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////

  // Soft reset and destroy (all the vectors have references to the same piece of data)
  MM->free(vec2->storage_);
  MM->free(vec2);
}

int main()
{
  vec = VECTOR_create(6000);
	TESTBASE_generateDataForComparative();
	calculateTimeForInsertFirst();
  calculateTimeForExtractFirst();
  calculateTimeForInsertLast();
  calculateTimeForExtractLast();
  calculateTimeForInsertAt();
  calculateTimeForExtractAt();
  calculateTimeForConcat();
  vec->ops_->destroy(vec); // I can destroy without errors because concat makes copies
  MM->free(data);   // Unique piece of data for the comparative
	MM->status();
	MM->destroy();
	printf("Press ENTER to continue\n");
	getchar();
	return 0;
}

#endif