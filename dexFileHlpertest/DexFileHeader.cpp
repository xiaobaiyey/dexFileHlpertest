#include "DexFileHeader.h"
#include "pch.h"
#include "DexFileUtils.h"


DexFileHeader::DexFileHeader(DexFileHelper* dex_file_hepter, void* memPtr)
{
	this->dex_file_hepter_ = dex_file_hepter;
	if (DexFileUtils::isDex(memPtr))
	{
		dataBasePtr = DexFileUtils::getBase(memPtr);
		u1* temp = (new u1[sizeof(DexHeader)]);
		memcpy(temp, dataBasePtr, sizeof(DexHeader));
		dex_header_ = (DexHeader*)temp;
		LOGD("[=] backup header success");
	}
	else
	{
		isDex = false;
		dex_header_ = nullptr;
		LOGE("[-]target address not dex formart");
	}
}

bool DexFileHeader::check()
{
	return isDex;
}


u4 DexFileHeader::getFileSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->fileSize;
	}
	else
	{
		return -1;
	}
}

u4 DexFileHeader::getLinkSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->linkSize;
	}
	return 0;
}

u4 DexFileHeader::getLinkOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->linkOff;
	}
	return 0;
}

u4 DexFileHeader::getMapOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->mapOff;
	}
	return 0;
}

u4 DexFileHeader::getStringIdsSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->stringIdsSize;
	}
	return 0;
}

u4 DexFileHeader::getStringIdsOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->stringIdsOff;
	}

	return 0;
}

u4 DexFileHeader::getTypeIdsSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->typeIdsSize;
	}
	return 0;
}

u4 DexFileHeader::getTypeIdsOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->typeIdsOff;
	}
	return 0;
}

u4 DexFileHeader::getProtoIdsSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->protoIdsSize;
	}
	return 0;
}

u4 DexFileHeader::getProtoIdsOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->protoIdsOff;
	}
	return 0;
}

u4 DexFileHeader::getFieldIdsSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->fieldIdsSize;
	}
	return 0;
}

u4 DexFileHeader::getFieldIdsOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->fieldIdsOff;
	}
	return 0;
}

u4 DexFileHeader::getMethodIdsSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->methodIdsSize;
	}
	return 0;
}

u4 DexFileHeader::getMethodIdsOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->methodIdsOff;
	}
	return 0;
}

u4 DexFileHeader::getClassDefsSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->classDefsSize;
	}
	return 0;
}

u4 DexFileHeader::getClassDefsOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->classDefsOff;
	}
	return 0;
}

u4 DexFileHeader::getDataSize()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->dataSize;
	}
	return 0;
}

u4 DexFileHeader::getDataOff()
{
	if (dex_header_ != nullptr)
	{
		return dex_header_->dataOff;
	}
	return 0;
}

u1* DexFileHeader::getBaseAddress()
{
	return dataBasePtr;
}

DexFileHeader::~DexFileHeader()
{
}
