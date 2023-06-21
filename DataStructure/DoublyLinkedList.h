#ifndef DOUBLY_LINKED_LIST
#define DOUBLY_LINKED_LIST

#include <stddef.h>

/* Symbolic Constants */

#define FALSE		0
#define TRUE		1

/* Type Declarations */

typedef int BOOL;

typedef struct tagPERSON {
	char name[32];
	int no;
} PERSON;

typedef PERSON LL_DATATYPE;

typedef struct tagNODE {
	LL_DATATYPE val;
	struct tagNODE* pNext;
	struct tagNODE* pPrev;
} NODE;

typedef struct tagLLIST {
	NODE* pHead;
	NODE* pTail;
	size_t count;
} LLIST, * HLLIST;

/* Fuciton Prototypes */

HLLIST LL_CreateLList(void);
NODE* LL_AddItemHead(HLLIST hLList, const LL_DATATYPE* pVal);
NODE* LL_AddItemTail(HLLIST hLList, const LL_DATATYPE* pVal);
NODE* LL_InsertItemNext(HLLIST hLList, NODE* pInsertNode, const LL_DATATYPE* pVal);
NODE* LL_InsertItemPrev(HLLIST hLList, NODE* pInsertNode, const LL_DATATYPE* pVal);
NODE* LL_InsertItemIndex(HLLIST hLList, size_t index, const LL_DATATYPE* pVal);
void LL_DeleteItem(HLLIST hLList, NODE* pDeleteNode);
BOOL LL_DeleteItemIndex(HLLIST hLList, size_t index);
void LL_ClearList(HLLIST hLList);
void LL_CloseList(HLLIST hLList);
LL_DATATYPE* LL_FindItem(HLLIST hLList, BOOL(*Compare)(const LL_DATATYPE*));
NODE* LL_FindItemNode(HLLIST hLList, BOOL(*Compare)(const LL_DATATYPE*));
BOOL LL_WalkList(HLLIST hLList, BOOL(*Proc) (LL_DATATYPE*));
BOOL LL_WalkListRev(HLLIST hLList, BOOL(*Proc) (LL_DATATYPE*));

#endif
