#include <sdk_mp.h>
#include <sdk_memory.h>
#include <assert.h>
#include <limits.h>
#include <sdk_xp.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */
#define BASE (1<<8)
#define sign(x) ((x)[nbytes-1]>>shift)
#define ones(n) (~(~0UL<<(((n)-1)%8+1)))
#define iszero(x) (sdk_xp_length(nbytes,(x))==1 && (x)[0]==0)
#define bitop(op) \
	int i; assert(z); assert(x); assert(y); \
	for (i = 0; i < nbytes; i++) z[i] = x[i] op y[i]; \
	return z
#define bitopi(op) assert(z); assert(x); \
	applyu(op, z, x, y); \
	return z
#define shft(fill, op) \
	assert(x); assert(z); assert(s >= 0); \
	if (s >= nbits) memset(z, fill, nbytes); \
	else op(nbytes, z, nbytes, x, s, fill); \
	z[nbytes-1] &= msb; \
	return z

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
static int nbits  =  32;
static int nbytes = (32-1)/8 + 1;
static int shift  = (32-1)%8;
static unsigned char msb = 0xFF;
static unsigned char temp[16 + 16 + 16 + 2*16+2];
static sdk_mp_t tmp[] = {temp, temp+1*16, temp+2*16, temp+3*16};

static int applyu(sdk_mp_t op(sdk_mp_t, sdk_mp_t, sdk_mp_t), sdk_mp_t z, sdk_mp_t x,
                  unsigned long u) {
    unsigned long carry;
    { sdk_mp_t z = tmp[2]; carry = sdk_xp_fromint(nbytes, z, u);
        carry |= z[nbytes-1]&~msb;
        z[nbytes-1] &= msb; }
    op(z, x, tmp[2]);
    return carry != 0;
}

static int apply(sdk_mp_t op(sdk_mp_t, sdk_mp_t, sdk_mp_t), sdk_mp_t z, sdk_mp_t x, long v) {
    { sdk_mp_t z = tmp[2]; if (v == LONG_MIN) {
            sdk_xp_fromint(nbytes, z, LONG_MAX + 1UL);
            sdk_xp_neg(nbytes, z, z, 1);
        } else if (v < 0) {
            sdk_xp_fromint(nbytes, z, -v);
            sdk_xp_neg(nbytes, z, z, 1);
        } else
            sdk_xp_fromint(nbytes, z, v);
        z[nbytes-1] &= msb; }
    op(z, x, tmp[2]);
    return (nbits < 8*(int)sizeof (v) &&
            (v < -(1L<<(nbits-1)) || v >= (1L<<(nbits-1))));
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int sdk_mp_set(int n) {
    int prev = nbits;
    assert(n > 1);
    nbits  = n;
    nbytes = (n-1)/8 + 1;
    shift  = (n-1)%8;
    msb    = ones(n);
    if (tmp[0] != temp)
        SDK_FREE(tmp[0]);
    if (nbytes <= 16)
        tmp[0] = temp;
    else
        tmp[0] = SDK_ALLOC(3*nbytes + 2*nbytes + 2);
    tmp[1] = tmp[0] + 1*nbytes;
    tmp[2] = tmp[0] + 2*nbytes;
    tmp[3] = tmp[0] + 3*nbytes;
    return prev;
}

sdk_mp_t sdk_mp_new(unsigned long u) {
    return sdk_mp_fromintu(SDK_ALLOC(nbytes), u);
}

sdk_mp_t sdk_mp_fromintu(sdk_mp_t z, unsigned long u) {
    unsigned long carry;
    assert(z);
    carry = sdk_xp_fromint(nbytes, z, u);
    carry |= z[nbytes-1]&~msb;
    z[nbytes-1] &= msb;
    if (carry)
        return 0;
    return z;
}

sdk_mp_t sdk_mp_fromint(sdk_mp_t z, long v) {
    assert(z);
    if (v == LONG_MIN) {
        sdk_xp_fromint(nbytes, z, LONG_MAX + 1UL);
        sdk_xp_neg(nbytes, z, z, 1);
    } else if (v < 0) {
        sdk_xp_fromint(nbytes, z, -v);
        sdk_xp_neg(nbytes, z, z, 1);
    } else
        sdk_xp_fromint(nbytes, z, v);
    z[nbytes-1] &= msb;
    if ((nbits < 8*(int)sizeof (v) &&
         (v < -(1L<<(nbits-1)) || v >= (1L<<(nbits-1)))))
        return 0;
    return z;
}

long sdk_mp_toint(sdk_mp_t x) {
    unsigned char d[sizeof (unsigned long)];
    assert(x);
    sdk_mp_cvt(8*sizeof d, d, x);
    return (long)sdk_xp_toint(sizeof d, d);
}

sdk_mp_t sdk_mp_cvt(int m, sdk_mp_t z, sdk_mp_t x) {
    int fill, i, mbytes = (m - 1)/8 + 1;
    assert(m > 1);
    assert(x); assert(z);
    fill = sign(x) ? 0xFF : 0;
    if (m < nbits) {
        int carry = (x[mbytes-1]^fill)&~ones(m);
        for (i = mbytes; i < nbytes; i++)
            carry |= x[i]^fill;
        memcpy(z, x, mbytes);
        z[mbytes-1] &= ones(m);
        if (carry)
            return 0;
    } else {
        memcpy(z, x, nbytes);
        z[nbytes-1] |= fill&~msb;
        for (i = nbytes; i < mbytes; i++)
            z[i] = fill;
        z[mbytes-1] &= ones(m);
    }
    return z;
}

unsigned long sdk_mp_tointu(sdk_mp_t x) {
    unsigned char d[sizeof (unsigned long)];
    assert(x);
    sdk_mp_cvtu(8*sizeof d, d, x);
    return sdk_xp_toint(sizeof d, d);
}

sdk_mp_t sdk_mp_cvtu(int m, sdk_mp_t z, sdk_mp_t x) {
    int i, mbytes = (m - 1)/8 + 1;
    assert(m > 1);
    assert(x); assert(z);
    if (m < nbits) {
        int carry = x[mbytes-1]&~ones(m);
        for (i = mbytes; i < nbytes; i++)
            carry |= x[i];
        memcpy(z, x, mbytes);
        z[mbytes-1] &= ones(m);
        if (carry)
            return 0;
    } else {
        memcpy(z, x, nbytes);
        for (i = nbytes; i < mbytes; i++)
            z[i] = 0;
    }
    return z;
}

sdk_mp_t sdk_mp_addu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int carry;
    assert(x); assert(y); assert(z);
    carry = sdk_xp_add(nbytes, z, x, y, 0);
    carry |= z[nbytes-1]&~msb;
    z[nbytes-1] &= msb;
    if (carry)
        return 0;
    return z;
}

sdk_mp_t sdk_mp_subu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int borrow;
    assert(x); assert(y); assert(z);
    borrow = sdk_xp_sub(nbytes, z, x, y, 0);
    borrow |= z[nbytes-1]&~msb;
    z[nbytes-1] &= msb;
    if (borrow)
        return 0;
    return z;
}

sdk_mp_t sdk_mp_mul2u(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    assert(x); assert(y); assert(z);
    memset(tmp[3], '\0', 2*nbytes);
    sdk_xp_mul(tmp[3], nbytes, x, nbytes, y);
    memcpy(z, tmp[3], (2*nbits - 1)/8 + 1);
    return z;
}

sdk_mp_t sdk_mp_mulu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    assert(x); assert(y); assert(z);
    memset(tmp[3], '\0', 2*nbytes);
    sdk_xp_mul(tmp[3], nbytes, x, nbytes, y);
    memcpy(z, tmp[3], nbytes);
    z[nbytes-1] &= msb;
    {
        int i;
        if (tmp[3][nbytes-1]&~msb)
            return 0;
        for (i = 0; i < nbytes; i++)
            if (tmp[3][i+nbytes] != 0)
                return 0;
    }
    return z;
}

sdk_mp_t sdk_mp_divu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    assert(x); assert(y); assert(z);
    {
        memcpy(tmp[1], y, nbytes);
        y = tmp[1];
    }
    if (!sdk_xp_div(nbytes, z, x, nbytes, y, tmp[2], tmp[3]))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_modu(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    assert(x); assert(y); assert(z);
    {
        memcpy(tmp[1], y, nbytes);
        y = tmp[1];
    }
    if (!sdk_xp_div(nbytes, tmp[2], x, nbytes, y, z, tmp[3]))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_add(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x); assert(y); assert(z);
    sx = sign(x);
    sy = sign(y);
    sdk_xp_add(nbytes, z, x, y, 0);
    z[nbytes-1] &= msb;
    if (sx == sy && sy != sign(z))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_sub(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x); assert(y); assert(z);
    sx = sign(x);
    sy = sign(y);
    sdk_xp_sub(nbytes, z, x, y, 0);
    z[nbytes-1] &= msb;
    if (sx != sy && sy == sign(z))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_neg(sdk_mp_t z, sdk_mp_t x) {
    int sx;
    assert(x); assert(z);
    sx = sign(x);
    sdk_xp_neg(nbytes, z, x, 1);
    z[nbytes-1] &= msb;
    if (sx && sx == sign(z))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_mul2(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x); assert(y); assert(z);
    sx = sign(x);
    sy = sign(y);
    if (sx) {
        sdk_xp_neg(nbytes, tmp[0], x, 1);
        x = tmp[0];
        x[nbytes-1] &= msb;
    }
    if (sy) {
        sdk_xp_neg(nbytes, tmp[1], y, 1);
        y = tmp[1];
        y[nbytes-1] &= msb;
    }
    memset(tmp[3], '\0', 2*nbytes);
    sdk_xp_mul(tmp[3], nbytes, x, nbytes, y);
    if (sx != sy)
        sdk_xp_neg((2*nbits - 1)/8 + 1, z, tmp[3], 1);
    else
        memcpy(z, tmp[3], (2*nbits - 1)/8 + 1);
    return z;
}

sdk_mp_t sdk_mp_mul(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x); assert(y); assert(z);
    sx = sign(x);
    sy = sign(y);
    if (sx) {
        sdk_xp_neg(nbytes, tmp[0], x, 1);
        x = tmp[0];
        x[nbytes-1] &= msb;
    }
    if (sy) {
        sdk_xp_neg(nbytes, tmp[1], y, 1);
        y = tmp[1];
        y[nbytes-1] &= msb;
    }
    memset(tmp[3], '\0', 2*nbytes);
    sdk_xp_mul(tmp[3], nbytes, x, nbytes, y);
    if (sx != sy)
        sdk_xp_neg(nbytes, z, tmp[3], 1);
    else
        memcpy(z, tmp[3], nbytes);
    z[nbytes-1] &= msb;
    {
        int i;
        if (tmp[3][nbytes-1]&~msb)
            return 0;
        for (i = 0; i < nbytes; i++)
            if (tmp[3][i+nbytes] != 0)
                return 0;
    }
    if (sx == sy && sign(z))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_div(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x); assert(y); assert(z);
    sx = sign(x);
    sy = sign(y);
    if (sx) {
        sdk_xp_neg(nbytes, tmp[0], x, 1);
        x = tmp[0];
        x[nbytes-1] &= msb;
    }
    if (sy) {
        sdk_xp_neg(nbytes, tmp[1], y, 1);
        y = tmp[1];
        y[nbytes-1] &= msb;
    } else {
        memcpy(tmp[1], y, nbytes);
        y = tmp[1];
    }
    if (!sdk_xp_div(nbytes, z, x, nbytes, y, tmp[2], tmp[3]))
        return 0;
    if (sx != sy) {
        sdk_xp_neg(nbytes, z, z, 1);
        if (!iszero(tmp[2]))
            sdk_xp_diff(nbytes, z, z, 1);
        z[nbytes-1] &= msb;
    } else if (sx && sign(z))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_mod(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x); assert(y); assert(z);
    sx = sign(x);
    sy = sign(y);
    if (sx) {
        sdk_xp_neg(nbytes, tmp[0], x, 1);
        x = tmp[0];
        x[nbytes-1] &= msb;
    }
    if (sy) {
        sdk_xp_neg(nbytes, tmp[1], y, 1);
        y = tmp[1];
        y[nbytes-1] &= msb;
    } else {
        memcpy(tmp[1], y, nbytes);
        y = tmp[1];
    }
    if (!sdk_xp_div(nbytes, tmp[2], x, nbytes, y, z, tmp[3]))
        return 0;
    if (sx != sy) {
        if (!iszero(z))
            sdk_xp_sub(nbytes, z, y, z, 0);
    } else if (sx && sign(tmp[2]))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_addui(sdk_mp_t z, sdk_mp_t x, unsigned long y) {
    assert(x); assert(z);
    if (y < BASE) {
        int carry = sdk_xp_sum(nbytes, z, x, y);
        carry |= z[nbytes-1]&~msb;
        z[nbytes-1] &= msb;
        if (carry)
            return 0;
    } else if (applyu(sdk_mp_addu, z, x, y))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_subui(sdk_mp_t z, sdk_mp_t x, unsigned long y) {
    assert(x); assert(z);
    if (y < BASE) {
        int borrow = sdk_xp_diff(nbytes, z, x, y);
        borrow |= z[nbytes-1]&~msb;
        z[nbytes-1] &= msb;
        if (borrow)
            return 0;
    } else if (applyu(sdk_mp_subu, z, x, y))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_mului(sdk_mp_t z, sdk_mp_t x, unsigned long y) {
    assert(x); assert(z);
    if (y < BASE) {
        int carry = sdk_xp_product(nbytes, z, x, y);
        carry |= z[nbytes-1]&~msb;
        z[nbytes-1] &= msb;
        if (carry)
            return 0;
        if (nbits < 8 && y >= (1U<<nbits))
            return 0;
    } else if (applyu(sdk_mp_mulu, z, x, y))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_divui(sdk_mp_t z, sdk_mp_t x, unsigned long y) {
    assert(x); assert(z);
    if (y == 0)
        return 0;
    else if (y < BASE) {
        sdk_xp_quotient(nbytes, z, x, y);
        if (nbits < 8 && y >= (1U<<nbits))
            return 0;
    } else if (applyu(sdk_mp_divu, z, x, y))
        return 0;
    return z;
}

unsigned long sdk_mp_modui(sdk_mp_t x, unsigned long y) {
    assert(x);
    if (y == 0)
        return 0;
    else if (y < BASE) {
        int r = sdk_xp_quotient(nbytes, tmp[2], x, y);
        if (nbits < 8 && y >= (1U<<nbits))
            return 0;
        return r;
    } else if (applyu(sdk_mp_modu, tmp[2], x, y))
        return 0;
    return sdk_xp_toint(nbytes, tmp[2]);
}

sdk_mp_t sdk_mp_addi(sdk_mp_t z, sdk_mp_t x, long y) {
    assert(x); assert(z);
    if (-BASE < y && y < BASE) {
        int sx = sign(x), sy = y < 0;
        if (sy)
            sdk_xp_diff(nbytes, z, x, -y);
        else
            sdk_xp_sum (nbytes, z, x,  y);
        z[nbytes-1] &= msb;
        if (sx == sy && sy != sign(z))
            return 0;
        if (nbits < 8
            && (y < -(1<<(nbits-1)) || y >= (1<<(nbits-1))))
            return 0;
    } else if (apply(sdk_mp_add, z, x, y))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_subi(sdk_mp_t z, sdk_mp_t x, long y) {
    assert(x); assert(z);
    if (-BASE < y && y < BASE) {
        int sx = sign(x), sy = y < 0;
        if (sy)
            sdk_xp_sum (nbytes, z, x, -y);
        else
            sdk_xp_diff(nbytes, z, x,  y);
        z[nbytes-1] &= msb;
        if (sx != sy && sy == sign(z))
            return 0;
        if (nbits < 8
            && (y < -(1<<(nbits-1)) || y >= (1<<(nbits-1))))
            return 0;
    } else if (apply(sdk_mp_sub, z, x, y))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_muli(sdk_mp_t z, sdk_mp_t x, long y) {
    assert(x); assert(z);
    if (-BASE < y && y < BASE) {
        int sx = sign(x), sy = y < 0;
        if (sx) {
            sdk_xp_neg(nbytes, tmp[0], x, 1);
            x = tmp[0];
            x[nbytes-1] &= msb;
        }
        sdk_xp_product(nbytes, z, x, sy ? -y : y);
        if (sx != sy)
            sdk_xp_neg(nbytes, z, x, 1);
        z[nbytes-1] &= msb;
        if (sx == sy && sign(z))
            return 0;
        if (nbits < 8
            && (y < -(1<<(nbits-1)) || y >= (1<<(nbits-1))))
            return 0;
    } else if (apply(sdk_mp_mul, z, x, y))
        return 0;
    return z;
}

sdk_mp_t sdk_mp_divi(sdk_mp_t z, sdk_mp_t x, long y) {
    assert(x); assert(z);
    if (y == 0)
        return 0;
    else if (-BASE < y && y < BASE) {
        int r;
        int sx = sign(x), sy = y < 0;
        if (sx) {
            sdk_xp_neg(nbytes, tmp[0], x, 1);
            x = tmp[0];
            x[nbytes-1] &= msb;
        }
        r = sdk_xp_quotient(nbytes, z, x, sy ? -y : y);
        if (sx != sy) {
            sdk_xp_neg(nbytes, z, z, 1);
            if (r != 0) {
                sdk_xp_diff(nbytes, z, z, 1);
                r = y - r;
            }
            z[nbytes-1] &= msb;
        } else if (sx && sign(z))
            return 0;
        if (nbits < 8
            && (y < -(1<<(nbits-1)) || y >= (1<<(nbits-1))))
            return 0;
    } else if (apply(sdk_mp_div, z, x, y))
        return 0;
    return z;
}

long sdk_mp_modi(sdk_mp_t x, long y) {
    assert(x);
    if (y == 0)
        return 0;
    else if (-BASE < y && y < BASE) {
        sdk_mp_t z = tmp[2];
        int r;
        int sx = sign(x), sy = y < 0;
        if (sx) {
            sdk_xp_neg(nbytes, tmp[0], x, 1);
            x = tmp[0];
            x[nbytes-1] &= msb;
        }
        r = sdk_xp_quotient(nbytes, z, x, sy ? -y : y);
        if (sx != sy) {
            sdk_xp_neg(nbytes, z, z, 1);
            if (r != 0) {
                sdk_xp_diff(nbytes, z, z, 1);
                r = y - r;
            }
            z[nbytes-1] &= msb;
        } else if (sx && sign(z))
            return 0;
        if (nbits < 8
            && (y < -(1<<(nbits-1)) || y >= (1<<(nbits-1))))
            return 0;
        return r;
    } else if (apply(sdk_mp_mod, tmp[2], x, y))
        return 0;
    return sdk_mp_toint(tmp[2]);
}

int sdk_mp_cmpu(sdk_mp_t x, sdk_mp_t y) {
    assert(x);
    assert(y);
    return sdk_xp_cmp(nbytes, x, y);
}

int sdk_mp_cmp(sdk_mp_t x, sdk_mp_t y) {
    int sx, sy;
    assert(x);
    assert(y);
    sx = sign(x);
    sy = sign(y);
    if (sx != sy)
        return sy - sx;
    else
        return sdk_xp_cmp(nbytes, x, y);
}

int sdk_mp_cmpui(sdk_mp_t x, unsigned long y) {
    assert(x);
    if ((int)sizeof y >= nbytes) {
        unsigned long v = sdk_xp_toint(nbytes, x);
        if (v < y)
            return -1;
        else if (v > y)
            return 1;
        else
            return 0;
    } else {
        sdk_xp_fromint(nbytes, tmp[2], y);
        return sdk_xp_cmp(nbytes, x, tmp[2]);
    }
}

int sdk_mp_cmpi(sdk_mp_t x, long y) {
    int sx, sy = y < 0;
    assert(x);
    sx = sign(x);
    if (sx != sy)
        return sy - sx;
    else if ((int)sizeof y >= nbytes) {
        long v = sdk_mp_toint(x);
        if (v < y)
            return -1;
        else if (v > y)
            return 1;
        else
            return 0;
    } else {
        sdk_mp_fromint(tmp[2], y);
        return sdk_xp_cmp(nbytes, x, tmp[2]);
    }
}

sdk_mp_t sdk_mp_and(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) { bitop(&); }
sdk_mp_t sdk_mp_or (sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) { bitop(|); }
sdk_mp_t sdk_mp_xor(sdk_mp_t z, sdk_mp_t x, sdk_mp_t y) { bitop(^); }
sdk_mp_t sdk_mp_not(sdk_mp_t z, sdk_mp_t x) {
    int i;
    assert(x); assert(z);
    for (i = 0; i < nbytes; i++)
        z[i] = ~x[i];
    z[nbytes-1] &= msb;
    return z;
}
sdk_mp_t sdk_mp_andi(sdk_mp_t z, sdk_mp_t x, unsigned long y) { bitopi(sdk_mp_and); }
sdk_mp_t sdk_mp_ori (sdk_mp_t z, sdk_mp_t x, unsigned long y) { bitopi(sdk_mp_or);  }
sdk_mp_t sdk_mp_xori(sdk_mp_t z, sdk_mp_t x, unsigned long y) { bitopi(sdk_mp_xor); }
sdk_mp_t sdk_mp_lshift(sdk_mp_t z, sdk_mp_t x, int s) { shft(0, sdk_xp_lshift); }
sdk_mp_t sdk_mp_rshift(sdk_mp_t z, sdk_mp_t x, int s) { shft(0, sdk_xp_rshift); }
sdk_mp_t sdk_mp_ashift(sdk_mp_t z, sdk_mp_t x, int s) { shft(sign(x),sdk_xp_rshift); }
sdk_mp_t sdk_mp_fromstr(sdk_mp_t z, const char *str, int base, char **end){
    int carry;
    assert(z);
    memset(z, '\0', nbytes);
    carry = sdk_xp_fromstr(nbytes, z, str, base, end);
    carry |= z[nbytes-1]&~msb;
    z[nbytes-1] &= msb;
    if (carry)
        return 0;
    return z;
}
char *sdk_mp_tostr(char *str, int size, int base, sdk_mp_t x) {
    assert(x);
    assert(base >= 2 && base <= 36);
    assert(str == NULL || size > 1);
    if (str == NULL) {
        {
            int k;
            for (k = 5; (1<<k) > base; k--)
                ;
            size = nbits/k + 1 + 1;
        }
        str = SDK_ALLOC(size);
    }
    memcpy(tmp[1], x, nbytes);
    sdk_xp_tostr(str, size, base, nbytes, tmp[1]);
    return str;
}
void sdk_mp_fmtu(int code, va_list_box *box,
             int put(int c, void *cl), void *cl,
             unsigned char flags[], int width, int precision) {
    sdk_mp_t x;
    char *buf;
    assert(box && flags);
    x = va_arg(box->ap, sdk_mp_t);
    assert(x);
    buf = sdk_mp_tostr(NULL, 0, va_arg(box->ap, int), x);
    sdk_fmt_putd(buf, strlen(buf), put, cl, flags,
             width, precision);
    SDK_FREE(buf);
}

void sdk_mp_fmt(int code, va_list_box *box,
            int put(int c, void *cl), void *cl,
            unsigned char flags[], int width, int precision) {
    sdk_mp_t x;
    int base, size, sx;
    char *buf;
    assert(box && flags);
    x = va_arg(box->ap, sdk_mp_t);
    assert(x);
    base = va_arg(box->ap, int);
    assert(base >= 2 && base <= 36);
    sx = sign(x);
    if (sx) {
        sdk_xp_neg(nbytes, tmp[0], x, 1);
        x = tmp[0];
        x[nbytes-1] &= msb;
    }
    {
        int k;
        for (k = 5; (1<<k) > base; k--)
            ;
        size = nbits/k + 1 + 1;
    }
    buf = SDK_ALLOC(size+1);
    if (sx) {
        buf[0] = '-';
        sdk_mp_tostr(buf + 1, size, base, x);
    } else
        sdk_mp_tostr(buf, size + 1, base, x);
    sdk_fmt_putd(buf, strlen(buf), put, cl, flags,
             width, precision);
    SDK_FREE(buf);
}
