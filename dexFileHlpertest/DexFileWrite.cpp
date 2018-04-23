#include "DexFileWrite.h"
#include "DexDefine.h"
#include "DexFileHelper.h"


DexFileWrite::DexFileWrite(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
}

void DexFileWrite::write()
{
	//write len
	u4 header_len = sizeof(DexHeader);
	header = new u1[header_len];
	memset(header, 0, header_len);

	u4 string_idx_len = dex_file_helper_->dex_file_string->getStringIdxSize() * sizeof(u4);
	string_offset = header_len;
	string_idx_data = new u1[string_idx_len];
	memset(string_idx_data, 0, string_idx_len);

	u4 type_idx_len = dex_file_helper_->dex_file_type->getTypeIdxSize() * sizeof(u4);
	type_offset = string_offset + string_idx_len;

	u4 proto_idx_len = dex_file_helper_->dex_file_proto->getProtoIdxSize() * (sizeof(u4) * 3);
	proto_offset = type_offset + type_idx_len;

	u4 method_idx_len = dex_file_helper_->dex_file_method_idx->getDexMethodIdxSize() * (sizeof(u4) * 2);
	method_offset = proto_offset + proto_idx_len;

	u4 index_len = header_len + string_idx_len + proto_idx_len + method_idx_len + type_idx_len;
}


DexFileWrite::~DexFileWrite()
{
}

void DexFileWrite::writeStringIdx()
{
}
