#include "DexFileClass.h"
#include "DexFileHelper.h"
#include "pch.h"
#include "DexFileUtils.h"


DexFileClass::DexFileClass(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	this->base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeClassDefItem);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeAnnotationSetItem is nullptr");
		return;
	}
	u1* data = base + dex_map_item->offset;
	u4 set_item_len = dex_map_item->size;
	dex_file_class_method_ = dex_file_helper_->dex_file_class_method;
	parseClassDef(data, set_item_len);
	LOGI("[+]kDexTypeClassDefItem size:0x%08x",dex_class_def_maps.size());
}

DexFileClass::~DexFileClass()
{
}


void DexFileClass::parseClassDef(u1* data, u4 defSize)
{
	DexClassDef* dex_class_def = reinterpret_cast<DexClassDef*>(data);
	for (u4 i = 0; i < defSize; ++i)
	{
		DexClass* dex_class = new DexClass;
		DexTypeIdx* classType = dex_file_helper_->dex_file_type->getTypeById(dex_class_def->classIdx);
		dex_class->classType = classType;
		u4 accessFlags = dex_class_def->accessFlags;
		dex_class->accessFlags = accessFlags;
		DexTypeIdx* superclassType = dex_file_helper_->dex_file_type->getTypeById(dex_class_def->superclassIdx);
		dex_class->superclassType = superclassType;

		DexTypeList* interfacesList = dex_class_def->interfacesOff == 0
			                              ? nullptr
			                              : dex_file_helper_->dex_file_type_list->getDexFileTypeListByOffset(
				                              dex_class_def->interfacesOff);
		dex_class->interfacesList = interfacesList;


		DexStringIdx* sourceFileString = dex_class_def->sourceFileIdx == 0 || dex_class_def->sourceFileIdx == kDexNoIndex
			                                 ? nullptr
			                                 : dex_file_helper_->dex_file_string->getDexStringById(dex_class_def->sourceFileIdx);
		dex_class->sourceFileString = sourceFileString;


		DexAnnotationsDirectoryItem* dex_annotations_directory_item = dex_class_def->annotationsOff == 0
			                                                              ? nullptr
			                                                              : dex_file_helper_
			                                                                ->dex_file_annotations_directory_item->
			                                                                getDexAnnotationsDirectoryItemByOffset(
				                                                                dex_class_def->annotationsOff);
		dex_class->dex_annotations_directory_item = dex_annotations_directory_item;


		DexEncodedArray* dex_encoded_array = dex_class_def->staticValuesOff == 0
			                                     ? nullptr
			                                     : dex_file_helper_->dex_file_encoded_array->getDexEncodedArrayByOffset(
				                                     dex_class_def->staticValuesOff);
		dex_class->dex_encoded_array = dex_encoded_array;


		DexClassData* dex_class_data = dex_class_def->classDataOff == 0
			                               ? nullptr
			                               : parseClassData(dex_class, dex_class_def->classDataOff);
		dex_class->dex_class_data = dex_class_data;
		dex_class_def_maps.insert(std::make_pair(i, dex_class));
		dex_class_def++;
	}
}

DexClassData* DexFileClass::parseClassData(DexClass* dex_class, u4 offset)
{
	DexClassData* dex_class_data = new DexClassData;

	u1* Pdata = dex_file_helper_->dex_file_header->getBaseAddress();
	const u1* pdata = Pdata + offset;
	//Pdata = Pdata + offset;
	DexClassDataHeader* dex_class_data_header = ReadClassDataHeader(&pdata);
	dex_class_data->header = dex_class_data_header;

	if (dex_class_data_header->staticFieldsSize > 0)
	{
		int count = (int)dex_class_data_header->staticFieldsSize;
		u4 lastIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			DexField* dex_field = ReadClassDataField(&pdata, &lastIndex);
			dex_class_data->staticFields.push_back(dex_field);
		}
	}
	if (dex_class_data_header->instanceFieldsSize > 0)
	{
		int count = (int)dex_class_data_header->instanceFieldsSize;
		u4 lastIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			DexField* dex_field = ReadClassDataField(&pdata, &lastIndex);
			dex_class_data->instanceFields.push_back(dex_field);
		}
	}

	if (dex_class_data_header->directMethodsSize > 0)
	{
		int count = (int)dex_class_data_header->directMethodsSize;
		u4 lastIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			DexMethod* dex_method = dex_file_class_method_->parseMethod(dex_class, &pdata, &lastIndex);
			dex_class_data->directMethods.push_back(dex_method);
		}
	}

	if (dex_class_data_header->virtualMethodsSize > 0)
	{
		int count = (int)dex_class_data_header->virtualMethodsSize;
		u4 lastIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			DexMethod* dex_method = dex_file_class_method_->parseMethod(dex_class, &pdata, &lastIndex);
			dex_class_data->virtualMethods.push_back(dex_method);
		}
	}
	return dex_class_data;
}

DexClassDataHeader* DexFileClass::ReadClassDataHeader(const u1** pData)
{
	DexClassDataHeader* pHeader = new DexClassDataHeader;
	pHeader->staticFieldsSize = DexFileUtils::readUnsignedLeb128(pData);
	pHeader->instanceFieldsSize = DexFileUtils::readUnsignedLeb128(pData);
	pHeader->directMethodsSize = DexFileUtils::readUnsignedLeb128(pData);
	pHeader->virtualMethodsSize = DexFileUtils::readUnsignedLeb128(pData);
	return pHeader;
}

DexField* DexFileClass::ReadClassDataField(const u1** pData, u4* lastIndex)
{
	DexField* dex_field = new DexField;
	u4 index = *lastIndex + DexFileUtils::readUnsignedLeb128(pData);
	dex_field->accessFlags = DexFileUtils::readUnsignedLeb128(pData);
	dex_field->fieldIdx = dex_file_helper_->dex_file_field_idx->getDexFieldIdxById(index);
	*lastIndex = index;
	return dex_field;
}
