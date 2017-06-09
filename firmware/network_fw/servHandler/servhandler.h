/*header file for management the services*/
/*version 1.0*/
#ifndef SERVICE_HANDLER_BLE_H
#define SERVICE_HANDLER_BLE_H

#include <ble_firmware.h>

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
#define DISC_CHAR			FIND_SPE_CHAR	
#endif



void service_handler(connection_t * connection, net * flags);









#endif /* SERVICE_HANDLER_BLE_H */
/**/