#pragma once
#include <map>
#include "DexDefine.h"
#include "DexFileString.h"
class DexFileHelper;


class DexFileType
{
public:
	DexFileType(DexFileHelper* dex_file_helper);
	~DexFileType();
	DexType* getTypeById(u4 idx);
	u4 getTypeByString(const char* str);
private:
	DexFileHelper* dex_file_helper_;
	/*id,dexType*/
	std::map<u4, DexType*> typeData;
};
