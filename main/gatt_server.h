#ifndef GATT_SERVER_H
#define GATT_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************ GLOBAL ENUMS ***************/

/************ GLOBAL TYPES ***************/
typedef void (* gatt_server_callback)(uint8_t*, uint16_t);

/************ GLOBAL FUNCTIONS ***************/
void gatt_server_Init(void);
void gatt_server_registerCbk(gatt_server_callback cbk);

#endif /*GATT_SERVER_H*/
