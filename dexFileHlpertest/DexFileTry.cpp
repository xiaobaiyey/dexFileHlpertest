#include "DexFileTry.h"
#include "DexFileHelper.h"
#include "DexFileUtils.h"


u4 DexFileTry::dexGetHandlersSize(DexCode* pCode)
{
	if (pCode->triesSize == 0)
	{
		return 0;
	}
	const u1* data = dexGetCatchHandlerData(pCode);

	return DexFileUtils::readUnsignedLeb128(&data);
}

u4 DexFileTry::dexGetFirstHandlerOffset(DexCode* pCode)
{
	if (pCode->triesSize == 0)
	{
		return 0;
	}
	const u1* baseData = dexGetCatchHandlerData(pCode);
	const u1* data = baseData;
	DexFileUtils::readUnsignedLeb128(&data);

	return data - baseData;
}

u1* DexFileTry::dexGetCatchHandlerData(DexCode* pCode)
{
	const DexTry* pTries = dexGetTries(pCode);
	return (u1*)&pTries[pCode->triesSize];
}

DexTry* DexFileTry::dexGetTries(const DexCode* pCode)
{
	const u2* insnsEnd = &pCode->insns[pCode->insnsSize];

	// Round to four bytes.
	if ((((uintptr_t)insnsEnd) & 3) != 0)
	{
		insnsEnd++;
	}
	return (DexTry*)insnsEnd;
}

void DexFileTry::dexCatchIteratorInit(DexCatchIterator* pIterator, DexCode* pCode, u4 offset)
{
	dexCatchIteratorInitToPointer(pIterator,
	                              dexGetCatchHandlerData(pCode) + offset);
}

void DexFileTry::dexCatchIteratorInitToPointer(DexCatchIterator* pIterator, u1* pEncodedData)
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

u4 DexFileTry::dexCatchIteratorGetEndOffset(DexCatchIterator* pIterator, DexCode* pCode)
{
	while (dexCatchIteratorNext(pIterator) != nullptr) /* empty */;

	return (u4)(pIterator->pEncodedData - dexGetCatchHandlerData(pCode));
}

DexCatchHandler* DexFileTry::dexCatchIteratorNext(DexCatchIterator* pIterator)
{
	if (pIterator->countRemaining == 0)
	{
		if (!pIterator->catchesAll)
		{
			return nullptr;
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

void DexFileTry::scanTryCatchBlocks(DexCode* pCode, DexCodeMemory* dex_code_memory, DexFileHelper* dex_file_helper)
{
	u4 triesSize = pCode->triesSize;
	DexTry* pTries;
	u4 idx;
	pTries = dexGetTries(pCode);

	/*new memory*/
	DexTryMemory* dex_try_memory = new DexTryMemory;
	/*collect DexTry*/
	for (idx = 0; idx < triesSize; idx++)
	{
		DexTry* pTry = &pTries[idx];
		dex_try_memory->dex_tries.push_back(pTry);
	}
	/*collect handle list*/
	/* Iterate over each of the handlers to verify target addresses. */
	u4 handlersSize = dexGetHandlersSize(pCode);
	u4 offset = dexGetFirstHandlerOffset(pCode);
	for (idx = 0; idx < handlersSize; idx++)
	{
		DexCatchIterator iterator;
		dexCatchIteratorInit(&iterator, pCode, offset);

		DexTryItem* dex_try_item = new DexTryItem;
		dex_try_item->catchesAll = iterator.catchesAll;
		dex_try_item->count = dex_try_item->catchesAll ? -(iterator.countRemaining) : iterator.countRemaining;


		for (;;)
		{
			DexCatchHandler* handler = dexCatchIteratorNext(&iterator);
			u4 addr;
			if (handler == nullptr)
			{
				break;
			}
			DexCatchHandlerMemory* dex_catch_handler_memory = new DexCatchHandlerMemory;
			s4 typeidx = handler->typeIdx;

			dex_catch_handler_memory->typeIdx = typeidx == kDexNoIndex
				                                    ? nullptr
				                                    : dex_file_helper->dex_file_type->getTypeById(handler->typeIdx);
			dex_catch_handler_memory->offset = handler->address;
			dex_try_item->dex_catch_handler_memories.push_back(dex_catch_handler_memory);
		}

		offset = dexCatchIteratorGetEndOffset(&iterator, pCode);
		dex_code_memory->dex_try_memories = dex_try_memory;
		dex_code_memory->dex_try_memories->dex_try_items.push_back(dex_try_item);
	}
}
