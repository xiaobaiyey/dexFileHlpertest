#pragma once
#include <map>
#include <vector>
#include "DexDefine.h"


class DexFileHelper;
class DexFileTypeList
{
public:
	DexFileTypeList(DexFileHelper* dex_file_helper);
	~DexFileTypeList();
	DexTypeList* getDexFileTypeListByOffset(u4 offset);
private:
	DexFileHelper* dex_file_helper_;
	/*offset typelist*/
	std::map<u4, DexTypeList*> dexfile_type_list;
};
