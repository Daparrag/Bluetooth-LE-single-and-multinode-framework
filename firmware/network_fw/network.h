/*header file for network_interface*/
/*version 1.0*/
#ifndef NET_BLE_H
#define NET_BLE_H

#include <ble_firmware.h>
#include <chandler.h>
#include <servhandler.h>






typedef enum{
	NET_NOT_INITIALIZED,
	NET_INITALIZED
}net_status_t;


NET_Status init_network(net_type_t net_type, dv_type_t device_type ,network_t ** net_output);/*initialized the network module*/

NET_Status net_setup_connection_config(config_connection_t * config, 
										uint8_t * list_index, 
										size_t list_index_size);/*allows setup a specific network configuiration requirements*/

NET_Status net_setup_profile_definition(app_profile_t * profile_def, 
									  uint8_t * list_index, 
									  size_t list_index_size);/*allows setup a profile(services and characteristics) in a connection*/

void get_connection_status_by_addrs(uint8_t * slave_addrs, cn_state_t ** cstatus);/*return the status of the connection based on the slave address*/


void get_services_status_by_addrs(uint8_t * slave_addrs, sv_state_t ** cstatus);/*return the service status based on the slave address*/


NET_Status network_process(void);/*main network function: running the connection handler and the services handler */

void network_update_status(net_status_t newstatus);/*change the status of the network*/

#endif /* NET_BLE_H*/
