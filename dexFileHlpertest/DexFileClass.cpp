#include "DexFileClass.h"
#include "DexFileHelper.h"
#include "pch.h"


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
}

DexFileClass::~DexFileClass()
{
}


void DexFileClass::parseClassDef(u1* data, u4 defSize)
{
	DexClassDef* dex_class_def = reinterpret_cast<DexClassDef*>(data);
	for (u4 i = 0; i < defSize; ++i)
	{
		DexType* classType = dex_file_helper_->dex_file_type->getTypeById(dex_class_def->classIdx);
		u4 accessFlags = dex_class_def->accessFlags;
		DexType* superclassType = dex_file_helper_->dex_file_type->getTypeById(dex_class_def->superclassIdx);
		DexTypeList* interfacesList = dex_file_helper_->dex_file_type_list->getDexFileTypeListByOffset(
			dex_class_def->interfacesOff);
		DexString* sourceFileString = dex_file_helper_->dex_file_string->getDexStringById(dex_class_def->sourceFileIdx);
		DexAnnotationsDirectoryItem* dex_annotations_directory_item = dex_file_helper_
		                                                              ->dex_file_annotations_directory_item->
		                                                              getDexAnnotationsDirectoryItemByOffset(
			                                                              dex_class_def->annotationsOff);
		DexEncodedArray* dex_encoded_array = dex_file_helper_->dex_file_encoded_array->getDexEncodedArrayByOffset(
			dex_class_def->staticValuesOff);


		dex_class_def++;
	}
}

void DexFileClass::parseClassData(u4 offset)
{

}
