#pragma once
#include "DexDefine.h"
class DexFileHelper;

class DexFileWrite
{
public:
	DexFileWrite(DexFileHelper* dex_file_helper);
	void write();
	~DexFileWrite();
private:
	DexFileHelper* dex_file_helper_;

	u1* header;
	u4 header_offset;
	u1* string_idx_data;
	u4 string_offset;
	u1* type_idx_data;
	u4 type_offset;
	u1* proto_idx_data;
	u4 proto_offset;
	u1* method_idx_data;
	u4 method_offset;

	void writeStringIdx();

	/*
	 * sizeof(DexHeader)
	 * sizeof(u4)*stringidxSize
	 * 
	 *
	 *
	 */
};
