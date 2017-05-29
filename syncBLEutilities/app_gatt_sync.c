/*GATT fuctions*/

// DEBUG Macros
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif



static uint8_t cfg_server_scan=0;
static uint8_t cfg_server_connect=0;
static uint8_t config_client=0;



static _APP_Status _APP_MULT_CONNECTION_SETUP_CONFIG(App * app, int16_t Sinterval, 
								  uint16_t Intv_windows,uint16_t ConnInter_max,uint16_t ConnInter_min,
								  uint16_t C_Latency,uint16_t Conn_Len_Min, uint16_t Conn_Len_Max
								  uint16_t LE_Timeout_min){
									  
	app->app_Master_broadcast_conf.CN_c.LE_scan_interval=Sinterval;/*Scan_Interval*/
	app->app_Master_broadcast_conf.CN_c.LE_scan_windows=Intv_windows;/*Scan_Window*/
	app->app_Master_broadcast_conf.CN_c.LE_CI_max=ConnInter_max;
	app->app_Master_broadcast_conf.CN_c.LE_CI_min=ConnInter_min;
	app->app_Master_broadcast_conf.CN_c.Conn_Latency=C_Latency;
	app->app_Master_broadcast_conf.CN_c.Conn_Len_Min=Conn_Len_Min;
	app->app_Master_broadcast_conf.CN_c.Conn_Len_Max=Conn_Len_Max;
	app->app_Master_broadcast_conf.CN_c.LE_Timeout_min=LE_Timeout_min;
	cfg_server_connect=1;
	return _OPT_SUCCESS;
								  
}

/**
* @brief  This function is called from the server in order to go in discovery mode for N-childs.
* @param  None
* @retval 
*/

static _APP_Status _APP_MULT_SERVER_SCAN_CONFIG(App * app, uint16_t n_slaves, uint32_t Role, uint16_t Sinterval, 
								  uint16_t Intv_windows){
	
	app->app_Master_broadcast_conf.num_slaves=n_slaves; 						/*number of slaves*/
	app->app_Master_broadcast_conf.scan_adv_c.LE_scan_interval=Sinterval;		/*scan interval*/
	app->app_Master_broadcast_conf.scan_adv_c.LE_scan_windows=Intv_windows;		/*scan windows*/
	/*used length=num_slaves*Intv_windows+10ms*/
	cfg_server_scan=1;
	return _OPT_SUCCESS;
	
}



_APP_Status _APP_MULT_SERVER_DISCOVERY_PROCEDURE(App * app){
	uint8_t ret;
	uint16_t i;
	if(!config_server_scan_config){
		
		PRINTF("error: Not configuration in the discovery procedure \n")
		return _APP_NOT_CONFIG;	
	}
	for(i=0;i < app->app_Master_broadcast_conf.num_slaves){
		 ret = aci_gap_start_general_discovery_proc(app->app_Master_broadcast_conf.scan_adv_c.LE_scan_interval,/*scan interval*/
		 	                                        app->app_Master_broadcast_conf.scan_adv_c.LE_scan_windows, /*scan windows*/
													0x00, /**/
													0x01/**/
													); 
		 if (ret != BLE_STATUS_SUCCESS)
			{
				PRINTF("error: while add server discovery procedure error num %d \n",ret);
				return _OPT_ERROR;
			}											
	}
	return _OPT_SUCCESS;
}


_APP_Status _APP_MULT_SERVER_SETUP_CONNECTION(App * app,uint32_t peer_addr_type,uint8_t *Peer_Address, 
											  uint32_t Own_Address_Type){
												  
	 /*
   Scan_Interval, Scan_Window, Peer_Address_Type, Peer_Address, Own_Address_Type, Conn_Interval_Min,
   Conn_Interval_Max, Conn_Latency, Supervision_Timeout, Conn_Len_Min, Conn_Len_Max
   */
   uint8_t ret;
   if(!config_server_connect_config){
		
		PRINTF("error: Not configuration in the setup connection \n")
		return _APP_NOT_CONFIG;	
	}
   
   ret = aci_gap_create_connection(app_Master_broadcast_conf.CN_c.LE_scan_interval, /*Scan_Interval*/
							 app_Master_broadcast_conf.CN_c.LE_scan_windows,  /*Scan_Window*/
							 Own_Address_Type,								  /*Peer_Address_Type*/
                             Peer_Address, 									  /*Peer_Address*/
							 Own_Address_Type, 								  /*Own_Address_Type*/
							 app_Master_broadcast_conf.CN_c.LE_CI_min,		  /*Conn_Interval_Min*/	
                             app_Master_broadcast_conf.CN_c.LE_CI_max,		  /*Conn_Interval_Max*/
							 app_Master_broadcast_conf.CN_c.Conn_Latency,	  /*Conn_Latency*/
							 app_Master_broadcast_conf.CN_c.LE_Timeout_,	  /*Supervision_Timeout*/	
							app_Master_broadcast_conf.CN_c.Conn_Len_Min, 	  /*Conn_Len_Min*/
							app_Master_broadcast_conf.CN_c.Conn_Len_Max		  /*Conn_Len_Max*/
							);
							
	 if (ret != BLE_STATUS_SUCCESS)
			{
				PRINTF("error: while connection setup, error value: %d \n",ret);
				return _OPT_ERROR;
			}												
                            
  return _OPT_SUCCESS;
   
   
}


/**
  * @brief  This function is called to add Service at the app level.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval BV_Status: Value indicating success or error code.
  */

_APP_Status _APP_MULT_SERVER_ADD_SERVICE(uint8_t *service_uuid,uint16_t *service_handle){
		/*include BLUEVOICE + SYNC + CTRL services*/
		
}

