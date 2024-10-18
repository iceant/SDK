#ifndef INCLUDED_MODBUS_CRC16_H
#define INCLUDED_MODBUS_CRC16_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

uint16_t Modbus_CRC16(const uint8_t *nData, size_t wLength);


#endif /*INCLUDED_MODBUS_CRC16_H*/
