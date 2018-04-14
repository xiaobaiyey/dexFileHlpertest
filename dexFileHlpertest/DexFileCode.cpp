#include "DexFileCode.h"
#include "pch.h"
#include "DexFileHelper.h"
#include "DexFileUtils.h"

/*
 * [android dex文件中try块数据结构中各个字段的含义](https://blog.csdn.net/u012684933/article/details/43954091)
 */
DexFileCode::DexFileCode(DexFileHelper* dex_file_helper)
{
	this->dex_file_helper_ = dex_file_helper;
	this->base = dex_file_helper->dex_file_header->getBaseAddress();
	DexMapItem* dex_map_item = dex_file_helper->dex_file_map->getMapItemByType(kDexTypeCodeItem);
	if (dex_map_item == nullptr)
	{
		LOGE("[-]kDexTypeCodeItem is nullptr");
		return;
	}
	u1* data = base + dex_map_item->offset;
	u4 set_item_len = dex_map_item->size;
	DexCode* dex_code = reinterpret_cast<DexCode*>(data);
	for (u4 i = 0; i < set_item_len; ++i)
	{
		dex_code = reinterpret_cast<DexCode*>(data);
		DexCodeMemory* dex_code_memory = new DexCodeMemory;
		u4 offset = data - base;
		dex_code_memory->offset = offset;
		dex_code_memory->dex_code = dex_code;
		dex_code_memory->triesSize = dex_code->triesSize;
		dex_code_memory->dex_code = dex_code;
		dex_code_memory->dex_code_len = getDexCodeSize(data);
		data = data + dex_code_memory->dex_code_len;
	}
}

bool DexFileCode::checkPadding(DexCode* dex_code)
{
	u2* end = &dex_code->insns[dex_code->insSize + 1];
	if (*end == 0x0000)
	{
		return true;
	}
	return false;
}

void DexFileCode::parseTries(u1* data)
{
	struct DexTryHandleEntry
	{
		u4
	};
	struct DexTryMemory
	{
		u4 startAddr; /* start address, in 16-bit code units */
		u2 insnCount; /* instruction count, in 16-bit code units */
		u2 handlerOff; /* offset in encoded handler data to handlers */
		
	};
	/*DexTry*/
	//DexTry* pTries = dexGetTries(dex_code);
	DexCode* dex_code = reinterpret_cast<DexCode*>(data);
	/*read try data*/
	u4 dex_try_size = dex_code->triesSize;
	u2* dex_try_data = &dex_code->insns[dex_code->insnsSize];
	// Round to four bytes.
	if ((((uintptr_t)dex_try_data) & 3) != 0)
	{
		dex_try_data++;
	}
	DexTry* dex_try = reinterpret_cast<DexTry*>(dex_try_data);
	u1* handle_data_list = reinterpret_cast<u1*>(dex_try_data) + sizeof(DexTry) * dex_try_size;
	const u1* temp_handle_data = handle_data_list;
	u4 handle_list_size = DexFileUtils::readUnsignedLeb128(&temp_handle_data);
	for (u4 i = 0; i < dex_try_size; ++i)
	{
		DexTry temp = dex_try[i];
		//read handle data
		u1* handle_data = handle_data_list + temp.handlerOff;
		u4 handle_size = DexFileUtils::readSignedLeb128(&handle_data);
		//foreach entry
		for (u4 j_i = 0; j_i < handle_size; ++j_i)
		{
			u4 typeidx = DexFileUtils::readSignedLeb128(&handle_data);
			DexType* dex_type = dex_file_helper_->dex_file_type->getTypeById(typeidx);
			//catch offset in inxxx
			u4 offset = DexFileUtils::readSignedLeb128(&handle_data);
		}

	}
}

void DexFileCode::dexCatchIteratorInitToPointer(DexCatchIterator* pIterator, u1* pEncodedData)
{
	s4 count = DexFileUtils::readSignedLeb128(&pEncodedData);

	if (count <= 0)
	{
		pIterator->catchesAll = true;
		count = -count;
	}
	else
	{
		pIterator->catchesAll = false;
	}

	pIterator->pEncodedData = pEncodedData;
	pIterator->countRemaining = count;
}

u1* DexFileCode::dexGetCatchHandlerData(DexCode* pCode)
{
	DexTry* pTries = dexGetTries(pCode);
	return (u1*)&pTries[pCode->triesSize];
}

void DexFileCode::dexCatchIteratorInit(DexCatchIterator* pIterator, DexCode* pCode, u4 offset)
{
	dexCatchIteratorInitToPointer(pIterator,
	                              dexGetCatchHandlerData(pCode) + offset);
}

DexCatchHandler* DexFileCode::dexCatchIteratorNext(DexCatchIterator* pIterator)
{
	if (pIterator->countRemaining == 0)
	{
		if (!pIterator->catchesAll)
		{
			return NULL;
		}

		pIterator->catchesAll = false;
		pIterator->handler.typeIdx = kDexNoIndex;
	}
	else
	{
		u4 typeIdx = DexFileUtils::readUnsignedLeb128(&pIterator->pEncodedData);
		pIterator->handler.typeIdx = typeIdx;
		pIterator->countRemaining--;
	}

	pIterator->handler.address = DexFileUtils::readUnsignedLeb128(&pIterator->pEncodedData);
	return &pIterator->handler;
}

void DexFileCode::addOpCode(u1* data, u4 len)
{
}

/**
 * \brief get DexCode Size
 * \param data ptr 
 * \return 
 */
u4 DexFileCode::getDexCodeSize(u1* data)
{
	u1* temp = data;
	DexCode* dex_code = reinterpret_cast<DexCode*>(temp);
	u4 size = dex_code->insSize * 2 + 0x10;
	size = DexFileUtils::align_4(size);
	u4 try_size = dex_code->triesSize;
	if (try_size != 0)
	{
		size = size + try_size * 8;
		const u1* catch_ = temp + size;
		u4 catch_block = DexFileUtils::readUnsignedLeb128(&catch_);
		for (u4 j_i = 0; j_i < catch_block; ++j_i)
		{
			u4 mSize_Handler = DexFileUtils::readUnsignedLeb128(&catch_);
			if (mSize_Handler == 0)
			{
				DexFileUtils::readUnsignedLeb128(&catch_);
			}
			else if (mSize_Handler > 0x40)
			{
				int m_Handler = (0 - mSize_Handler) & 0x7F;
				//Type
				DexFileUtils::readUnsignedLeb128(&catch_);
				for (int m_j = 0; m_j < m_Handler; m_j++)
				{
					DexFileUtils::readUnsignedLeb128(&catch_);
					DexFileUtils::readUnsignedLeb128(&catch_);
				}
			}
			else if (mSize_Handler < 0x40)
			{
				for (u4 m_j = 0; m_j < mSize_Handler; m_j++)
				{
					DexFileUtils::readUnsignedLeb128(&catch_);
					DexFileUtils::readUnsignedLeb128(&catch_);
				}
			}
		}
		size = catch_ - temp;
	}
	size = DexFileUtils::align_4(size);
	return size;
}


DexFileCode::~DexFileCode()
{
}

DexTry* DexFileCode::dexGetTries(DexCode* pCode)
{
	const u2* insnsEnd = &pCode->insns[pCode->insnsSize];

	// Round to four bytes.
	if ((((uintptr_t)insnsEnd) & 3) != 0)
	{
		insnsEnd++;
	}

	return (DexTry*)insnsEnd;
}
