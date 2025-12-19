#include <Windows.h>
#include <iostream>
#include "Functions.h"
#include <cstdint>

BYTE* parserJmpBack = 0;

BYTE* FileHeaderRegister = 0;
DWORD FileComponentRegister = 0;

DWORD FilenameHash = 0;

DWORD HookCount = 0;

DWORD WINAPI OverwriteValues();

__declspec(naked) void RedirectEntries()
{	
	__asm pushad
	__asm pushfd
	
	__asm inc HookCount
	
	__asm mov FileHeaderRegister, edi
	__asm mov FileComponentRegister, eax
	
	__asm mov ecx, [edi+0x4]
	__asm mov FilenameHash, ecx
	
	__asm call OverwriteValues
	
	__asm popfd
	__asm popad
	
	__asm add [esp+0x14], 4
	
	
	
	__asm jmp parserJmpBack
	
	
}