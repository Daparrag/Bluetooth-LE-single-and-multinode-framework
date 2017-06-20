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

/****************** Variable Declaration **************************/
net_type_t    net_mode =  NET_CONNECTED;/*the default connection mode*/
dv_type_t     device_role = DEVICE_CENTRAL; /*by default the devices will be cental*/
net_status_t  net_status = NET_NOT_INITIALIZED;
static uint32_t led_toggle_count = 0;

network_t network;
/*********************************************************/

/*********************Static func************************/
static void network_set_wait_end_procedure(void);
static void network_clean_wait_end_procedure(void);
static uint8_t network_get_wait_end_procedure(void);
static NET_Status network_process_central(event_t * event);
static NET_Status network_process_pherispheral(event_t * event);
static uint8_t validate_new_pherispheral_address(uint8_t *peer_address);
static void init_device(void);

static void init_service_handler(void);
static void init_service_handler(void);

static connection_t * NET_Get_currentConnection_CB(void);
static connection_t * NET_Connection_to_stablished_CB(void);
static connection_t * NET_get_connection_by_address_BLE(uint8_t * addrss);
static connection_t * NET_get_connection_to_service_Scan_BLE(void);
static connection_t * NET_get_connection_to_char_Scan_BLE(void);
static connection_t * NET_get_connection_by_status_CB(uint8_t _status);
static connection_t * NET_get_connection_by_chandler_BLE(uint16_t chandler);

/*led static adverticements*/

static void NET_Control_led_status_BLE(void);
static void connection_intermidle_toggle(void);
static void connection_unitialized_toggle(void);
static void connection_interchange_toggle(void);
static void connection_ready_toggle(void);


/**
  * @brief  This function is used for indicate the connection_stablished state.
  * @param void.
  * @retval void.
  */
void connection_intermidle_toggle(void){

if(led_toggle_count++ > LED_TOGGLE_INTERMIDLE)
	{
		led_toggle_count=0;
		BSP_LED_Toggle(LED2);
	}
}


/**
  * @brief  This function is used for indicate the connection_unestablished state.
  * @param void.
  * @retval void.
  */

void connection_unitialized_toggle(void){

if(led_toggle_count++ > LED_TOGGLE_UNITIALIZED)
	{
		led_toggle_count=0;
		BSP_LED_Toggle(LED2);
	}
}

/**
  * @brief  This function is used for indicate the connection_unestablished state.
  * @param void.
  * @retval void.
  */

void connection_interchange_toggle(void){

if(led_toggle_count++ > LED_TOGGLE_INTERCHANGE)
	{
		led_toggle_count=0;
		BSP_LED_Toggle(LED2);
	}
}


/**
  * @brief  This function is used for indicate the connection_unestablished state.
  * @param void.
  * @retval void.
  */

void connection_ready_toggle(void){

if(led_toggle_count++ > LED_TOGGLE_READY)
	{
		led_toggle_count=0;
		BSP_LED_Toggle(LED2);
	}
}




void network_set_wait_end_procedure(void ){

	network.flags.wait_end_procedure=1;
}

void network_clean_wait_end_procedure(void ){

	network.flags.wait_end_procedure=0;
}


 uint8_t network_get_wait_end_procedure(void ){

	return network.flags.wait_end_procedure;
}



NET_Status init_network(net_type_t net_type, dv_type_t device_type ,network_t ** net_output){

	network.flags.wait_end_procedure=0;
    network.flags.connection_stablishment_complete=0;
    network.flags.services_discovery_complete=0;
	net_mode = net_type;
	device_role = device_type;
	network.num_device_found=0;
	network.num_device_connected=0;
	init_device();
	init_service_handler();
	net_status = NET_INITALIZED;
	*net_output=&network;
	return NET_SUCCESS;
}


void init_device(void){

    dv_state_t device_init_config =  DEVICE_UNITIALIZED;

	if (device_role == DEVICE_CENTRAL)            device_init_config = DEVICE_DISCOVERY_MODE;
	if (device_role == DEVICE_PHERISPHERAL)       device_init_config = DEVICE_ADVERTISEMENT_MODE;
	if (device_role == DEVICE_BROADCASTER)        device_init_config = DEVICE_ADVERTISEMENT_MODE;
  	if (device_role == DEVICE_OBSERVER)             device_init_config = DEVICE_SCAN_MODE;
network.device_cstatus = device_init_config;

}


void init_service_handler(void){
	#ifdef MULTINODE
		uint8_t i;
		for(i=0; i < EXPECTED_NODES; i++)
			network.mMSConnection[i].service_status = ST_SERVICE_DISCOVERY;

	#else
			network.mMSConnection.service_status = ST_SERVICE_DISCOVERY;
	#endif
}

void init_connection_handler(void){
#ifdef MULTINODE
		uint8_t i;
		for(i=0; i < EXPECTED_NODES; i++)
			network.mMSConnection[i].connection_status = ST_UNESTABLISHED;
	#else
			network.mMSConnection.connection_status = ST_UNESTABLISHED;
	#endif
}


NET_Status network_process(event_t * event){
NET_Status ret;
	switch(device_role){
		case DEVICE_CENTRAL:
		{
			ret = network_process_central(event);
		}
		break;

		case DEVICE_PHERISPHERAL:
		{
			ret = network_process_pherispheral(event);
		}
		break;

	}

        NET_Control_led_status_BLE();
 
return ret;

}



void NET_Control_led_status_BLE(void){
  switch(network.device_cstatus)
  {
  case DEVICE_UNITIALIZED:
    {
      connection_unitialized_toggle();
    }
    break;
  case DEVICE_READY_TO_INTERCHANGE:
    {
      connection_interchange_toggle();
    }
    break;
    
  case DEVICE_READY:
    {
      connection_ready_toggle();
    }
    break;
  default:
    {
      connection_intermidle_toggle();
      break;
    }
  
  }
  
}


NET_Status network_process_central(event_t * event){/*we have to deal with the events maybe wr cant catch witout passing parameters*/

CHADLE_Status ch_ret;
SERV_Status   serv_ret;
connection_t * connection;

	if(device_role==DEVICE_CENTRAL){
		switch(network.device_cstatus){
			case DEVICE_UNITIALIZED:
			{
				 PRINTDEBUG("the devices is not correct initialized please check the network initialization\n");
				 return NET_ERROR;
			}
			break;

			case DEVICE_DISCOVERY_MODE:
			{
				if(event!=NULL){
					switch (event->event_type){
						case EVT_LE_ADVERTISING_REPORT:
						{
							connection = NET_Get_currentConnection_CB();
							if(connection!=NULL){
								le_advertising_info *pr = (le_advertising_info*) event->evt_data; 
                                                               if(!validate_new_pherispheral_address(pr->bdaddr))break;
                                                                ch_ret = CH_new_device_found_BLE(connection, (void *) pr);
                                                

                                                                if(ch_ret!=CHADLE_SUCCESS){
                                                                  PRINTDEBUG(" An Error occur during CH_new_device_found_BLE procedure please check the parameters\n");
                                                                  return NET_ERROR;
                                                                }

                                                            network.num_device_found+=1;

                                                            connection->connection_status=ST_CREATE_CONNECTION;
							}
                                                        
							if(network.num_device_found == EXPECTED_NODES){
                                                            network.device_cstatus=DEVICE_READY_TO_CONNECT;
                                                            network_clean_wait_end_procedure();
                                                        }
                                               
						}
						break;

					}
				
				}else if(network_get_wait_end_procedure()==0){
					 if(DISCOVERY_MODE == GENERAL_DISCOVERY) ch_ret= CH_run_discovery_BLE ();/*issue*/
            		 if(DISCOVERY_MODE == SELECTIVE_DISCOVERY) ch_ret = CH_run_selective_discovery_BLE();

            		 if(ch_ret!=CHADLE_SUCCESS){
            		 	PRINTDEBUG(" An Error occur during general-discovery-setup please check the configuration parameters\n");
            		 	return NET_ERROR;
            		 }

            		 network_set_wait_end_procedure();
				}
			}
			break;

			case DEVICE_READY_TO_CONNECT:
			{
				if(event!=NULL){
					switch (event->event_type){
						case EVT_LE_CONN_COMPLETE:
						{
					/*execute the connection complete procedure*/
							
                			evt_le_connection_complete *cc =  (void*) event->evt_data;

                			connection = NET_get_connection_by_address_BLE(cc->peer_bdaddr);/*issue*/
                			ch_ret = CH_Connection_Complete_BLE(connection,cc->handle,cc->peer_bdaddr);

                			if(ch_ret!=CHADLE_SUCCESS){
                 				  	PRINTDEBUG(" An Error occur during CH_Connection_Complete_BLE procedure please check the parameters\n");
                 				  	return NET_ERROR;
                 			}

                			network.num_device_connected+=1;
                			if(network.num_device_connected == network.num_device_found){
                				network.device_cstatus=DEVICE_READY_TO_INTERCHANGE;
                                                network.flags.connection_stablishment_complete=1;
                			}
                			connection->connection_status = ST_CONNECTED_WAIT_DISC;
                			network_clean_wait_end_procedure();


						}
						break;
					}
				}else if(network_get_wait_end_procedure()==0){
					connection=NET_get_connection_by_status_CB(ST_CREATE_CONNECTION);
					ch_ret = CH_run_create_connection_BLE(connection);

						if(ch_ret!=CHADLE_SUCCESS){
                 			PRINTDEBUG(" An Error occur during CH_run_create_connection_BLE procedure please check the parameters\n");
                 			return NET_ERROR;
                 		}

					network_set_wait_end_procedure();
				}

			}
			break;
			case DEVICE_READY_TO_INTERCHANGE:
			{
				if(event!=NULL){
                                  switch(event->event_type)
                                  {
                                  case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
                                    {
                                       evt_gatt_procedure_complete * pr =(evt_gatt_procedure_complete *) event->evt_data; 
                                       if(pr->error_code != BLE_STATUS_SUCCESS){
                                         PRINTDEBUG(" An Error: (0x%04x) occur in the discovery process please check all the paramters\n", pr->error_code);
                                         return NET_ERROR; 
                                          
                                       }
                                       
                                       connection = NET_get_connection_by_chandler_BLE(pr->conn_handle);
                                       
                                       if(connection==NULL){
                                          PRINTDEBUG("An Error occur in the discovery process because an invalid connection handler has been received please check it. \n");
                                          return NET_ERROR;
                                       }
                                       
                                      network_clean_wait_end_procedure(); 
                                       
                                       
                                    }
                                    break;
                                    
                                  }

				}else if((network_get_wait_end_procedure()==0) && (network.flags.connection_stablishment_complete!=0)){
					if(network.num_device_serv_discovery==network.num_device_connected) network.device_cstatus =  DEVICE_READY;
                                        connection = NET_get_connection_by_status_CB(ST_CONNECTED_WAIT_DISC);
                                        if((connection==NULL)){
                                        /*something is wrong*/
                                          return NET_ERROR;
                                        }
					switch(connection->service_status)
					{
						case ST_SERVICE_DISCOVERY:
						{
							if(DISC_SERVICE==FIND_SPE_SERVICE)serv_ret = DSCV_primary_services_by_uuid(connection);
                                                        
                                                       

							if(serv_ret!=SERV_SUCCESS){
							PRINTDEBUG(" error occur during the discovery services procedure DSCV_primary_services_by_uuid please check it \n");
							return NET_ERROR;
							}
                                                        
                                                        
                                                        if(connection->Node_profile->svflags.services_success_scanned==1){      
                                                            connection->service_status = ST_CHAR_DISCOVERY;
                                                            return NET_SUCCESS;
                                                        }
                                                        
                                                        
                                                          network_set_wait_end_procedure();

						}
						break;

						case ST_CHAR_DISCOVERY:
						{
                                                        if(DISC_CHAR==FIND_SPE_CHAR) serv_ret = DSCV_primary_char_by_uuid(connection);
                                                        
                                                        if(serv_ret!=SERV_SUCCESS)
                                                        {
                                                            PRINTDEBUG(" error occur during the discovery characterictics procedure DSCV_primary_char_by_uuid please check it \n");
                                                            return NET_ERROR;
                                                        }
                                                        
                                                         if((connection->Node_profile->svflags.attr_success_scanned==1) 
                                                            && (connection->Node_profile->svflags.services_success_scanned!=1))
                                                         {
                                                           /*this status is not possible something is wrong*/
                                                           PRINTDEBUG(" Error, It is not possible to scan atributes witout first finish the scanning services please check it\n");
                                                           return NET_ERROR;
                                                         }else if ((connection->Node_profile->svflags.attr_success_scanned==1) 
                                                                   && (connection->Node_profile->svflags.services_success_scanned==1))
                                                          {
                                                                   network.num_device_serv_discovery+=1;
                                                                   connection->connection_status =  ST_STABLISHED;
                                                                   return  NET_SUCCESS;
                                                          }
                                                        
                                                        
                                                        network_set_wait_end_procedure();
                                                        
						}
						break;

					}
				}
			}
			break;
                        case DEVICE_READY:
                        {
                          
                        }
                        break;

		}

	}else{
		PRINTDEBUG(" Error: This network process is not according with your device role please check it\n");
		return NET_ERROR;
	}
        

return NET_SUCCESS;
}



NET_Status network_process_pherispheral(event_t * event){/*we have to deal with the events maybe wr cant catch witout passing parameters*/
CHADLE_Status ch_ret;
SERV_Status   serv_ret;
connection_t * connection;

		if(device_role == DEVICE_PHERISPHERAL){
			switch(network.device_cstatus){
				case DEVICE_UNITIALIZED:
				{
				 	PRINTDEBUG("the devices is not correct initialized please check the network initialization\n");
				 	return NET_ERROR;

				}
				break;
                        case DEVICE_ADVERTISEMENT_MODE:
                          {
					if(event!=NULL){
						case EVT_LE_CONN_COMPLETE:
						{
							connection=NET_get_connection_by_status_CB(ST_UNESTABLISHED);
							evt_le_connection_complete *cc =  (void*) event->evt_data;
							ch_ret = CH_Connection_Complete_perispheral_BLE(connection,cc->handle,cc->peer_bdaddr );
							
							if(ch_ret != CHADLE_SUCCESS){
								PRINTDEBUG(" An Error occur during CH_Connection_Complete_perispheral_BLE procedure please check the parameters\n");
                 				return NET_ERROR;
							}

							network.num_device_connected+=1;

							if(network.num_device_connected == EXPECTED_CENTRAL_NODES){
								network.device_cstatus = DEVICE_READY_TO_INTERCHANGE;
                                                                network.flags.connection_stablishment_complete=1;
								network_clean_wait_end_procedure();
							}

							connection->connection_status=ST_CONNECTED_WAIT_DISC;
							
						}
						break;
					}else if(network_get_wait_end_procedure()==0){
						if(ADVERTICEMENT_MODE == GENERAL_ADVERTICEMENT) ch_ret=CH_run_advertise_BLE(); 

						if(ch_ret!=CHADLE_SUCCESS){
							PRINTDEBUG(" An Error occur during the adverticement setup procedure please check the parameters\n");
							return NET_ERROR;

						}
						network_set_wait_end_procedure();

					}

				}
				break;
				case DEVICE_READY_TO_INTERCHANGE:
				{
                                  if(event!=NULL){
                                    switch(event->event_type)
                                     {
                                     case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
                                       {
                                          evt_gatt_procedure_complete * pr =(evt_gatt_procedure_complete *) event->evt_data; 
                                          if(pr->error_code != BLE_STATUS_SUCCESS){
                                              PRINTDEBUG(" An Error: (0x%04x) occur in the discovery process please check all the paramters\n", pr->error_code);
                                              return NET_ERROR; 
                                          }
                                          
                                          connection = NET_get_connection_by_chandler_BLE(pr->conn_handle);
                                          
                                          if(connection==NULL){
                                              PRINTDEBUG("An Error occur in the discovery process because an invalid connection handler has been received please check it. \n");
                                              return NET_ERROR;
                                          }
                                       
                                            network_clean_wait_end_procedure(); 
                                          
                                          
                                       }
                                       break;
                                  
                                     }
                                  }else if((network_get_wait_end_procedure()==0) && (network.flags.connection_stablishment_complete!=0)){
                                        
                                        if(network.num_device_serv_discovery==network.num_device_connected) network.device_cstatus =  DEVICE_READY;
                                        connection = NET_get_connection_by_status_CB(ST_CONNECTED_WAIT_DISC);
                                    
                                    
                                        if((connection==NULL)){
                                            /*something is wrong*/
                                              return NET_ERROR;
                                        }
                                        
                                        switch(connection->service_status)
                                        {
                                          case ST_SERVICE_DISCOVERY:
                                            {
                                              if(DISC_SERVICE==FIND_SPE_SERVICE)serv_ret = DSCV_primary_services_by_uuid(connection);
                                              
                                              
                                              if(serv_ret!=SERV_SUCCESS){
						PRINTDEBUG(" error occur during the discovery services procedure DSCV_primary_services_by_uuid please check it \n");
						return NET_ERROR;
                                              }
                                              
                                              if(connection->Node_profile->svflags.services_success_scanned==1){      
                                                    connection->service_status = ST_CHAR_DISCOVERY;
                                                    return NET_SUCCESS;
                                                }
                                              
                                               network_set_wait_end_procedure();
                                            }
                                            break;
                                           case ST_CHAR_DISCOVERY:
                                            {
                                                 if(DISC_CHAR==FIND_SPE_CHAR) serv_ret = DSCV_primary_char_by_uuid(connection);
                                                 
                                                 if(serv_ret!=SERV_SUCCESS)
                                                        {
                                                            PRINTDEBUG(" error occur during the discovery characterictics procedure DSCV_primary_char_by_uuid please check it \n");
                                                            return NET_ERROR;
                                                        }
                                                 
                                                  if((connection->Node_profile->svflags.attr_success_scanned==1) 
                                                            && (connection->Node_profile->svflags.services_success_scanned!=1))
                                                         {
                                                           /*this status is not possible something is wrong*/
                                                           PRINTDEBUG(" Error, It is not possible to scan atributes witout first finish the scanning services please check it\n");
                                                           return NET_ERROR;
                                                         }else if((connection->Node_profile->svflags.attr_success_scanned==1) 
                                                                   && (connection->Node_profile->svflags.services_success_scanned==1))
                                                         {
                                                                network.num_device_serv_discovery+=1;
                                                                connection->connection_status =  ST_STABLISHED;
                                                                return  NET_SUCCESS;
                                                         }
                                                 
                                                  network_set_wait_end_procedure();
                                                 
                                                 
                                            }
                                            break;
                                            
                                        
                                        }
                                    
                                  }
                                }
				break;
                                case DEVICE_READY:
                                {
                          
                                }
                                break;
			}

		}else{

			PRINTDEBUG(" Error: This network process is not according with your device role please check it\n");
			return NET_ERROR;
		}
return NET_SUCCESS;
}



connection_t * NET_Get_currentConnection_CB(void){
	connection_t * connection = NULL;
#ifdef MULTINODE
	uint8_t index = network.num_device_found;
	uint8_t connection_status =  network.mMSConnection[index].connection_status;
	if(connection_status==ST_UNESTABLISHED) connection = &network.mMSConnection[index];
	
#else
	uint8_t connection_status =  network.mMSConnection.connection_status;
	if(connection_status==ST_UNESTABLISHED)connection = &network.mMSConnection;

#endif	
 return connection;	
}


connection_t *  NET_Connection_to_stablished_CB(void){
	connection_t * connection = NULL;
	#ifdef MULTINODE
	uint8_t i;
	for (i=0; i < network.num_device_found; i++ ){
		uint8_t connection_status =  network.mMSConnection[i].connection_status;
		if(connection_status==ST_CREATE_CONNECTION)
		{
			connection = &network.mMSConnection[i]; 
			break;
		}
	}
	#else
		uint8_t connection_status =  network.mMSConnection.connection_status;
		if(connection_status==ST_CREATE_CONNECTION)connection = &network.mMSConnection;

	#endif	

 return connection;	
}

connection_t * NET_get_connection_by_status_CB(uint8_t _status){
	connection_t * connection = NULL;
	#ifdef MULTINODE
        uint8_t i;
	for (i=0; i < network.num_device_found; i++ ){
		uint8_t connection_status =  network.mMSConnection[i].connection_status;
		if(connection_status == _status){
			connection = &network.mMSConnection[i];
			break; 
		}
	}
	#else
	uint8_t connection_status =  network.mMSConnection.connection_status;
	if(connection_status==_status)connection = &network.mMSConnection;
	#endif
return 	connection;
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

if(list_index_size-1 >= EXPECTED_NODES || list_index== NULL){
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



uint8_t validate_new_pherispheral_address(uint8_t *peer_address){
	
	uint8_t i;
	uint8_t j;
    uint8_t address_valid=1;
#ifdef MULTINODE

	for (i=0; i < network.num_device_found; i ++){
			address_valid=0;
			uint8_t * pr = network.mMSConnection[i].device_address;
			uint8_t * pcomp = peer_address;
		for(j=0; j < DEVICE_ADDRS_LENGTH ; j++){
			uint8_t local_adds = *pr++;
			uint8_t remote_adds = *pcomp++; 
			if(local_adds!=remote_adds){
				address_valid=1;
                break;
			}
		}
	}
#endif        

return address_valid;
}


connection_t * NET_get_connection_by_address_BLE(uint8_t * peer_address){


	#ifdef MULTINODE
	uint8_t i;
	uint8_t j;
			for (i=0; i < network.num_device_found; i ++){
				uint8_t address_match=1;
				uint8_t * pr = network.mMSConnection[i].device_address;
				uint8_t * pcomp = peer_address;
				for(j=0; j < DEVICE_ADDRS_LENGTH ; j++){
					uint8_t local_adds = *pr++;
					uint8_t remote_adds = *pcomp++;
					if(local_adds!=remote_adds){
						address_match=0;
						break;
					}	
				}
				if(address_match){
					return &network.mMSConnection[i];
				}

			}


	#else
		uint8_t i;
		uint8_t address_match=1;
		uint8_t * pr = network.mMSConnection.device_address;
		uint8_t * pcomp = peer_address;
		for(i=0; i < DEVICE_ADDRS_LENGTH ; i++){
			uint8_t local_adds = *pr++;
			uint8_t remote_adds = *pcomp++;
			if(local_adds!=remote_adds){
			address_match=0;
			break;
			}

		}

		if(address_match)
			return  &network.mMSConnection;
	#endif
return NULL;

}

connection_t * NET_get_connection_by_chandler_BLE(uint16_t chandler)
{
  connection_t * connection = NULL;
  uint16_t C_Handle;
  
  #ifdef MULTINODE
    uint8_t i;
    for (i = 0; i < network.num_device_connected; i ++ )
    {
        C_Handle = network.mMSConnection[i].Connection_Handle;
        if(C_Handle == chandler ){
          connection = &network.mMSConnection[i];
          break;
        }
    }
  #else
      C_Handle = network.mMSConnection.Connection_Handle;
    if(C_Handle == chandler)connection = &network.mMSConnection;
  #endif
return connection;
}

