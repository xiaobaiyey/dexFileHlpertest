#pragma once
#include <vector>
#ifdef HAVE_STDINT_H
# include <stdint.h>    /* C99 */
typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;
typedef unsigned char       U1;
typedef unsigned short      U2;
typedef unsigned int        U4;
typedef unsigned long long  U8;
typedef signed char         S1;
typedef signed short        S2;
typedef signed int          S4;
typedef signed long long    S8;
typedef unsigned char       Char;
typedef unsigned short      Short;
typedef unsigned int        Int;
typedef unsigned long long  Long;
typedef unsigned char       UChar;
typedef unsigned short      UShort;
typedef unsigned int        UInt;
typedef unsigned long long  ULong;
typedef signed char         SChar;
typedef signed short        SUShort;
typedef signed int          SInt;
typedef signed long long    SLong;
#else
typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;
typedef unsigned long long u8;
typedef signed char s1;
typedef signed short s2;
typedef signed int s4;
typedef signed long long s8;
typedef unsigned char U1;
typedef unsigned short U2;
typedef unsigned int U4;
typedef unsigned long long U8;
typedef signed char S1;
typedef signed short S2;
typedef signed int S4;
typedef signed long long S8;
#endif

union JValue
{
	u1 z;
	s1 b;
	u2 c;
	s2 s;
	s4 i;
	s8 j;
	float f;
	double d;
	void* l;
};

typedef struct DexHeader
{
	u1 magic[8]; /* includes version number */
	u4 checksum; /* adler32 checksum */
	u1 signature[20]; /* SHA-1 hash */
	u4 fileSize; /* length of entire file */
	u4 headerSize; /* offset to start of next section */
	u4 endianTag;
	u4 linkSize;
	u4 linkOff;
	u4 mapOff;
	u4 stringIdsSize;
	u4 stringIdsOff;
	u4 typeIdsSize;
	u4 typeIdsOff;
	u4 protoIdsSize;
	u4 protoIdsOff;
	u4 fieldIdsSize;
	u4 fieldIdsOff;
	u4 methodIdsSize;
	u4 methodIdsOff;
	u4 classDefsSize;
	u4 classDefsOff;
	u4 dataSize;
	u4 dataOff;
} DexHeader;

/* map item type codes */
enum
{
	kDexTypeHeaderItem = 0x0000,
	kDexTypeStringIdItem = 0x0001,
	kDexTypeTypeIdItem = 0x0002,
	kDexTypeProtoIdItem = 0x0003,
	kDexTypeFieldIdItem = 0x0004,
	kDexTypeMethodIdItem = 0x0005,
	kDexTypeClassDefItem = 0x0006,
	kDexTypeMapList = 0x1000,
	kDexTypeTypeList = 0x1001,
	kDexTypeAnnotationSetRefList = 0x1002,
	kDexTypeAnnotationSetItem = 0x1003,
	kDexTypeClassDataItem = 0x2000,
	kDexTypeCodeItem = 0x2001,
	kDexTypeStringDataItem = 0x2002,
	kDexTypeDebugInfoItem = 0x2003,
	kDexTypeAnnotationItem = 0x2004,
	kDexTypeEncodedArrayItem = 0x2005,
	kDexTypeAnnotationsDirectoryItem = 0x2006,
};

/* annotation constants */
enum
{
	kDexVisibilityBuild = 0x00,
	/* annotation visibility */
	kDexVisibilityRuntime = 0x01,
	kDexVisibilitySystem = 0x02,

	kDexAnnotationByte = 0x00,
	kDexAnnotationShort = 0x02,
	kDexAnnotationChar = 0x03,
	kDexAnnotationInt = 0x04,
	kDexAnnotationLong = 0x06,
	kDexAnnotationFloat = 0x10,
	kDexAnnotationDouble = 0x11,
	kDexAnnotationString = 0x17,
	kDexAnnotationType = 0x18,
	kDexAnnotationField = 0x19,
	kDexAnnotationMethod = 0x1a,
	kDexAnnotationEnum = 0x1b,
	kDexAnnotationArray = 0x1c,
	kDexAnnotationAnnotation = 0x1d,
	kDexAnnotationNull = 0x1e,
	kDexAnnotationBoolean = 0x1f,

	kDexAnnotationValueTypeMask = 0x1f,
	/* low 5 bits */
	kDexAnnotationValueArgShift = 5,
};

/* general constants */
enum
{
	kDexEndianConstant = 0x12345678,
	/* the endianness indicator */
	kDexNoIndex = 0xffffffff,
	/* not a valid index value */
};

typedef struct DexMapItem
{
	u2 type; /* type code (see kDexType* above) */
	u2 unused;
	u4 size; /* count of items of the indicated type */
	u4 offset; /* file offset to the start of data */
} DexMapItem;

typedef struct DexStringIdx
{
	u4 idx;
	u4 hash;
	u4 ulebLen;
	char* data;
} DexStringIdx;


typedef struct DexTypeIdx
{
	u4 idx;
	DexStringIdx* dex_string;
} DexTypeIdx;

typedef struct DexTypeList
{
	u4 offset;
	u4 size;
	std::vector<DexTypeIdx*> typeItems;
} DexTypeList;

typedef struct DexProtoIdx
{
	u4 idx;
	DexStringIdx* shorty_string;
	DexTypeIdx* returnType_type;
	DexTypeList* dex_type_list;
} DexProtoIdx;

typedef struct DexFieldIdx
{
	u4 idx;
	DexTypeIdx* class_type;
	DexTypeIdx* type_type;
	DexStringIdx* name_string;
} DexFieldIdx;


typedef struct DexMethodIdx
{
	u4 idx;
	DexTypeIdx* class_type;
	DexProtoIdx* type_type;
	DexStringIdx* name_string;
} DexMethodIdx;


typedef struct DexAnnotationItem
{
	u4 offset;
	u1 visibility;
	u4 data_len;
	u1* annotation;
} DexAnnotationItem;

typedef struct DexAnnotationSetItem
{
	u4 offset;
	u4 size;
	std::vector<DexAnnotationItem*> annotation_items_vector; /* offset to DexAnnotationItem */
} DexAnnotationSetItem;


/*
* Direct-mapped "field_annotations_item".
*/
typedef struct DexFieldAnnotationsItem
{
	u4 fieldIdx;
	u4 annotationsOff; /* offset to DexAnnotationSetItem */
	DexAnnotationSetItem* dex_annotation_set_item;
} DexFieldAnnotationsItem;


/*
* Direct-mapped "method_annotations_item".
*/
typedef struct DexMethodAnnotationsItem
{
	u4 methodIdx;
	u4 annotationsOff; /* offset to DexAnnotationSetItem */
	DexAnnotationSetItem* dex_annotation_set_item;
} DexMethodAnnotationsItem;


/*
* Direct-mapped "annotation_set_ref_list".
*/
struct DexAnnotationSetRefList
{
	u4 size;
	u4 offset;
	//DexAnnotationSetRefItem list[1];
	std::vector<DexAnnotationSetItem*> dex_annotation_set_items;
};


/*
* Direct-mapped "parameter_annotations_item".
*/
typedef struct DexParameterAnnotationsItem
{
	u4 methodIdx;
	u4 annotationsOff; /* offset to DexAnotationSetRefList */
	DexAnnotationSetRefList* dex_annotation_set_ref_list;
} DexParameterAnnotationsItem;


/*
* Direct-mapped "annotations_directory_item".
*/
typedef struct DexAnnotationsDirectoryItem
{
	u4 offset;
	DexAnnotationSetItem* classAnnotationsOff; /* offset to DexAnnotationSetItem */
	u4 fieldsSize; /* count of DexFieldAnnotationsItem */
	u4 methodsSize; /* count of DexMethodAnnotationsItem */
	u4 parametersSize; /* count of DexParameterAnnotationsItem */
	/* followed by DexFieldAnnotationsItem[fieldsSize] */
	std::vector<DexFieldAnnotationsItem*> field_annotations_items;
	/* followed by DexMethodAnnotationsItem[methodsSize] */
	std::vector<DexMethodAnnotationsItem*> method_annotations_items;
	/* followed by DexParameterAnnotationsItem[parametersSize] */
	std::vector<DexParameterAnnotationsItem*> parameter_annotations_items;
} DexAnnotationsDirectoryItem;


typedef struct AnnotationValue
{
	JValue value;
	u1 type;
} AnnotationValue;


typedef struct DexEncodedArray
{
	u4 offset;
	u4 len;
	u1* data;
} DexEncodedArray;


typedef struct DexClassDef
{
	u4 classIdx; /* index into typeIds for this class */
	u4 accessFlags;
	u4 superclassIdx; /* index into typeIds for superclass */
	u4 interfacesOff; /* file offset to DexTypeList */
	u4 sourceFileIdx; /* index into stringIds for source file name */
	u4 annotationsOff; /* file offset to annotations_directory_item */
	u4 classDataOff; /* file offset to class_data_item */
	u4 staticValuesOff; /* file offset to DexEncodedArray */
} DexClassDef;


typedef struct DexCode
{
	u2 registersSize;
	u2 insSize;
	u2 outsSize;
	u2 triesSize;
	u4 debugInfoOff; /* file offset to debug info stream */
	u4 insnsSize; /* size of the insns array, in u2 units */
	u2 insns[1];
	/* followed by optional u2 padding */
	/* followed by try_item[triesSize] */
	/* followed by uleb128 handlersSize */
	/* followed by catch_handler_item[handlersSize] */
} DexCode;


typedef struct DexCatchHandlerMemory
{
	DexTypeIdx* typeIdx; /* type index of the caught exception type */
	u4 offset; /* handler address */
} DexCatchHandlerMemory;

typedef struct DexTryItem
{
	s4 count;
	bool catchesAll;
	std::vector<DexCatchHandlerMemory*> dex_catch_handler_memories;
} DexTryItem;


;


typedef struct DexTry
{
	u4 startAddr; /* start address, in 16-bit code units */
	u2 insnCount; /* instruction count, in 16-bit code units */
	u2 handlerOff; /* offset in encoded handler data to handlers */
} DexTry;


typedef struct DexTryMemory
{
	std::vector<DexTry*> dex_tries;
	std::vector<DexTryItem*> dex_try_items;
} DexTryMemory;


typedef struct DexCodeMemory
{
	u4 offset;
	u2 registersSize;
	u2 insSize;
	u2 outsSize;
	u2 triesSize;
	u4 debugInfoOff; /* file offset to debug info stream */
	u4 opcodelen; /* size of the insns array, in u2 units */
	u2 insns[1];
	u4 dexcodelen;
	bool padding;
	DexTryMemory* dex_try_memories;
} DexCodeMemory;

/*
* Catch handler entry, used while iterating over catch_handler_items.
*/
typedef struct DexCatchHandler
{
	u4 typeIdx; /* type index of the caught exception type */
	u4 address; /* handler address */
} DexCatchHandler;


/*
* Iterator over catch handler data. This structure should be treated as
* opaque.
*/
typedef struct DexCatchIterator
{
	const u1* pEncodedData;
	bool catchesAll;
	s4 countRemaining;
	DexCatchHandler handler;
} DexCatchIterator;


/* expanded form of encoded_field */
typedef struct DexField
{
	DexFieldIdx* fieldIdx; /* index to a field_id_item */
	u4 accessFlags;
} DexField;

/* expanded form of a class_data_item header */
typedef struct DexClassDataHeader
{
	u4 staticFieldsSize;
	u4 instanceFieldsSize;
	u4 directMethodsSize;
	u4 virtualMethodsSize;
} DexClassDataHeader;

/* expanded form of encoded_method */
typedef struct DexMethod
{
	struct DexClass* dex_class;
	DexMethodIdx* methodIdx; /* index to a method_id_item */
	u4 accessFlags;
	DexCodeMemory* codeOff; /* file offset to a code_item */
} DexMethod;

/* expanded form of class_data_item. Note: If a particular item is
* absent (e.g., no static fields), then the corresponding pointer
* is set to NULL. */
typedef struct DexClassData
{
	DexClassDataHeader* header;
	std::vector<DexField*> staticFields;
	std::vector<DexField*> instanceFields;
	std::vector<DexMethod*> directMethods;
	std::vector<DexMethod*> virtualMethods;
} DexClassData;

typedef struct DexClass
{
	u4 idx;
	DexTypeIdx* classType;
	u4 accessFlags;
	DexTypeIdx* superclassType;
	DexTypeList* interfacesList;
	DexStringIdx* sourceFileString;
	DexAnnotationsDirectoryItem* dex_annotations_directory_item;
	DexClassData* dex_class_data;
	DexEncodedArray* dex_encoded_array;
} DexClass;
