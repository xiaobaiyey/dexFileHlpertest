#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileProtoTypeIdx
{
public:
	DexFileProtoTypeIdx(DexFileHelper* dex_file_helper);
	~DexFileProtoTypeIdx();
	DexProto* getDexProtoByidx(u4 idx);
private:
	DexFileHelper* dex_file_helper_;
	/*idx,DexProto*/
	std::map<u4, DexProto*> dex_proto_maps;
};
