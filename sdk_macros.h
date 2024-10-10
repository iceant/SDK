#ifndef INCLUDED_SDK_MACROS_H
#define INCLUDED_SDK_MACROS_H

#define SDK_MIN(x, y)           (((x) < (y)) ?  (x) : (y))
#define SDK_MAX(x, y)           (((x) > (y)) ?  (x) : (y))
#define SDK_ABS(x)              (((x) <  0) ? -(x) : (x))

#define SDK_IS_NAN(x)           ((x) != (x))

#define SDK_PAGE(T, x)          (((T) + (x) - 1)/(x))

////////////////////////////////////////////////////////////////////////////////
//// BIT

#define SDK_BIT(x)              (1<<(x))
#define SDK_BIT_SET(x,p)        ((x)|(1<<(p)))
#define SDK_BIT_CLEAR(x,p)      ((x)&(~(1<<(p))))
#define SDK_BIT_GET(x,p)        (((x)>>(p))&1)
#define SDK_BIT_TOGGLE(x,p)     ((x)^(1<<(p)))

////////////////////////////////////////////////////////////////////////////////
//// ARRAY

#define SDK_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SDK_SET(d, n, v)  do{ size_t i_, n_; \
                      for ( n_ = (n), i_ = 0; n_ > 0; --n_, ++i_) \
                      (d)[i_] = (v); } while(0)
#define SDK_ZERO(d, n)    SDK_SET(d, n, 0)
#define SDK_COLUMNS(S,E)  ( (E) - (S) + 1 )
#define SDK_IS_ARRAY(a)   ((void *)&a == (void *)a)

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_ALIGN_DOWN(x, align)        ((x) & ~((align)-1))
#define SDK_ALIGN_UP(x, align)          (((x) + ((align)-1)) & ~((align)-1))

////////////////////////////////////////////////////////////////////////////////
//// ONCE

#define SDK_ONCE2(stmts, i)     {static int i = 1;\
		                        if(i){stmts\
			                      i = 0;}}
#define SDK_ONCE(stmts)         SDK_ONCE2(stmts, __sdk_once_var_##__COUNTER__)

#endif /*INCLUDED_SDK_MACROS_H*/
