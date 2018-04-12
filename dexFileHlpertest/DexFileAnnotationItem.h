#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;

/*和位置无关，但是需要注意在文件中偏移，直接复制整个内存块，整个块解析起来太复杂*/
class DexFileAnnotationItem
{
public:
	DexFileAnnotationItem(DexFileHelper* dex_file_helper);
	~DexFileAnnotationItem();
	DexAnnotationItem* getDexAnnotationItembyOffset(u4 offset);
private:
	DexFileHelper* dex_file_helper_;
	/*offset,item*/
	std::map<u4, DexAnnotationItem*> dex_annotation_item_maps;

	const u1* verifyEncodedAnnotation(const u1* data);
	const u1* verifyEncodedValue(const u1* data);
	const u1* verifyEncodedArray(const u1* data);
};
