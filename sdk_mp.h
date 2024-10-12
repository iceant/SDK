#ifndef INCLUDED_SDK_MP_H
#define INCLUDED_SDK_MP_H

#ifndef INCLUDED_SDK_FMT_H
#include <sdk_fmt.h>
#endif /*INCLUDED_SDK_FMT_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef unsigned char *sdk_mp_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int sdk_mp_set(int n);
sdk_mp_t sdk_mp_new(unsigned long u);
sdk_mp_t sdk_mp_fromint (sdk_mp_t z, long v);
sdk_mp_t sdk_mp_fromintu(sdk_mp_t z, unsigned long u);
unsigned long sdk_mp_tointu(sdk_mp_t x);
         long sdk_mp_toint (sdk_mp_t x);
sdk_mp_t sdk_mp_cvt (int m, sdk_mp_t z, sdk_mp_t x);
sdk_mp_t sdk_mp_cvtu(int m, sdk_mp_t z, sdk_mp_t x);
sdk_mp_t sdk_mp_add (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_sub (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_mul (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_div (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_mod (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_neg (sdk_mp_t z, sdk_mp_t x);
sdk_mp_t sdk_mp_addu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_subu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_mulu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_divu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_modu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_mul2u(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_mul2 (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_addi (sdk_mp_t z, sdk_mp_t x, long y);
sdk_mp_t sdk_mp_subi (sdk_mp_t z, sdk_mp_t x, long y);
sdk_mp_t sdk_mp_muli (sdk_mp_t z, sdk_mp_t x, long y);
sdk_mp_t sdk_mp_divi (sdk_mp_t z, sdk_mp_t x, long y);
sdk_mp_t sdk_mp_addui(sdk_mp_t z, sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_subui(sdk_mp_t z, sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_mului(sdk_mp_t z, sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_divui(sdk_mp_t z, sdk_mp_t x, unsigned long y);
         long sdk_mp_modi (sdk_mp_t x,          long y);
unsigned long sdk_mp_modui(sdk_mp_t x, unsigned long y);
int sdk_mp_cmp  (sdk_mp_t x, sdk_mp_t y);
int sdk_mp_cmpi (sdk_mp_t x, long y);
int sdk_mp_cmpu (sdk_mp_t x, sdk_mp_t y);
int sdk_mp_cmpui(sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_and (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_or  (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_xor (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y);
sdk_mp_t sdk_mp_not (sdk_mp_t z, sdk_mp_t x);
sdk_mp_t sdk_mp_andi(sdk_mp_t z, sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_ori (sdk_mp_t z, sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_xori(sdk_mp_t z, sdk_mp_t x, unsigned long y);
sdk_mp_t sdk_mp_lshift(sdk_mp_t z, sdk_mp_t x, int s);
sdk_mp_t sdk_mp_rshift(sdk_mp_t z, sdk_mp_t x, int s);
sdk_mp_t sdk_mp_ashift(sdk_mp_t z, sdk_mp_t x, int s);
sdk_mp_t     sdk_mp_fromstr(sdk_mp_t z, const char *str,
                        int base, char **end);
char *sdk_mp_tostr  (char *str, int size,
                        int base, sdk_mp_t x);
void  sdk_mp_fmt    (int code, va_list_box *box,
                        int put(int c, void *cl), void *cl,
                        unsigned char flags[], int width, int precision);
void  sdk_mp_fmtu   (int code, va_list_box *box,
                        int put(int c, void *cl), void *cl,
                        unsigned char flags[], int width, int precision);

#endif /* INCLUDED_SDK_MP_H */
