#pragma once

#include "SpideyZ.h"
#include "Patcher.h"

namespace FileSystem
{
	// This is a file entry in the global PKR file
	struct PkrFile 
	{
		char name[0x20];
		uint32_t crc;
		int32_t compressionType;
		uint32_t fileOffset;
		uint32_t uncompressedSize;
		uint32_t compressedSize;
	};
	
	// Stored in memory, represents entire PKR
	struct GlobalPkrFile
	{
	  FILE *handle;
	  char file_name[20];
	  uint32_t padding[60];
	  int32_t neg_1;
	  char pkr3_magic[4];
	  uint32_t uncompressed_size_maybe;
	  uint32_t four;
	  uint32_t six;
	  uint32_t file_count_maybe;
	  uint32_t *ptr_to;
	  char *bmp_pointer;
	  uint32_t unk_value;
	};
	
	void Initialize();
	void PatchHandlers(SpideyZ::Patcher patcher);
}
