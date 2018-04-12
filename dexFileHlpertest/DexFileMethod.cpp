#include "DexFileMethod.h"
#include "DexFileHelper.h"
#include "pch.h"


DexFileMethod::DexFileMethod(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	u4 methodOff = dex_file_helper->dex_file_header->getMethodIdsOff();
	u4 methodSize = dex_file_helper->dex_file_header->getMethodIdsSize();

	u1* data = base + methodOff;
	for (u4 i = 0; i < methodSize; ++i)
	{
		DexMethod* dex_method = new DexMethod;
		dex_method->idx = i;
		//read classidx
		u2 classIdx = *reinterpret_cast<u2*>(data);
		dex_method->class_type = dex_file_helper->dex_file_type->getTypeById(classIdx);
		data = data + sizeof(classIdx);
		//read typeIdx
		u2 typeIdx = *reinterpret_cast<u2*>(data);
		dex_method->type_type = dex_file_helper->dex_file_proto->getDexProtoByidx(typeIdx);
		data = data + sizeof(typeIdx);
		//read nameIdx
		u4 nameIdx = *reinterpret_cast<u4*>(data);
		dex_method->name_string = dex_file_helper->dex_file_string->getDexStringById(nameIdx);
		data = data + sizeof(nameIdx);
		//add to maps
		dex_method_maps.insert(std::make_pair(i, dex_method));
	}
	LOGI("[+]read methods size:0x%08x", methodSize);
}

DexFileMethod::~DexFileMethod()
{
}
