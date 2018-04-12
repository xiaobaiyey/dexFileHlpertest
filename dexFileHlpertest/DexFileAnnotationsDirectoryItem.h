#pragma once
#include <map>
#include "DexDefine.h"

class DexFileHelper;

class DexFileAnnotationsDirectoryItem
{
public:
	DexFileAnnotationsDirectoryItem(DexFileHelper* dex_file_helper);
	~DexFileAnnotationsDirectoryItem();
	DexAnnotationsDirectoryItem* getDexAnnotationsDirectoryItemByOffset(u4 offset);
private:
	DexFileHelper* dex_file_helper_;
	std::map<u4, DexAnnotationsDirectoryItem*> dex_annotations_directory_item_maps;
};
