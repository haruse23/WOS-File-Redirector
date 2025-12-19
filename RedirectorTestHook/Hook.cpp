#include "Hook.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

DWORD parserAPKF = 0;

DWORD InitiateHook()
{	

	if (parserAPKF) return parserAPKF;
	
	DWORD parserAPKF = FindPattern("Spider-Man Web of Shadows.exe", "\x83\x44\x24\x14\x04\x83\xC0\x01\x83\xC3\x18\x3B\x46\x08", "xxxxxxxxxxxxxx");
	
	//DWORD parserAPKF = 0xA6B4FA;
	
	if (!parserAPKF) {
    //MessageBoxA(0, "Pattern not found!", "Error", MB_OK);
    return 0;
}

	MsgBoxOmar(parserAPKF);
	
	parserJmpBack = (BYTE*)parserAPKF + 0x5;
	
	Sleep(150);
	char buf[64];
	sprintf_s(buf, sizeof(buf), "ParserJMPBack: %p", parserJmpBack);
	//MessageBoxA(nullptr, buf, "Debug", MB_OK);

	
	PlaceJMP((BYTE*) parserAPKF, (DWORD)RedirectEntries, 0x5);
	
	
	
	
	return 0;
	
	
}



struct LoadedFile {
    std::string hash;
    std::string name;
    std::string component;
    std::string extension;
    size_t size;
    uint8_t* data; // pointer to file contents
};



LoadedFile ParseAndLoad(const std::string& folder, const std::string& filename) {
    // Split filename: [Hash].[Name].[Component].[Ext]
    size_t p1 = filename.find('.');
    size_t p2 = filename.find('.', p1 + 1);
    size_t p3 = filename.find('.', p2 + 1);

    std::string hash      = filename.substr(0, p1);
    std::string name      = filename.substr(p1 + 1, p2 - p1 - 1);
    std::string component = filename.substr(p2 + 1, p3 - p2 - 1);
    std::string extension = filename.substr(p3 + 1);

    // Load file into memory
    std::ifstream file(folder + "\\" + filename, std::ios::binary | std::ios::ate);
    size_t size = file.tellg();
    file.seekg(0);

    uint8_t* buffer = new uint8_t[size];
    file.read(reinterpret_cast<char*>(buffer), size);

    return { hash, name, component, extension, size, buffer };
}


std::vector<LoadedFile> LoadAllFiles(const std::string& folder) {
    std::vector<LoadedFile> results;
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA((folder + "\\*").c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE) return results;

    do {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            results.push_back(ParseAndLoad(folder, ffd.cFileName));
        }
    } while (FindNextFileA(hFind, &ffd));

    FindClose(hFind);
    return results;
}



auto Files = LoadAllFiles("RedirectorTest");

DWORD WINAPI OverwriteValues()
{	
	if(FilenameHash) {
		char buffer[1024];
		sprintf(buffer, "Hash: %u", FilenameHash);
		//MessageBox(0, buffer, "Debug", MB_OK);
		
		sprintf(buffer, "HookCount: %u", HookCount);
		//MessageBox(NULL, buffer, "Debug", MB_OK);
		
		sprintf(buffer, "FileComponent: %u", FileComponentRegister);
		//MessageBox(0, buffer, "Debug", MB_OK);
		
		
		
		
		for (auto& file : Files) {
			//sprintf(buffer, "ExtractedHash: %u", std::stoul(file.hash, nullptr, 0));
			//MessageBox(0, buffer, "Debug", MB_OK);
			
			//sprintf(buffer, "ExtractedComponent: %u", std::stoul(file.component));
			//MessageBox(0, buffer, "Debug", MB_OK);
			
			if(std::stoul(file.hash, nullptr, 0) == FilenameHash && std::stoul(file.component) == FileComponentRegister) 
			{		
					DWORD AllocatedFileAddr = (DWORD)file.data;
					
					
					
					
					sprintf(buffer, "AllocAddress: %u", AllocatedFileAddr);
					MessageBox(0, buffer, "Debug", MB_OK);
					
					
					
					
					
					*(DWORD*)(FileHeaderRegister + FileComponentRegister * 4 + 8) = AllocatedFileAddr;
					break;
					
					
					
			}
				
				
		}
			
	}
	
	return 0;
}
	
	
	
	
	
	
	



DWORD WINAPI MainThread(LPVOID)
{	InitiateHook();
    return 0;
}






BOOL WINAPI DllMain(HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
			break;
	
		case DLL_PROCESS_DETACH:
			char Buffer[1024];
			sprintf(Buffer, "Dll Un-Injected");
			//MessageBox(NULL, Buffer, "Detached", MB_OK);
			break;
	}
	
	return TRUE;
}
