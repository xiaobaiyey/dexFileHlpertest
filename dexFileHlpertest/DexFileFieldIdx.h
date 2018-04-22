#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileFieldIdx
{
public:
	DexFileFieldIdx(DexFileHelper* dex_file_helper);
	~DexFileFieldIdx();
	DexFieldIdx* getDexFieldIdxById(u4 idx);
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexFieldIdx*> dex_field_maps;
};

