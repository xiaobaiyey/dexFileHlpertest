#pragma once
class DexFileHelper;
class DexFileClass
{
public:
	DexFileClass(DexFileHelper* dex_file_helper);
	~DexFileClass();
private:
	DexFileHelper * dex_file_helper_;
};

