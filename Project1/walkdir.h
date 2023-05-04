#pragma once

#include <Windows.h>

BOOL WalkDir(LPCSTR path, BOOL(*f)(const WIN32_FIND_DATA*, int));
