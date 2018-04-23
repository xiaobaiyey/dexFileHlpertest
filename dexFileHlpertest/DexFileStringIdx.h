#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;


class DexFileStringIdx
{
public:
	DexFileStringIdx(DexFileHelper* dex_file_helper);
	DexStringIdx* getDexStringById(u4 idx);
	u4 getStringIdx(const char* str);
	u4 getStringIdxSize();
	~DexFileStringIdx();

private:
	DexFileHelper* dex_file_helper_;
	/*id,Data*/
	std::map<u4, DexStringIdx*> stringData;
};
