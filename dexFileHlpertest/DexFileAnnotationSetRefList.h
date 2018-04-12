#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;

class DexFileAnnotationSetRefList
{
public:
	DexFileAnnotationSetRefList(DexFileHelper* dex_file_helper);
	~DexFileAnnotationSetRefList();
private:
	DexFileHelper * dex_file_helper_;
	
};

