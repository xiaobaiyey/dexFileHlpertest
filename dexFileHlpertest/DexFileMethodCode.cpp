#include "pch.h"
#include "DexFileHelper.h"
#include "DexFileUtils.h"
#include "DexFileTry.h"
#include "DexFileMethodCode.h"

/*
 * [android dex文件中try块数据结构中各个字段的含义](https://blog.csdn.net/u012684933/article/details/43954091)
 */
DexFileMethodCode::DexFileMethodCode(DexFileHelper* dex_file_helper)
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
		dex_code_memory->registersSize = dex_code->registersSize;
		dex_code_memory->insSize = dex_code->insSize;
		dex_code_memory->outsSize = dex_code->outsSize;
		dex_code_memory->triesSize = dex_code->triesSize;
		/*clear debug info*/
		dex_code_memory->debugInfoOff = 0;
		dex_code_memory->opcodelen = dex_code->insnsSize;
		/*check padding*/
		dex_code_memory->padding = checkPadding(dex_code);
		/*parse try items*/
		u1* temp = data;
		/*get Dexcode len*/
		dex_code_memory->dexcodelen = DexFileUtils::align_4(GetDexCodeSize(temp));
		//LOGI("index %d offset 0x%08x len:%d", i, offset, dex_code_memory->dexcodelen);
		dex_code_memory_maps.insert(std::make_pair(offset, dex_code_memory));
		if (dex_code->triesSize > 0)
		{
			//这里暂时不做解析，解析起来太麻烦，其主要用的是typeidx,typeidx我们暂时不做修改。
			//想了想 还是解析把,为了后面更好的扩展，虽然有点浪费内存
			DexFileTry::scanTryCatchBlocks(dex_code, dex_code_memory, dex_file_helper);
		}
		/*get next DexCode*/
		data = data + dex_code_memory->dexcodelen;
		
	}
	LOGI("[+]kDexTypeCodeItem size:0x%04x", dex_code_memory_maps.size());
}

bool DexFileMethodCode::checkPadding(DexCode* dex_code)
{
	u2* end = &dex_code->insns[dex_code->insSize + 1];
	if (*end == 0x0000)
	{
		return true;
	}
	return false;
}


void DexFileMethodCode::addOpCode(u1* data, u4 len)
{
}

size_t DexFileMethodCode::GetDexCodeSize(DexCode* pCode)
{
	/*
	* The catch handler data is the last entry.  It has a variable number
	* of variable-size pieces, so we need to create an iterator.
	*/
	u4 handlersSize;
	u4 offset;
	u4 ui;

	if (pCode->triesSize != 0)
	{
		handlersSize = DexFileTry::dexGetHandlersSize(pCode);
		offset = DexFileTry::dexGetFirstHandlerOffset(pCode);
	}
	else
	{
		handlersSize = 0;
		offset = 0;
	}

	for (ui = 0; ui < handlersSize; ui++)
	{
		DexCatchIterator iterator;
		DexFileTry::dexCatchIteratorInit(&iterator, pCode, offset);
		offset = DexFileTry::dexCatchIteratorGetEndOffset(&iterator, pCode);
	}

	const u1* handlerData = DexFileTry::dexGetCatchHandlerData(pCode);
	return (handlerData - (u1*)pCode) + offset;
}

size_t DexFileMethodCode::GetDexCodeSize(const u1* pCode)
{
	DexCode* dex_code = (DexCode*)pCode;
	return GetDexCodeSize(dex_code);
}

DexCodeMemory* DexFileMethodCode::getDexCodeByOffset(u4 offset)
{
	DexCodeMemory* dex_code_memory = dex_code_memory_maps[offset];

	if (dex_code_memory==nullptr)
	{
		LOGE("[-] code offset erro 0x%08x",offset);
	}
	return dex_code_memory;
}


DexFileMethodCode::~DexFileMethodCode()
{
}
