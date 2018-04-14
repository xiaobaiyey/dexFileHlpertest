#pragma once
class DexFileHelper;
class DexFileTry
{
public:
	DexFileTry(DexFileHelper* dex_file_helper);
	~DexFileTry();
private:
	DexFileHelper * dex_file_helper_;

};
