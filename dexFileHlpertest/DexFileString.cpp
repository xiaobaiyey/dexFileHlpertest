#include "DexFileString.h"
#include "pch.h"
#include "DexFileHelper.h"
#include "DexFileUtils.h"

struct DexStringId
{
	u4 stringDataOff; /* file offset to string_data_item */
};

DexFileString::DexFileString(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	u4 stringOff = dex_file_helper->dex_file_header->getStringIdsOff();
	u4 stringSize = dex_file_helper->dex_file_header->getStringIdsSize();

	u1* data = base + stringOff;
	DexStringId* dex_string_id = (DexStringId*)data;
	stringData.clear();
	for (u4 i = 0; i < stringSize; ++i)
	{
		DexString* dex_string = new DexString;
		u1* ptr = base + dex_string_id->stringDataOff;
		while (*(ptr++) > 0x7f) /* empty */;
		//int uleblen = DexFileUtils::readSLeb128(&ptr);
		dex_string->data = (char*)ptr;
		dex_string->hash = DexFileUtils::hash(dex_string->data);
		dex_string->idx = i;
		dex_string_id++;
		stringData.insert(std::make_pair(i, dex_string));
		//LOGD("[=]read String:%s",dex_string->data);
	}
	LOGI("[+]parse StringID over :0x%08x",stringData.size());
}

DexString* DexFileString::getDexStringById(u4 idx)
{
	u4 size = stringData.size();
	if (idx > size)
	{
		LOGE("[-] string idx > stringData size!");
		return nullptr;
	}
	else
	{
		return stringData[idx];
	}
}

u4 DexFileString::getStringIdx(const char* str)
{
	u4 hash = DexFileUtils::hash(str);
	u4 size = stringData.size();
	for (u4 i = 0; i < size; ++i)
	{
		DexString* temp = stringData[i];
		if (temp->hash == hash)
		{
			return i;
		}
	}
	LOGE("[-]getStringIdx fail!!!");
	return -1;
}

DexFileString::~DexFileString()
{
}
