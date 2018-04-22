#pragma once
#include "DexDefine.h"
#include <map>
class DexFileHelper;

class DexFileClassMethod
{
public:
	static DexFileClassMethod* getInstance(DexFileHelper* dex_file_helper);

	DexMethod* parseMethod(DexClass* dex_class,const u1** pdata, u4* lastindex);
	~DexFileClassMethod();
private:
	DexFileClassMethod(DexFileHelper* dex_file_helper);
	DexFileHelper* dex_file_helper_;
	static DexFileClassMethod* dex_file_class_method_;
	std::map<u4, DexMethod*> dex_file_method_maps;
};
