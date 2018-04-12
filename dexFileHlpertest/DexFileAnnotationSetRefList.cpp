#include "DexFileAnnotationSetRefList.h"
#include "pch.h"
#include "DexFileHelper.h"

DexFileAnnotationSetRefList::DexFileAnnotationSetRefList(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeAnnotationSetRefList);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeAnnotationSetItem is nullptr");
		return;
	}
	u1* data = base + dex_map_item->offset;
	u4 set_item_len = dex_map_item->size;

}

DexFileAnnotationSetRefList::~DexFileAnnotationSetRefList()
{
}
