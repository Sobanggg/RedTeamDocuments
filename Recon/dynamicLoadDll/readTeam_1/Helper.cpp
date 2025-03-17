#include <Windows.h>
#include <winternl.h>
#include <malloc.h>
#include <tlhelp32.h>

#ifdef _M_AMD64
#include <intrin.h>
#endif

#ifdef _M_IX86 
static __inline PEB __declspec(naked) __forceinline* GetPEBx86()
{
	__asm
	{
		mov eax, dword ptr fs : [0x30] ;
		retn;
	}
}
#endif

HMODULE WINAPI GetModuleBaseAddress(LPCWSTR moduleName)
{
	PEB* pPeb = NULL;
	LIST_ENTRY* pListEntry = NULL;
	LDR_DATA_TABLE_ENTRY* pLdrDataTableEntry = NULL;

#ifdef _M_IX86 
	pPeb = GetPEBx86();
#elif defined(_M_AMD64)
	pPeb = (PPEB)__readgsqword(0x60);
#endif

	if (pPeb == NULL)
		return NULL;

	pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)pPeb->Ldr->InMemoryOrderModuleList.Flink;
	pListEntry = pPeb->Ldr->InMemoryOrderModuleList.Flink;

	do
	{
		if (lstrcmpiW(pLdrDataTableEntry->FullDllName.Buffer, moduleName) == 0)
			return (HMODULE)pLdrDataTableEntry->Reserved2[0];

		pListEntry = pListEntry->Flink;
		pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)(pListEntry->Flink);

	} while (pListEntry != pPeb->Ldr->InMemoryOrderModuleList.Flink);

	return NULL;
}
void* WINAPI GetExportAddress(HMODULE hMod, const char* lpProcName)
{
	char* pBaseAddress = (char*)hMod;

	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)pBaseAddress;
	IMAGE_NT_HEADERS* pNtHeaders = (IMAGE_NT_HEADERS*)(pBaseAddress + pDosHeader->e_lfanew);
	IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNtHeaders->OptionalHeader;
	IMAGE_DATA_DIRECTORY* pDataDirectory = (IMAGE_DATA_DIRECTORY*)(&pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	IMAGE_EXPORT_DIRECTORY* pExportDirectory = (IMAGE_EXPORT_DIRECTORY*)(pBaseAddress + pDataDirectory->VirtualAddress);

	void** ppFunctions = (void**)(pBaseAddress + pExportDirectory->AddressOfFunctions);
	WORD* pOrdinals = (WORD*)(pBaseAddress + pExportDirectory->AddressOfNameOrdinals);
	ULONG* pNames = (ULONG*)(pBaseAddress + pExportDirectory->AddressOfNames);

	void* pAddress = NULL;

	typedef HMODULE(WINAPI* LoadLibraryAF)(LPCSTR lpFileName);
	LoadLibraryAF pLoadLibraryA = NULL;

	DWORD i;

	if (((DWORD_PTR)lpProcName >> 16) == 0)
	{
		WORD ordinal = LOWORD(lpProcName);
		DWORD dwOrdinalBase = pExportDirectory->Base;

		if (ordinal < dwOrdinalBase || ordinal >= dwOrdinalBase + pExportDirectory->NumberOfFunctions)
			return NULL;

		pAddress = (FARPROC)(pBaseAddress + (DWORD_PTR)ppFunctions[ordinal - dwOrdinalBase]);
	}
	else
	{
		for (i = 0; i < pExportDirectory->NumberOfNames; i++)
		{
			char* szName = (char*)pBaseAddress + (DWORD_PTR)pNames[i];
			if (strcmp(lpProcName, szName) == 0)
			{
				pAddress = (FARPROC)(pBaseAddress + ((ULONG*)(pBaseAddress + pExportDirectory->AddressOfFunctions))[pOrdinals[i]]);
				break;
			}
		}
	}

	if ((char*)pAddress >= (char*)pExportDirectory && (char*)pAddress < (char*)pExportDirectory + pDataDirectory->Size)
	{
		char* szDllName, * szFunctionName;
		HMODULE hForward;

		szDllName = _strdup((const char*)pAddress);
		if (!szDllName)
			return NULL;

		pAddress = NULL;
		szFunctionName = strchr(szDllName, '.');
		*szFunctionName++ = 0;

		pLoadLibraryA = (LoadLibraryAF)GetExportAddress(GetModuleBaseAddress(L"KERNEL32.DLL"), "LoadLibraryA");

		if (pLoadLibraryA == NULL)
			return NULL;

		hForward = pLoadLibraryA(szDllName);
		free(szDllName);

		if (!hForward)
			return NULL;

		pAddress = GetExportAddress(hForward, szFunctionName);
	}

	return pAddress;
}
HMODULE WINAPI LoadLibraryAF( const char* loadedDllName) {
	HMODULE hKernel32 = GetModuleBaseAddress(L"kernel32.dll");
	typedef HMODULE(WINAPI* LoadLibraryAF)(LPCSTR lpLibFileName);
	typedef FARPROC(WINAPI* GetProcAddressF)(HMODULE hModule, LPCSTR  lpProcName);
	LoadLibraryAF pLoadLibraryAF = (LoadLibraryAF)GetExportAddress(hKernel32, "LoadLibraryA");
	HMODULE hLoadedDll = pLoadLibraryAF(loadedDllName); //load a dll in  memory process
	return hLoadedDll;

}
void* WINAPI GetProcAddressF(HMODULE hDll ,const char* ProcName  ) {
	typedef FARPROC(WINAPI* GetProcAddressF)(HMODULE hModule, LPCSTR  lpProcName);
	GetProcAddressF pGetProcAddressF = (GetProcAddressF)GetExportAddress(hDll, ProcName);
	return pGetProcAddressF;
}

int atoiF (char* p) {
	int k = 0;
	while (*p) {
		k = (k << 3) + (k << 1) + (*p) - '0';
		p++;
	}
	return k;
}