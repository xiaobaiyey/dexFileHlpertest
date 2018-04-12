#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileMethod
{
public:
	DexFileMethod(DexFileHelper* dex_file_helper);
	~DexFileMethod();
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexMethod*> dex_method_maps;
};

