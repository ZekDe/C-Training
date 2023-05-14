#include <stdio.h>
#include <string.h>
#include "walkdir.h"
#include "stdint.h"
#include "display_hexchar.h"
#include <stdbool.h>
#include "utils.h"
#include <Psapi.h>
#include <tchar.h>

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



int APP_DispMenu(void);
bool Callback_WalkDir(const WIN32_FIND_DATA* wfd, int level);
void APP_ErrorHandler(void);
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
	ListModuleOfProcesses_Example();

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
		fprintf(stderr, "freopen!..\n");
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
		fprintf(stderr, "CITY!..\n");
		exit(EXIT_FAILURE);
	}
	printf("%s",env);
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





