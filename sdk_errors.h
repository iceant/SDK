#ifndef INCLUDED_SDK_ERRORS_H
#define INCLUDED_SDK_ERRORS_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif // INCLUDED_SDK_TYPES_H

////////////////////////////////////////////////////////////////////////////////
////
typedef sdk_int_t sdk_err_t;

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_ERR_OK          0
#define SDK_ERR_ERROR       (-1)
#define SDK_ERR_NOMEM       (-2)
#define SDK_ERR_INVAL       (-3)
#define SDK_ERR_FULL        (-4)
#define SDK_ERR_EMPTY       (-5)
#define SDK_ERR_TIMEOUT     (-6)


#endif /*INCLUDED_SDK_ERRORS_H*/
