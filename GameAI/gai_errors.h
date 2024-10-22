#ifndef INCLUDED_GAI_ERRORS_H
#define INCLUDED_GAI_ERRORS_H


#ifndef INCLUDED_GAI_TYPES_H
#include <gai_types.h>
#endif /*INCLUDED_GAI_TYPES_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef int gai_err_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define GAI_ERR_OK          0
#define GAI_ERR_ERROR       (-1)
#define GAI_ERR_NOMEM       (-2)

#endif /* INCLUDED_GAI_ERRORS_H */
