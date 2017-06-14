/*connection handler 
* version 1.0
* task:
* -Connection stablishment single or multiple connections  
*  according to the network topology.
*
*- Management the broadcast transmissions in case of broadcast or observer roles.
*
* NOTE:
* -In case of multiple connections this module is able to handler specific 
*  requirements for each connection, allowing and efficient management of the BLE resources
*  since now it is possible to stablish an specific connection parameter per an specific node.
*
*/
#include <chandler.h>


#define SEG_DELAY 0x03E8 /*1seg delay*/
#define HALF_SEG_DELAY 0x1F4 /*1/2 seg delay*/
static uint32_t led_toggle_count = 0;
dv_state_t bnrg_device_status = DEVICE_UNITIALIZED; /*device status initialized by default*/
app_discovery_t * DV_config = NULL;
app_advertise_t  * AV_config= NULL;
const app_discovery_t DV_default_config = { SCAN_INTV, SCAN_WIN, 0x00,0x01}; /*default configuration for the scan procedure*/
const config_connection_t CN_default_config = {SCAN_P, SCAN_L, OUR_ADDRS_TYPE, CONN_P1, CONN_P2, LATENCY, SUPERV_TIMEOUT, CONN_L1, CONN_L2};/*connection default configuration*/
/************************************Module Flags*****************************************/



/******************************Static func************************************************/ 
static void connection_handler_broadcast(void);
static void connection_unestablished_toggle(void);
static void connection_stablished_toggle(void);
static void connection_handler_error(void);
/*****************************************************************************************/

/**
  * @brief  This function is called to setup the discovery configuration into the connection handler module.
  * @param  app_discovery_t * dv_conf: contain the discovery configuration.
  * @retval void.
  */
  
void connection_handler_set_discovery_config(app_discovery_t * dv_conf){
	DV_config = dv_conf;
};


/**
  * @brief  This function is called to setup the advertisement configuration into the connection handler module.
  * @param  app_discovery_t * dv_conf: contain the discovery configuration.
  * @retval void.
  */
  
void connection_handler_set_advertise_config(app_advertise_t  * adv_conf){
	AV_config = adv_conf;
}


/**
  * @brief  This function is used for indicate the connection_unestablished state.
  * @param void.
  * @retval void.
  */

void connection_unestablished_toggle(void){

if(led_toggle_count++ > LED_TOGGLE_UNESTABLISHED)
	{
		led_toggle_count=0;
		BSP_LED_Toggle(LED2);
	}
}


/**
  * @brief  This function is used for indicate the connection_stablished state.
  * @param void.
  * @retval void.
  */
void connection_stablished_toggle(void){

if(led_toggle_count++ > LED_TOGGLE_CONNECTED)
	{
		led_toggle_count=0;
		BSP_LED_Toggle(LED2);
	}
}


/**
  * @brief  This function is called to handler the connection between two devices acting as a client server.
  * @param  connection_t * connection: this is an specific connection structure;
  * @param   net_flags * flags: flags to handler the status of the network;
  * @retval void.
  */
void connection_handler_coriented (connection_t * connection, net_flags * flags){
CHADLE_Status ret;	
/*input verification*/
if(connection==NULL || flags==NULL){
	PRINTF("some of the imput parameters on the connection handler is NULL  please verify\n");
	return;
}
/*fsm*/
	switch(connection->connection_status){
		case ST_UNESTABLISHED:
		{
			connection_unestablished_toggle();
			switch(connection->device_cstatus){
				case DEVICE_UNITIALIZED:
				{
					PRINTF("device unitialized please reinitialize \n");
					HAL_Delay(SEG_DELAY);
				}
				break;
				case DEVICE_DISCOVERY_MODE:
				{/*connection UNESTABLISHED & device in DISCOVERY_MODE*/
					
					if(flags->device_found && !(flags->wait_end_procedure))
						{/*single connection setup*/
							ret=CH_create_connection_BLE(connection->cconfig, connection->device_type_addrs, (void*)&(connection->device_address));	
							if(ret != CHADLE_SUCCESS)
							{
								PRINTF("error has been occour during the single connection setup \n");
								connection_handler_error();
							}
							/*update_flag*/
							flags->device_found=0;
							
						}
					else if(!(flags->device_found) && !(flags->wait_end_procedure))
						{/*setup the discovery procedure.*/
							ret= CH_set_discovery_BLE (DV_config);/*issue*/
							if(ret != CHADLE_SUCCESS)
							{
								PRINTF("error has been occour during the setting the discovery procedure \n");
								connection_handler_error();
							}
							flags->wait_end_procedure=1;

						}			
				}
				break;
			} 
		}
		break;
		case ST_STABLISHED:
		{
			connection_stablished_toggle();
		}
		break;
		case ST_BROADCAST:
		{
			connection_stablished_toggle();
		}
		break;
		case ST_TIME_OUT:
		{
			connection_stablished_toggle();
		}
		break;
		default:
		break;
	}
}

/**
  * @brief  This function management the connection setup procedure.
  * @param  void *connect_config: user configuration (optional)
  * @param  uint8_t peer_addrtype: 0x00 public device address 0x01 random device adress
  * @param  void * peer_addrs: address of the peer device
  * @retval APP_Status: Value indicating success or error code.
  */
CHADLE_Status CH_create_connection_BLE(void *connect_config, 
                                    uint8_t peer_addrtype, 
                                    void * peer_addrs){/*used by setup connection by the master node*/
 
 tBleStatus ret;
 config_connection_t * user_config;

  if(connect_config==NULL){
  /*uses the default configuration*/
    ret = aci_gap_create_connection(CN_default_config.sinterval,
                                    CN_default_config.swindows,
                                    peer_addrtype,
                                    (uint8_t *) peer_addrs,
                                    CN_default_config.ownaddrtype,
                                    CN_default_config.cintervalmin,
                                    CN_default_config.cintervalmax,
                                    CN_default_config.clatency,
                                    CN_default_config.stimeout,
                                    CN_default_config.clengthmin,
                                    CN_default_config.clengthmax
                                    ); 

  }else{
    /*uses the user configuration*/
    user_config=(config_connection_t *) connect_config;

     ret = aci_gap_create_connection(user_config->sinterval,
                                    user_config->swindows,
                                    peer_addrtype,
                                    (uint8_t *) peer_addrs,
                                    user_config->ownaddrtype,
                                    user_config->cintervalmin,
                                    user_config->cintervalmax,
                                    user_config->clatency,
                                    user_config->stimeout,
                                    user_config->clengthmin,
                                    user_config->clengthmax
                                    ); 

  }

  if (ret != BLE_STATUS_SUCCESS){
     return CHADLE_ERROR;
   }

   return CHADLE_SUCCESS;
}



/**
  * @brief  This function management the node discovery procedure(for limited discoverable devices).
  * @param  void * dicovery_config : user configuration (optional)
  * @retval APP_Status: Value indicating success or error code.
  */

CHADLE_Status CH_set_discovery_specific_BLE(void * dicovery_config){/*this is used for receive advertisements called only by limited discoverable devices*/
   
     return CHADLE_SUCCESS;
}





/**
  * @brief  This function management the node discovery procedure(for specific device).
  * @param  void * dicovery_config : user configuration (optional)
  * @retval APP_Status: Value indicating success or error code.
  */

CHADLE_Status CH_set_discovery_limited_BLE(void * dicovery_config){/*this is used for receive advertisements called by an specific client identified by an UUID*/

     return CHADLE_SUCCESS;
}





/**
  * @brief  This function management the node discovery procedure(general discoverable procedure).
  * @param  void * dicovery_config : user configuration (optional)
  * @retval APP_Status: Value indicating success or error code.
  */

CHADLE_Status CH_set_discovery_BLE(void * dicovery_config){/*this is used for receive advertisements called by clients*/
 
  tBleStatus ret;
  app_discovery_t * user_config;

  if(dicovery_config==NULL){
    /*uses the default configuration*/
    /*default_config*/
    ret = aci_gap_start_general_discovery_proc(DV_default_config.sinterval,
                                                DV_default_config.swindows,
                                                DV_default_config.ownaddrtype,
                                                DV_default_config.fduplicates);

  }else{
    /*uses the user config*/
    user_config = (app_discovery_t*)dicovery_config;
    ret = aci_gap_start_general_discovery_proc(user_config->sinterval,
                                              user_config->swindows,
                                              user_config->ownaddrtype,
                                              user_config->fduplicates);

  }

   if (ret != BLE_STATUS_SUCCESS){
     return CHADLE_ERROR;
   }

   return CHADLE_SUCCESS;

}




void connection_handler_broadcast(){
}


void connection_handler_error(void){
	BSP_LED_Off(LED2);
	while(1);
}
