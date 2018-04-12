#pragma once
#include <stddef.h>
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileEncodedArray
{
public:
	DexFileEncodedArray(DexFileHelper* dex_file_helper);
	~DexFileEncodedArray();
	DexEncodedArray* getDexEncodedArrayByOffset(u4 offset);
private:
	DexFileHelper * dex_file_helper_;

	std::map<u4, DexEncodedArray*> dex_encoded_array_maps;

	size_t getValueLen(int inType);
};

