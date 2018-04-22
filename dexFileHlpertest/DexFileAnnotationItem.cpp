#include "DexFileAnnotationItem.h"
#include "DexFileHelper.h"
#include "DexFileUtils.h"
#include "pch.h"


DexFileAnnotationItem::DexFileAnnotationItem(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeAnnotationItem);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeAnnotationItem is nullptr");
		return;
	}
	u1* start = base + dex_map_item->offset;
	u1* temp = start;
	int len = dex_map_item->size;

	for (int i = 0; i < len; ++i)
	{
		DexAnnotationItem* dex_annotation_item = new DexAnnotationItem;
		//get data offset in  dexfile;
		u4 offset = temp - base;
		//set offset
		dex_annotation_item->offset = offset;
		u1* visibility = temp;
		dex_annotation_item->visibility = *visibility;
		// Skip the visibility byte.
		temp++;
		u1* vertemp = temp;
		//get data len
		const u1* tempend = verifyEncodedAnnotation(vertemp);
		u4 enlen = tempend - temp;
		//set data len
		dex_annotation_item->data_len = enlen;
		//set data
		dex_annotation_item->annotation = temp;
		//add to maps
		dex_annotation_item_maps.insert(std::make_pair(offset, dex_annotation_item));
		temp = temp + enlen;
	}
	LOGI("[+]read DexAnnotationItem :0x%08x", dex_annotation_item_maps.size());
	/* data in encoded_annotation format */
}

DexFileAnnotationItem::~DexFileAnnotationItem()
{
}

DexAnnotationItem* DexFileAnnotationItem::getDexAnnotationItembyOffset(u4 offset)
{
	DexAnnotationItem* dex_annotation_item = dex_annotation_item_maps[offset];
	if (dex_annotation_item == nullptr)
	{
		LOGE("[-]getDexAnnotationItem by offset fail:0x%08x",offset);
	}
	return dex_annotation_item;
}

/**
 * \brief just for test
 * \param data 
 */

const u1* DexFileAnnotationItem::verifyEncodedAnnotation(const u1* temp)
{
	const u1* data = temp;
	u4 idx = DexFileUtils::readUnsignedLeb128(&data);
	DexTypeIdx* dex_type = dex_file_helper_->dex_file_type->getTypeById(idx);
	const char* descriptor = dex_type->dex_string->data;
	//LOGD("[=]descriptor:%s", descriptor);
	u4 size = DexFileUtils::readUnsignedLeb128(&data);

	u4 lastIdx = 0;
	bool first = true;
	while (size--)
	{
		idx = DexFileUtils::readUnsignedLeb128(&data);
		char* name = dex_file_helper_->dex_file_string->getDexStringById(idx)->data;
		if (first)
		{
			first = false;
		}
		data = verifyEncodedValue(data);
		lastIdx = idx;
	}
	return data;
}

const u1* DexFileAnnotationItem::verifyEncodedValue(const u1* data)
{
	u1 headerByte = *(data++);
	u4 valueType = headerByte & kDexAnnotationValueTypeMask;
	u4 valueArg = headerByte >> kDexAnnotationValueArgShift;
	switch (valueType)
	{
	case kDexAnnotationByte:
		{
			if (valueArg != 0)
			{
				LOGE("Bogus byte size %#x", valueArg);
				return nullptr;
			}
			data++;
			break;
		}
	case kDexAnnotationShort:
	case kDexAnnotationChar:
		{
			if (valueArg > 1)
			{
				LOGE("Bogus char/short size %#x", valueArg);
				return nullptr;
			}
			data += valueArg + 1;
			break;
		}
	case kDexAnnotationInt:
	case kDexAnnotationFloat:
		{
			if (valueArg > 3)
			{
				LOGE("Bogus int/float size %#x", valueArg);
				return nullptr;
			}
			data += valueArg + 1;
			break;
		}
	case kDexAnnotationLong:
	case kDexAnnotationDouble:
		{
			data += valueArg + 1;
			break;
		}
	case kDexAnnotationString:
		{
			if (valueArg > 3)
			{
				LOGE("Bogus string size %#x", valueArg);
				return nullptr;
			}
			u4 idx = DexFileUtils::readUnsignedLittleEndian(&data, valueArg + 1);

			break;
		}
	case kDexAnnotationType:
		{
			if (valueArg > 3)
			{
				LOGE("Bogus type size %#x", valueArg);
				return nullptr;
			}
			u4 idx = DexFileUtils::readUnsignedLittleEndian(&data, valueArg + 1);

			break;
		}
	case kDexAnnotationField:
	case kDexAnnotationEnum:
		{
			if (valueArg > 3)
			{
				LOGE("Bogus field/enum size %#x", valueArg);
				return nullptr;
			}
			u4 idx = DexFileUtils::readUnsignedLittleEndian(&data, valueArg + 1);

			break;
		}
	case kDexAnnotationMethod:
		{
			if (valueArg > 3)
			{
				LOGE("Bogus method size %#x", valueArg);
				return nullptr;
			}
			u4 idx = DexFileUtils::readUnsignedLittleEndian(&data, valueArg + 1);
			break;
		}
	case kDexAnnotationArray:
		{
			if (valueArg != 0)
			{
				LOGE("Bogus array value_arg %#x", valueArg);
				return nullptr;
			}
			data = verifyEncodedArray(data);
			break;
		}
	case kDexAnnotationAnnotation:
		{
			if (valueArg != 0)
			{
				LOGE("Bogus annotation value_arg %#x", valueArg);
				return nullptr;
			}
			data = verifyEncodedAnnotation(data);
			break;
		}
	case kDexAnnotationNull:
		{
			if (valueArg != 0)
			{
				LOGE("Bogus null value_arg %#x", valueArg);
				return nullptr;
			}
			// Nothing else to do for this type.
			break;
		}
	case kDexAnnotationBoolean:
		{
			if (valueArg > 1)
			{
				LOGE("Bogus boolean value_arg %#x", valueArg);
				return nullptr;
			}
			// Nothing else to do for this type.
			break;
		}
	default:
		{
			LOGE("Bogus value_type %#x", valueType);
			return nullptr;
		}
	}

	return data;
}

const u1* DexFileAnnotationItem::verifyEncodedArray(const u1* data)
{
	u4 size = DexFileUtils::readUnsignedLeb128(&data);

	while (size--)
	{
		data = verifyEncodedValue(data);
		if (data == nullptr)
		{
			LOGE("Bogus encoded_array value");
			return nullptr;
		}
	}
	return data;
}
