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
	for (u4 i = 0; i < set_item_len; ++i)
	{
		DexAnnotationSetRefList* dex_annotation_set_ref_list = new DexAnnotationSetRefList;
		u4 offset = data - base;
		u4 item_len = *reinterpret_cast<u4*>(data);
		dex_annotation_set_ref_list->size = item_len;
		dex_annotation_set_ref_list->offset = offset;
		data = data + sizeof(item_len);
		for (u4 j = 0; j < item_len; ++j)
		{
			u4 set_item_offset = *reinterpret_cast<u4*>(data);
			if (set_item_offset!=0)
			{
				DexAnnotationSetItem* dex_annotation_set_item = dex_file_helper
					->dex_file_annotation_set_item->getDexAnnotationSetItemByOffset(
						set_item_offset);
				dex_annotation_set_ref_list->dex_annotation_set_items.push_back(dex_annotation_set_item);
			}
			data = data + sizeof(item_len);
		}
		dex_annotation_set_ref_list_maps.insert(std::make_pair(offset, dex_annotation_set_ref_list));
	}
	LOGI("[+]read kDexTypeAnnotationSetRefList size:0x%08x",dex_annotation_set_ref_list_maps.size());
}

DexFileAnnotationSetRefList::~DexFileAnnotationSetRefList()
{
}

DexAnnotationSetRefList* DexFileAnnotationSetRefList::getDexAnnotationSetRefListByOffset(u4 offset)
{
	DexAnnotationSetRefList* temp = dex_annotation_set_ref_list_maps[offset];
	if (temp == nullptr)
	{
		LOGE("[-]getDexAnnotationSetRefListByOffset fail:0x%04x", offset);
	}
	return temp;
}
