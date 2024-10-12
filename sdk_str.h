#ifndef INCLUDED_SDK_STR_H
#define INCLUDED_SDK_STR_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif /*INCLUDED_STDARG_H*/

#ifndef INCLUDED_SDK_FMT_H
#include <sdk_fmt.h>
#endif /*INCLUDED_SDK_FMT_H*/

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif /*INCLUDED_STRING_H*/



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

char *sdk_str_sub(const char *s, int i, int j);
char *sdk_str_dup(const char *s, int i, int j, int n);
char *sdk_str_cat(const char *s1, int i1, int j1,
                     const char *s2, int i2, int j2);
char *sdk_str_catv   (const char *s, ...);
char *sdk_str_reverse(const char *s, int i, int j);
char *sdk_str_map    (const char *s, int i, int j,
                         const char *from, const char *to);

int sdk_str_pos(const char *s, int i);
int sdk_str_len(const char *s, int i, int j);
int sdk_str_cmp(const char *s1, int i1, int j1,
                   const char *s2, int i2, int j2);
int sdk_str_chr  (const char *s, int i, int j, int c);
int sdk_str_rchr (const char *s, int i, int j, int c);

/* find any char in s list in set */
int sdk_str_upto (const char *s, int i, int j,
                     const char *set);

/* find any char in s list in set */
int sdk_str_rupto(const char *s, int i, int j,
                     const char *set);

int sdk_str_find (const char *s, int i, int j,
                     const char *str);
int sdk_str_rfind(const char *s, int i, int j,
                     const char *str);
int sdk_str_any   (const char *s, int i,
                      const char *set);
int sdk_str_many  (const char *s, int i, int j,
                      const char *set);
int sdk_str_rmany (const char *s, int i, int j,
                      const char *set);

int sdk_str_match (const char *s, int i, int j,
                      const char *str);

int sdk_str_rmatch(const char *s, int i, int j,
                      const char *str);

void sdk_str_fmt(int code, va_list_box *box,
                    int put(int c, void *cl), void *cl,
                    unsigned char flags[], int width, int precision);

void sdk_str_free(char* s);

#endif /* INCLUDED_SDK_STR_H */
