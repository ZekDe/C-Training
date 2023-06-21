#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vector.h"


HVector CreateVector(void)
{
	return CreateVectorWithCapacity(DEF_CAPACITY);
}

HVector CreateVectorWithCapacity(int capacity)
{
	HVector hDArray;

	if ((hDArray = (HVector)malloc(sizeof(Vector))) == NULL)
		return NULL;

	if ((hDArray->pArray = (DATATYPE*)malloc(sizeof(DATATYPE) * capacity)) == NULL) {
		free(hDArray);
		return NULL;
	}
	hDArray->capacity = capacity;
	hDArray->size = 0;

	return hDArray;
}

int AddItem(HVector hArray, DATATYPE val)
{
	if (hArray->size == hArray->capacity && SetCapacity(hArray, hArray->size * 2) == -1)
		return -1;

	hArray->pArray[hArray->size++] = val;

	return hArray->size - 1;
}

int InsertItem(HVector hArray, int index, DATATYPE val)
{
	if (hArray->size == hArray->capacity && SetCapacity(hArray, hArray->size * 2) == -1)
		return -1;

	memmove(&hArray->pArray[index + 1], &hArray->pArray[index], (hArray->size - index) * sizeof(DATATYPE));
	hArray->pArray[index] = val;
	++hArray->size;

	return index;
}

void DeleteItemIndex(HVector hArray, int index)
{
	memmove(&hArray->pArray[index], &hArray->pArray[index + 1], (hArray->size - index - 1) * sizeof(DATATYPE));
	--hArray->size;
}

int DeleteItem(HVector hArray, DATATYPE val)
{
	size_t i;

	for (i = 0; i < hArray->size; ++i)
		if (hArray->pArray[i] == val) {
			DeleteItemIndex(hArray, i);
			return i;
		}

	return -1;
}

DATATYPE* FindItem(HVector hArray, DATATYPE val)
{
	size_t i;

	for (i = 0; i < hArray->size; ++i)
		if (hArray->pArray[i] == val)
			return &hArray->pArray[i];

	return NULL;
}

DATATYPE* FindItemLast(HVector hArray, DATATYPE val)
{
	for (int i = hArray->size - 1; i >= 0; --i) {
		if (hArray->pArray[i] == val)
			return &hArray->pArray[i]; 
	}

	return NULL;
}

int SetCapacity(HVector hArray, int newCapacity)
{
	DATATYPE* pArray;

	if (newCapacity < hArray->size)
		return -1;

	if ((pArray = (DATATYPE*)realloc(hArray->pArray, sizeof(DATATYPE) * newCapacity)) == NULL)
		return -1;
	hArray->pArray = pArray;
	hArray->capacity = newCapacity;

	return newCapacity;
}

void TrimToSize(HVector hArray)
{
	hArray->pArray = realloc(hArray->pArray, hArray->size);

	hArray->capacity = hArray->size;
}

void CloseVector(HVector hArray)
{
	free(hArray->pArray);
	free(hArray);
}