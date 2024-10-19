#ifndef INCLUDED_SDK_TYPES_H
#define INCLUDED_SDK_TYPES_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif // INCLUDED_STDINT_H

#ifndef INCLUDED_STDBOOL_H
#define INCLUDED_STDBOOL_H
#include <stdbool.h>
#endif // INCLUDED_STDBOOL_H

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif // INCLUDED_STDDEF_H

#ifndef INCLUDED_CTYPE_H
#define INCLUDED_CTYPE_H
#include <ctype.h>
#endif // INCLUDED_CTYPE_H

#ifndef INCLUDED_INTTYPES_H
#define INCLUDED_INTTYPES_H
#include <inttypes.h>
#endif // INCLUDED_INTTYPES_H

#ifndef INCLUDED_SDK_DEFINITIONS_H
#include <sdk_definitions.h>
#endif // INCLUDED_SDK_DEFINITIONS_H

////////////////////////////////////////////////////////////////////////////////
////
#if (SDK_SIZEOF_VOID_P==1)
    typedef int8_t      sdk_int_t;
    typedef uint8_t     sdk_uint_t;
    typedef sdk_uint_t  sdk_size_t;
    #define SDK_PRId    PRId8
    #define SDK_PRIi    PRIi8
    #define SDK_PRIo    PRIo8
    #define SDK_PRIu    PRIu8
    #define SDK_PRIx    PRIx8
    #define SDK_PRIX    PRIX8
    #define SDK_SCNd    SCNd8
    #define SDK_SCNi    SCNi8
    #define SDK_SCNo    SCNo8
    #define SDK_SCNu    SCNu8
    #define SDK_SCNx    SCNx8
    #define SDK_INT_MIN INT8_MIN
    #define SDK_INT_MAX INT8_MAX
    #define SDK_UINT_MAX UINT8_MAX
#elif (SDK_SIZEOF_VOID_P==2)
    typedef int16_t     sdk_int_t;
    typedef uint16_t    sdk_uint_t;
    typedef sdk_uint_t  sdk_size_t;
    #define SDK_PRId    PRId16
    #define SDK_PRIi    PRIi16
    #define SDK_PRIo    PRIo16
    #define SDK_PRIu    PRIu16
    #define SDK_PRIx    PRIx16
    #define SDK_PRIX    PRIX16
    #define SDK_SCNd    SCNd16
    #define SDK_SCNi    SCNi16
    #define SDK_SCNo    SCNo16
    #define SDK_SCNu    SCNu16
    #define SDK_SCNx    SCNx16
    #define SDK_INT_MIN INT16_MIN
    #define SDK_INT_MAX INT16_MAX
    #define SDK_UINT_MAX UINT16_MAX
#elif (SDK_SIZEOF_VOID_P==4)
    typedef int32_t     sdk_int_t;
    typedef uint32_t    sdk_uint_t;
    typedef sdk_uint_t  sdk_size_t;
    #define SDK_PRId    PRId32
    #define SDK_PRIi    PRIi32
    #define SDK_PRIo    PRIo32
    #define SDK_PRIu    PRIu32
    #define SDK_PRIx    PRIx32
    #define SDK_PRIX    PRIX32
    #define SDK_SCNd    SCNd32
    #define SDK_SCNi    SCNi32
    #define SDK_SCNo    SCNo32
    #define SDK_SCNu    SCNu32
    #define SDK_SCNx    SCNx32
    #define SDK_INT_MIN INT32_MIN
    #define SDK_INT_MAX INT32_MAX
    #define SDK_UINT_MAX UINT32_MAX
#elif (SDK_SIZEOF_VOID_P==8)
    typedef int64_t     sdk_int_t;
    typedef uint64_t    sdk_uint_t;
    typedef sdk_uint_t  sdk_size_t;
    #define SDK_PRId    PRId64
    #define SDK_PRIi    PRIi64
    #define SDK_PRIo    PRIo64
    #define SDK_PRIu    PRIu64
    #define SDK_PRIx    PRIx64
    #define SDK_PRIX    PRIX64
    #define SDK_SCNd    SCNd64
    #define SDK_SCNi    SCNi64
    #define SDK_SCNo    SCNo64
    #define SDK_SCNu    SCNu64
    #define SDK_SCNx    SCNx64
    #define SDK_INT_MIN INT64_MIN
    #define SDK_INT_MAX INT64_MAX
    #define SDK_UINT_MAX UINT64_MAX
#endif

#define sdk_bool_t      bool
#define SDK_TRUE        true
#define SDK_FALSE       false

#endif /*INCLUDED_SDK_TYPES_H*/
