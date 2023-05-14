#include "walkdir.h"
#include <stdio.h>
#include <string.h>
#include <io.h>

static void WalkDir_ErrorHandler(LPCSTR lpszMsg);

BOOL WalkDirRecur(LPCSTR path, BOOL(*f)(const WIN32_FIND_DATA*, int), int level)
{
	HANDLE hfind;
	WIN32_FIND_DATA wfd;
	BOOL retval = TRUE;
	int state = 0;


	if (!SetCurrentDirectory(path))
	{
		WalkDir_ErrorHandler("SetCurrentDirectory");
		return FALSE;
	}

	if ((hfind = FindFirstFile("*.*", &wfd)) == INVALID_HANDLE_VALUE)
	{
		SetCurrentDirectory("FindFirstFile");
		return FALSE;
	}

	do
	{
		if (!f(&wfd, level))
		{
			retval = FALSE;
			break;
		}

		if(!strcmp(wfd.cFileName, ".") || !strcmp(wfd.cFileName, ".."))
		{
			continue;
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!WalkDirRecur(wfd.cFileName, f, level+1))
			{
				retval = FALSE;
				break;
			}
			if (!SetCurrentDirectory("..")) // if it is not directory, go to parent directory
			{
				WalkDir_ErrorHandler("..");
				retval = FALSE;
				break;
			}
		}

	} while (FindNextFile(hfind, &wfd));

	FindClose(hfind);
	return retval; 

}

BOOL WalkDir(LPCSTR path, BOOL(*f)(const WIN32_FIND_DATA*, int))
{
	char cwd[MAX_PATH] = "";
	BOOL retval;
	// check the path
	if (_access(path, 0))
	{
		fprintf(stderr, "path not found: %s\n", path);
		return FALSE;
	}
	// hold the current path
	if (!GetCurrentDirectory(MAX_PATH, cwd))
	{
		WalkDir_ErrorHandler("GetCurrentDirectory");
		return FALSE;
	}
	printf("\n\n%s\n\n", cwd);
	retval = WalkDirRecur(path, f, 0); // start scanning

	// set the start path
	if (!SetCurrentDirectory(cwd))
	{
		WalkDir_ErrorHandler("SetCurrentDirectory First");
		return FALSE;
	}

	return retval;
}


static void WalkDir_ErrorHandler(LPCSTR lpszMsg)
{
	DWORD dwLastError = GetLastError();
	LPTSTR lpszErr;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL))
	{
		fprintf(stderr, "%s: %s\n", lpszMsg, lpszErr);
		LocalFree(lpszErr);
	}
}