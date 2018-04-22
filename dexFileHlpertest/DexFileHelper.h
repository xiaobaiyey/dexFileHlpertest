#pragma once
#include "DexDefine.h"
#include "DexFileHeader.h"

#include "DexFileMap.h"
#include "DexFileTypeList.h"
#include "DexFileAnnotationItem.h"
#include "DexFileAnnotationSetItem.h"
#include "DexFileAnnotationsDirectoryItem.h"
#include "DexFileAnnotationSetRefList.h"
#include "DexFileEncodedArray.h"
#include "DexFileClass.h"
#include "DexFileMethodCode.h"
#include "DexFileMethodIdx.h"
#include "DexFileFieldIdx.h"
#include "DexFileProtoTypeIdx.h"
#include "DexFileStringIdx.h"
#include "DexFileTypeIdx.h"
#include "DexFileClassMethod.h"

class DexFileHelper
{
public:
	DexFileHelper(u1* data, u4 len);
	static DexFileHelper* getDexFileHelper(const char* path);
	DexFileHeader* dex_file_header;
	DexFileStringIdx* dex_file_string;
	DexFileTypeIdx* dex_file_type;
	DexFileProtoTypeIdx* dex_file_proto;
	DexFileTypeList* dex_file_type_list;
	DexFileFieldIdx* dex_file_field_idx;
	DexFileMethodIdx* dex_file_method_idx;
	DexFileMap* dex_file_map;
	DexFileAnnotationItem* dex_file_annotation_item;
	DexFileAnnotationSetItem* dex_file_annotation_set_item;
	DexFileAnnotationsDirectoryItem* dex_file_annotations_directory_item;
	DexFileAnnotationSetRefList* dex_file_annotation_set_ref_list;
	DexFileEncodedArray* dex_file_encoded_array;
	DexFileMethodCode* dex_file_code;
	DexFileClass* dex_file_class;
	DexFileClassMethod* dex_file_class_method;
private:
	u1 * dex_address;
};
