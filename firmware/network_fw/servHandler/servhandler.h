/*header file for management the services*/
/*version 1.0*/
#ifndef SERVICE_HANDLER_BLE_H
#define SERVICE_HANDLER_BLE_H

#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif

#include <ble_firmware.h>

#ifdef DEBUG
#include "debug.h"
#endif

/********************SERVICE HANDLER DEFAULT CONFIG*************/
#define DONT_FIND_SERVICE 	0x00
#define FIND_SPE_SERVICE  	0x01	
#define FIND_GEN_SERVICE  	0x02	

#define DONT_FIND_CHAR		0x01
#define FIND_SPE_CHAR  		0x02	
#define FIND_GEN_CHAR  		0x04	

#ifndef DISC_SERVICE
#define DISC_SERVICE		FIND_SPE_SERVICE
#endif

#ifndef DISC_CHAR
#define DISC_CHAR	        FIND_SPE_CHAR	
#endif



void service_handler(connection_t * connection, net_flags * flags);









#endif /* SERVICE_HANDLER_BLE_H */
/**/