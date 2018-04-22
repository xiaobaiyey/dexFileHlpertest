#pragma once
#include "DexDefine.h"
class DexFileHelper;

class DexFileTry
{
public:
	static u4 dexGetHandlersSize(DexCode* pCode);


	/* Get the first handler offset for the given DexCode.
	* It's not 0 because the handlers list is prefixed with its size
	* (in entries) as a uleb128. */
	static u4 dexGetFirstHandlerOffset(DexCode* pCode);

	static u1* dexGetCatchHandlerData(DexCode* pCode);

	static DexTry* dexGetTries(const DexCode* pCode);

	/* Initialize a DexCatchIterator to a particular handler offset. */
	static void dexCatchIteratorInit(DexCatchIterator* pIterator, DexCode* pCode, u4 offset);


	/* Initialize a DexCatchIterator with a direct pointer to encoded handlers. */
	static void dexCatchIteratorInitToPointer(DexCatchIterator* pIterator,
	                                          u1* pEncodedData);


	/* Get the handler offset just past the end of the one just iterated over.
	* This ends the iteration if it wasn't already. */
	static  u4 dexCatchIteratorGetEndOffset(DexCatchIterator* pIterator,
		 DexCode* pCode);


	static DexCatchHandler* dexCatchIteratorNext(DexCatchIterator* pIterator);

	static void scanTryCatchBlocks(DexCode* dex_code, DexCodeMemory* dex_code_memory, DexFileHelper* dex_file_helper);
};
