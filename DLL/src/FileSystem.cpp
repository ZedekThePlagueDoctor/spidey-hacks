#include "FileSystem.h"
#include "Patcher.h"
#include "Logger.h"
#include "GameConfig.h"
#include "SpideyConstants.h"

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>

// COMPRESSION TYPES
const char* decompNames[4] = {
	"Unknown A",
	"Unknown B",
	"ZLib",
	"BZip"
};

//------------------------------------------------------

char *fileName;
char *fileDirectory;
FileSystem::PkrFile *curPkr = nullptr;

typedef void* lockFile_Call(FILE *fp);
typedef void* unlockFile_Call(FILE *fp);
typedef void* freadSpidey_Call(void *buffer, size_t size, size_t count_of, FILE *fp);
typedef void* new_Call(int size);
typedef void* delete_Call(void *Address);

new_Call *spNew = (new_Call*) 0x00529BA2;
delete_Call *spDelete = (delete_Call*) 0x00529BB0;
lockFile_Call *lockFile = (lockFile_Call*) 0x0052BEBD;
unlockFile_Call *unlockFile = (unlockFile_Call*) 0x0052BF0F;
freadSpidey_Call *freadSpidey = (freadSpidey_Call*) 0x0052AA25;

//------------------------------------------------------

namespace FileSystem
{
	SpideyZ::Patcher g_patcher;
	
	//------------------------
	// Get filesize our own way
	//------------------------
	
	DWORD MyGetFileSize(FILE *fp)
	{
		fseek(fp, 0, SEEK_END);
		DWORD size = ftell(fp);
		rewind(fp);
		
		return size;
	}

	//------------------------
	// Open file from disk, or PKR if necessary
	//------------------------
	
	BOOL OpenFileFromDisk(void* unused_mem, DWORD compressedSize, DWORD unused1, FILE *pkr)
	{
		static char path[64];
		strcpy(path, fileDirectory);
		strcat(path, fileName);
		
		FILE *fp = fopen(path, "rb");
		
		//-------------------
		// Couldn't open the file locally, it likely didn't exist!
		//-------------------
		
		if (!fp)
		{
			GlobalPkrFile* glob_pkr = *(GlobalPkrFile**) ADDR_CurrentPKR;
			
			Logger::Log("Will open from %s: %s, Size: %d bytes\n", glob_pkr -> file_name, fileName, compressedSize);
			
			void* buffer = spNew(compressedSize);
			
			if (!buffer)
			{
				printf("  Wasn't able to create a buffer for the file being read off the PKR\n");
				return FALSE;
			}

			lockFile(glob_pkr -> handle);
			
			if(!freadSpidey(buffer, compressedSize, 1, glob_pkr -> handle))
			{
				Logger::Log("  Couldn't read file off pkr: %s\n", fileName);
				spDelete(buffer);
				return FALSE;
			}
			
			unlockFile(glob_pkr -> handle);
			
			// Do we need to decompress it? (-2 is uncompressed)
			
			if (curPkr -> compressionType != -2)
			{
				Logger::Log("  File uses %s compression\n", decompNames[curPkr -> compressionType]);
				
				// Setup reference to the function in memory
				typedef void* compCall(void *buf, int32_t comSize, uint32_t uncomSize);
				compCall *compFunction = *(compCall **) (ADDR_DecompMethodTable + (4 * curPkr -> compressionType));
				
				void* decomp = compFunction(buffer, compressedSize, curPkr -> uncompressedSize);
				spDelete(buffer);
				
				Logger::Log("  Success!\n", path);
				
				curPkr -> compressionType = -2;
				g_patcher.WriteInt32((void *) ADDR_CurrentFileBuffer, (uint32_t) decomp);
				g_patcher.WriteInt32((void *) ADDR_CurrentFileSize, (uint32_t) curPkr -> uncompressedSize);
				
				return TRUE;
			}
			
			Logger::Log("  Success!\n", path);
			
			curPkr -> compressionType = -2;
			g_patcher.WriteInt32((void *) ADDR_CurrentFileBuffer, (uint32_t) buffer);
			g_patcher.WriteInt32((void *) ADDR_CurrentFileSize, (uint32_t) curPkr -> uncompressedSize);

			return TRUE;
		}
		
		Logger::Log("File opened: %s\n", path);
		
		DWORD fileSize = MyGetFileSize(fp);
		
		void* buffer = spNew(fileSize);
		
		if (!buffer)
		{
			Logger::Log("  Couldn't allocate space file: %s\n", fileName);
			fclose(fp);
			return FALSE;
		}

		if(!fread(buffer, fileSize, 1, fp))
		{
			Logger::Log("  Error reading file: %s\n", fileName);
			fclose(fp);
			spDelete(buffer);
			return FALSE;
		}
		
		fclose(fp);
		
		Logger::Log("  File has been loaded: %s - %d bytes\n", curPkr -> name, fileSize);
		
		curPkr -> uncompressedSize = fileSize;
		curPkr -> compressionType = -2;
		
		// Set things memory, this is important!
		g_patcher.WriteInt32((void *) ADDR_CurrentFileBuffer, (uint32_t) buffer);
		g_patcher.WriteInt32((void *) ADDR_CurrentFileSize, (uint32_t) fileSize);

		return TRUE;
	}
	
	//------------------------
	// Prepare the fs
	// (Nothing for now)
	//------------------------
	
	void Initialize() {}
	
	//------------------------
	// (Thanks, Krystal)
	//------------------------
	
	__declspec(naked) fread_hook(PkrFile *pkrFile, PVOID loadBuf, DWORD one)
	{
		// AT&T syntax because GCC cross-compile, gross
		// (Could we get these values from memory instead? It's possible!)
		
		asm (
			"movl 0x28(%esp), %eax\n\
			movl %eax, _fileName\n\
			movl 0xA8(%esp), %eax\n\
			movl %eax, _fileDirectory\n\
			leal 0x5C(%esp), %eax\n\
			movl %eax, _curPkr"
		);
		
		asm ("jmp *%0" :: "d" (OpenFileFromDisk));
	}

	//------------------------
	// Patch stuff
	//------------------------
	
	void PatchHandlers(SpideyZ::Patcher patcher)
	{
		g_patcher = patcher;
		
		if (GameConfig::GetValue("Debug", "UseFileLoader", 1))
		{
			// Disable memory allocation after file loading
			patcher.WriteNOPs((void *) 0x00519376, 5);
			
			// Disable check after memory allocation
			patcher.WriteInt8((void *) 0x0051938B, 0xEB);
			
			// FRead patch
			patcher.WriteCall((void *) 0x005193C5, (void *) fread_hook);
			
			// Disable free after FRead fail
			patcher.WriteNOPs((void *) 0x005193DC, 5);
			
			// Disable CRC check
			patcher.WriteNOPs((void *) 0x0051941E, 5);
			
			// Disable jnz after CRC Check
			patcher.WriteInt8((void *) 0x0051942D, 0xEB);
			
			// Disable setting of buffer and filesize vars
			// (We know where they get stored in memory, don't bother)
			patcher.WriteNOPs((void *) 0x00519445, 16);
		}
	}
}
