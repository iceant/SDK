#ifndef INCLUDED_SDK_XP_H
#define INCLUDED_SDK_XP_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef unsigned char *sdk_xp_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int sdk_xp_add(int n, sdk_xp_t z, sdk_xp_t x, sdk_xp_t y, int carry);
int sdk_xp_sub(int n, sdk_xp_t z, sdk_xp_t x, sdk_xp_t y, int borrow);
int sdk_xp_mul(sdk_xp_t z, int n, sdk_xp_t x, int m, sdk_xp_t y);
int sdk_xp_div(int n, sdk_xp_t q, sdk_xp_t x, int m, sdk_xp_t y, sdk_xp_t r,sdk_xp_t tmp);
int sdk_xp_sum     (int n, sdk_xp_t z, sdk_xp_t x, int y);
int sdk_xp_diff    (int n, sdk_xp_t z, sdk_xp_t x, int y);
int sdk_xp_product (int n, sdk_xp_t z, sdk_xp_t x, int y);
int sdk_xp_quotient(int n, sdk_xp_t z, sdk_xp_t x, int y);
int sdk_xp_neg(int n, sdk_xp_t z, sdk_xp_t x, int carry);
int sdk_xp_cmp(int n, sdk_xp_t x, sdk_xp_t y);
void sdk_xp_lshift(int n, sdk_xp_t z, int m, sdk_xp_t x, int s, int fill);
void sdk_xp_rshift(int n, sdk_xp_t z, int m, sdk_xp_t x, int s, int fill);
int sdk_xp_length (int n, sdk_xp_t x);
unsigned long sdk_xp_fromint(int n, sdk_xp_t z, unsigned long u);
unsigned long sdk_xp_toint  (int n, sdk_xp_t x);
int   sdk_xp_fromstr(int n, sdk_xp_t z, const char *str, int base, char **end);
char *sdk_xp_tostr  (char *str, int size, int base, int n, sdk_xp_t x);

#endif /* INCLUDED_SDK_XP_H */
