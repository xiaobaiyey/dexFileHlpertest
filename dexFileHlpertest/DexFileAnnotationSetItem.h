#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;

class DexFileAnnotationSetItem
{
public:
	DexFileAnnotationSetItem(DexFileHelper* dex_file_helper);
	~DexFileAnnotationSetItem();
	DexAnnotationSetItem* getDexAnnotationSetItemByOffset(u4 offset);
private:
	DexFileHelper* dex_file_helper_;
	/*offset DexAnnotationSetItem*/
	std::map<u4, DexAnnotationSetItem*> dex_annotation_setitem_maps;
};
