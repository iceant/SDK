#ifndef INCLUDED_SDK_FMT_H
#define INCLUDED_SDK_FMT_H

/* -------------------------------------------------------------------------------------------------------------- */
/* DEPENDENCIES */

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif /*INCLUDED_STDARG_H*/

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif /*INCLUDED_STDIO_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/* TYPES  */
typedef struct va_list_box{
    va_list ap;
}va_list_box;

typedef void (*sdk_fmt_t)(int code, va_list_box* box,
        int put(int c, void* cl), void* cl,
        unsigned char flags[256], int width, int precision);

/* -------------------------------------------------------------------------------------------------------------- */
/* METHODS */

void sdk_fmt_fmt(int put(int c, void* cl), void* cl, const char* fmt, ...);

void sdk_fmt_vfmt(int put(int c, void* cl), void* cl, const char* fmt, va_list_box * box);

void sdk_fmt_print(const char* fmt, ...);

void sdk_fmt_fprint(FILE* stream, const char* fmt, ...);

int sdk_fmt_sfmt(char* buf, int size, const char* fmt, ...);

int sdk_fmt_vsfmt(char* buf, int size, const char* fmt, va_list_box * box);

sdk_fmt_t sdk_fmt_register(int code, sdk_fmt_t cvt);

void sdk_fmt_putd(const char* str, int len,
                  int put(int, void*), void*,
                  unsigned char flags[256], int width, int precision);

void sdk_fmt_puts(const char* str, int len,
                  int put(int, void*), void*,
                  unsigned char flags[256], int width, int precision);
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

/* NEED FREE */
char* sdk_fmt_string(const char* fmt, ...);

char* sdk_fmt_vstring(const char* fmt, va_list_box* box);

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void sdk_fmt_free(char* str);

#endif /* INCLUDED_SDK_FMT_H */
