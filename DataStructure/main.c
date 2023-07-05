#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Vector.h"
#include "DoublyLinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "GenericList.h"

BOOL Disp(PERSON* pPerson);
BOOL MyCompare(const PERSON* pPer);
void GetRandomName(char* name, size_t size);


void Vector_Example(void);
void LinkList_Example(void);
void Queue_Example(void);
void Stack_Example(void);
void GenericList_Example(void);

int main(int arc, int* argv[])
{
	//Vector_Example();
	//LinkList_Example();
	//Queue_Example();
	//Stack_Example();
	GenericList_Example();
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

void Queue_Example(void)
{
	HQUEUE hQueue;
	int i;
	QPERSON per;

	srand(time(NULL));

	if ((hQueue = Q_CreateQueue(10)) == NULL) {
		fprintf(stderr, "Cannot create queue!..\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 10; ++i) {
		GetRandomName(per.name, 32);
		per.no = i;
		Q_PutItem(hQueue, &per);
	}

	for (i = 0; i < 5; ++i) {
		Q_GetItem(hQueue, &per);
		printf("%d, %s\n", per.no, per.name);
	}

	printf("---------------\n");

	for (i = 0; i < 5; ++i) {
		GetRandomName(per.name, 32);
		per.no = i * 10;
		Q_PutItem(hQueue, &per);
	}

	while (!Q_IsEmpty(hQueue)) {
		Q_GetItem(hQueue, &per);
		printf("%d, %s\n", per.no, per.name);
	}
}

void Stack_Example(void)
{
	HSTACK hStack;
	SPERSON per;

	if ((hStack = S_CreateStack(10)) == NULL) {
		fprintf(stderr, "Cannot create stack!..\n");
		exit(EXIT_FAILURE);
	}

	strcpy(per.name, "ABC");
	per.no = 123;

	S_Push(hStack, &per);

	strcpy(per.name, "DEF");
	per.no = 123;

	S_Push(hStack, &per);

	strcpy(per.name, "GHI");
	per.no = 976;

	S_Push(hStack, &per);


	while (!S_IsEmpty(hStack)) {
		S_Pop(hStack, &per);
		printf("%s, %d\n", per.name, per.no);
	}

	S_CloseStack(hStack);
}


BOOL DispPerson(GLNODE* pNode);
BOOL DispCity(GLNODE* pNode);

struct GLPERSON {
	char name[32];
	int no;
	GLNODE node;
};

GLNODE g_listPerson = { &g_listPerson, &g_listPerson };

struct GLCITY {
	char city[64];
	GLNODE node;
	int plate;
};

GLNODE g_listCity = { &g_listCity, &g_listCity };

void GenericList_Example(void)
{
	
	struct GLPERSON persons[] = { {"ABC", 10}, {"DEF", 20}, {"GHI", 30}, {"JKL", 40},
		{"MNO", 50}, {"PRS", 60 }, {"TUV", 70}, {"YZA", 80}};

	struct GLCITY cities[] = { {.city = "Bursa", .plate = 16}, {.city = "Eskisehir", .plate = 26}, 
		{.city = "izmir", .plate = 35}, {.city = "istanbul", .plate = 34} };

	for (int i = 0; i < 8; ++i)
		GL_AddItemTail(&g_listPerson, &persons[i].node);

	GL_WalkList(&g_listPerson, DispPerson);

	printf("---------------------\n");

	for (int i = 0; i < 4; ++i)
		GL_AddItemTail(&g_listCity, &cities[i].node);

	GL_WalkList(&g_listCity, DispCity);

}

BOOL DispPerson(GLNODE* pNode)
{
	struct GLPERSON* pperson;

	pperson = container_of(pNode, struct GLPERSON, node);
	printf("%s, %d\n", pperson->name, pperson->no);

	return TRUE;
}

BOOL DispCity(GLNODE* pNode)
{
	struct GLCITY* pcity;

	pcity = container_of(pNode, struct GLCITY, node);
	printf("%s, %d\n", pcity->city, pcity->plate);

	return TRUE;
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

void GetRandomName(char* name, size_t size)
{
	size_t i;

	for (i = 0; i < size - 1; ++i)
		name[i] = 'A' + rand() % 26;
	name[i] = '\0';
}

