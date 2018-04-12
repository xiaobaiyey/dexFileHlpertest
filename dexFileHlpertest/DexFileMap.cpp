#include "DexFileMap.h"
#include "DexFileHelper.h"
#include "pch.h"


DexFileMap::DexFileMap(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u4 mapOff = dex_file_helper->dex_file_header->getMapOff();
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	u4 mapSize = *reinterpret_cast<u4*>(base + mapOff);
	u1* items = (base + mapOff + sizeof(mapSize));

	DexMapItem* dex_map_item = reinterpret_cast<DexMapItem*>(items);
	for (u4 i = 0; i < mapSize; ++i)
	{
		u4 typeidx = dex_map_item->type;
		dexfile_maps.insert(std::make_pair(typeidx, dex_map_item));
		LOGD("[=]read Map type:0x%08x",typeidx);
		dex_map_item++;
	}
	LOGI("[+]read maps size:0x%08x",mapSize);
}

DexFileMap::~DexFileMap()
{
}

DexMapItem* DexFileMap::getMapItemByType(u4 idx)
{
	DexMapItem* item = dexfile_maps[idx];
	if (item == nullptr)
	{
		LOGE("[-]get map by typeid fail,idx:0x%08x",idx);
	}
	return item;
}
