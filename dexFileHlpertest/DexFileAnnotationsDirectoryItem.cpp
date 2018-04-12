#include "DexFileAnnotationsDirectoryItem.h"
#include "DexFileHelper.h"
#include "pch.h"


DexFileAnnotationsDirectoryItem::DexFileAnnotationsDirectoryItem(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	u1* base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeAnnotationsDirectoryItem);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeAnnotationsDirectoryItem is nullptr");
		return;
	}
	u1* data = base + dex_map_item->offset;
	u4 size = dex_map_item->size;

	for (u4 i = 0; i < size; ++i)
	{
		DexAnnotationsDirectoryItem* dex_annotations_directory_item = new DexAnnotationsDirectoryItem;
		u4 offset = data - base;

		//set offset
		dex_annotations_directory_item->offset = offset;
		//read classAnnotationsOff
		u4 classAnnotationsOff = *reinterpret_cast<u4*>(data);
		dex_annotations_directory_item->classAnnotationsOff = dex_file_helper
		                                                      ->dex_file_annotation_set_item->getDexAnnotationSetItemByOffset(
			                                                      classAnnotationsOff);
		data = data + sizeof(classAnnotationsOff);
		//read fieldsSize
		u4 fieldsSize = *reinterpret_cast<u4*>(data);
		dex_annotations_directory_item->fieldsSize = fieldsSize;
		data = data + sizeof(fieldsSize);
		//read methodsSize
		u4 methodsSize = *reinterpret_cast<u4*>(data);
		dex_annotations_directory_item->methodsSize = methodsSize;
		data = data + sizeof(methodsSize);
		//read parametersSize
		u4 parametersSize = *reinterpret_cast<u4*>(data);
		dex_annotations_directory_item->parametersSize = parametersSize;
		data = data + sizeof(parametersSize);

		//read DexFieldAnnotationsItems
		for (u4 j = 0; j < fieldsSize; ++j)
		{
			DexFieldAnnotationsItem* dex_field_annotations = new DexFieldAnnotationsItem;
			u4 fieldIdx = *reinterpret_cast<u4*>(data);
			dex_field_annotations->fieldIdx = fieldIdx;
			data = data + sizeof(fieldIdx);
			u4 annotationsOff = *reinterpret_cast<u4*>(data);
			dex_field_annotations->annotationsOff = annotationsOff;
			dex_field_annotations->dex_annotation_set_item = dex_file_helper
			                                                 ->dex_file_annotation_set_item->getDexAnnotationSetItemByOffset(
				                                                 annotationsOff);
			data = data + sizeof(annotationsOff);
			dex_annotations_directory_item->field_annotations_items.push_back(dex_field_annotations);
		}
		//read DexMethodAnnotationsItems
		for (u4 j = 0; j < methodsSize; ++j)
		{
			DexMethodAnnotationsItem* dex_method_annotations_item = new DexMethodAnnotationsItem;
			u4 methodIdx = *reinterpret_cast<u4*>(data);
			dex_method_annotations_item->methodIdx = methodIdx;
			data = data + sizeof(methodIdx);
			u4 annotationsOff = *reinterpret_cast<u4*>(data);
			dex_method_annotations_item->annotationsOff = annotationsOff;
			dex_method_annotations_item->dex_annotation_set_item = dex_file_helper
			                                                       ->dex_file_annotation_set_item->
			                                                       getDexAnnotationSetItemByOffset(annotationsOff);
			data = data + sizeof(annotationsOff);
		}
		//read DexParameterAnnotationsItems
		for (u4 j = 0; j < parametersSize; ++j)
		{

		}
	}
}

DexFileAnnotationsDirectoryItem::~DexFileAnnotationsDirectoryItem()
{
}
