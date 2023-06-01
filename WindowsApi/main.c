#include <stdio.h>
#include <string.h>
#include "walkdir.h"
#include "stdint.h"
#include "display_hexchar.h"
#include <stdbool.h>
#include "utils.h"
#include <Psapi.h>
#include <tchar.h>
#include <stdarg.h>
#include <stdlib.h>


#define IS_FILE()	(wfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "<DIR>" : ""

/* Small Talk */
void WalkDir_Example(int argc, char* argv[]);
void DisplayHexChar_Example(void);
void GetStdHandle_Example(void);
void Redirect_stdout_Example(void);
void getchar_Example(void);
void fgetc_Example(void);
void gets_s_Example(void);
void Menu_Example(void);
void DateTime_Example(void);
void GetCurrentDirectory_Example(void);
void GetEnvironmentVariable_Example(void);
void SetEnvironmentVariable_Example(void);
void putenv_Exampe(void);
void GetEnvironmentString_Example(void);
void getenv_Example(void);
void ShellExecute_Example(void);
void ListModuleOfProcesses_Example(void);
void MemoryMappedFiles_Example(void);
void Thread_Example(void);
void CriticalSection_Example(void);
void ConsumerProducer_Example(void);



int APP_DispMenu(void);
bool Callback_WalkDir(const WIN32_FIND_DATA* wfd, int level);
void APP_ErrorHandler(void);
static printerr(const char* format, ...);
static void ExitSys(LPCSTR lpszMsg);

// different approach for error handling
bool o_err_fopen;
bool o_err_hstdin;
bool o_err_hstdout;


int main(int argc, char *argv[])
{
	//WalkDir_Example(argc, argv);
	//DisplayHexChar_Example();
	//GetStdHandle_Example();
	//Redirect_stdout_Example();
	//getchar_Example();
	//fgetc_Example();
	//gets_s_Example();
	//Menu_Example();
	//DateTime_Example();
	//GetCurrentDirectory_Example();
	//GetEnvironmentVariable_Example();
	//SetEnvironmentVariable_Example();
	//putenv_Exampe();
	//GetEnvironmentString_Example();
	//getenv_Example();
	//ShellExecute_Example();
	//ListModuleOfProcesses_Example();
	//MemoryMappedFiles_Example();
	//Thread_Example();
	//CriticalSection_Example();
	ConsumerProducer_Example();




	return 0;
}


void WalkDir_Example(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "enter 2 argument..\n");
		exit(EXIT_FAILURE);
	}
	WalkDir(argv[1], Callback_WalkDir);
}

void DisplayHexChar_Example(void)
{
	FILE* f;
	char buf[BUFSIZ];
	int ch;
	int i;


	o_err_fopen = ((f = fopen("test.txt", "r+")) == NULL);
	APP_ErrorHandler();

	setvbuf(f, buf, _IOFBF, BUFSIZ);

	ch = fgetc(f);

	DisplayHexchar(buf, BUFSIZ, 16);

	fclose(f);
}

void GetStdHandle_Example(void)
{
	HANDLE hstdin, hstdout;
	char buf[4096];
	DWORD dw_read, dw_write;

	o_err_hstdin = (hstdin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE;
	APP_ErrorHandler();
	o_err_hstdout = (hstdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE;
	APP_ErrorHandler();

	if (!ReadFile(hstdin, buf, 4096, &dw_read, NULL))
		ExitSys("ReadFile");

	if (!WriteFile(hstdout, buf, dw_read, &dw_write, NULL))
		ExitSys("WriteFile");
}

void Redirect_stdout_Example(void)
{
	FILE* f;
	int i;

	if ((f = freopen("test.txt", "w", stdout)) == NULL) // stdout has been closed now
	{
		fprintf(stderr, "freopen\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 10; ++i)
		printf("%d\n", i);

	fclose(f);
}

void getchar_Example(void)
{
	int ch;

	ch = getchar();
	printf("%c\n", ch);
	// line buffer and EOF effect
	while ((ch = getchar()) != '\n' && ch != EOF)
		;

	ch = getchar();
	printf("%c\n", ch);
}

void fgetc_Example(void)
{
	int ch;

	// line buffer causes this effect
	while ((ch = fgetc(stdin)) != EOF)
	{
		printf("%c", ch);
	}

}

void gets_s_Example(void)
{
	char s[50];
	puts(my_gets_s(s, 50));
}

void Menu_Example(void)
{
	int option;

	for (;;) 
	{
		option = APP_DispMenu();

		switch (option)
		{
		case 1:
			printf("Perform 1\n\n");
			break;
		case 2:
			printf("Perform 2\n\n");
			break;
		case 3:
			return;
			break;
		default:
			printf("No Section\n\n");
		}
	}
}


int APP_DispMenu(void)
{
	int option;

	printf("1) Section1\n");
	printf("2) Section2\n");
	printf("3) Exit\n\n");
	
	printf("Selection:");

	if (scanf("%d", &option) == 0) 
	{
		clean_stdin_buff(); // clean stdin buffer to prevent malfunction
		return 0;
	}

	return option;
}

void DateTime_Example(void)
{
	int day, month, year;
	int res;

	printf("dd/mm/yy: ");
	fflush(stdin);

	res = scanf("%d/%d/%d", &day, &month, &year);

	if (res == 3)
	{
		printf("%d-%d-%d\n", day, month, year);
	}
	else
	{
		clean_stdin_buff();
		printf("invalid input!\n");
	}
	
}


void GetCurrentDirectory_Example(int argc, char* argv[])
{
	char cwd[1024];
	if (!GetCurrentDirectory(1024, cwd))
	{
		ExitSys("GetCurrentDirectory");
	}
	printf("%s\n", cwd);
}


void GetEnvironmentVariable_Example(void)
{
	enum{SIZE = 4096};
	char arr[SIZE];
	DWORD dw_res;
	if (((dw_res = GetEnvironmentVariable("home", arr, SIZE)) == 0) || (dw_res > SIZE))
	{
		ExitSys("GetEnvironmentVariable");
	}
	
	printf("%s\n", arr);
}

void SetEnvironmentVariable_Example(void)
{
	enum { SIZE = 4096 };
	char arr[SIZE];
	DWORD dw_res;

	if (!SetEnvironmentVariable("CITY", "Bursa"))
	{
		ExitSys("SetEnvironmentVariable");
	}

	if (((dw_res = GetEnvironmentVariable("CITY", arr, SIZE)) == 0) || (dw_res > SIZE))
	{
		ExitSys("GetEnvironmentVariable");
	}

	printf("%s\n", arr);
}

void putenv_Exampe(void)
{
	char* env;
	
	if (_putenv("CITY=Bursa") == -1)
	{
		ExitSys("putenv");
	}
		

	if ((env = getenv("CITY")) == NULL) 
	{
		printerr("CITY");
		//fprintf(stderr, "CITY!..\n");
		exit(EXIT_FAILURE);
	}
	printf("%s", env);
}

void GetEnvironmentString_Example(void)
{
	LPCH envStr;

	if ((envStr = GetEnvironmentStrings()) == NULL) 
	{
		fprintf(stderr, "GetEnvironmentStrings\n");
		exit(EXIT_FAILURE);
	}

	while (*envStr != '\0') {
		puts(envStr);
		envStr += strlen(envStr) + 1;
	}

}

void getenv_Example(void)
{
	char* val;
	char path[4096];
	FILE* f;

	char* env;

	//if (_putenv("DATADIR=C:\\Users\\Duatepe\\Desktop") == -1) // or create a environment variable
	//{
	//	ExitSys("putenv");
	//}


	if ((val = getenv("DATADIR")) != NULL)
	{
		sprintf(path, "%s\\%s", val, "x.dat");
	}	
	else
	{
		sprintf(path, ".\\%s", "x.dat");
		
	}
		
	printf("%s\n", path);

	if ((f = fopen(path, "rb")) == NULL) 
	{
		fprintf(stderr, "cannot open data file!..\n");
		exit(EXIT_FAILURE);
	}

	printf("Ok\n");


	fclose(f);
}

void ShellExecute_Example(void)
{
	HINSTANCE hInstance;

	hInstance = ShellExecute(NULL, "open", "C:\\Users\\Duatepe\\Desktop\\abc.txt", NULL, NULL, SW_NORMAL);
	if ((int)hInstance < 32)
	{
		ExitSys("ShellExecute");
	}
}

// compatible for unicode
void ListModuleOfProcesses_Example(void)
{
	DWORD dwProcessIds[1000];
	DWORD dwNeeded, dwProcessCount, dwModuleCount;
	DWORD i, k;
	HANDLE hProcess;
	HMODULE hModules[1000];
	TCHAR szModuleName[1024];
	DWORD listedCount = 0;

	if (!EnumProcesses(dwProcessIds, sizeof(dwProcessIds), &dwNeeded))
	{
		ExitSys(TEXT("EnumProcesses"));
	}

	dwProcessCount = dwNeeded / sizeof(DWORD);

	for (i = 0; i < dwProcessCount; ++i)
	{
		// get the process
		if ((hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessIds[i])) == NULL)
		{
			continue;
		}

		// get the modules of process - 32bit process cannot read 64bit process memory area 
		// and this function might be failed
		if (!EnumProcessModules(hProcess, hModules, sizeof(hModules), &dwNeeded))
		{
			continue;
		}
	
		++listedCount;
		dwModuleCount = dwNeeded / sizeof(DWORD);

		//get the name of modules
		for (k = 0; k < dwModuleCount; ++k)
		{
			if (!GetModuleBaseName(hProcess, hModules[k], szModuleName, 1024))
			{
				continue;
			}
			if (k == 0) // First one is always executable file(.exe)
			{
				_tprintf(TEXT("%s%s"), szModuleName, dwModuleCount > 1 ? TEXT(": ") : TEXT(""));
			}
			else if (k != 1) // Now print modules 
			{
				_tprintf(TEXT(", "));
			}

			_tprintf(TEXT("%s"), szModuleName);
		}

		_tprintf(TEXT("\n\n"));
		CloseHandle(hProcess);
	}

	_tprintf(TEXT("%lu process Ids listed...\n"), listedCount);
}

void MemoryMappedFiles_Example(void)
{
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID pvAddr;
	LPSTR str;
	int i;
	char text[] = "01234567890123456789";

	if ((hFile = CreateFile("test.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
		ExitSys("CreateFile");

	// mapped the test.txt to memory
	if ((hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0/*mapped whole test file*/, NULL/*this time is NULL*/)) == NULL)
		ExitSys("CreateFileMapping");

	if ((pvAddr = MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0/*from head of file*/, 0)) == NULL)
		ExitSys("MapViewOfFile");

	str = (LPSTR)pvAddr;

	for (i = 0; str[i] != NULL; ++i)
		putchar(str[i]);

	putchar('\n');

	str[i] = '\n';
	memcpy(&str[i + 1], text, strlen(text));

	UnmapViewOfFile(pvAddr);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);

	return 0;
}

DWORD WINAPI ThreadProc1(LPVOID lpvParam);
DWORD WINAPI ThreadProc2(LPVOID lpvParam);

void Thread_Example(void)
{
	HANDLE hThread[2];
	DWORD dwThreadID[2];
	DWORD dwExitCode = 0;
	char* str = NULL;
	char* str1 = NULL;

	if ((str = malloc(64)) == NULL) {
		ExitSys("memory allocation");
	}
	if ((str1 = malloc(64)) == NULL) {
		ExitSys("memory allocation");
	}

	sprintf(str, "Thread1");
	if ((hThread[0] = CreateThread(NULL, 0, ThreadProc1, str, 0, &dwThreadID[0])) == NULL)
		ExitSys("CreateThread");
	 
	sprintf(str1, "Thread2");
	if ((hThread[1] = CreateThread(NULL, 0, ThreadProc2, str1, 0, &dwThreadID[1])) == NULL)
		ExitSys("CreateThread");

	// No equal function in linux, pthread_join is used for that.
	WaitForMultipleObjects(2, hThread, FALSE/*any thread */, INFINITE);

	if (!GetExitCodeThread(hThread[0], &dwExitCode))
		ExitSys("GetExitCodeThread");

	printf("Thread-2 has been finished with exit code %lu\n", (unsigned long)dwExitCode);

}

DWORD WINAPI ThreadProc1(LPVOID lpvParam)
{
	char* str = (char*)lpvParam;
	printf("ThreadProc1 Param: %s\n", str);

	for (int i = 0; i < 5; ++i) {
		printf("ThreadProc1: %d\n", i);
		Sleep(500);
	}

	free(str);
	return 100;
}

DWORD WINAPI ThreadProc2(LPVOID lpvParam)
{
	char* str1 = (char*)lpvParam;
	printf("ThreadProc2 Param: %s\n", str1);

	for (int i = 0; i < 10; ++i) {
		printf("ThreadProc2: %d\n", i);
		Sleep(500);
	}

	free(str1);

	return 200;
}

DWORD WINAPI ThreadProc3(LPVOID lpvParam);
DWORD WINAPI ThreadProc4(LPVOID lpvParam);

// mutex can be used for communication between process, 
CRITICAL_SECTION g_flag;
int g_cnt;
void CriticalSection_Example(void)
{
	HANDLE hThread[2];
	DWORD dwThreadID[2];
	DWORD dwExitCode = 0;
	char* str = NULL;
	char* str1 = NULL;

	if ((str = malloc(64)) == NULL) {
		ExitSys("memory allocation");
	}
	if ((str1 = malloc(64)) == NULL) {
		ExitSys("memory allocation");
	}

	InitializeCriticalSection(&g_flag);

	sprintf(str, "Thread3");
	if ((hThread[0] = CreateThread(NULL, 0, ThreadProc3, str, 0, &dwThreadID[0])) == NULL)
		ExitSys("CreateThread");

	sprintf(str1, "Thread4");
	if ((hThread[1] = CreateThread(NULL, 0, ThreadProc4, str1, 0, &dwThreadID[1])) == NULL)
		ExitSys("CreateThread");

	// No equal function in linux, pthread_join is used for that.
	WaitForMultipleObjects(2, hThread, TRUE/*any thread */, INFINITE);

	if (!GetExitCodeThread(hThread[0], &dwExitCode))
		ExitSys("GetExitCodeThread");

	DeleteCriticalSection(&g_flag);

	printf("%d\n", g_cnt);

	printf("Thread-2 has been finished with exit code %lu\n", (unsigned long)dwExitCode);

}

DWORD WINAPI ThreadProc3(LPVOID lpvParam)
{
	char* str = (char*)lpvParam;
	printf("ThreadProc3 Param: %s\n", str);

	for (int i = 0; i < 1000; ++i) {
		EnterCriticalSection(&g_flag);
		++g_cnt;
		LeaveCriticalSection(&g_flag);
	}
	
	free(str);
	return 100;
}

DWORD WINAPI ThreadProc4(LPVOID lpvParam)
{
	char* str1 = (char*)lpvParam;
	printf("ThreadProc4 Param: %s\n", str1);

	
	for (int i = 0; i < 1000; ++i) {
		EnterCriticalSection(&g_flag);
		++g_cnt;
		LeaveCriticalSection(&g_flag);
	}
	

	free(str1);

	return 200;
}

DWORD WINAPI ThreadProducer(LPVOID lpvParam);
DWORD WINAPI ThreadConsumer(LPVOID lpvParam);

HANDLE g_hSemProducer;
HANDLE g_hSemConsumer;
int g_shared;

void ConsumerProducer_Example(void)
{
	HANDLE hThreadProducer, hThreadConsumer;
	DWORD dwThreadIDProducer, dwThreadIDConsumer;

	srand(time(NULL));

	if ((g_hSemProducer = CreateSemaphore(NULL, 1, 1, NULL)) == NULL)
		ExitSys("CreateSemaphore");

	if ((g_hSemConsumer = CreateSemaphore(NULL, 0, 1, NULL)) == NULL)
		ExitSys("CreateSemaphore");

	if ((hThreadProducer = CreateThread(NULL, 0, ThreadProducer, NULL, 0, &dwThreadIDProducer)) == NULL)
		ExitSys("CreateThread");

	if ((hThreadConsumer = CreateThread(NULL, 0, ThreadConsumer, NULL, 0, &dwThreadIDConsumer)) == NULL)
		ExitSys("CreateThread");

	WaitForSingleObject(hThreadProducer, INFINITE);
	WaitForSingleObject(hThreadConsumer, INFINITE);

	CloseHandle(g_hSemProducer);
	CloseHandle(g_hSemConsumer);
}


DWORD WINAPI ThreadProducer(LPVOID lpvParam)
{
	int i;

	i = 0;
	for (;;) {
		Sleep(rand() % 300);

		WaitForSingleObject(g_hSemProducer, INFINITE);
		g_shared = i;
		ReleaseSemaphore(g_hSemConsumer, 1, NULL);

		++i;
		if (i == 100)
			break;
	}
	return 0;
}

DWORD WINAPI ThreadConsumer(LPVOID lpvParam)
{
	int val;

	for (;;) {
		WaitForSingleObject(g_hSemConsumer, INFINITE);
		val = g_shared;
		ReleaseSemaphore(g_hSemProducer, 1, NULL);

		Sleep(rand() % 200);
		printf("%d ", val);
		fflush(stdout);
		if (val == 99)
			break;
	}
	putchar('\n');
	return 0;
}


bool Callback_WalkDir(const WIN32_FIND_DATA* wfd, int level)
{
	printf("%*s%s %s\n", level * 4, "", wfd->cFileName, IS_FILE());
	if (!_strcmpi(wfd->cFileName, "MD5"))
	{
		return FALSE;
	}
	return TRUE;
}






void APP_ErrorHandler(void)
{
	o_err_fopen && (fprintf(stderr, "error fopen..."), exit(EXIT_FAILURE), 0);
	o_err_hstdin && (ExitSys("error stdin"), 0);
	o_err_hstdout && (ExitSys("error stdout"), 0);
}


static printerr(const char* format, ...)
{
	va_list va;

	va_start(va, format);

	fprintf(stderr, "Error: ");
	vfprintf(stderr, format, va);

	va_end(va);
}

static void ExitSys(LPCTSTR lpszMsg)
{
	DWORD dwLastError = GetLastError();
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPCTSTR)&lpszErr, 0, NULL))
	{
		_ftprintf(stderr, TEXT("%s: %s\n"), lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(EXIT_FAILURE);
}





