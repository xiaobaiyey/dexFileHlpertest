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

typedef struct DexMapItem
{
	u2 type; /* type code (see kDexType* above) */
	u2 unused;
	u4 size; /* count of items of the indicated type */
	u4 offset; /* file offset to the start of data */
} DexMapItem;

typedef struct DexString
{
	u4 idx;
	u4 hash;
	u4 ulebLen;
	char* data;
} DexString;


typedef struct DexType
{
	u4 idx;
	DexString* dex_string;
} DexType;

typedef struct DexTypeList
{
	u4 offset;
	u4 size;
	std::vector<DexType*> typeItems;
} DexTypeList;

typedef struct DexProto
{
	u4 idx;
	DexString* shorty_string;
	DexType* returnType_type;
	DexTypeList* dex_type_list;
} DexProto;

typedef struct DexField
{
	u4 idx;
	DexType* class_type;
	DexType* type_type;
	DexString* name_string;
} DexField;


typedef struct DexMethod
{
	u4 idx;
	DexType* class_type;
	DexProto* type_type;
	DexString* name_string;
} DexMethod;


typedef struct DexClassDef
{
	u4 idx;
	DexType* class_type;
	u4 accessFlags;
	DexType* superclass_type;
	DexTypeList* interface_typelist;
	DexString* sourceFile_string;
	///todo
	u4 annotationsOff;
	u4 classDataOff;
	u4 staticValuesOff; /* file offset to DexEncodedArray */
} DexClassDef;

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
