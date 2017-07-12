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
uint8_t reconnection_tries;
static uint32_t led_toggle_count = 0;

network_t network;
/*********************************************************/

/*********************Static func************************/

/************************ NETWORK HANDLER FUNCTIONS ********************/
static void network_set_wait_end_procedure(void); /*fire when the network module has to wait for ending a process*/
static void network_clean_wait_end_procedure(void);/*fire when the network module has finished a process*/
static uint8_t network_get_wait_end_procedure(void);/*get the status of the network 1=wait for ending a process, 0=process finished */
static NET_Status network_process_conn_oriented(event_t * event);/*handler the network as a connection oriented*/
static uint8_t validate_new_pherispheral_address(uint8_t *peer_address);/*  Used to verify that this is efectivelly a new device*/
static void init_device(void);/*initialized the device for an specific network type*/
static void init_service_handler(void); /* Used to initialized the event handler states for an specific network type*/                 

static void reset_profile_flags(app_profile_t * profile); /* Used to clean the status flags */


static connection_t * NET_Get_currentConnection_CB(void); /* retreave the current connection */

static connection_t * NET_get_connection_by_address_BLE(uint8_t * addrss); /* retreave a connection characterized by and specific address */





/************************ NETWORK  LED ADVERTISEMENT STATUS  FUNCTIONS ********************/

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
  * @brief  This function is used for indicate the connection status as interchange charactersitics and services state.
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
  * @brief  This function is used for indicate the connection_ready to transmit data state.
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


/**
  * @brief  This function set the network for wait until a process finish.
  * @param void.
  * @retval void.
  */
void network_set_wait_end_procedure(void ){

	network.flags.wait_end_procedure=1;
}

/**
  * @brief  This function is used for clean the network flag wait for an process to finish .
  * @param void.
  * @retval void.
  */
void network_clean_wait_end_procedure(void ){

	network.flags.wait_end_procedure=0;
}

/**
  * @brief  This function is used for retrieve the status of the network wait or not for and end process.
  * @param void.
  * @retval void.
  */
 uint8_t network_get_wait_end_procedure(void ){

	return network.flags.wait_end_procedure;
}

/**
  * @brief  This function is used for inform the general status of the connection.
  * @param void.
  * @retval void.
  */ 
        
uint8_t network_get_status(void){

	if(network.device_cstatus==DEVICE_READY) return 1;
        return 0;
}

   

/**
  * @brief  This function Initialize the network module.
  * @param net_type_t net_type: Network is connected or broadcast.
  * @param dv_type_t device_type: Central, Perisperal,broadcaster,or observer.
  * @param uint8_t num_reconnection: Number of times for retry a connection.
  * @param network_t ** net_output: Pointer to the network configuration.
  * @retval NET_Status: Value indicating success or error code
  */
NET_Status init_network(net_type_t net_type, 
                        dv_type_t device_type,
                        uint8_t num_reconnection, 
                        network_t ** net_output)
{

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
        reconnection_tries = num_reconnection;
return NET_SUCCESS;
}


/**
  * @brief  This function Initialize the device according to his role.
  * @param  none
  * @retval none
  */
void init_device(void){

    dv_state_t device_init_config; 

    switch (device_role)
    {
      case DEVICE_CENTRAL:
      {
        device_init_config = DEVICE_DISCOVERY_MODE;
        
      }
      break;
      case DEVICE_PERISPHERAL:
      {
        device_init_config = DEVICE_ADVERTISEMENT_MODE;
      }
      break;
      case DEVICE_BROADCASTER:
      {
        device_init_config = DEVICE_ADVERTISEMENT_MODE;
      }
      break;
      case DEVICE_OBSERVER:
      {
        device_init_config = DEVICE_SCAN_MODE;
      }
      break;
    default:
     device_init_config =  DEVICE_UNITIALIZED;
     break;
      
    }
    
network.device_cstatus = device_init_config;

}

/**
  * @brief  Initialize the service handler status.
  * @param  none
  * @retval none
  */
void init_service_handler(void){
	#ifdef MULTINODE
		uint8_t i;
		for(i=0; i < EXPECTED_NODES; i++){
			network.mMSConnection[i].service_status = ST_SERVICE_DISCOVERY;
                        network.mMSConnection[i].sconfig.char_disc_mode = DISC_CHAR;
                        network.mMSConnection[i].sconfig.serv_disc_mode = DISC_SERVICE; 
                }

	#else
			network.mMSConnection.service_status = ST_SERVICE_DISCOVERY;
                        network.mMSConnection.sconfig.char_disc_mode = DISC_CHAR;
                        network.mMSConnection.sconfig.serv_disc_mode = DISC_SERVICE; 
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



/**
  * @brief  handler the network according to the role of the device and the type of event.
  * @param  event_t * event: specific event
  * @retval none
  */
NET_Status network_process(/*event_t * event*/){
event_t * event;
  if(network.device_cstatus!=DEVICE_READY) event = (event_t *)HCI_Get_Event_CB();
  else  event = NULL;
NET_Status ret;
	switch(device_role){
		case DEVICE_CENTRAL:
		{
			ret = network_process_conn_oriented(event);
                        if(ret != NET_SUCCESS) while(1);
		}
		break;

		case DEVICE_PERISPHERAL:
		{
			ret = network_process_conn_oriented(event);
		}
		break;

	}

        NET_Control_led_status_BLE();
 
return ret;

}


/**
  * @brief  advertise the device status .
  * @param  none
  * @retval none
  */
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

/**
  * @brief  handler a central - multiperispheral nodes connection .
  * @param  event_t * event: specific event
  * @retval NET_Status: Value indicating success or error code
  */
NET_Status network_process_conn_oriented(event_t * event){/*we have to deal with the events maybe wr cant catch witout passing parameters*/

CHADLE_Status ch_ret;
SERV_Status   serv_ret;
connection_t * connection;
 
	switch(network.device_cstatus)
	{
			case DEVICE_UNITIALIZED:
			{
			 	PRINTDEBUG("the devices is not correct initialized please check the network initialization\n");
			 	return NET_ERROR;
			}
			break;
			
			case DEVICE_DISCOVERY_MODE:
			{
				if(device_role!=DEVICE_CENTRAL)
				{
					PRINTDEBUG("the devices is not correct initialized please check the network initialization\n");
					return NET_ERROR;
				}
				
				if(event!=NULL)
				{
					switch (event->event_type){
					
						case EVT_LE_ADVERTISING_REPORT:
						{
                                                  
                                                        PRINTDEBUG("event_received EVT_LE_ADVERTISING_REPORT at time: %d \n", event->ISR_timestamp);
							connection = NET_Get_currentConnection_CB();
							if(connection!=NULL)
							{
								le_advertising_info *pr = (le_advertising_info*) event->evt_data; 
								if(!validate_new_pherispheral_address(pr->bdaddr))break;
								
								ch_ret = CH_new_device_found_BLE(connection, (void *) pr);
								
								if(ch_ret!=CHADLE_SUCCESS)
								{
									PRINTDEBUG(" An Error occur during CH_new_device_found_BLE procedure please check the parameters\n");
									return NET_ERROR;
								}
								
								network.num_device_found+=1;
								
								connection->connection_status=ST_CREATE_CONNECTION;
							}
							
							if(network.num_device_found == EXPECTED_NODES)
							{
								network.device_cstatus=DEVICE_READY_TO_CONNECT;
								network_clean_wait_end_procedure();
							}
						}
						break;
                                                
                                                default:
                                                break;	
					}
			
				}else if(network_get_wait_end_procedure()==0)
				{
					if(DISCOVERY_MODE == GENERAL_DISCOVERY) ch_ret= CH_run_discovery_BLE ();/*issue*/
					if(DISCOVERY_MODE == SELECTIVE_DISCOVERY) ch_ret = CH_run_selective_discovery_BLE();
					
					if(ch_ret!=CHADLE_SUCCESS)
					{
						PRINTDEBUG(" An Error occur during general-discovery-setup please check the configuration parameters\n");
						return NET_ERROR;
					}
                                        
					Timer_Set(&network.time_alive, CLOCK_SECOND * 36);
					network_set_wait_end_procedure();
				}else if(network_get_wait_end_procedure()==1)
                                {
                                  if(Timer_Expired(&network.time_alive)){
                                    PRINTDEBUG("The device was not able to identify any perispheral:\n");
                                    
                                    if(network.num_device_found!=0){
                                      PRINTDEBUG("the device will try to set up a connection with %d pherispherals\n",network.num_device_found);
                                      network.device_cstatus=DEVICE_READY_TO_CONNECT;
                                    }else{
                                      PRINTDEBUG("the device will restart the scanning of perispherals\n");
                                      Timer_Set(&network.time_alive, CLOCK_SECOND * 36);
                                      network_set_wait_end_procedure();
                                      return NET_SUCCESS;
                                    }
                                    
                                    if(network.num_device_found!=0)
                                      network_clean_wait_end_procedure();
                                  }
                                }
			}
			break;
		
			case DEVICE_ADVERTISEMENT_MODE:
			{
				if(device_role!=DEVICE_PERISPHERAL){
					PRINTDEBUG("the devices is not correct initialized please check the network initialization\n");
					return NET_ERROR;
				}
				
				if(event!=NULL){
					switch (event->event_type)
					{			
						case EVT_LE_CONN_COMPLETE:
						{
                                                        PRINTDEBUG("event_received EVT_LE_CONN_COMPLETE at time: %d \n", event->ISR_timestamp);
							connection=NET_get_connection_by_status_CB(ST_UNESTABLISHED);
							evt_le_connection_complete *cc =  (void*) event->evt_data;
							ch_ret = CH_Connection_Complete_perispheral_BLE(connection,cc->handle,cc->peer_bdaddr );
						
							if(ch_ret != CHADLE_SUCCESS)
							{
								PRINTDEBUG(" An Error occur during CH_Connection_Complete_perispheral_BLE procedure please check the parameters\n");
								return NET_ERROR;
							}
						
							network.num_device_connected+=1;
				
							if(network.num_device_connected == EXPECTED_CENTRAL_NODES)
							{
								network.device_cstatus = DEVICE_READY_TO_INTERCHANGE;
								network.flags.connection_stablishment_complete=1;
								network_clean_wait_end_procedure();
							}
						
							connection->connection_status=ST_CONNECTED_WAIT_DISC;
						}
						break;
                                                default:
                                                break;
                                                
					}
				}else if(network_get_wait_end_procedure()==0)
				{
					if(ADVERTISEMENT_MODE == GENERAL_ADVERTISEMENT) ch_ret=CH_run_advertise_BLE(); 
				
					if(ch_ret!=CHADLE_SUCCESS)
					{
						PRINTDEBUG(" An Error occur during the adverticement setup procedure please check the parameters\n");
						return NET_ERROR;
					
					}
					network_set_wait_end_procedure();
					
				}
			
		}
		break;
		case DEVICE_READY_TO_CONNECT:
		{
			if(event!=NULL)
			{
				switch (event->event_type)
				{
					case EVT_LE_CONN_COMPLETE:
					{
						/*execute the connection complete procedure*/
                                                PRINTDEBUG("event_received EVT_LE_CONN_COMPLETE at time: %d \n", event->ISR_timestamp);
						evt_le_connection_complete *cc =  (void*) event->evt_data;
					
						connection = NET_get_connection_by_address_BLE(cc->peer_bdaddr);/*issue*/
                                                if(connection==NULL) 
                                                {
                                                  /*this is not and interesting address for us since this device has not part of the network*/
                                                  //network_clean_wait_end_procedure();
                                                  return NET_SUCCESS;
                                                } 
						ch_ret = CH_Connection_Complete_BLE(connection,cc->handle,cc->peer_bdaddr);
					
						if(ch_ret!=CHADLE_SUCCESS)
						{
							PRINTDEBUG(" An Error occur during CH_Connection_Complete_BLE procedure please check the parameters\n");
							return NET_ERROR;
						}
					
						network.num_device_connected+=1;
						if(network.num_device_connected == network.num_device_found)
						{
							network.device_cstatus=DEVICE_READY_TO_INTERCHANGE;
							network.flags.connection_stablishment_complete=1;
						}
						connection->connection_status = ST_CONNECTED_WAIT_DISC;
						network_clean_wait_end_procedure();
					
					
					}
					break;
                                default:
                                break; 
				}
			}else if(network_get_wait_end_procedure()==0)
			{
				connection=NET_get_connection_by_status_CB(ST_CREATE_CONNECTION);
                                if(connection==NULL)
                                {
                                  /*that means that the coonection state has been completed*/
                                  if(network.num_device_connected <= 0 && reconnection_tries > 0)
                                  {
                                    PRINTDEBUG(" Was imposible to connect with any device \n");
                                    PRINTDEBUG(" Now the device will start the reconnection procedure \n");
                                    
                                  }else if(network.num_device_connected < network.num_device_found && reconnection_tries > 0)
                                  {
                                    PRINTDEBUG(" Was imposible to connect with %d devices \n", (network.num_device_found - network.num_device_connected));
                                    PRINTDEBUG(" Now the device will start the reconnection procedure \n");
                                  }else if(network.num_device_connected == network.num_device_found)
                                  {
                                    /*something was wrong at this point, this is an impossible state*/
                                     return NET_ERROR;
                                  }
                                  
                                  /*device is trying to reconnect*/
                                  {
                                   
                                    if(reconnection_tries<=0)
                                    {
                                      /*check it that there is at least one perispheral connected*/
                                      if(network.num_device_connected > 0)
                                      {
                                        network.device_cstatus = DEVICE_READY_TO_INTERCHANGE;
                                        
                                        do{
                                          connection = NET_get_connection_by_status_CB(ST_TIME_OUT);
                                          if(connection!=NULL) connection->connection_status = ST_CONNECTION_LOST;   
                                        }while(connection!=NULL);
                                        PRINTDEBUG(" Was imposible to connect with %d devices \n", (network.num_device_found - network.num_device_connected));
                                        PRINTDEBUG(" Now the device will start the service_discovery_procedure \n");
                                        network.flags.connection_stablishment_complete=1;
                                        return NET_SUCCESS;
                                      }
                                      PRINTDEBUG(" Was imposible to connect with any device \n");
                                      return NET_ERROR;
                                    }
                                    
                                    connection=NET_get_connection_by_status_CB(ST_TIME_OUT);
                                    
                                    if(connection==NULL)return NET_ERROR; /*something was wrong*/
                                     reconnection_tries-=1;
                                    
                                 }
                                  
                              }
				if(CONNECTION_MODE == GENERAL_CONNECTION) ch_ret = CH_run_create_connection_BLE(connection);/*improviment point*/
			
				if(ch_ret!=CHADLE_SUCCESS)
				{
					PRINTDEBUG(" An Error occur during CH_run_create_connection_BLE procedure please check the parameters\n");
					return NET_ERROR;
				}
                                
                                Timer_Set(&network.time_alive, CLOCK_SECOND * 60);
				network_set_wait_end_procedure();
                                
			}else if(network_get_wait_end_procedure()==1)
                        {
                          if(Timer_Expired(&network.time_alive)){
                              if(CONNECTION_MODE == GENERAL_CONNECTION) ch_ret = CH_finish_the_connection_BLE();
                              if(ch_ret!=CHADLE_SUCCESS)
                              {
                                PRINTDEBUG(" An Error occur clossing the connection please verify\n");
                                return NET_ERROR;
                              }
                              connection=NET_get_connection_by_status_CB(ST_CREATE_CONNECTION);
                              if(connection!=NULL) connection->connection_status = ST_TIME_OUT;
                              network_clean_wait_end_procedure();     
                          }
                        }
		
		}
		break;
		case DEVICE_READY_TO_INTERCHANGE:
		{
			if(event!=NULL)
			{
				switch(event->event_type)
				{
					case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
					{     
                                                PRINTDEBUG("event_received EVT_BLUE_GATT_PROCEDURE_COMPLETE at time: %d \n", event->ISR_timestamp);
						evt_gatt_procedure_complete * pr =(evt_gatt_procedure_complete *) event->evt_data; 
                                                
						if(pr->error_code != BLE_STATUS_SUCCESS)
						{
							PRINTDEBUG(" An Error: (0x%04x) occur in the discovery process please check all the paramters\n", pr->error_code);
							return NET_ERROR; 
						}
						
						connection = NET_get_connection_by_chandler_BLE(pr->conn_handle);
                      
						if(connection==NULL)
						{
							PRINTDEBUG("An Error occur in the interchange process because an invalid connection handler has been received please check it. \n");
							return NET_ERROR;
						}
						
						network_clean_wait_end_procedure(); 
						
					}
					break;
                                        case EVT_BLUE_GATT_DISC_READ_CHAR_BY_UUID_RESP:
                                        {
                                             PRINTDEBUG("event_received EVT_BLUE_GATT_DISC_READ_CHAR_BY_UUID_RESP at time: %d (ms)\n", event->ISR_timestamp);
                                             evt_gatt_disc_read_char_by_uuid_resp * resp = (evt_gatt_disc_read_char_by_uuid_resp *)event->evt_data;
                                             connection = NET_get_connection_by_chandler_BLE(resp->conn_handle);
                                             
                                             if(connection==NULL){
                                                PRINTDEBUG("An Error occur in the interchange attribute process because an invalid connection handler has been received please check it. \n");
                                                 return NET_ERROR;
                                             }
                                             serv_ret = SH_Associate_att_handler_CB(connection,resp->attr_handle);
                                             
                                             //network_clean_wait_end_procedure();
                                             
                                        }
                                        break;
                                        case EVT_BLUE_ATT_FIND_BY_TYPE_VAL_RESP:
                                        {
                                          PRINTDEBUG("event_received EVT_BLUE_ATT_FIND_BY_TYPE_VAL_RESP at time: %d (ms)\n", event->ISR_timestamp);
                                        }
                                        break;
                                        default:
                                        break;
				}
			}else if((network_get_wait_end_procedure()==0) 
					  && (network.flags.connection_stablishment_complete!=0))
			{
				if( (network.num_device_serv_discovery==network.num_device_connected))
				{
					network.device_cstatus =  DEVICE_READY;
					return NET_SUCCESS;
				}
				
				connection = NET_get_connection_by_status_CB(ST_CONNECTED_WAIT_DISC);
				
				if((connection==NULL))
				{
					/*something is wrong*/
					return NET_ERROR;
				}else if( (connection->Node_profile->n_service > 0 && connection->Node_profile->services==NULL) || (connection->Node_profile==NULL))
				{
					PRINTDEBUG("Error: there is not a correct profile service association.\n Please verfy that you set up correctly the service and profile to this connection \n using the functions APP_add_BLE_Service, APP_add_BLE_attr, and net_setup_profile_definition \n");
					return NET_ERROR;                        
				}
				
				switch(connection->service_status)
				{
					case ST_SERVICE_DISCOVERY:
					{
                                                
                                            /*idealy this  has to be by connection not as a general flag configuration*/
						if(connection->sconfig.serv_disc_mode == FIND_SPE_SERVICE)
                                                {
                                                  serv_ret = DSCV_primary_services_by_uuid(connection);
                                                  
                                                }else if(connection->sconfig.serv_disc_mode == DONT_FIND_SERVICE){
                                                  /*This is used in case in which the devices is only a server*/
                                                  connection->Node_profile->svflags.services_success_scanned=1;
                                                  connection->service_status = ST_CHAR_DISCOVERY;
                                                  return NET_SUCCESS;
                                                }
						
						if(serv_ret!=SERV_SUCCESS)
						{
							PRINTDEBUG(" error occur during the discovery services procedure DSCV_primary_services_by_uuid please check it \n");
							return NET_ERROR;
						}
						
						if(connection->Node_profile->svflags.services_success_scanned==1)
						{
							connection->service_status = ST_CHAR_DISCOVERY;
							return NET_SUCCESS;
						}
						
						network_set_wait_end_procedure();
					}
					break;
					
					case ST_CHAR_DISCOVERY:
					{
                                          
                                              /*idealy this  has to be by connection not as a general flag configuration*/  
                                              if(connection->sconfig.char_disc_mode==FIND_SPE_CHAR){
                                                  serv_ret = DSCV_primary_char_by_uuid(connection);
                                                  
                                              }else if (connection->sconfig.char_disc_mode==DONT_FIND_CHAR){
                                                connection->Node_profile->svflags.attr_success_scanned=1;
                                                serv_ret=SERV_SUCCESS;
                                              }
						
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
							connection->connection_status = ST_STABLISHED;    
							reset_profile_flags(connection->Node_profile);
                                                        return NET_SUCCESS;
						}
						
						network_set_wait_end_procedure();
					}
					break;
                                default:
                                break;  	
				}
			}
		}
		break;
		case DEVICE_READY:
		{       

                                       
                }/*end ready*/
                break;
                default:
                break;
          }
return NET_SUCCESS;       
}


/**
  * @brief  resent the control flags for initialize a new service-attribute scanning .
  * @param  app_profile_t * profile: profile to clean
  * @retval none
  */
void reset_profile_flags(app_profile_t * profile){
  uint8_t i;
  app_service_t * service;
  uint8_t nservices;
  
  nservices=profile->n_service;
  profile->svflags.attr_success_scanned=0;
  profile->svflags.services_success_scanned=0;
  profile->svflags.services_to_find = nservices;
  
  service = profile->services;
  
  for(i=0;i < nservices; i++){
    if(service==NULL) break;
        service->chrflags.char_discovery_success=0;
        service->chrflags.char_scanned=0;
        service = service->next_service;
  }  
}

/**
  * @brief  retreive one connection in status ST_UNESTABLISHED 
  * @param  none
  * @retval connection_t *: Pointer to the retrieved connection. 
  */

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


/**
  * @brief  retrieve one connection in status a specific status 
  * @param  uint8_t _status: status of the connection to retrieve
  * @retval connection_t *: Pointer to the retrieved connection. 
  */
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


/**
  * @brief  define a specific profile for a list of connections
  * @param  app_profile_t * profile_def: Profile to define
  * @param   uint8_t * list_index: list of connections
  * @retval NET_Status: success or error code. 
  */
/*setup a profile for an specific connection*/
NET_Status net_setup_profile_definition(app_profile_t * profile_def, 
					uint8_t * list_index, 
				        size_t list_index_size){

/*check the input*/
if(profile_def==NULL){

	PRINTF("net_setup_profile_definition: input profile_def could not be NULL\n");
	return NET_ERROR;
}

#ifdef MULTINODE
uint8_t i;
uint8_t index;
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

/**
  * @brief  define a specific services_handler configuration used for setup the device as a client and/or server
  * @param  uint8_t serv_disc_mode: define how the services will be discovery.
  * @param  uint8_t char_disc_mode: define how the attributes will be discovery.
  * @param  uint8_t * list_index: list of connections
  * @param  size_t list_index_size: size of the lists of connections
  * @retval NET_Status: success or error code. 
  */
/*setup a service_handler configuration for an specific connection*/
NET_Status service_handler_config(uint8_t serv_disc_mode, 
                                  uint8_t char_disc_mode,
                                  uint8_t * connection_index, 
				  size_t connection_index_size){
                                                        
#ifdef MULTINODE
uint8_t i;
uint8_t index;    
      if(connection_index_size-1 >= EXPECTED_NODES || connection_index== NULL){
            PRINTF("error during service_handler_config: wrong imput parameters\n");
            return NET_ERROR;
      }
      
      for(i=0; i < connection_index_size; i++){
          index = *connection_index++;
          network.mMSConnection[index].sconfig.char_disc_mode=char_disc_mode;
          network.mMSConnection[index].sconfig.serv_disc_mode=serv_disc_mode;
      }      
#else
     network.mMSConnection.sconfig.char_disc_mode=char_disc_mode;
     network.mMSConnection.sconfig.serv_disc_mode=serv_disc_mode;
#endif
                                    
                                    
 
/*setup_ service handler configuration  ok*/
return NET_SUCCESS;
}

/**
  * @brief  define a specific connection configuration  for a list of connections
  * @param  config_connection_t * config: connection to configure
  * @param   uint8_t * list_index: list of connections
  * @retval NET_Status: sucess or error code. 
  */
/*setup a connection configuration for an specific list of connection*/
NET_Status net_setup_connection_config(config_connection_t * config, 
					uint8_t * list_index, 
                                        size_t list_index_size){

/*check the input*/
if(config==NULL){

	PRINTF("input config could not be NULL \n");
	return NET_ERROR;

}

#ifdef MULTINODE
uint8_t i;
uint8_t index;
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

/**
  * @brief  check if it is a valid perispheral address
  * @param  uint8_t *peer_address: pointer to the peer address
  * @retval uint8_t: address_valid=1,address_not_valid=0. 
  */

uint8_t validate_new_pherispheral_address(uint8_t *peer_address){
	

    uint8_t address_valid=1;
#ifdef MULTINODE
    	uint8_t i;
	uint8_t j;

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

/**
  * @brief  retreve a connection by address
  * @param  uint8_t *peer_address: connection address to retrieve
  * @retval connection_t *: pointer to the valid address connection . 
  */
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

/**
  * @brief  retreve a connection by connection handler
  * @param  uint16_t chandler: connection handler to search
  * @retval connection_t *: pointer to the valid connnection handler connection . 
  */

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


void NET_get_service_and_attributes_by_chandler_BLE(uint16_t chandler, uint16_t attrhandler, app_service_t ** serv, app_attr_t ** att){/* retreave a connection characterized by and specific connection handler and atribute handler associated*/
connection_t * connection = NULL;
 uint8_t i;
 uint8_t j;
 uint8_t nservices;
 uint8_t nattr;
 app_service_t * services; 
 app_attr_t * attr;

 
  connection =  NET_get_connection_by_chandler_BLE(chandler);
  nservices = connection->Node_profile->n_service;
  services= connection->Node_profile->services;
    for(i=0; i < nservices; i++ ){
      attr = services->attrs;
      nattr = services->n_attr;
      for(j=0; j < nattr; j++){
        if(attr->Associate_CharHandler+1==attrhandler){
         *att= &(*(app_attr_t *)attr);
         *serv = &(*(app_service_t *)services);
         break;
        }
        attr=attr->next_attr;
      }
      services=services->next_service; 
    }
}    


/**
  * @brief  retrieve one connection by its index
  * @param  uint8_t index: index of the connection to retrieve
  * @retval connection_t *: Pointer to the retrieved connection. 
  */
connection_t * NET_get_connection_by_index_CB(uint8_t _index){
  connection_t * connection = NULL;
  #ifdef MULTINODE
  connection=&network.mMSConnection[_index];
  #else
  connection=&network.mMSConnection;
  #endif
  
  return connection;

}

uint8_t NET_get_num_connections(void)
{
	uint8_t n_connections;
	n_connections = network.num_device_connected;
	return n_connections;

}
