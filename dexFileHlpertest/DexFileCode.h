#pragma once
#include "DexDefine.h"
class DexFileHelper;
class DexFileCode
{
public:
	DexFileCode(DexFileHelper* dex_file_helper);
	
	~DexFileCode();
private:
	DexFileHelper * dex_file_helper_;
	u1* base;
};

