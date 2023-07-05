#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/* Symbolic Constants */

#define FALSE		0
#define TRUE		1

/* Type Declarations */

typedef int BOOL;

typedef struct tagQPERSON {
	char name[32];
	int no;
} QPERSON;

typedef QPERSON QDATATYPE;

typedef struct tagQUEUE {
	QDATATYPE* pQueue;
	size_t qsize;
	size_t head;
	size_t tail;
	size_t count;
} QUEUE, * HQUEUE;

/* Function Prototypes */

HQUEUE Q_CreateQueue(size_t qsize);
BOOL Q_PutItem(HQUEUE hQueue, const QDATATYPE* pVal);
BOOL Q_GetItem(HQUEUE hQueue, QDATATYPE* pVal);
void Q_Clear(HQUEUE hQueue);
void Q_CloseQueue(HQUEUE hQueue);

#define Q_GetCount(hQueue)	((hQueue)->count)
#define Q_GetSize(hQueue)		((hQueue)->qsize)
#define Q_IsEmpty(hQueue)		(!(hQueue)->count)


#endif
