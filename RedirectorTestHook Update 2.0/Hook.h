#include <Windows.h>
#include <iostream>
#include "Functions.h"
#include <cstdint>
#include <vector>

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

BYTE* pointerPatchJmpBack = 0;


DWORD PatcherValue = 0;

DWORD PatcherValue2 = 0;

DWORD* TargetPtrAddr = 0;


DWORD WINAPI FixReferences();

__declspec(naked) void RedirectReferences()
{	
	__asm pushad
	__asm pushfd
	
	__asm mov PatcherValue, eax
	__asm mov PatcherValue2, edx
	__asm mov TargetPtrAddr, esi
	
	__asm call FixReferences
	
	__asm popfd
	__asm popad
	
	__asm mov eax, [ebx]
	__asm cmp dword ptr [eax], 0xFFFFFFFF

	
	
	__asm jmp pointerPatchJmpBack
	
	
}