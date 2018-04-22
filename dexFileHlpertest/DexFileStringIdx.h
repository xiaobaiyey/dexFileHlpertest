#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;



class DexFileStringIdx
{
public:
	DexFileStringIdx(DexFileHelper* dex_file_helper);
	DexString* getDexStringById(u4 idx);
	u4 getStringIdx(const char* str);
	~DexFileStringIdx();
private:
	DexFileHelper* dex_file_helper_;
	/*id,Data*/
	std::map<u4, DexString*> stringData;
};