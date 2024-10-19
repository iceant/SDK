#include <sdk_fmt.h>
#include <sdk_memory.h>
#include <assert.h>
#include <limits.h> /*INT_MAX, INT_MIN*/
#include <float.h>


/* -------------------------------------------------------------------------------------------------------------- */
/* TYPES */
struct buf{
    char* buf;
    char* bp;
    int size;
};

/* -------------------------------------------------------------------------------------------------------------- */
/* MACROS */

#define pad(n,c) do { int nn = (n); \
	while (nn-- > 0) \
		put((c), cl); } while (0)


/* -------------------------------------------------------------------------------------------------------------- */
/* STATIC */
static char *sdk_fmt_flags = "-+ 0";

static void cvt_s(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    char *str = va_arg(box->ap, char *);
    assert(str);
    sdk_fmt_puts(str, (int)strlen(str), put, cl, flags,
             width, precision);
}
static void cvt_d(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    int val = va_arg(box->ap, int);
    unsigned m;
    char buf[43];
    char *p = buf + sizeof buf;
    if (val == INT_MIN)
        m = INT_MAX + 1U;
    else if (val < 0)
        m = -val;
    else
        m = val;
    do
        *--p = (char)(m%10 + '0');
    while ((m /= 10) > 0);
    if (val < 0)
        *--p = '-';
    sdk_fmt_putd(p, (int)((buf + sizeof buf) - p), put, cl, flags,
             width, precision);
}

static void cvt_u(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    unsigned m = va_arg(box->ap, unsigned);
    char buf[43];
    char *p = buf + sizeof buf;
    do
        *--p = (char)(m%10 + '0');
    while ((m /= 10) > 0);
    sdk_fmt_putd(p, (int)((buf + sizeof buf) - p), put, cl, flags,
             width, precision);
}

static void cvt_o(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    unsigned m = va_arg(box->ap, unsigned);
    char buf[43];
    char *p = buf + sizeof buf;
    do
        *--p = (char)((m&0x7) + '0');
    while ((m>>= 3) != 0);
    sdk_fmt_putd(p, (int)((buf + sizeof buf) - p), put, cl, flags,
             width, precision);
}

static void cvt_x(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    unsigned m = va_arg(box->ap, unsigned);
    char buf[43];
    char *p = buf + sizeof buf;
    do
        *--p = "0123456789abcdef"[m&0xf];
    while ((m>>= 4) != 0);
    sdk_fmt_putd(p, (int)((buf + sizeof buf) - p), put, cl, flags,
             width, precision);
}
static void cvt_p(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    
    uintptr_t m = (uintptr_t)va_arg(box->ap, void*);
    char buf[43];
    char *p = buf + sizeof buf;
    precision = INT_MIN;
    do
        *--p = "0123456789abcdef"[m&0xf];
    while ((m>>= 4) != 0);
    sdk_fmt_putd(p, (int)((buf + sizeof buf) - p), put, cl, flags,
             width, precision);
}

static void cvt_c(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    if (width == INT_MIN)
        width = 0;
    if (width < 0) {
        flags['-'] = 1;
        width = -width;
    }
    if (!flags['-'])
        pad(width - 1, ' ');
    put((unsigned char)va_arg(box->ap, int), cl);
    if ( flags['-'])
        pad(width - 1, ' ');
}

static void cvt_f(int code, va_list_box *box,
                  int put(int c, void *cl), void *cl,
                  unsigned char flags[], int width, int precision) {
    char buf[DBL_MAX_10_EXP+1+1+99+1];
    if (precision < 0)
        precision = 6;
    if (code == 'g' && precision == 0)
        precision = 1;
    {
        static char fmt[] = "%.dd?";
        assert(precision <= 99);
        fmt[4] = (char)code;
        fmt[3] = (char)(precision%10 + '0');
        fmt[2] = (char)((precision/10)%10 + '0');
        sprintf(buf, fmt, va_arg(box->ap, double));
    }
    sdk_fmt_putd(buf, (int)strlen(buf), put, cl, flags,
             width, precision);
}

static sdk_fmt_t cvt[256] = {
        /*   0-  7 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*   8- 15 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  16- 23 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  24- 31 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  32- 39 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  40- 47 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  48- 55 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  56- 63 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  64- 71 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  72- 79 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  80- 87 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  88- 95 */ 0,     0, 0,     0,     0,     0,     0,     0,
        /*  96-103 */ 0,     0, 0, cvt_c, cvt_d, cvt_f, cvt_f, cvt_f,
        /* 104-111 */ 0,     0, 0,     0,     0,     0,     0, cvt_o,
        /* 112-119 */ cvt_p, 0, 0, cvt_s,     0, cvt_u,     0,     0,
        /* 120-127 */ cvt_x, 0, 0,     0,     0,     0,     0,     0
};

static int outc(int c, void *cl) {
    FILE *f = cl;
    return putc(c, f);
}

static int insert(int c, void *cl) {
    struct buf *p = cl;
    assert(p->bp < ( p->buf + p->size));
    *p->bp++ = (char)c;
    return c;
}

static int append(int c, void *cl) {
    struct buf *p = cl;
    if (p->bp >= p->buf + p->size) {
        SDK_RESIZE(p->buf, 2*p->size);
        p->bp = p->buf + p->size;
        p->size *= 2;
    }
    *p->bp++ = (char)c;
    return c;
}

/* -------------------------------------------------------------------------------------------------------------- */
/* METHODS */

void sdk_fmt_fmt(int put(int c, void* cl), void* cl, const char* fmt, ...){
    va_list_box box;
    va_start(box.ap, fmt);
    sdk_fmt_vfmt(put, cl, fmt, &box);
    va_end(box.ap);
}

void sdk_fmt_vfmt(int put(int c, void* cl), void* cl, const char* fmt, va_list_box * box){
    assert(put);
    assert(fmt);
    while (*fmt){
        if (*fmt != '%' || *++fmt == '%'){
            put((unsigned char)*fmt++, cl);
        }else{
            unsigned char c, flags[256];
            int width = INT_MIN, precision = INT_MIN;
            memset(flags, '\0', sizeof flags);
            if (sdk_fmt_flags) {
                c = *fmt;
                for ( ; c && strchr(sdk_fmt_flags, c); c = *++fmt) {
                    assert(flags[c] < 255);
                    flags[c]++;
                }
            }
            if (*fmt == '*' || isdigit(*fmt)) {
                int n;
                if (*fmt == '*') {
                    n = va_arg(box->ap, int);
                    assert(n != INT_MIN);
                    fmt++;
                } else{
                    for (n = 0; isdigit(*fmt); fmt++) {
                        int d = *fmt - '0';
                        assert(n <= (INT_MAX - d)/10);
                        n = 10*n + d;
                    }
                }
                width = n;
            }
            if (*fmt == '.' && (*++fmt == '*' || isdigit(*fmt))) {
                int n;
                if (*fmt == '*') {
                    n = va_arg(box->ap, int);
                    assert(n != INT_MIN);
                    fmt++;
                } else{
                    for (n = 0; isdigit(*fmt); fmt++) {
                        int d = *fmt - '0';
                        assert(n <= (INT_MAX - d)/10);
                        n = 10*n + d;
                    }
                }
                precision = n;
            }
            c = *fmt++;
            assert(cvt[c]);
            (*cvt[c])(c, box, put, cl, flags, width, precision);
        }
    }
}

void sdk_fmt_print(const char* fmt, ...){
    va_list_box box;
    va_start(box.ap, fmt);
    sdk_fmt_vfmt(outc, stdout, fmt, &box);
    va_end(box.ap);
}

void sdk_fmt_fprint(FILE* stream, const char* fmt, ...){
    va_list_box box;
    va_start(box.ap, fmt);
    sdk_fmt_vfmt(outc, stream, fmt, &box);
    va_end(box.ap);
}

int sdk_fmt_sfmt(char* buf, int size, const char* fmt, ...){
    int len;
    va_list_box box;
    va_start(box.ap, fmt);
    len = sdk_fmt_vsfmt(buf, size, fmt, &box);
    va_end(box.ap);
    return len;
}

int sdk_fmt_vsfmt(char* buf, int size, const char* fmt, va_list_box * box){
    struct buf cl;
    assert(buf);
    assert(size > 0);
    assert(fmt);
    cl.buf = cl.bp = buf;
    cl.size = size;
    sdk_fmt_vfmt(insert, &cl, fmt, box);
    insert(0, &cl);
    return (int)(cl.bp - cl.buf - 1);
}

sdk_fmt_t sdk_fmt_register(int code, sdk_fmt_t newcvt){
    sdk_fmt_t old;
    assert(0 < code
           && code < (int)(sizeof (cvt)/sizeof (cvt[0])));
    old = cvt[code];
    cvt[code] = newcvt;
    return old;
}

void sdk_fmt_putd(const char* str, int len,
                  int put(int, void*), void* cl,
                  unsigned char flags[256], int width, int precision){
    int sign;
    assert(str);
    assert(len >= 0);
    assert(flags);
    if (width == INT_MIN){
        width = 0;
    }
    if (width < 0) {
        flags['-'] = 1;
        width = -width;
    }
    if (precision >= 0){
        flags['0'] = 0;
    }
    if (len > 0 && (*str == '-' || *str == '+')) {
        sign = (int)*str++;
        len--;
    } else if (flags['+']){
        sign = '+';
    }else if (flags[' ']){
        sign = ' ';
    }else{
        sign = 0;
    }
    {
        int n;
        if (precision < 0){
            precision = 1;
        }
        if (len < precision){
            n = precision;
        }
        else if (precision == 0 && len == 1 && str[0] == '0'){
            n = 0;
        }else{
            n = len;
        }
        if (sign){
            n++;
        }
        if (flags['-']) {
            if (sign){
                put(sign, cl);
            }
        } else if (flags['0']) {
            if (sign){
                put(sign, cl);
            }
            pad(width - n, '0');
        } else {
            pad(width - n, ' ');
            if (sign){
                put(sign, cl);
            }
        }
        pad(precision - len, '0');
        {
            int i;
            for (i = 0; i < len; i++){
                put((unsigned char)*str++, cl);
            }
        }
        if (flags['-']) {
            pad(width - n, ' ');
        }
    }
}

void sdk_fmt_puts(const char* str, int len,
                  int put(int c, void* cl), void* cl,
                  unsigned char flags[256], int width, int precision)
{
    assert(str);
    assert(len >= 0);
    assert(flags);
    if (width == INT_MIN)
        width = 0;
    if (width < 0) {
        flags['-'] = 1;
        width = -width;
    }
    if (precision >= 0)
        flags['0'] = 0;
    if (precision >= 0 && precision < len)
        len = precision;
    if (!flags['-'])
        pad(width - len, ' ');
    {
        int i;
        for (i = 0; i < len; i++)
            put((unsigned char)*str++, cl);
    }
    if ( flags['-'])
        pad(width - len, ' ');
}

/* NEED FREE */
char* sdk_fmt_string(const char* fmt, ...){
    char *str;
    va_list_box box;
    assert(fmt);
    va_start(box.ap, fmt);
    str = sdk_fmt_vstring(fmt, &box);
    va_end(box.ap);
    return str;
}

char* sdk_fmt_vstring(const char* fmt, va_list_box* box){
    struct buf cl;
    assert(fmt);
    cl.size = 256;
    cl.buf = cl.bp = SDK_ALLOC(cl.size);
    sdk_fmt_vfmt(append, &cl, fmt, box);
    append(0, &cl);
    return SDK_RESIZE(cl.buf, cl.bp - cl.buf);
}

void sdk_fmt_free(char* str){
    SDK_FREE(str);
}
