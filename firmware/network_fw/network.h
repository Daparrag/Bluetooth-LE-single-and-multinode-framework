/*header file for network_interface*/
/*version 1.0*/
#ifndef NET_BLE_H
#define NET_BLE_H

#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif


#include <ble_firmware.h>
#include <chandler.h>
#include <servhandler.h>



#ifdef DEBUG
#include "debug.h"
#endif



typedef enum{/*since we are using a flags for the network module this could be removed*/
	NET_NOT_INITIALIZED,
	NET_INITALIZED,
}net_status_t;



NET_Status init_network(net_type_t net_type, dv_type_t device_type ,uint8_t N_reconnections, network_t ** net_output);/*initialized the network module*/

NET_Status network_process(/*event_t * event*/);/*<! main network function: running the connection handler and the services handler >*/

NET_Status net_setup_connection_config(config_connection_t * config, 
                                       uint8_t * list_index, 
                                      size_t list_index_size); /*<! setup a connection parameters for a specific device (in the future QoS) > */

NET_Status net_setup_profile_definition(app_profile_t * profile_def, 
					uint8_t * list_index, 
				        size_t list_index_size); /*<! Used by the application to indicate a specific profiles to one or set of connection(s) (in the future QoS)>*/

NET_Status service_handler_config(uint8_t serv_disc_mode, 
                                  uint8_t char_disc_mode,
                                  uint8_t * connection_index, 
				  size_t connection_index_size); /*<! Used by the application to indicate a specific service_handler configuration  to one or a set of connection(s) (used to define a node as a client or as a server)>*/

connection_t * NET_get_connection_by_chandler_BLE(uint16_t chandler);    /* retreave a connection characterized by one specific connection handler */

connection_t * NET_get_connection_by_status_CB(uint8_t _status);   /* retreave a connection characterized by one specific status */

connection_t * NET_get_connection_by_index_CB(uint8_t _index);    /* retreave a connection characterized by one specific index */

void NET_get_service_and_attributes_by_chandler_BLE(uint16_t chandler, uint16_t attrhandler,app_service_t ** serv, app_attr_t ** att);/* retreave a connection characterized by and specific connection handler and atribute handler associated*/

uint8_t network_get_status(void);

uint8_t get_num_connections(void);/*<! return the number of connections>*/
#endif /*NET_BLE_H*/