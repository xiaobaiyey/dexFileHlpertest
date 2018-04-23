#include "DexFileHelper.h"
#include "DexDefine.h"
#include "pch.h"


DexFileProtoTypeIdx::DexFileProtoTypeIdx(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	u4 protoOff = dex_file_helper->dex_file_header->getProtoIdsOff();
	u4 protoSize = dex_file_helper->dex_file_header->getProtoIdsSize();

	u1* data = base + protoOff;
	for (u4 i = 0; i < protoSize; ++i)
	{
		DexProtoIdx* dex_proto = new DexProtoIdx;
		//read shortyIdx
		u4 shortyIdx = *reinterpret_cast<u4*>(data);
		dex_proto->idx = i;
		dex_proto->shorty_string = dex_file_helper->dex_file_string->getDexStringById(shortyIdx);
		data = data + sizeof(shortyIdx);
		//read returnType_type
		u4 returnTypeIdx = *reinterpret_cast<u4*>(data);
		dex_proto->returnType_type = dex_file_helper->dex_file_type->getTypeById(returnTypeIdx);
		data = data + sizeof(returnTypeIdx);
		//read parametersOff
		u4 parametersOff = *reinterpret_cast<u4*>(data);
		if (parametersOff!=0)
		{
			dex_proto->dex_type_list = dex_file_helper->dex_file_type_list->getDexFileTypeListByOffset(parametersOff);
		}
		data = data + sizeof(parametersOff);
		dex_proto_maps.insert(std::make_pair(i, dex_proto));
	}
	LOGI("[+]read protoSize:0x%08x",protoSize);
}

DexFileProtoTypeIdx::~DexFileProtoTypeIdx()
{
}

DexProtoIdx* DexFileProtoTypeIdx::getDexProtoByidx(u4 idx)
{
	if (idx > dex_proto_maps.size())
	{
		LOGE("[-]idx > dex_proto_maps.size()");
		return nullptr;
	}
	else
	{
		return dex_proto_maps[idx];
	}
}

u4 DexFileProtoTypeIdx::getProtoSize()
{
	return dex_proto_maps.size();
}
