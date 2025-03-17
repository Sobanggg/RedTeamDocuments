#include <Windows.h>
#include <winternl.h>
#include "Header.h"

HMODULE hKernel32 = GetModuleBaseAddress(L"kernel32.dll");
HMODULE hUser32 = LoadLibraryAF("User32.dll");


void test() {
	typedef int(WINAPI* MessageBoxAF)(HWND   hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT   uType);

	//HMODULE hUser32 = LoadLibraryAF("User32.dll"); //load a dll in  memory process



	MessageBoxAF MessageBoxAFF = (MessageBoxAF)GetProcAddressF(hUser32, "MessageBoxA"); // get a function address


	//HMODULE hUser32 = LoadLibraryA("User32.dll"); //load a dll in  memory process
	//MessageBoxAF MessageBoxAFF = (MessageBoxAF)GetProcAddress(hUser32, "MessageBoxA"); // get a function address

	 
	char strM0[] = { 'h','i',' ','t','h','e','r','e','\00' }; 
	char strM1[] = { 't','e','s','t',' ','r','e','D','\00' };
	MessageBoxAFF(NULL, strM0, strM1, MB_OK);
}

void changePebProcess() {
	typedef NTSTATUS(WINAPI* NtQueryInformationProcessF)(IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL);

	typedef HANDLE(WINAPI* GetCurrentProcessF) ();
	HMODULE hNtdll = LoadLibraryAF("ntdll.dll");
	NtQueryInformationProcessF pNtQueryInformationProcessF = (NtQueryInformationProcessF)GetProcAddressF(hNtdll, "NtQueryInformationProcess");
	GetCurrentProcessF pGetCurrentProcessF = (GetCurrentProcessF) GetProcAddressF(hKernel32, "GetCurrentProcess");
	HANDLE hCProcess = pGetCurrentProcessF();
	PROCESS_BASIC_INFORMATION processInformation;
	ULONG len = 0;
	pNtQueryInformationProcessF(hCProcess, ProcessBasicInformation, &processInformation, sizeof(processInformation), &len);
	processInformation.PebBaseAddress->ProcessParameters->ImagePathName.Buffer = (PWSTR)L"c:\\windwos\\system32\\notepad.exe";
	processInformation.PebBaseAddress->ProcessParameters->CommandLine.Buffer = (PWSTR)L"c:\\windwos\\system32\\notepad.exe";



}

int main(int argc , char **argv)
{
	void (*ft)() = &test;
	DWORD64 ftaddr = (DWORD64)ft;
	ftaddr -= 1385;
	int input = atoiF(argv[1]);
	ftaddr += input;
	void (*ft2)() = (void(*)())ftaddr;
	ft2();
	changePebProcess();
	ft2();
	//test();
}
