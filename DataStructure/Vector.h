#ifndef VECTOR_H
#define VECTOR_H

/* Symbolic Constants */

#define DEF_CAPACITY	4

#include <stddef.h>

/* Type Declarations */

typedef int DATATYPE;

typedef struct tagVector {
	DATATYPE* pArray;
	int size;
	int capacity;
} Vector, * HVector;

/* Function Prototypes */

HVector CreateVector(void);
HVector CreateVectorWithCapacity(int capacity);
int AddItem(HVector hArray, DATATYPE val);
void DeleteItemIndex(HVector hArray, int index);
int DeleteItem(HVector hArray, DATATYPE val);
int InsertItem(HVector hArray, int index, DATATYPE val);
DATATYPE* FindItem(HVector hArray, DATATYPE val);
DATATYPE* FindItemLast(HVector hArray, DATATYPE val);
int SetCapacity(HVector hArray, int newCapacity);
void TrimToSize(HVector hArray);
void CloseVector(HVector hArray);

/* Macros */

#define GetCapacity(hArray)		((hArray)->capacity)
#define GetSize(hArray)			((hArray)->size)
#define GetItem(hArray, index)		((hArray)->pArray[index])
#define Clear(hArray)				((hArray)->size = 0)

#endif
