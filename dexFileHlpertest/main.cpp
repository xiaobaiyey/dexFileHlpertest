#include <stdlib.h>
#include <stdio.h>
#include "DexFileHelper.h"


int main(int argc, char* argv[])
{
	system("chcp 65001");
	DexFileHelper* dex_file_helper = DexFileHelper::getDexFileHelper(
		"C:\\Users\\xiaobai\\Desktop\\classes_Mod.dex");
	DexFileHeader* dex_file_header = dex_file_helper->dex_file_header;
	dex_file_header->getBaseAddress();
	getchar();
	return 0;
}
