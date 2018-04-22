#include "DexFileTypeList.h"
#include "DexFileHelper.h"
#include "pch.h"


DexFileTypeList::DexFileTypeList(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeTypeList);
	if (dex_map_item == nullptr)
	{
		LOGE("[-] get kDexTypeTypeList fail");
	}
	u4 size = dex_map_item->size;
	u4 offset = dex_map_item->offset;

	u1* data = base + offset;
	for (u4 i = 0; i < size; ++i)
	{
		DexTypeList* dex_type_list = new DexTypeList;
		//get offset
		u4 type_list_offset = data - base;
		dex_type_list->offset = type_list_offset;
		//read size;
		u4 list_size = *reinterpret_cast<u4*>(data);
		dex_type_list->size = list_size;
		data = data + sizeof(list_size);
		//read typeid;
		for (u4 j = 0; j < list_size; ++j)
		{
			u2 typeidx = *(u2*)data;
			DexTypeIdx* dex_type = dex_file_helper->dex_file_type->getTypeById(typeidx);
			dex_type_list->typeItems.push_back(dex_type);
			data = data + sizeof(typeidx);
		}
		//注意字节对齐
		if (list_size%2!=0)
		{
			data = data + 2;
		}
		//add to maps
		dexfile_type_list.insert(std::make_pair(dex_type_list->offset, dex_type_list));
	}
	LOGI("[+]read typelist size:0x%08x",size);
}

DexFileTypeList::~DexFileTypeList()
{
}

DexTypeList* DexFileTypeList::getDexFileTypeListByOffset(u4 offset)
{
	DexTypeList* temp = dexfile_type_list[offset];
	if (temp != nullptr)
	{
		return temp;
	}
	LOGE("[+]getDexFileTypeListByOffset fail");
	return nullptr;
}
