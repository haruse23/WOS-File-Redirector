#include <iostream>
#include <Windows.h>
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>

static char persistentBuf[256]; // adjust size

void PatchTexture(const std::string& filename, uint8_t* data) {
    memcpy(persistentBuf, filename.c_str(), filename.size() + 1);
    *(DWORD*)(data + 8) = (DWORD)persistentBuf;

    // Dump buffer to file
    /*FILE* f = fopen("buffer_dump.txt", "w");
    if (f) {
        // Write raw hex
        fprintf(f, "Hex dump:\n");
        for (size_t i = 0; i < filename.size() + 1; i++) {
            fprintf(f, "%02X ", (unsigned char)persistentBuf[i]);
        }

        // Read back the stored pointer
        DWORD stored = *(DWORD*)(data + 8);

        // Dump pointer info and string
        fprintf(f, "\nbuf=%p stored=%p string='%s'\n",
                persistentBuf,
                (void*)stored,
                (char*)stored);

        fprintf(f, "\n\nASCII string:\n%s\n", persistentBuf);
        fclose(f);
    }*/
}



void ParseAndPatchMesh(uint8_t* MeshFile) {
	
	DWORD pFilename = *(DWORD*)(MeshFile);
	
	DWORD FilenameHash = *(DWORD*)(MeshFile + 4);
	
	DWORD parsedFlag = *(DWORD*)(MeshFile + 8);
	
	DWORD meshCount = *(DWORD*)(MeshFile + 12);
	
	
	DWORD MeshTableOffset = *(DWORD*)(MeshFile + 16); // pmeshTable
		
	BYTE* pmeshTable = MeshFile + MeshTableOffset;
	
	*(DWORD*)(MeshFile + 16) = (DWORD)pmeshTable;
	
	std::vector<BYTE*> meshInfos(meshCount);
	
	for(int i = 0; i < meshCount; i++){
		DWORD parsedFlag = *(DWORD*)pmeshTable;
		
		DWORD MeshInfoOffset = *(DWORD*)(pmeshTable + 4); // pmeshTable
		
		meshInfos[i] = MeshFile + MeshInfoOffset;
		
		*(DWORD*)(pmeshTable + 4) = (DWORD)meshInfos[i];
		

		
		
		pmeshTable += 8;
		
	}
	
	
	for(int j = 0; j<meshCount; j++) {
		
		 
		
		*(DWORD*)(meshInfos[j] + 32) = (DWORD)(MeshFile + *(DWORD*)(meshInfos[j] + 32)); // pMeshMaterialData
		
		
		
		
		*(DWORD*)(meshInfos[j] + 32 + 4) = (DWORD)(MeshFile + *(DWORD*)(meshInfos[j] + 32 + 4)); // pBonePaletteIndices
		
		
		
		
		*(DWORD*)(meshInfos[j] + 32 + 12) = (DWORD)(MeshFile + *(DWORD*)(meshInfos[j] + 32 + 12)); // pVertexBuffer
		
		
		
		
		*(DWORD*)(meshInfos[j] +32 +28) = (DWORD)(MeshFile + *(DWORD*)(meshInfos[j] +32 +28)); // pIndexBuffer
		
	
		
		
		DWORD SchemaTableOffset = *(DWORD*)(meshInfos[j] +32+ 44); // pSchemaTable
		
		BYTE* pSchemaTable = MeshFile + SchemaTableOffset;
		
		*(DWORD*)(meshInfos[j] +32+ 44) = (DWORD)pSchemaTable;
		
		
		*(DWORD*)(pSchemaTable + 4) = (DWORD)(MeshFile + *(DWORD*)(pSchemaTable + 4)); //pVertexSchema
		
	}
	
	
}
		
		