#pragma once
#include <map>
#include "DexDefine.h"
class DexFileHelper;
class DexFileProtoType
{
public:
	DexFileProtoType(DexFileHelper* dex_file_helper);
	~DexFileProtoType();
	DexProto* getDexProtoByidx(u4 idx);
private:
	DexFileHelper* dex_file_helper_;
	/*idx,DexProto*/
	std::map<u4, DexProto*> dex_proto_maps;
};
