// common_def.h
// Toni Barella
// Inteligencia Artificial paa Videojuegos
// ESAT 2017/2020
//
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#define VERBOSE_


typedef enum
{
	kErrorCode_Ok = 0,
	kErrorCode_GenericError = -1,

	kErrorCode_NodeNullPointer = -2,
	kErrorCode_DataNullPointer = -3,
	kErrorCode_SRCNullPointer = -4,
	kErrorCode_AllocationFail = -5,
	kErrorCode_InvalidInput = -6,
	kErrorCode_VectorNullPointer = -7,
	kErrorCode_StorageNullPointer = -8,
	kErrorCode_ListNullPointer = -9,
	kErrorCode_StorageFull = -10,
  kErrorCode_File = -11

} ErrorCode;

#endif // __COMMON_DEF_H__
