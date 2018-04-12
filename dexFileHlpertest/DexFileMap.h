#pragma once
#include "DexDefine.h"
#include <map>

class DexFileHelper;
class DexFileMap
{
public:
	DexFileMap(DexFileHelper* dex_file_helper);
	~DexFileMap();
	DexMapItem* getMapItemByType(u4 idx);
private:
	DexFileHelper* dex_file_helper_;
	std::map<u4, DexMapItem*> dexfile_maps;
};
