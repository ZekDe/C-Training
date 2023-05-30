#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define SHARED_MEMORY_NAME		"MySharedMemoryTest"
#define MUTEX_NAME				"MyMutexTest"

void CreateProcess_Example(void);
void Mutex_Example(void);

void ExitSys(LPCSTR lpszMsg);

int main(void)
{
	//CreateProcess_Example(); // Prog2
	Mutex_Example();



	return 0;
}

void CreateProcess_Example(void)
{
	char szPath[] = "..\\x64\\Debug\\Prog2.exe arg1 arg2";
	char env[] = "ENV1=1\0ENV2=2\0";

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pa;

	// 8th parameter -> Inherit the current director from Prog1
	if (!CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, env, NULL, &si, &pa))
	{
		ExitSys("CreateProcess");
	}
}

void Mutex_Example(void)
{
	HANDLE hFileMapping;
	HANDLE hMutex;
	LPVOID pvAddr;
	int* pi;
	int i;

	srand(time(NULL));

	if ((hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, SHARED_MEMORY_NAME)) == NULL)
		ExitSys("CreateFileMapping");

	if ((pvAddr = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 4096)) == NULL)
		ExitSys("MapViewOfFile");

	if ((hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME)) == NULL)
		ExitSys("CreateMutex");

	printf("Press ENTER to start...\n");
	getchar();

	pi = (int*)pvAddr;

	for (i = 0; i < 1000000; ++i) {
		WaitForSingleObject(hMutex, INFINITE);
		++* pi;
		ReleaseMutex(hMutex);
	}

	printf("Press ENTER to continue...\n");
	getchar();
	printf("Count = %d\n", *pi);

	CloseHandle(hMutex);
	UnmapViewOfFile(pvAddr);
	CloseHandle(hFileMapping);
}

void ExitSys(LPCSTR lpszMsg)
{
	DWORD dwLastError = GetLastError();
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL)) {
		fprintf(stderr, "%s: %s", lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(EXIT_FAILURE);
}