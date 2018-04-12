#include "DexFileEncodedArray.h"
#include "DexFileHelper.h"
#include "pch.h"
#include "DexFileUtils.h"


DexFileEncodedArray::DexFileEncodedArray(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeEncodedArrayItem);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeAnnotationSetItem is nullptr");
		return;
	}
	u1* data = base + dex_map_item->offset;
	u4 set_item_len = dex_map_item->size;

	for (u4 i = 0; i < set_item_len; ++i)
	{
		DexEncodedArray* dex_encoded_array = new DexEncodedArray;
		u4 offset = data - base;
		dex_encoded_array->offset = offset;
		dex_encoded_array->data = data;
		U4 itemsSize = DexFileUtils::readUnsignedLeb128((const u1**)&data);
		for (u4 m_j = 0; m_j < itemsSize; m_j++)
		{
			int mType = *data++;
			int mValueLen = getValueLen(mType);
			data += mValueLen;
		}
		dex_encoded_array->len = dex_encoded_array->data - data;
		dex_encoded_array_maps.insert(std::make_pair(offset, dex_encoded_array));
	}
	LOGI("[+]kDexTypeEncodedArrayItem size:0x%04x",dex_encoded_array_maps.size());
}

DexFileEncodedArray::~DexFileEncodedArray()
{
}

DexEncodedArray* DexFileEncodedArray::getDexEncodedArrayByOffset(u4 offset)
{
	DexEncodedArray* encoded_array = dex_encoded_array_maps[offset];
	if (encoded_array == nullptr)
	{
		LOGE("[-]getDexEncodedArrayByOffset offset:0x%04x", offset);
	}
	return encoded_array;
}

size_t DexFileEncodedArray::getValueLen(int inType)
{
	int mTypeClass = inType & kDexAnnotationValueTypeMask;
	int mTypeLen = inType >> kDexAnnotationValueArgShift;
	switch (mTypeClass)
	{
	case kDexAnnotationNull:
	case kDexAnnotationBoolean:
		return 0;
	}
	return mTypeLen + 1;
}
