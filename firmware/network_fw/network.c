/*Network inteface implementation:
* This is the interface between the low level primitives and the application interface,
* as well it allows the user set up configuration.
*/

/*task:
*
*	1. set up a connection configuration
*   2. initialized the connection and services handlers 
*   3. inform the handlers status to other interfaces
*	4. receives events notification from the event_handler or application parameters (we can implement and event queue but this is so advance for this application but could be introduce an efficient resouce management)
*/
#include <network.h>
/*****************************macros**********************************************/

/*********************Static func************************/
static void init_connection_handler(void);
static void init_services_handler(void);
static void init_device(void);
static int get_connection_index_by_addrs(uint8_t * addrs);
static void Get_Device_From_Conn_Handle(uint16_t conn_handler);
static uint8_t verify_Conn_Handle (uint16_t handle, uint8_t *index);
net_type_t    net_mode =  NET_CONNECTED;/*the default connection mode*/
dv_type_t     device_role = DEVICE_CENTRAL; /*by default the devices will be cental*/
net_status_t  net_status = NET_NOT_INITIALIZED;

network_t network;
/*********************************************************/

NET_Status init_network(net_type_t net_type, dv_type_t device_type ,network_t ** net_output){
/*initialized the network flags*/
network.flags.new_net_event=0;
network.flags.device_found=0;
network.flags.wait_end_procedure=0;
network.flags.service_discovery_evt=0;
network.flags.attr_discovery_evt=0;
network.flags.retry_conn=0;
/*here it is possible to setup different configurations based on the network type*/
net_mode = net_type;
device_role = device_type;
init_connection_handler();
init_services_handler();
init_device();
net_status = NET_INITALIZED;
*net_output=&network;
return NET_SUCCESS;
}


void init_device(void){
  uint8_t i;
  dv_state_t device_init_config =  DEVICE_UNITIALIZED;
  
  if (device_role == DEVICE_CENTRAL)            device_init_config = DEVICE_DISCOVERY_MODE;
  if (device_role == DEVICE_PHERISPHERAL)       device_init_config = DEVICE_ADVERTISEMENT_MODE;    
  if (device_role == DEVICE_BROADCASTER)        device_init_config = DEVICE_ADVERTISEMENT_MODE;
  if (device_role == DEVICE_OBSERVER)           device_init_config = DEVICE_SCAN_MODE;
    

 #ifdef MULTINODE
for(i=0; i <  EXPECTED_NODES; i++)
network.mMSConnection[i].device_cstatus = device_init_config;
 #else
    network.mMSConnection.device_cstatus = device_init_config;
 #endif
  
}

void init_connection_handler(void){
	/*this procedure will initialized all the connection flags*/
uint8_t i;
if(net_mode == NET_CONNECTED){
/*this is a connection oriented network*/
  #ifdef MULTINODE
for(i=0; i <  EXPECTED_NODES; i++)   
  network.mMSConnection[i].connection_status =  ST_UNESTABLISHED;
//network.mMSConnection[i].connection_status = ST_UNESTABLISHED;
  #else
  network.mMSConnection.connection_status = ST_UNESTABLISHED;
  #endif
  }

}


void init_services_handler(void){
	/*this procedure will initialized all the service flags x connection*/
uint8_t i;

/*the services_handler will management the service discovery only if the device role is CENTRAL*/
if(device_role == DEVICE_CENTRAL){
    #ifdef MULTINODE
      for(i=0; i <= EXPECTED_NODES; i++)network.mMSConnection[i].service_status = ST_SERVICE_DISCOVERY;
    #else
      network.mMSConnection.service_status = ST_SERVICE_DISCOVERY; /*always at the bigining */	
    #endif	    
  }
}


NET_Status net_setup_connection_config(config_connection_t * config, 
					uint8_t * list_index, 
                                        size_t list_index_size){

uint8_t i;
uint8_t index;
/*check the input*/
if(config==NULL){

	PRINTF("input config could not be NULL \n");
	return NET_ERROR;

}

#ifdef MULTINODE
if(list_index_size >= EXPECTED_NODES || list_index== NULL){
	PRINTF("net_setup_connection_config: error during net_setup_connection_config: wrong imput parameters\n");
	return NET_ERROR;
}

	for(i=0; i < list_index_size; i++){

		index = * list_index++;
		network.mMSConnection[index].cconfig=config;

		if(network.mMSConnection[index].cconfig==NULL){
		/*something is wrong*/
		PRINTF("error during net_setup_connection_config: wrong  confguration setup\n");
		return NET_ERROR;
		}	

	}

#else

	network.mMSConnection.cconfig = config;

	if(network.mMSConnection.cconfig==NULL){
		/*something is wrong*/
		PRINTF("error during net_setup_connection_config: wrong  confguration setup\n");
		return NET_ERROR;
	}
#endif

/*setup_ conection configuration  ok*/
return NET_SUCCESS;
}


/*setup a profile for an specific connection*/
NET_Status net_setup_profile_definition(app_profile_t * profile_def, 
					uint8_t * list_index, 
				        size_t list_index_size){
uint8_t i;
uint8_t index;
/*check the input*/
if(profile_def==NULL){

	PRINTF("net_setup_profile_definition: input profile_def could not be NULL \n");
	return NET_ERROR;
}

#ifdef MULTINODE

if(list_index_size >= EXPECTED_NODES || list_index== NULL){
	PRINTF("error during net_setup_profile_definition: wrong imput parameters\n");
	return NET_ERROR;
}

for(i=0; i < list_index_size; i++){
	index = *list_index++;
	network.mMSConnection[index].Node_profile=profile_def;

	if(network.mMSConnection[index].Node_profile==NULL){
		/*something is wrong*/
		PRINTF("error during net_setup_profile_definition: wrong  confguration setup\n");
		return NET_ERROR;
	}
}

#else
network.mMSConnection.Node_profile = profile_def;

if(network.mMSConnection.Node_profile==NULL){
		/*something is wrong*/
		PRINTF("error during net_setup_profile_definition: wrong  confguration setup\n");
		return NET_ERROR;
	}

#endif
/*setup_ conection configuration  ok*/
return NET_SUCCESS;
}



/*get_connection_status*/
void get_connection_status_by_addrs(uint8_t * slave_addrs, cn_state_t ** cstatus){
	int index;
	index = get_connection_index_by_addrs(slave_addrs);

	if(index==-1){
		/*devices address does not match*/
		*cstatus=NULL;
		return;
	}
#ifdef MULTINODE
	*cstatus = &network.mMSConnection[index].connection_status; 
#else
	*cstatus = &network.mMSConnection.connection_status;
#endif	
}



/*get_service_status*/
void get_services_status_by_addrs(uint8_t * slave_addrs, sv_state_t ** cstatus){

	int index;
	index = get_connection_index_by_addrs(slave_addrs);

if(index==-1){
		/*devices address does not match*/
		*cstatus=NULL;
		return;
	}

#ifdef MULTINODE
	*cstatus = &network.mMSConnection[index].service_status; 
#else
	*cstatus = &network.mMSConnection.service_status;
#endif	


}

/*get_connection_index_by_addrs*/
int get_connection_index_by_addrs(uint8_t * slave_addrs){
uint8_t i;
#ifdef MULTINODE
for(i = 0; i < network.num_device_found; i++){
	if(network.mMSConnection[i].device_address[1]==slave_addrs[0] 
		&& network.mMSConnection[i].device_address[0]==slave_addrs[0]) return i;
}
#else
if(network.mMSConnection.device_address[1]==slave_addrs[0] 
	&& network.mMSConnection.device_address[0]==slave_addrs[0]) return 1;
#endif
PRINTF(" get_connection_index_by_addrs: error address does not exist\n");	
return -1;
}


void network_update_status(net_status_t new_status){
	net_status = new_status;
}

void network_set_event(void){

	if(network.flags.new_net_event){
		//get_event();
	}

}

NET_Status network_process(){


	return NET_SUCCESS;
}




/**
 * @brief  This function retrieves the peripheral device index and address 
 *         from the connection handle
 * @param  connection handle
 * @param  address of the device
 * @retval None
 */
void Get_Device_From_Conn_Handle(uint16_t handle)
{

}


/**
 * @brief  This function retrieves the peripheral device index 
 *         from the connection handle
 * @param  connection handle
 * @param  index
 * @retval None
 */

uint8_t verify_Conn_Handle (uint16_t handle, uint8_t *index){
    
uint8_t i;

 #ifdef MULTINODE  
      
      for (i=0; i < network.num_device_found; i++) {
        
        if(network.mMSConnection[i].Connection_Handle== handle);
         *index = i;
         return 1;   
       }
      
#else
      
      if (network.mMSConnection.Connection_Handle== handle) return 1;
      
      
#endif 
      
    return 0;  
}

/**********************EVENT_HANDLER_NETWORK_FUNCTIONS******************************/
/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  The connection handle
 * @retval None
 */

NET_Status NET_DisconnectionComplete_CB(uint16_t conn_handle){
  
uint8_t  index;
  if(device_role == DEVICE_CENTRAL || device_role == DEVICE_PHERISPHERAL){
#ifdef MULTINODE
      if(verify_Conn_Handle(conn_handle,&index)){
          /*if this event handler exist then*/
          network.mMSConnection[index].connection_status = ST_UNESTABLISHED;
          if(device_role == DEVICE_CENTRAL) network.mMSConnection[index].device_cstatus=DEVICE_DISCOVERY_MODE;
          if(device_role == DEVICE_PHERISPHERAL) network.mMSConnection[index].device_cstatus = DEVICE_ADVERTISEMENT_MODE;
      }
#else
      network.mMSConnection.connection_status = ST_UNESTABLISHED;
      if(device_role == DEVICE_CENTRAL)network.mMSConnection.device_cstatus = DEVICE_DISCOVERY_MODE;
      if(device_role == DEVICE_PHERISPHERAL) network.mMSConnection.device_cstatus = DEVICE_ADVERTISEMENT_MODE;
#endif    
  }
  
 return NET_SUCCESS; 
}

/**
 * @brief  This function is called when the connection to a peer device is complete.
 * @param  The connection handle
 * @retval None
 */
NET_Status NET_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle){
  
#ifdef MULTINODE
      uint8_t  index = network.num_device_found;
      network.mMSConnection[index].Connection_Handle = handle;
      network.mMSConnection[index].connection_status = ST_CONNECTED_WAIT_CHAR_DISC;
      if(device_role == DEVICE_CENTRAL)network.mMSConnection[index].device_cstatus = DEVICE_READY_TO_INTERCHANGE; 
      //COPY_VAR(network.mMSConnection[index].device_address)
#else
  
#endif   
}






