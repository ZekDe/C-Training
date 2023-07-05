#include <stdio.h>
#include <stdlib.h>
#include "GenericList.h"

GLNODE* GL_AddItemHead(GLNODE* pList, GLNODE* pNewNode)
{
	return GL_InsertItemNext(pList, pNewNode);
}

GLNODE* GL_AddItemTail(GLNODE* pList, GLNODE* pNewNode)
{
	return GL_InsertItemPrev(pList, pNewNode);
}

GLNODE* GL_InsertItemNext(GLNODE* pInsertNode, GLNODE* pNewNode)
{
	pNewNode->pPrev = pInsertNode;
	pNewNode->pNext = pInsertNode->pNext;
	pInsertNode->pNext->pPrev = pNewNode;
	pInsertNode->pNext = pNewNode;

	return pNewNode;
}

GLNODE* GL_InsertItemPrev(GLNODE* pInsertNode, GLNODE* pNewNode)
{
	pInsertNode->pPrev->pNext = pNewNode;
	pNewNode->pNext = pInsertNode;
	pNewNode->pPrev = pInsertNode->pPrev;
	pInsertNode->pPrev = pNewNode;

	return pNewNode;
}

void GL_DeleteItem(GLNODE* pDeleteNode)
{
	pDeleteNode->pPrev->pNext = pDeleteNode->pNext;
	pDeleteNode->pNext->pPrev = pDeleteNode->pPrev;
}

BOOL GL_WalkList(GLNODE* pList, BOOL(*Proc) (GLNODE*))
{
	GLNODE* pNode;

	for (pNode = pList->pNext; pNode != pList; pNode = pNode->pNext)
		if (!Proc(pNode))
			return FALSE;

	return TRUE;
}

BOOL GL_WalkListRev(GLNODE* pList, BOOL(*Proc) (GLNODE*))
{
	GLNODE* pNode;

	for (pNode = pList->pPrev; pNode != pList; pNode = pNode->pPrev)
		if (!Proc(pNode))
			return FALSE;

	return TRUE;
}