#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vector.h"
#include "DoublyLinkedList.h"

BOOL Disp(PERSON* pPerson);
BOOL MyCompare(const PERSON* pPer);


void Vector_Example(void);
void LinkList_Example(void);

int main(int arc, int* argv[])
{
	//Vector_Example();
	LinkList_Example();
}

void Vector_Example(void)
{
	HVector hDArray;
	LL_DATATYPE val;
	int i;

	if ((hDArray = CreateVectorWithCapacity(30)) == NULL) {
		fprintf(stderr, "Cannot create dynamic array!..\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 30; ++i)
		AddItem(hDArray, i);

	for (size_t i = 0; i < GetSize(hDArray); ++i)
		printf("%d ", GetItem(hDArray, i));

	printf("\nDelete Item 20 and Insert 99\n");
	InsertItem(hDArray, DeleteItem(hDArray, 20), 99); // 20'nin yerine 99 ekle
	
	printf("FindItem(hdArray, 7) and change value with %d\n", *FindItem(hDArray, 7) = 77);

	for (size_t i = 0; i < GetSize(hDArray); ++i)
		printf("%d ", GetItem(hDArray, i));
	printf("\n");

	

	printf("Capacity: %lu, Size: %lu\n", (unsigned long)GetCapacity(hDArray), (unsigned long)GetSize(hDArray));
	TrimToSize(hDArray);
	printf("Capacity: %lu, Size: %lu\n", (unsigned long)GetCapacity(hDArray), (unsigned long)GetSize(hDArray));
	Clear(hDArray);
	printf("Capacity: %lu, Size: %lu\n", (unsigned long)GetCapacity(hDArray), (unsigned long)GetSize(hDArray));

	CloseVector(hDArray);
}

void LinkList_Example(void)
{
	HLLIST hLList;
	PERSON per, * pper;
	NODE* pNode;

	if ((hLList = LL_CreateLList()) == NULL) {
		fprintf(stderr, "Cannot create linked list!..\n");
		exit(EXIT_FAILURE);
	}

	strcpy(per.name, "ABC");
	per.no = 123;

	LL_AddItemTail(hLList, &per);


	strcpy(per.name, "CDE");
	per.no = 678;

	LL_AddItemTail(hLList, &per);

	strcpy(per.name, "FGH");
	per.no = 378;

	LL_AddItemTail(hLList, &per);

	strcpy(per.name, "IJK");
	per.no = 845;

	LL_AddItemTail(hLList, &per);


	LL_WalkList(hLList, Disp);

	printf("------------------------\n");

	strcpy(per.name, "FGH");
	per.no = 845;

	if ((pper = LL_FindItem(hLList, MyCompare)) != NULL)
		printf("Name: %s, No: %d\n", pper->name, pper->no);
	else
		printf("Cannot find!..\n");

}


BOOL MyCompare(const PERSON* pPer)
{
	if (strcmp(pPer->name, "FGH") == 0)
		return TRUE;

	return FALSE;
}
BOOL Disp(PERSON* pPerson)
{
	printf("%s, %d\n", pPerson->name, pPerson->no);

	return TRUE;
}
