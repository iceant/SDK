#ifndef INCLUDED_SDK_COMPILER_H
#define INCLUDED_SDK_COMPILER_H

#define C_STATIC_FORCE_INLINE   static inline __attribute__((always_inline))
#define C_ALIGN(x)              __attribute__((aligned(x)))
#define C_SECTION(x)            __attribute__((section (#x)))
#define C_WEAK                  __attribute__((weak))
#define C_CONSTRUCTOR           __attribute__((constructor))
#define C_DESTRUCTOR            __attribute__((destructor))
#define C_CONSTRUCTOR_P(x)      __attribute__((constructor(x)))
#define C_DESTRUCTOR_P(x)       __attribute__((destructor(x)))
#define C_PACKED_STRUCT         struct __attribute__((packed))

#endif /*INCLUDED_SDK_COMPILER_H*/
