#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#define FALSE		0
#define TRUE		1

typedef int BOOL;

typedef struct tagSPERSON {
	char name[32];
	int no;
} SPERSON;

typedef SPERSON SDATATYPE;

typedef struct tagSTACK {
	SDATATYPE* pStack;
	SDATATYPE* SP;
	size_t size;
	size_t count;
} STACK, * HSTACK;

/* Function Prototypes */

HSTACK S_CreateStack(size_t size);
BOOL S_Push(HSTACK hStack, const SDATATYPE* pVal);
BOOL S_Pop(HSTACK hStack, SDATATYPE* pVal);
void S_Clear(HSTACK hStack);
void S_CloseStack(HSTACK hStack);

/* Macros */

#define S_GetCount(hStack)		((hStack)->count)
#define S_GetSize(hStack)			((hStack)->size)
#define S_IsEmpty(hStack)			((hStack)->count == 0)

#endif
