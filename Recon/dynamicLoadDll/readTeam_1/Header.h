#pragma once
#include <Windows.h>
HMODULE WINAPI GetModuleBaseAddress(LPCWSTR moduleName);
void* WINAPI GetExportAddress(HMODULE hMod, const char* lpProcName);
HMODULE WINAPI LoadLibraryAF(const char* loadedDllName);

void* WINAPI GetProcAddressF(HMODULE hDll, const char* ProcName);
int atoiF(char* p);