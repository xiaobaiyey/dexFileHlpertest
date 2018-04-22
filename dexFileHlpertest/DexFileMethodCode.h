#pragma once
#include "DexDefine.h"
#include <map>
class DexFileHelper;

class DexFileMethodCode
{
public:
	DexFileMethodCode(DexFileHelper* dex_file_helper);

	~DexFileMethodCode();
	void addOpCode(u1* data, u4 len);
	static size_t GetDexCodeSize( DexCode* pCode);
	static size_t GetDexCodeSize(const u1* pCode);
	DexCodeMemory* getDexCodeByOffset(u4 offset);
private:
	DexFileHelper* dex_file_helper_;
	u1* base;
	std::map<u4, DexCodeMemory*> dex_code_memory_maps;
	bool checkPadding(DexCode* dex_code);
};
