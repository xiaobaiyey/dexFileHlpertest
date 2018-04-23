#include "DexFileHelper.h"
#include "pch.h"
#include "DexFileUtils.h"

/*
* Direct-mapped "type_id_item".
*/
struct DexTypeId
{
	u4 descriptorIdx; /* index into stringIds list for type descriptor */
};

DexFileTypeIdx::DexFileTypeIdx(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	u4 typeOff = dex_file_helper->dex_file_header->getTypeIdsOff();
	u4 typeSize = dex_file_helper->dex_file_header->getTypeIdsSize();
	u1* data = base + typeOff;
	DexTypeId* dex_type_id = (DexTypeId*)data;
	typeData.clear();
	for (u4 i = 0; i < typeSize; ++i)
	{
		DexTypeIdx* dex_type = new DexTypeIdx;
		u4 strindex = dex_type_id->descriptorIdx;
		DexStringIdx* str = dex_file_helper->dex_file_string->getDexStringById(strindex);
		dex_type->idx = i;
		dex_type->dex_string = str;
		typeData.insert(std::make_pair(i, dex_type));
		dex_type_id++;
		//LOGD("[=]read Type:%s", dex_type->dex_string->data);
	}
	LOGI("[+]parse TypeID over :0x%08x", typeData.size());
}

DexFileTypeIdx::~DexFileTypeIdx()
{
}

DexTypeIdx* DexFileTypeIdx::getTypeById(u4 idx)
{
	u4 size = typeData.size();

	if (idx > size)
	{
		LOGE("[-]get type idx>TypeData.size() :0x%08x",idx);
		return nullptr;
	}
	else
	{
		return typeData[idx];
	}
}

u4 DexFileTypeIdx::getTypeIdxSize()
{
	return typeData.size();
}

u4 DexFileTypeIdx::getTypeByString(const char* str)
{
	u4 hash = DexFileUtils::hash(str);
	u4 size = typeData.size();

	for (u4 i = 0; i < size; ++i)
	{
		DexTypeIdx* dex_type = typeData[i];
		if (dex_type->dex_string->hash == hash)
		{
			return i;
		}
	}
	LOGE("[-]getTypeByString fail");
	return 0;
}
