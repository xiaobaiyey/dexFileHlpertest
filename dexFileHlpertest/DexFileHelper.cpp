#include "dexFileHelper.h"
#include "DexFileUtils.h"
#include "pch.h"
#include "DexFileEncodedArray.h"
#include "DexFileMethodIdx.h"


DexFileHelper::DexFileHelper(u1* data, u4 len)
{
	if (!DexFileUtils::isDex(data))
	{
		LOGE("[-] address is not dex formart");
		return;
	}
	dex_address = data;
	dex_file_header = new DexFileHeader(this, data);
	dex_file_map = new DexFileMap(this);
	dex_file_string = new DexFileStringIdx(this);
	dex_file_type = new DexFileTypeIdx(this);
	dex_file_type_list = new DexFileTypeList(this);
	dex_file_proto = new DexFileProtoTypeIdx(this);
	dex_file_field_idx = new DexFileFieldIdx(this);
	dex_file_method_idx = new DexFileMethodIdx(this);
	dex_file_annotation_item = new DexFileAnnotationItem(this);
	dex_file_annotation_set_item = new DexFileAnnotationSetItem(this);
	dex_file_annotation_set_ref_list = new DexFileAnnotationSetRefList(this);
	dex_file_annotations_directory_item = new DexFileAnnotationsDirectoryItem(this);
	dex_file_encoded_array = new DexFileEncodedArray(this);
	dex_file_code = new DexFileMethodCode(this);
	dex_file_class_method = DexFileClassMethod::getInstance(this);
	dex_file_class = new DexFileClass(this);
}

DexFileHelper* DexFileHelper::getDexFileHelper(const char* path)
{
	u4 len = 0;

	u1* res = DexFileUtils::readFile(path, &len);

	if (res == nullptr || len == 0)
	{
		LOGE("[-]read file fail");
		return nullptr;
	}
	return new DexFileHelper(res, len);
}
