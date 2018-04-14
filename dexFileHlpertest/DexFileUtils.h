#pragma once
#include "DexDefine.h"

class DexFileUtils
{
public:
	static bool isDex(void* inAddr);

	static unsigned char* getBase(void* inAddr);

	static unsigned int hash(const char* str);

	static unsigned char* readFile(const char* inPath, size_t* inReadSize);

	static int readSignedLeb128(u1** pStream);

	static u4 readAndVerifyUnsignedLeb128(const u1** pStream, const u1* limit, bool* okay);

	static int readUnsignedLeb128(const u1** pStream);

	static u4 readUnsignedLittleEndian(const u1** pData,
	                                   u4 size);

	static u4 align_4(size_t size);
};
