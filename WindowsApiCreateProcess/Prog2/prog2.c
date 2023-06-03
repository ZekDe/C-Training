#include <stdio.h>
#include <Windows.h>
#include <time.h>

#define SHARED_MEMORY_NAME		"MySharedMemoryTest"
#define MUTEX_NAME				"MyMutexTest"

void CreateProcess_Example(void);
void GetEnvironmentString_Example(void);
void Mutex_Example(void);
void Consumer_Example(void); // Prog2 (Producer_Example)

void ExitSys(LPCSTR lpszMsg);

int main(int argc, char *argv[])
{
	//CreateProcess_Example(argc, argv); //Prog1
	//GetEnvironmentString_Example();
	//Mutex_Example();
	Consumer_Example();


	return 0;
}

void CreateProcess_Example(int argc, char* argv[])
{
	char cwd[1024];
	if (!GetCurrentDirectory(1024, cwd))
	{
		ExitSys("GetCurrentDirectory");
	}

	printf("Current Dir: %s\n", cwd);

	for (int i = 0; i < argc; ++i)
	{
		printf("arg[%d] = %s\n", i, argv[i]);
	}
}

void GetEnvironmentString_Example(void)
{
	LPCH envStr;

	if ((envStr = GetEnvironmentStrings()) == NULL)
	{
		fprintf(stderr, "GetEnvironmentStrings\n");
		exit(EXIT_FAILURE);
	}

	while (*envStr != '\0') 
	{
		puts(envStr);
		envStr += strlen(envStr) + 1;
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

void Consumer_Example(void)
{
	HANDLE hFileMapping;
	LPVOID pvAddr;
	int* pshared;
	HANDLE hSemProducer;
	HANDLE hSemConsumer;
	int val;

	srand(time(NULL));
	if ((hSemProducer = CreateSemaphore(NULL, 1, 1, "MyProducerSemaphore")) == NULL)
		ExitSys("CreateFileMapping");

	if ((hSemConsumer = CreateSemaphore(NULL, 0, 1, "MyConsumerSemaphore")) == NULL)
		ExitSys("CreateFileMapping");

	// Create shared memory
	if ((hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, "MySharedMemory")) == NULL)
		ExitSys("CreateFileMapping");

	if ((pvAddr = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 4096)) == NULL)
		ExitSys("MapViewOfFile");

	pshared = (int*)pvAddr;

	for (;;) {
		WaitForSingleObject(hSemConsumer, INFINITE);
		val = *pshared;
		ReleaseSemaphore(hSemProducer, 1, NULL);
		Sleep(rand() % 200);
		printf("%d ", val), fflush(stdout);
		if (val == 99)
			break;
	}
	putchar('\n');

	UnmapViewOfFile(pvAddr);
	CloseHandle(hFileMapping);
	CloseHandle(hSemProducer);
	CloseHandle(hSemConsumer);
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