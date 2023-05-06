#include <stdio.h>
#include <string.h>
#include "walkdir.h"
#include "stdint.h"
#include "display_hexchar.h"
#include <stdbool.h>
#include "utils.h"

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


static void ExitSys(LPCSTR lpszMsg)
{
	DWORD dwLastError = GetLastError();
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL))
	{
		fprintf(stderr, "%s: %s\n", lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}

	exit(EXIT_FAILURE);
}





