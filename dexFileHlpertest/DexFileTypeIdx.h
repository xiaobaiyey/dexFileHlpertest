#pragma once
#include <map>
#include "DexDefine.h"

class DexFileHelper;


class DexFileTypeIdx
{
public:
	DexFileTypeIdx(DexFileHelper* dex_file_helper);
	~DexFileTypeIdx();
	DexTypeIdx* getTypeById(u4 idx);
	u4 getTypeIdxSize();
	u4 getTypeByString(const char* str);
private:
	DexFileHelper* dex_file_helper_;
	/*id,dexType*/
	std::map<u4, DexTypeIdx*> typeData;
};
