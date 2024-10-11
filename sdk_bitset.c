#include <sdk_bitset.h>
#include <sdk_memory.h>
#include <string.h>
#include <assert.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

struct sdk_bitset_s {
    int length;
    unsigned char *bytes;
    sdk_int_t *words;
};

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define BPW (8 * sizeof(sdk_int_t))
#define nwords(len) ((((len) + BPW - 1)&(~(BPW-1)))/BPW)
#define nbytes(len) ((((len) + 8 - 1)&(~(8-1)))/8)
#define setop(sequal, snull, tnull, op) \
	if (s == t) { assert(s); return sequal; } \
	else if (s == NULL) { assert(t); return snull; } \
	else if (t == NULL) return tnull; \
	else { \
		int i; sdk_bitset_t* set; \
		assert(s->length == t->length); \
		set = sdk_bitset_new(s->length); \
		for (i = nwords(s->length); --i >= 0; ) \
			set->words[i] = s->words[i] op t->words[i]; \
		return set; }

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static unsigned char msbmask[] = {
        0xFF, 0xFE, 0xFC, 0xF8,
        0xF0, 0xE0, 0xC0, 0x80
};

static unsigned char lsbmask[] = {
        0x01, 0x03, 0x07, 0x0F,
        0x1F, 0x3F, 0x7F, 0xFF
};

static sdk_bitset_t* copy(sdk_bitset_t* t) {
    sdk_bitset_t* set;
    assert(t);
    set = sdk_bitset_new(t->length);
    if (t->length > 0)
        memcpy(set->bytes, t->bytes, nbytes(t->length));
    return set;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_bitset_t* sdk_bitset_new(int length) {
    sdk_bitset_t* set;
    assert(length >= 0);
    SDK_NEW(set);
    if (length > 0){
        set->words = SDK_CALLOC(nwords(length), sizeof (sdk_int_t));
    }
    else{
        set->words = NULL;
    }
    set->bytes = (unsigned char *)set->words;
    set->length = length;
    return set;
}

void sdk_bitset_free(sdk_bitset_t* *set) {
    assert(set && *set);
    SDK_FREE((*set)->words);
    SDK_FREE(*set);
}

int sdk_bitset_length(sdk_bitset_t* set) {
    assert(set);
    return set->length;
}

int sdk_bitset_count(sdk_bitset_t* set) {
    int length = 0, n;
    static char count[] = { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4 };
    assert(set);
    for (n = nbytes(set->length); --n >= 0; ) {
        unsigned char c = set->bytes[n];
        length += count[c&0xF] + count[c>>4];
    }
    return length;
}

int sdk_bitset_get(sdk_bitset_t* set, int n) {
    assert(set);
    assert(0 <= n && n < set->length);
    return ((set->bytes[n/8]>>(n%8))&1);
}

int sdk_bitset_put(sdk_bitset_t* set, int n, int bit) {
    int prev;
    assert(set);
    assert(bit == 0 || bit == 1);
    assert(0 <= n && n < set->length);
    prev = ((set->bytes[n/8]>>(n%8))&1);
    if (bit == 1)
        set->bytes[n/8] |=   1<<(n%8);
    else
        set->bytes[n/8] &= ~(1<<(n%8));
    return prev;
}

void sdk_bitset_set(sdk_bitset_t* set, int lo, int hi) {
    assert(set);
    assert(0 <= lo && hi < set->length);
    assert(lo <= hi);
    if (lo/8 < hi/8) {
        set->bytes[lo/8] |= msbmask[lo%8];
        {
            int i;
            for (i = lo/8+1; i < hi/8; i++)
                set->bytes[i] = 0xFF;
        }
        set->bytes[hi/8] |= lsbmask[hi%8];
    } else
        set->bytes[lo/8] |= (msbmask[lo%8]&lsbmask[hi%8]);
}

void sdk_bitset_clear(sdk_bitset_t* set, int lo, int hi) {
    assert(set);
    assert(0 <= lo && hi < set->length);
    assert(lo <= hi);
    if (lo/8 < hi/8) {
        int i;
        set->bytes[lo/8] &= ~msbmask[lo%8];
        for (i = lo/8+1; i < hi/8; i++)
            set->bytes[i] = 0;
        set->bytes[hi/8] &= ~lsbmask[hi%8];
    } else
        set->bytes[lo/8] &= ~(msbmask[lo%8]&lsbmask[hi%8]);
}

void sdk_bitset_not(sdk_bitset_t* set, int lo, int hi) {
    assert(set);
    assert(0 <= lo && hi < set->length);
    assert(lo <= hi);
    if (lo/8 < hi/8) {
        int i;
        set->bytes[lo/8] ^= msbmask[lo%8];
        for (i = lo/8+1; i < hi/8; i++)
            set->bytes[i] ^= 0xFF;
        set->bytes[hi/8] ^= lsbmask[hi%8];
    } else
        set->bytes[lo/8] ^= (msbmask[lo%8]&lsbmask[hi%8]);
}

void sdk_bitset_map(sdk_bitset_t* set,
             void apply(int n, int bit, void *cl), void *cl) {
    int n;
    assert(set);
    for (n = 0; n < set->length; n++)
        apply(n, ((set->bytes[n/8]>>(n%8))&1), cl);
}

int sdk_bitset_eq(sdk_bitset_t* s, sdk_bitset_t* t) {
    int i;
    assert(s && t);
    assert(s->length == t->length);
    for (i = nwords(s->length); --i >= 0; )
        if (s->words[i] != t->words[i])
            return 0;
    return 1;
}

int sdk_bitset_leq(sdk_bitset_t* s, sdk_bitset_t* t) {
    int i;
    assert(s && t);
    assert(s->length == t->length);
    for (i = nwords(s->length); --i >= 0; )
        if ((s->words[i]&~t->words[i]) != 0)
            return 0;
    return 1;
}

int sdk_bitset_lt(sdk_bitset_t* s, sdk_bitset_t* t) {
    int i, lt = 0;
    assert(s && t);
    assert(s->length == t->length);
    for (i = nwords(s->length); --i >= 0; )
        if ((s->words[i]&~t->words[i]) != 0)
            return 0;
        else if (s->words[i] != t->words[i])
            lt |= 1;
    return lt;
}

sdk_bitset_t* sdk_bitset_union(sdk_bitset_t* s, sdk_bitset_t* t) {
    setop(copy(t), copy(t), copy(s), |)
}

sdk_bitset_t* sdk_bitset_inter(sdk_bitset_t* s, sdk_bitset_t* t) {
    setop(copy(t),
          sdk_bitset_new(t->length), sdk_bitset_new(s->length), &)
}

sdk_bitset_t* sdk_bitset_minus(sdk_bitset_t* s, sdk_bitset_t* t) {
    setop(sdk_bitset_new(s->length),
          sdk_bitset_new(t->length), copy(s), & ~)
}

sdk_bitset_t* sdk_bitset_diff(sdk_bitset_t* s, sdk_bitset_t* t) {
    setop(sdk_bitset_new(s->length), copy(t), copy(s), ^)
}

void sdk_bitset_fmt(int code, va_list_box* box,
                    int put(int c, void* cl), void* cl,
                    unsigned char flags[256], int width, int precision)
{
    sdk_bitset_t* set;
    set = va_arg(box->ap, sdk_bitset_t*);
    assert(set);
    int bitset_length = sdk_bitset_length(set);
    for(int i=0; i<bitset_length; i++){
        char c = (char)((sdk_bitset_get(set, i)==1)?'1':'0');
        sdk_fmt_putd(&c, 1, put, cl, flags, width, precision);
    }
}