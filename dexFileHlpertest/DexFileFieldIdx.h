#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileFieldIdx
{
public:
	DexFileFieldIdx(DexFileHelper* dex_file_helper);
	~DexFileFieldIdx();
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexField*> dex_field_maps;
};

