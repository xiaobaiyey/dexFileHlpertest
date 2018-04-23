#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileProtoTypeIdx
{
public:
	DexFileProtoTypeIdx(DexFileHelper* dex_file_helper);
	~DexFileProtoTypeIdx();
	DexProtoIdx* getDexProtoByidx(u4 idx);
	u4 getProtoSize();
private:
	DexFileHelper* dex_file_helper_;
	/*idx,DexProtoIdx*/
	std::map<u4, DexProtoIdx*> dex_proto_maps;
};
