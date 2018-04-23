#pragma once
#include "DexDefine.h"
#include <map>

class DexFileHelper;
class DexFileClassMethod;
class DexFileClass
{
public:
	DexFileClass(DexFileHelper* dex_file_helper);
	~DexFileClass();
private:
	DexFileHelper* dex_file_helper_;
	DexFileClassMethod* dex_file_class_method_;
	u1* base;
	std::map<u4, DexClass*> dex_class_def_maps;

	void parseClassDef(u1* data, u4 defSize);
	DexClassData* parseClassData(DexClass* dex_class,u4 offset);
	DexClassDataHeader* ReadClassDataHeader(const u1** data);
	DexField* ReadClassDataField(const u1** pData,u4* lastIndex);
};
