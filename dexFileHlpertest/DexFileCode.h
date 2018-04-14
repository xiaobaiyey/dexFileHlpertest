#pragma once
#include "DexDefine.h"
#include <map>
class DexFileHelper;

class DexFileCode
{
public:
	DexFileCode(DexFileHelper* dex_file_helper);

	~DexFileCode();
	void addOpCode(u1* data, u4 len);
	static u4 getDexCodeSize(u1* data);
private:
	DexFileHelper* dex_file_helper_;
	u1* base;
	std::map<u4*, DexCodeMemory*> dex_code_memory_maps;
	bool checkPadding(DexCode* dex_code);


	//try
	void parseTries(u1* dex_code);

	DexTry* dexGetTries(DexCode* pCode);

	void dexCatchIteratorInitToPointer(DexCatchIterator* pIterator, u1* pEncodedData);

	u1* dexGetCatchHandlerData(DexCode* pCode);

	void dexCatchIteratorInit(DexCatchIterator* pIterator,
	                          DexCode* pCode, u4 offset);

	DexCatchHandler* dexCatchIteratorNext(DexCatchIterator* pIterator);


};
