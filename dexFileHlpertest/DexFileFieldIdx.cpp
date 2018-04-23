#include "DexFileHelper.h"
#include "pch.h"


DexFileFieldIdx::DexFileFieldIdx(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	u4 fieldOff = dex_file_helper->dex_file_header->getFieldIdsOff();
	u4 fieldSize = dex_file_helper->dex_file_header->getFieldIdsSize();

	u1* data = base + fieldOff;
	for (u4 i = 0; i < fieldSize; ++i)
	{
		DexFieldIdx* dex_field = new DexFieldIdx;
		dex_field->idx = i;
		//read classidx
		u2 classIdx = *reinterpret_cast<u2*>(data);
		dex_field->class_type = dex_file_helper->dex_file_type->getTypeById(classIdx);
		data = data + sizeof(classIdx);
		//read typeIdx
		u2 typeIdx = *reinterpret_cast<u2*>(data);
		dex_field->type_type = dex_file_helper->dex_file_type->getTypeById(typeIdx);
		data = data + sizeof(typeIdx);
		//read nameIdx
		u4 nameIdx = *reinterpret_cast<u4*>(data);
		dex_field->name_string = dex_file_helper->dex_file_string->getDexStringById(nameIdx);
		data = data + sizeof(nameIdx);
		//add to maps
		/*LOGD("[=]idx:0x%04x classIdx:0x%02x %s,typeIdx:0x%02x %s,nameIdx:0x%04x %s",i,classIdx,dex_field->class_type->
			dex_string->data,typeIdx,dex_field->type_type->dex_string->data,nameIdx,dex_field->name_string->data);*/
		dex_field_maps.insert(std::make_pair(i, dex_field));
	}
	LOGI("[+]read fields size:0x%08x",fieldSize);
}

DexFileFieldIdx::~DexFileFieldIdx()
{
}

DexFieldIdx* DexFileFieldIdx::getDexFieldIdxById(u4 idx)
{
	DexFieldIdx* dex_field = dex_field_maps[idx];
	if (dex_field == nullptr)
	{
		LOGE("get DexFieldIdx fail %x",idx);
	}
	return dex_field;
}

u4 DexFileFieldIdx::getDexFieldIdxSize()
{
	return dex_field_maps.size();
}
