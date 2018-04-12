#include "dexFileHelper.h"
#include "DexFileUtils.h"
#include "pch.h"


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
	dex_file_string = new DexFileString(this);
	dex_file_type = new DexFileType(this);
	dex_file_type_list = new DexFileTypeList(this);
	dex_file_proto = new DexFileProtoType(this);
	dex_file_field = new DexFileField(this);
	dex_file_method = new DexFileMethod(this);
	dex_file_annotation_item = new DexFileAnnotationItem(this);
	dex_file_annotation_set_item = new DexFileAnnotationSetItem(this);
	dex_file_annotations_directory_item = new DexFileAnnotationsDirectoryItem(this);
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
