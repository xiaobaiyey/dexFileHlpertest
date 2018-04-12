#include "DexFileAnnotationSetItem.h"
#include "DexFileHelper.h"
#include "pch.h"


DexFileAnnotationSetItem::DexFileAnnotationSetItem(DexFileHelper* dex_file_helper)
{
	
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeAnnotationSetItem);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeAnnotationSetItem is nullptr");
		return;
	}
	u1* data = base + dex_map_item->offset;
	u4 set_item_len = dex_map_item->size;

	for (u4 i = 0; i < set_item_len; ++i)
	{
		DexAnnotationSetItem* dex_annotation_set_item = new DexAnnotationSetItem;
		//read size
		u4 size = *reinterpret_cast<u4*>(data);
		//get offset
		u4 offset = data - base;
		dex_annotation_set_item->size = size;
		dex_annotation_set_item->offset = offset;
		//read size
		data = data + sizeof(u4);
		if (size != 0)
		{
			//read entry
			for (u4 j = 0; j < size; ++j)
			{
				u4 entriy_offset = *reinterpret_cast<u4*>(data);
				DexAnnotationItem* dex_annotation_item = dex_file_helper
				                                         ->dex_file_annotation_item->getDexAnnotationItembyOffset(entriy_offset);
				dex_annotation_set_item->annotation_items_vector.push_back(dex_annotation_item);
				data = data + sizeof(u4);
			}
		}
		/*else
		{
			LOGD("[+]DexAnnotationSetItem size is zore");
		}*/
		dex_annotation_setitem_maps.insert(std::make_pair(offset, dex_annotation_set_item));
	}
	LOGI("[+]read DexAnnotationSetItem size:0x%08x",dex_annotation_setitem_maps.size());
}

DexFileAnnotationSetItem::~DexFileAnnotationSetItem()
{

}

DexAnnotationSetItem* DexFileAnnotationSetItem::getDexAnnotationSetItemByOffset(u4 offset)
{
	DexAnnotationSetItem* dex_annotation_set_item = dex_annotation_setitem_maps[offset];
	if (dex_annotation_set_item == nullptr)
	{
		LOGE("[-]get DexAnnotationSetItem fail by offset:0x%08x",offset);
	}
	return dex_annotation_set_item;
}
