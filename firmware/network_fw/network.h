/*header file for network_interface*/
/*version 1.0*/
#ifndef NET_BLE_H
#define NET_BLE_H

#include <ble_firmware.h>
#include <chandler.h>
#include <servhandler.h>
#include <eventhandler.h>

#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif

#ifdef DEBUG
#include "debug.h"
#endif



typedef enum{
	NET_NOT_INITIALIZED,
	NET_INITALIZED,
}net_status_t;



NET_Status init_network(net_type_t net_type, dv_type_t device_type ,network_t ** net_output);/*initialized the network module*/
NET_Status network_process(event_t * event);/*main network function: running the connection handler and the services handler */
NET_Status net_setup_connection_config(config_connection_t * config, 
										uint8_t * list_index, 
										size_t list_index_size);
NET_Status net_setup_profile_definition(app_profile_t * profile_def, 
					uint8_t * list_index, 
				        size_t list_index_size);

#endif /*NET_BLE_H*/