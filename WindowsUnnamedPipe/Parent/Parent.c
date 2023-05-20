#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void ExitSys(LPCSTR lpszMsg);

int main(void)
{
	HANDLE hReadPipe, hWritePipe;
	char szArgs[1024];
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pa;
	int i;
	DWORD dwWritten;

	if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0))
		ExitSys("CreatePipe");

	// pass the read-handle to child, beacuse child will be used to read
	// in linux system, default is inheritable compared to windows
	if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT))
		ExitSys("CreatePipe");

	sprintf(szArgs, "Child.exe %p", hReadPipe);

	if (!CreateProcess(NULL, szArgs, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pa))
		ExitSys("CreateProcess");

	CloseHandle(hReadPipe);

	for (i = 0; i < 100; ++i)
		if (!WriteFile(hWritePipe, &i, sizeof(int), &dwWritten, NULL))
			ExitSys("WriteFile");

	CloseHandle(hWritePipe);

	WaitForSingleObject(pa.hProcess, INFINITE);// wait until child terminated

	return 0;
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