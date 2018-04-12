#pragma once
//建立一个单向列表，供容器建立
typedef struct ItemDex_Vertor
{
	struct ItemDex_Vertor* next;
	void*  data;
	ItemDex_Vertor(void* inData)
	{
		next = nullptr;
		data = inData;
	}
}ItemDex_Vertor;

template <typename Type_>
class DexFileVertor
{
public:
	ItemDex_Vertor * First = nullptr;
	ItemDex_Vertor* Last = nullptr;
	ItemDex_Vertor* pType = nullptr;
	size_t VertorSize;
	size_t mNum;
	void Free(ItemDex_Vertor* inItem);
	size_t GetNum();
	void Clear();
	void Reset();
	size_t GetSize();
	Type_* Get();
	void Del(int inNo)
	{

	}
	Type_* Get(int inNo);
	void Set(void* inData);
	void Next();
	void Add(Type_* inType);
	void AddNorepeat(Type_* inType);
	void operator ++()
	{
		Next();
	}
	bool operator != (void* inArg)
	{
		if (pType != inArg)
			return true;
		return false;
	}
	bool operator == (void* inArg)
	{
		if (pType == inArg)
			return true;
		return false;
	}

	void operator =(size_t inNum)
	{
		Reset();
		while (inNum--)
		{
			Next();
		}
	}
	void operator ()(size_t inNum)
	{
		Reset();
		while (inNum--)
		{
			Next();
		}
	}
	Type_* operator ->()
	{
		if (pType == nullptr)
			return nullptr;
		return (Type_*)pType->data;
	}
};
