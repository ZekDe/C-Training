#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void ExitSys(LPCSTR lpszMsg);

int main(int argc, char *argv[])
{
	HANDLE hReadPipe;
	BOOL bResult;
	DWORD dwRead;
	int val;

	sscanf(argv[1], "%p", &hReadPipe);

	while (ReadFile(hReadPipe, &val, sizeof(int), &dwRead, NULL)) 
	{
		printf("%d ", val);
		fflush(stdout);
	}

	if (GetLastError() != ERROR_BROKEN_PIPE)
		ExitSys("ReadFile");

	CloseHandle(hReadPipe);
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