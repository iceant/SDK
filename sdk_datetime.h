#ifndef INCLUDED_SDK_DATETIME_H
#define INCLUDED_SDK_DATETIME_H

#ifndef INCLUDED_TIME_H
#define INCLUDED_TIME_H
#include <time.h>
#endif /*INCLUDED_TIME_H*/

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

time_t sdk_datetime_current_ms(void);


#endif /*INCLUDED_SDK_DATETIME_H*/
