#include "DexFileUtils.h"
#include "pch.h"
#include "DexDefine.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>


bool DexFileUtils::isDex(void* inAddr)
{
	if ((0 == memcmp(inAddr, "dex", 3)) || (0 == memcmp(inAddr, "dey", 3)))
	{
		return true;
	}
	return false;
}

u1* DexFileUtils::getBase(void* inAddr)
{
	unsigned char* mAddr = (unsigned char*)inAddr;
	if (0 == memcmp(mAddr, "dey", 3))
	{
		return mAddr + 0x28;
	}
	else if (0 == memcmp(mAddr, "dex", 3))
	{
		return mAddr;
	}
	return nullptr;
}

unsigned int DexFileUtils::hash(const char* temp)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	const char* str = temp;
	while (*str)
	{
		hash = (hash << 4) + (*str++); //hash左移4位，把当前字符ASCII存入hash低四位。 
		if ((x = hash & 0xF0000000L) != 0)
		{
			//如果最高的四位不为0，则说明字符多余7个，现在正在存第7个字符，如果不处理，再加下一个字符时，第一个字符会被移出，因此要有如下处理。
			//该处理，如果最高位为0，就会仅仅影响5-8位，否则会影响5-31位，因为C语言使用的算数移位
			//因为1-4位刚刚存储了新加入到字符，所以不能>>28
			hash ^= (x >> 24);
			//上面这行代码并不会对X有影响，本身X和hash的高4位相同，下面这行代码&~即对28-31(高4位)位清零。
			hash &= ~x;
		}
	}
	//返回一个符号位为0的数，即丢弃最高位，以免函数外产生影响。(我们可以考虑，如果只有字符，符号位不可能为负)
	return (hash & 0x7FFFFFFF);
}

unsigned char* DexFileUtils::readFile(const char* inPath, size_t* inReadSize)
{
	void* result = NULL;
	size_t mSize = 0;
	int mfd;
	struct stat mstat;
	if ((stat(inPath, &mstat) == -1) || (mstat.st_size == 0) ||
		((mfd = open(inPath, O_RDONLY | O_BINARY)) == -1))
	{
		LOGE("readFile is Fail!");
		goto bail;
	}
	result = calloc(1, mstat.st_size);

	read(mfd, result, mstat.st_size);
	close(mfd);
	mSize = mstat.st_size;
bail:
	if (inReadSize != NULL)
	{
		*inReadSize = mSize;
	}
	return static_cast<u1*>(result);
}

int DexFileUtils::readSignedLeb128(u1** pStream)
{
	unsigned char* ptr = (unsigned char*)*pStream;
	int result = *(ptr++);
	if (result <= 0x7f)
	{
		result = (result << 25) >> 25;
	}
	else
	{
		int cur = *(ptr++);
		result = (result & 0x7f) | ((cur & 0x7f) << 7);
		if (cur <= 0x7f)
		{
			result = (result << 18) >> 18;
		}
		else
		{
			cur = *(ptr++);
			result |= (cur & 0x7f) << 14;
			if (cur <= 0x7f)
			{
				result = (result << 11) >> 11;
			}
			else
			{
				cur = *(ptr++);
				result |= (cur & 0x7f) << 21;
				if (cur <= 0x7f)
				{
					result = (result << 4) >> 4;
				}
				else
				{
					/*
					* Note: We don't check to see if cur is out of
					* range here, meaning we tolerate garbage in the
					* high four-order bits.
					*/
					cur = *(ptr++);
					result |= cur << 28;
				}
			}
		}
	}
	*pStream = ptr;
	return result;
}

u4 DexFileUtils::readAndVerifyUnsignedLeb128(const u1** pStream, const u1* limit, bool* okay)
{
	const u1* ptr = *pStream;
	int result = readUnsignedLeb128(pStream);

	if (((limit != NULL) && (*pStream > limit))
		|| (((*pStream - ptr) == 5) && (ptr[4] > 0x0f)))
	{
		*okay = false;
	}

	return result;
}

int DexFileUtils::readUnsignedLeb128(const u1** pStream)
{
	const u1* ptr = *pStream;
	int result = *(ptr++);

	if (result > 0x7f)
	{
		int cur = *(ptr++);
		result = (result & 0x7f) | ((cur & 0x7f) << 7);
		if (cur > 0x7f)
		{
			cur = *(ptr++);
			result |= (cur & 0x7f) << 14;
			if (cur > 0x7f)
			{
				cur = *(ptr++);
				result |= (cur & 0x7f) << 21;
				if (cur > 0x7f)
				{
					/*
					* Note: We don't check to see if cur is out of
					* range here, meaning we tolerate garbage in the
					* high four-order bits.
					*/
					cur = *(ptr++);
					result |= cur << 28;
				}
			}
		}
	}

	*pStream = ptr;
	return result;
}

u4 DexFileUtils::readUnsignedLittleEndian(const u1** pData, u4 size)
{
	const u1* data = *pData;
	u4 result = 0;
	u4 i;
	for (i = 0; i < size; i++)
	{
		result |= ((u4)*(data++)) << (i * 8);
	}

	*pData = data;
	return result;
}

u4 DexFileUtils::align_4(size_t size)
{
	size_t mSzie = (size + 3) & (~3);
	return mSzie;
}
