#include "DexFileVertor.h"
#include "pch.h"

template <typename Type_>
void DexFileVertor<Type_>::Free(ItemDex_Vertor* inItem)
{
	if (inItem == nullptr)
		return;
	ItemDex_Vertor* mNext = inItem->next;
	free(inItem);
	Free(mNext);
}

template <typename Type_>
size_t DexFileVertor<Type_>::GetNum()
{
	return mNum;
}

template <typename Type_>
void DexFileVertor<Type_>::Clear()
{
	First = nullptr;
	Last = nullptr;
	pType = nullptr;
	VertorSize = 0;
	mNum = 0;
}

template <typename Type_>
void DexFileVertor<Type_>::Reset()
{
	pType = First;
	mNum = 0;
}

template <typename Type_>
size_t DexFileVertor<Type_>::GetSize()
{
	return VertorSize;
}

template <typename Type_>
Type_* DexFileVertor<Type_>::Get()
{
	if (pType == nullptr)
		return nullptr;
	return (Type_*)pType->data;
}

template <typename Type_>
Type_* DexFileVertor<Type_>::Get(int inNo)
{
	ItemDex_Vertor* mType = First;
	for (int m_i = 0; m_i < inNo; m_i++)
	{
		if (mType == nullptr)
			return nullptr;
		mType = mType->next;
	}
	return (Type_*)mType->data;
}

template<typename Type_>
void DexFileVertor<Type_>::Set(void * inData)
{
	if (pType == nullptr)
		return;
	pType->data = inData;
}

template<typename Type_>
void DexFileVertor<Type_>::Next()
{
	if (pType == nullptr)
		return;
	mNum++;
	pType = pType->next;
}

template<typename Type_>
void DexFileVertor<Type_>::Add(Type_ * inType)
{
	if (First == nullptr)
	{
		First = new ItemDex_Vertor(inType);
		Last = First;
		VertorSize = 1;
		return;
	}
	ItemDex_Vertor* mLink = new ItemDex_Vertor(inType);
	Last->next = mLink;
	Last = mLink;
	VertorSize++;
}

template <typename Type_>
void DexFileVertor<Type_>::AddNorepeat(Type_* inType)
{
	if (First == nullptr)
	{
		Add(inType);
	}
	else {
		ItemDex_Vertor* mLink = First;
		while (mLink != nullptr)
		{
			if (mLink->data == inType)
				return;
			mLink = mLink->next;
		}
		Add(inType);
	}
}
