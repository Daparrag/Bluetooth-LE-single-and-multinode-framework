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
#ifndef DONT_FIND_SERVICE
#define DONT_FIND_SERVICE 	0x00					/*!< Used for disable the service discovery in the device>*/
#endif

#ifndef FIND_SPE_SERVICE
#define FIND_SPE_SERVICE  	0x01					/*!< Used for enable specific service(s) discovery (services characterized by specific UUID) >*/	
#endif

#ifndef FIND_GEN_SERVICE
#define FIND_GEN_SERVICE  	0x02					/*!< Used for discovery any service on the peer device(s) >*/	
#endif

#ifndef DONT_FIND_CHAR
#define DONT_FIND_CHAR		0x01					/*!< Used for disable the attribute(s) discovery in the device>*/
#endif

#ifndef FIND_SPE_CHAR
#define FIND_SPE_CHAR  		0x02					/*!< Used for enable specific attribute(s) discovery (attribute(s) characterized by specific UUID) >*/	
#endif

#ifndef FIND_GEN_CHAR
#define FIND_GEN_CHAR  		0x04					/*!< Used for discovery any attribute(s) on the peer device(s) >*/	
#endif

#ifndef DISC_SERVICE
#define DISC_SERVICE		FIND_SPE_SERVICE		/*!< Use one of the follow: DONT_FIND_SERVICE, FIND_SPE_SERVICE, FIND_GEN_SERVICE >*/
#endif

#ifndef DISC_CHAR
#define DISC_CHAR	        FIND_SPE_CHAR			/*!< Use one of the follow: DONT_FIND_CHAR, FIND_SPE_CHAR, FIND_GEN_CHAR >*/	
#endif




SERV_Status DSCV_primary_services_by_uuid(connection_t * connection); /*!< primitive used for discover services characterized by a specific UUID >*/
SERV_Status DSCV_primary_char_by_uuid(connection_t * connection);     /*!< primitive used for discovery a specific characterstic identified by an UUID >*/
SERV_Status DSCV_Enable_Notify(connection_t * connection);            /*!< Enable notify for those characteristics who have this property>*/                     
SERV_Status SH_Associate_att_handler_CB(connection_t * connection,uint16_t peer_attr_handler);   /*!< This function is used for associate an attribute_handler to specific connection's attribute >*/                     
void service_error_handler(void);








#endif /* SERVICE_HANDLER_BLE_H */
/**/