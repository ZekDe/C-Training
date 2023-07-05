#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

HSTACK S_CreateStack(size_t size)
{
	HSTACK hStack;

	if ((hStack = (HSTACK)malloc(sizeof(STACK))) == NULL)
		return NULL;

	if ((hStack->pStack = (SDATATYPE*)malloc(sizeof(SDATATYPE) * size)) == NULL) {
		free(hStack);
		return NULL;
	}
	hStack->SP = hStack->pStack + size;
	hStack->size = size;
	hStack->count = 0;

	return hStack;
}

BOOL S_Push(HSTACK hStack, const SDATATYPE* pVal)
{
	if (hStack->SP == hStack->pStack)
		return FALSE;

	*--hStack->SP = *pVal;
	++hStack->count;

	return TRUE;
}

BOOL S_Pop(HSTACK hStack, SDATATYPE* pVal)
{
	if (hStack->SP == hStack->pStack + hStack->size)		/* assert(hStack->count == 0) */
		return FALSE;

	*pVal = *hStack->SP++;
	--hStack->count;

	return TRUE;
}

void S_Clear(HSTACK hStack)
{
	hStack->SP = hStack->pStack + hStack->size;
	hStack->count = 0;
}

void S_CloseStack(HSTACK hStack)
{
	free(hStack->pStack);
	free(hStack);
}