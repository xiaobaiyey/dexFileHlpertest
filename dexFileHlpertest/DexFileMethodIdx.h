#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileMethodIdx
{
public:
	DexFileMethodIdx(DexFileHelper* dex_file_helper);
	~DexFileMethodIdx();
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexMethod*> dex_method_maps;
};

