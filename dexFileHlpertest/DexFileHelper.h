#pragma once
#include "DexDefine.h"
#include "DexFileHeader.h"
#include "DexFileString.h"
#include "DexFileType.h"
#include "DexFileProtoType.h"
#include "DexFileMap.h"
#include "DexFileTypeList.h"
#include "DexFileAnnotationItem.h"
#include "DexFileAnnotationSetItem.h"
#include "DexFileAnnotationsDirectoryItem.h"
#include "DexFileField.h"
#include "DexFileMethod.h"
#include "DexFileAnnotationSetRefList.h"
#include "DexFileEncodedArray.h"
#include "DexFileClass.h"

class DexFileHelper
{
public:
	DexFileHelper(u1* data, u4 len);
	static DexFileHelper* getDexFileHelper(const char* path);
	DexFileHeader* dex_file_header;
	DexFileString* dex_file_string;
	DexFileType* dex_file_type;
	DexFileProtoType* dex_file_proto;
	DexFileTypeList* dex_file_type_list;
	DexFileField* dex_file_field;
	DexFileMethod* dex_file_method;
	DexFileMap* dex_file_map;
	DexFileAnnotationItem* dex_file_annotation_item;
	DexFileAnnotationSetItem* dex_file_annotation_set_item;
	DexFileAnnotationsDirectoryItem* dex_file_annotations_directory_item;
	DexFileAnnotationSetRefList* dex_file_annotation_set_ref_list;
	DexFileEncodedArray* dex_file_encoded_array;
	DexFileClass* dex_file_class;
private:
	u1 * dex_address;
};
