#include <stdio.h>
#include <stdlib.h>
#include "DoublyLinkedList.h"

HLLIST LL_CreateLList(void)
{
	HLLIST hLList;

	if ((hLList = (HLLIST)malloc(sizeof(LLIST))) == NULL)
		return NULL;

	hLList->pHead = hLList->pTail = NULL;
	hLList->count = 0;

	return hLList;
}

NODE* LL_AddItemHead(HLLIST hLList, const LL_DATATYPE* pVal)
{
	NODE* pNewNode;

	if ((pNewNode = (NODE*)malloc(sizeof(NODE))) == NULL)
		return NULL;
	pNewNode->val = *pVal;
	pNewNode->pPrev = NULL;

	pNewNode->pNext = hLList->pHead;
	if (hLList->pHead != NULL)
		hLList->pHead->pPrev = pNewNode;
	else
		hLList->pTail = pNewNode;
	hLList->pHead = pNewNode;

	++hLList->count;

	return pNewNode;
}

NODE* LL_AddItemTail(HLLIST hLList, const LL_DATATYPE* pVal)
{
	NODE* pNewNode;

	if ((pNewNode = (NODE*)malloc(sizeof(NODE))) == NULL)
		return NULL;
	pNewNode->val = *pVal;
	pNewNode->pNext = NULL;

	pNewNode->pPrev = hLList->pTail;
	if (hLList->pTail != NULL)
		hLList->pTail->pNext = pNewNode;
	else
		hLList->pHead = pNewNode;
	hLList->pTail = pNewNode;

	++hLList->count;

	return pNewNode;
}

NODE* LL_InsertItemNext(HLLIST hLList, NODE* pInsertNode, const LL_DATATYPE* pVal)
{
	NODE* pNewNode;

	if ((pNewNode = (NODE*)malloc(sizeof(NODE))) == NULL)
		return NULL;
	pNewNode->val = *pVal;

	pNewNode->pPrev = pInsertNode;
	pNewNode->pNext = pInsertNode->pNext;
	if (pInsertNode->pNext != NULL)
		pInsertNode->pNext->pPrev = pNewNode;
	else
		hLList->pTail = pNewNode;
	pInsertNode->pNext = pNewNode;

	return pNewNode;
}

NODE* LL_InsertItemPrev(HLLIST hLList, NODE* pInsertNode, const LL_DATATYPE* pVal)
{
	NODE* pNewNode;

	if ((pNewNode = (NODE*)malloc(sizeof(NODE))) == NULL)
		return NULL;
	pNewNode->val = *pVal;

	if (pInsertNode == hLList->pHead)
		hLList->pHead = pNewNode;
	else
		pInsertNode->pPrev->pNext = pNewNode;

	pNewNode->pNext = pInsertNode;
	pNewNode->pPrev = pInsertNode->pPrev;
	pInsertNode->pPrev = pNewNode;

	++hLList->count;

	return pNewNode;
}

NODE* LL_InsertItemIndex(HLLIST hLList, size_t index, const LL_DATATYPE* pVal)
{
	NODE* pNode;
	size_t i;

	if (index > hLList->count)
		return NULL;

	if (index == hLList->count)
		return LL_AddItemTail(hLList, pVal);

	if (index == 0)
		return LL_AddItemHead(hLList, pVal);

	pNode = hLList->pHead;
	for (i = 0; i < index; ++i)
		pNode = pNode->pNext;

	return LL_InsertItemPrev(hLList, pNode, pVal);
}

void LL_DeleteItem(HLLIST hLList, NODE* pDeleteNode)
{
	if (pDeleteNode == hLList->pHead)
		hLList->pHead = pDeleteNode->pNext;
	else
		pDeleteNode->pPrev->pNext = pDeleteNode->pNext;

	if (pDeleteNode == hLList->pTail)
		hLList->pTail = pDeleteNode->pPrev;
	else
		pDeleteNode->pNext->pPrev = pDeleteNode->pPrev;

	free(pDeleteNode);

	--hLList->count;
}

BOOL LL_DeleteItemIndex(HLLIST hLList, size_t index)
{
	NODE* pNode;
	size_t i;

	if (index >= hLList->count)
		return FALSE;

	pNode = hLList->pHead;
	for (i = 0; i < index; ++i)
		pNode = pNode->pNext;

	LL_DeleteItem(hLList, pNode);

	return TRUE;
}

void LL_ClearList(HLLIST hLList)
{
	NODE* pNode, * pTempNode;

	pNode = hLList->pHead;
	while (pNode != NULL) {
		pTempNode = pNode->pNext;
		free(pNode);
		pNode = pTempNode;
	}

	hLList->pHead = hLList->pTail = NULL;
	hLList->count = 0;
}

void LL_CloseList(HLLIST hLList)
{
	LL_ClearList(hLList);

	free(hLList);
}

LL_DATATYPE* LL_FindItem(HLLIST hLList, BOOL(*Compare)(const LL_DATATYPE*))
{
	NODE* pNode;

	for (pNode = hLList->pHead; pNode != NULL; pNode = pNode->pNext)
		if (Compare(&pNode->val))
			return &pNode->val;

	return NULL;
}

NODE* LL_FindItemNode(HLLIST hLList, BOOL(*Compare)(const LL_DATATYPE*))
{
	NODE* pNode;

	for (pNode = hLList->pHead; pNode != NULL; pNode = pNode->pNext)
		if (Compare(&pNode->val))
			return pNode;

	return NULL;
}

BOOL LL_WalkList(HLLIST hLList, BOOL(*Proc) (LL_DATATYPE*))
{
	NODE* pNode;

	for (pNode = hLList->pHead; pNode != NULL; pNode = pNode->pNext)
		if (!Proc(&pNode->val))
			return FALSE;

	return TRUE;
}

BOOL LL_WalkListRev(HLLIST hLList, BOOL(*Proc) (LL_DATATYPE*))
{
	NODE* pNode;

	for (pNode = hLList->pTail; pNode != NULL; pNode = pNode->pPrev)
		if (!Proc(&pNode->val))
			return FALSE;
	return TRUE;
}