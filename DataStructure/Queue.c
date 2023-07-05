#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

HQUEUE Q_CreateQueue(size_t qsize)
{
	HQUEUE hQueue;

	if ((hQueue = (HQUEUE)malloc(sizeof(QUEUE))) == NULL)
		return NULL;

	if ((hQueue->pQueue = (QDATATYPE*)malloc(qsize * sizeof(QDATATYPE))) == NULL) {
		free(hQueue);
		return NULL;
	}
	hQueue->head = hQueue->tail = hQueue->count = 0;
	hQueue->qsize = qsize;

	return hQueue;
}

BOOL Q_PutItem(HQUEUE hQueue, const QDATATYPE* pVal)
{
	if (hQueue->count == hQueue->qsize)
		return FALSE;

	hQueue->pQueue[hQueue->tail++] = *pVal;
	hQueue->tail = hQueue->tail % hQueue->qsize;
	++hQueue->count;

	return TRUE;
}

BOOL Q_GetItem(HQUEUE hQueue, QDATATYPE* pVal)
{
	if (hQueue->count == 0)
		return FALSE;

	*pVal = hQueue->pQueue[hQueue->head++];
	hQueue->head = hQueue->head % hQueue->qsize;
	--hQueue->count;

	return TRUE;
}

void Q_Clear(HQUEUE hQueue)
{
	hQueue->head = hQueue->tail = hQueue->count = 0;
}

void Q_CloseQueue(HQUEUE hQueue)
{
	free(hQueue->pQueue);
	free(hQueue);
}