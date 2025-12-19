#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

void MsgBoxOmar (DWORD omar)
{
	char szBuffer[1024];
	sprintf(szBuffer, "Omar: %02x", omar);
	MessageBox(NULL, szBuffer, "Injected", MB_OK);
}

void PlaceJMP(BYTE *Address, DWORD jumpTo, DWORD length)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;
	
	VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	
	dwRelAddr = (DWORD) (jumpTo - (DWORD) Address - 5);
	
	*Address = 0xE9;
	
	*(DWORD *)(Address + 0x1) = dwRelAddr;
	
	for (DWORD x = 0x5; x < length; x++) 
		*(Address + x) = 0x90;
	
	VirtualProtect(Address, length, dwOldProtect, &dwBkup);
}

MODULEINFO GetModuleInfo(char * module)
{
	MODULEINFO modInfo = {0};
	HMODULE hModule = GetModuleHandle(module);
	
	if(hModule == 0)
		return modInfo;
	
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
	return modInfo;
}

DWORD FindPattern(char* module, char* pattern, char* mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);
	
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;
	
	DWORD patternLength = (DWORD)strlen(mask);
	
	for(DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
	
		for(DWORD j = 0; j < patternLength; j++)
			found &=mask[j] == '?' || pattern[j] == *(char*)(base+i+j);
		
		if(found)
			return base+i;
		
	}
	
	return 0;
	
}