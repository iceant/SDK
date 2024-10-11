#ifndef INCLUDED_SDK_PRINTF_H
#define INCLUDED_SDK_PRINTF_H

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif /*INCLUDED_STDARG_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int sdk_printf(const char* fmt, ...);

#endif /* INCLUDED_SDK_PRINTF_H */
