#ifndef GENERICLIST_H
#define GENERICLIST_H

#include <stddef.h>

#define FALSE		0
#define TRUE		1

typedef int BOOL;

typedef struct tagGLNODE {
	struct tagGLNODE* pNext;
	struct tagGLNODE* pPrev;
} GLNODE;

GLNODE* GL_AddItemHead(GLNODE* pList, GLNODE* pNewNode);
GLNODE* GL_AddItemTail(GLNODE* pList, GLNODE* pNewNode);
GLNODE* GL_InsertItemNext(GLNODE* pInsertNode, GLNODE* pNewNode);
GLNODE* GL_InsertItemPrev(GLNODE* pInsertNode, GLNODE* pNewNode);
void GL_DeleteItem(GLNODE* pDeleteNode);
BOOL GL_WalkList(GLNODE* pList, BOOL(*Proc) (GLNODE*));
BOOL GL_WalkListRev(GLNODE* pList, BOOL(*Proc) (GLNODE*));

#define container_of(ptr, type, member)			((type *) ((char *) (ptr) - offsetof(type, member)))

#endif


