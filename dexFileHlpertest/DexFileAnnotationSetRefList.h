#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;

class DexFileAnnotationSetRefList
{
public:
	DexFileAnnotationSetRefList(DexFileHelper* dex_file_helper);
	~DexFileAnnotationSetRefList();
	DexAnnotationSetRefList* getDexAnnotationSetRefListByOffset(u4 offset);
private:
	DexFileHelper * dex_file_helper_;
	std::map<u4, DexAnnotationSetRefList*> dex_annotation_set_ref_list_maps;
};

