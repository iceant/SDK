#include <sdk_xp.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define BASE (1<<8)

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


static char map[] = {
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        36, 36, 36, 36, 36, 36, 36,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
        23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
        36, 36, 36, 36, 36, 36,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
        23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


unsigned long sdk_xp_fromint(int n, sdk_xp_t z, unsigned long u) {
    int i = 0;
    do
        z[i++] = u%BASE;
    while ((u /= BASE) > 0 && i < n);
    for ( ; i < n; i++)
        z[i] = 0;
    return u;
}

unsigned long sdk_xp_toint(int n, sdk_xp_t x) {
    unsigned long u = 0;
    int i = (int)sizeof u;
    if (i > n)
        i = n;
    while (--i >= 0)
        u = BASE*u + x[i];
    return u;
}

int sdk_xp_length(int n, sdk_xp_t x) {
    while (n > 1 && x[n-1] == 0)
        n--;
    return n;
}
int sdk_xp_add(int n, sdk_xp_t z, sdk_xp_t x, sdk_xp_t y, int carry) {
    int i;
    for (i = 0; i < n; i++) {
        carry += x[i] + y[i];
        z[i] = carry%BASE;
        carry /= BASE;
    }
    return carry;
}
int sdk_xp_sub(int n, sdk_xp_t z, sdk_xp_t x, sdk_xp_t y, int borrow) {
    int i;
    for (i = 0; i < n; i++) {
        int d = (x[i] + BASE) - borrow - y[i];
        z[i] = d%BASE;
        borrow = 1 - d/BASE;
    }
    return borrow;
}

int sdk_xp_sum(int n, sdk_xp_t z, sdk_xp_t x, int y) {
    int i;
    for (i = 0; i < n; i++) {
        y += x[i];
        z[i] = y%BASE;
        y /= BASE;
    }
    return y;
}

int sdk_xp_diff(int n, sdk_xp_t z, sdk_xp_t x, int y) {
    int i;
    for (i = 0; i < n; i++) {
        int d = (x[i] + BASE) - y;
        z[i] = d%BASE;
        y = 1 - d/BASE;
    }
    return y;
}

int sdk_xp_neg(int n, sdk_xp_t z, sdk_xp_t x, int carry) {
    int i;
    for (i = 0; i < n; i++) {
        carry += (unsigned char)~x[i];
        z[i] = carry%BASE;
        carry /= BASE;
    }
    return carry;
}

int sdk_xp_mul(sdk_xp_t z, int n, sdk_xp_t x, int m, sdk_xp_t y) {
    int i, j, carryout = 0;
    for (i = 0; i < n; i++) {
        unsigned carry = 0;
        for (j = 0; j < m; j++) {
            carry += x[i]*y[j] + z[i+j];
            z[i+j] = carry%BASE;
            carry /= BASE;
        }
        for ( ; j < n + m - i; j++) {
            carry += z[i+j];
            z[i+j] = carry%BASE;
            carry /= BASE;
        }
        carryout |= carry;
    }
    return carryout;
}
int sdk_xp_product(int n, sdk_xp_t z, sdk_xp_t x, int y) {
    int i;
    unsigned carry = 0;
    for (i = 0; i < n; i++) {
        carry += x[i]*y;
        z[i] = carry%BASE;
        carry /= BASE;
    }
    return carry;
}
int sdk_xp_div(int n, sdk_xp_t q, sdk_xp_t x, int m, sdk_xp_t y, sdk_xp_t r, sdk_xp_t tmp) {
    int nx = n, my = m;
    n = sdk_xp_length(n, x);
    m = sdk_xp_length(m, y);
    if (m == 1) {
        if (y[0] == 0)
            return 0;
        r[0] = sdk_xp_quotient(nx, q, x, y[0]);
        memset(r + 1, '\0', my - 1);
    } else if (m > n) {
        memset(q, '\0', nx);
        memcpy(r, x, n);
        memset(r + n, '\0', my - n);
    } else {
        int k;
        unsigned char *rem = tmp, *dq = tmp + n + 1;
        assert(2 <= m && m <= n);
        memcpy(rem, x, n);
        rem[n] = 0;
        for (k = n - m; k >= 0; k--) {
            int qk;
            {
                int i;
                assert(2 <= m && m <= k+m && k+m <= n);
                {
                    int km = k + m;
                    unsigned long y2 = y[m-1]*BASE + y[m-2];
                    unsigned long r3 = rem[km]*(BASE*BASE) +
                                       rem[km-1]*BASE + rem[km-2];
                    qk = r3/y2;
                    if (qk >= BASE)
                        qk = BASE - 1;
                }
                dq[m] = sdk_xp_product(m, dq, y, qk);
                for (i = m; i > 0; i--)
                    if (rem[i+k] != dq[i])
                        break;
                if (rem[i+k] < dq[i])
                    dq[m] = sdk_xp_product(m, dq, y, --qk);
            }
            q[k] = qk;
            {
                int borrow;
                assert(0 <= k && k <= k+m);
                borrow = sdk_xp_sub(m + 1, &rem[k], &rem[k], dq, 0);
                assert(borrow == 0);
            }
        }
        memcpy(r, rem, m);
        {
            int i;
            for (i = n-m+1; i < nx; i++)
                q[i] = 0;
            for (i = m; i < my; i++)
                r[i] = 0;
        }
    }
    return 1;
}
int sdk_xp_quotient(int n, sdk_xp_t z, sdk_xp_t x, int y) {
    int i;
    unsigned carry = 0;
    for (i = n - 1; i >= 0; i--) {
        carry = carry*BASE + x[i];
        z[i] = carry/y;
        carry %= y;
    }
    return carry;
}
int sdk_xp_cmp(int n, sdk_xp_t x, sdk_xp_t y) {
    int i = n - 1;
    while (i > 0 && x[i] == y[i])
        i--;
    return x[i] - y[i];
}
void sdk_xp_lshift(int n, sdk_xp_t z, int m, sdk_xp_t x, int s, int fill) {
    fill = fill ? 0xFF : 0;
    {
        int i, j = n - 1;
        if (n > m)
            i = m - 1;
        else
            i = n - s/8 - 1;
        for ( ; j >= m + s/8; j--)
            z[j] = 0;
        for ( ; i >= 0; i--, j--)
            z[j] = x[i];
        for ( ; j >= 0; j--)
            z[j] = fill;
    }
    s %= 8;
    if (s > 0)
    {
        sdk_xp_product(n, z, z, 1<<s);
        z[0] |= fill>>(8-s);
    }
}
void sdk_xp_rshift(int n, sdk_xp_t z, int m, sdk_xp_t x, int s, int fill) {
    fill = fill ? 0xFF : 0;
    {
        int i, j = 0;
        for (i = s/8; i < m && j < n; i++, j++)
            z[j] = x[i];
        for ( ; j < n; j++)
            z[j] = fill;
    }
    s %= 8;
    if (s > 0)
    {
        sdk_xp_quotient(n, z, z, 1<<s);
        z[n-1] |= fill<<(8-s);
    }
}
int sdk_xp_fromstr(int n, sdk_xp_t z, const char *str,
               int base, char **end) {
    const char *p = str;
    assert(p);
    assert(base >= 2 && base <= 36);
    while (*p && isspace(*p))
        p++;
    if ((*p && isalnum(*p) && map[*p-'0'] < base)) {
        int carry;
        for ( ; (*p && isalnum(*p) && map[*p-'0'] < base); p++) {
            carry = sdk_xp_product(n, z, z, base);
            if (carry)
                break;
            sdk_xp_sum(n, z, z, map[*p-'0']);
        }
        if (end)
            *end = (char *)p;
        return carry;
    } else {
        if (end)
            *end = (char *)str;
        return 0;
    }
}

char *sdk_xp_tostr(char *str, int size, int base,
               int n, sdk_xp_t x) {
    int i = 0;
    assert(str);
    assert(base >= 2 && base <= 36);
    do {
        int r = sdk_xp_quotient(n, x, x, base);
        assert(i < size);
        str[i++] =
                "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[r];
        while (n > 1 && x[n-1] == 0)
            n--;
    } while (n > 1 || x[0] != 0);
    assert(i < size);
    str[i] = '\0';
    {
        int j;
        for (j = 0; j < --i; j++) {
            char c = str[j];
            str[j] = str[i];
            str[i] = c;
        }
    }
    return str;
}
