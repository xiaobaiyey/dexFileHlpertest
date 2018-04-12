#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileField
{
public:
	DexFileField(DexFileHelper* dex_file_helper);
	~DexFileField();
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexField*> dex_field_maps;
};

