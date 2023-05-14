#include <stdio.h>
#include <Windows.h>

void GetEnvironmentString_Example(void);

void ExitSys(LPCSTR lpszMsg);

int main(int argc, char *argv[])
{

	char cwd[1024];
	if(!GetCurrentDirectory(1024, cwd))
	{
		ExitSys("GetCurrentDirectory");
	}

	printf("Current Dir: %s\n", cwd);

	for (int i = 0; i < argc; ++i)
	{
		printf("arg[%d] = %s\n", i, argv[i]);
	}

	GetEnvironmentString_Example();

	return 0;
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