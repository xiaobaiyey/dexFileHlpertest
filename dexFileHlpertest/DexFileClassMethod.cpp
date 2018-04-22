#include "DexFileClassMethod.h"
#include "DexFileUtils.h"
#include "DexFileHelper.h"
DexFileClassMethod* DexFileClassMethod::dex_file_class_method_ = nullptr;

DexFileClassMethod::DexFileClassMethod(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
}


DexFileClassMethod* DexFileClassMethod::getInstance(DexFileHelper* dex_file_helper)
{
	if (dex_file_class_method_ == nullptr)
	{
		dex_file_class_method_ = new DexFileClassMethod(dex_file_helper);
	}
	return dex_file_class_method_;
}

DexMethod* DexFileClassMethod::parseMethod(DexClass* dex_class, const u1** pdata, u4* lastindex)
{
	DexMethod* dex_method = new DexMethod;
	u4 index = *lastindex + DexFileUtils::readUnsignedLeb128(pdata);
	dex_method->accessFlags = DexFileUtils::readUnsignedLeb128(pdata);
	u4 codeoff = DexFileUtils::readUnsignedLeb128(pdata);
	dex_method->codeOff = codeoff == 0 ? nullptr : dex_file_helper_->dex_file_code->getDexCodeByOffset(codeoff);
	dex_method->methodIdx = dex_file_helper_->dex_file_method_idx->getDexFileMethodIdxById(index);
	dex_method->dex_class = dex_class;
	*lastindex = index;
	dex_file_method_maps.insert(std::make_pair(index, dex_method));
	return dex_method;
}

DexFileClassMethod::~DexFileClassMethod()
{
}
