#pragma once
#include "DexDefine.h"
class DexFileHelper;
class DexFileHeader
{
public:
	DexFileHeader(DexFileHelper* dex_file_hepter, void* memPtr);
	bool check();
	//for header
	u4 getFileSize();
	u4 getLinkSize();
	u4 getLinkOff();
	u4 getMapOff();
	u4 getStringIdsSize();
	u4 getStringIdsOff();
	u4 getTypeIdsSize();
	u4 getTypeIdsOff();
	u4 getProtoIdsSize();
	u4 getProtoIdsOff();
	u4 getFieldIdsSize();
	u4 getFieldIdsOff();
	u4 getMethodIdsSize();
	u4 getMethodIdsOff();
	u4 getClassDefsSize();
	u4 getClassDefsOff();
	u4 getDataSize();
	u4 getDataOff();
	u1* getBaseAddress();
	~DexFileHeader();
private:
	DexFileHelper * dex_file_hepter_;
	bool isDex;
	u1* dataBasePtr;
	DexHeader* dex_header_;
protected:
};
