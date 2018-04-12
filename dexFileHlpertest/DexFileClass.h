#pragma once
#include "DexDefine.h"
#include <map>
class DexFileHelper;

class DexFileClass
{
public:
	DexFileClass(DexFileHelper* dex_file_helper);
	~DexFileClass();
private:
	DexFileHelper* dex_file_helper_;
	u1* base;
	std::map<u4, DexClassDef*> dex_class_def_maps;

	void parseClassDef(u1* data, u4 defSize);
	void parseClassData(u4 offset);
};
