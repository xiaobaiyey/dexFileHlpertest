#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileMethodIdx
{
public:
	DexFileMethodIdx(DexFileHelper* dex_file_helper);
	DexMethodIdx* getDexFileMethodIdxById(u4 idx);
	~DexFileMethodIdx();
	u4 getDexMethodIdxSize();
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexMethodIdx*> dex_method_maps;
};

