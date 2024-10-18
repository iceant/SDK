#ifndef INCLUDED_SDK_H
#define INCLUDED_SDK_H

#ifndef INCLUDED_SDK_DEFINITIONS_H
#include <sdk_definitions.h>
#endif // INCLUDED_SDK_DEFINITIONS_H

#ifndef INCLUDED_SDK_COMPILER_H
#include <sdk_compiler.h>
#endif // INCLUDED_SDK_COMPILER_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif // INCLUDED_SDK_TYPES_H

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif // INCLUDED_SDK_ERRORS_H

#ifndef INCLUDED_SDK_MACROS_H
#include <sdk_macros.h>
#endif // INCLUDED_SDK_MACROS_H

#ifndef INCLUDED_SDK_MEMORY_H
#include <sdk_memory.h>
#endif // INCLUDED_SDK_MEMORY_H

#ifndef INCLUDED_SDK_LIST_H
#include <sdk_list.h>
#endif // INCLUDED_SDK_LIST_H

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif // INCLUDED_SDK_VECTOR_H

#ifndef INCLUDED_SDK_FMT_H
#include <sdk_fmt.h>
#endif /*INCLUDED_SDK_FMT_H*/

#ifndef INCLUDED_SDK_STR_H
#include <sdk_str.h>
#endif /*INCLUDED_SDK_STR_H*/

#ifndef INCLUDED_SDK_XP_H
#include <sdk_xp.h>
#endif /*INCLUDED_SDK_XP_H*/

#ifndef INCLUDED_SDK_MP_H
#include <sdk_mp.h>
#endif /*INCLUDED_SDK_MP_H*/

#ifndef INCLUDED_SDK_BITSET_H
#include <sdk_bitset.h>
#endif /*INCLUDED_SDK_BITSET_H*/

#ifndef INCLUDED_SDK_PRINTF_H
#include <sdk_printf.h>
#endif /*INCLUDED_SDK_PRINTF_H*/

#ifndef INCLUDED_SDK_NET_H
#include <sdk_net.h>
#endif /*INCLUDED_SDK_NET_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_MD5_H
#include <md5.h>
#endif /*INCLUDED_MD5_H*/

#ifndef INCLUDED_BASE64_H
#include <base64.h>
#endif /*INCLUDED_BASE64_H*/

#ifndef INCLUDED_SHA1_H
#include <sha1.h>
#endif /*INCLUDED_SHA1_H*/

#ifndef INCLUDED_SHA256_H
#include <sha256.h>
#endif /*INCLUDED_SHA256_H*/

#ifndef INCLUDED_AES_H
#include <aes.h>
#endif /*INCLUDED_AES_H*/

#ifndef INCLUDED_BLOWFISH_H
#include <blowfish.h>
#endif /*INCLUDED_BLOWFISH_H*/

#ifndef INCLUDED_MODBUS_CRC16_H
#include <modbus_crc16.h>
#endif /*INCLUDED_MODBUS_CRC16_H*/



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void sdk_init(void);

void sdk_destroy(void);

#endif /*INCLUDED_SDK_H*/
