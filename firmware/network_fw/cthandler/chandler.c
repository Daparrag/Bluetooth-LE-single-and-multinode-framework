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
dv_state_t bnrg_device_status = DEVICE_UNITIALIZED; /*device status initialized by default*/
app_discovery_t * DV_config = NULL;
app_advertise_t  * AV_config= NULL;
const app_discovery_t DV_default_config = {SCAN_TYPE ,SCAN_INTV, SCAN_WIN, 0x00,0x01,0x00,NULL}; /*default configuration for the scan procedure*/
const config_connection_t CN_default_config = {SCAN_P, SCAN_L, OUR_ADDRS_TYPE, CONN_P1, CONN_P2, LATENCY, SUPERV_TIMEOUT, CONN_L1, CONN_L2};/*connection default configuration*/
const app_advertise_t  AV_default_config = {ADV_EVT_TYPE, ADV_IT_MIN, ADV_IT_MAX, ADV_ADDR_TYPE, ADV_POLICY, SLAVE_INT_MIN, SLAVE_INT_MAX}; /*advertisement default configuration*/
/************************************Module Flags*****************************************/
uint8_t wait_end_procedure = 0;
/******************************Static func************************************************/ 
//static void connection_handler_broadcast(void);
//static void connection_handler_error(void);
static CHADLE_Status CH_set_advertise_BLE(void * advertise_conf, 
                                uint8_t scanres_data_size,
                                void * scanres_data,
                                uint8_t serviceuuidlength, 
                                void * serviceuuidlist);
/*****************************************************************************************/
/*selective discovery function independent of the architecture*/
/****************************************************************************************/



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
 uint8_t i;
 
 PRINTDEBUG("Creating connection  to the peripheral: ");
      for (i=0; i<5; i++) {
        PRINTDEBUG("0x%02X-", *((uint8_t*)peer_addrs+i));
      }
  PRINTDEBUG("0x%02X", *((uint8_t*)peer_addrs+i));     
 PRINTDEBUG("\n");

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
/**
  * @brief  This function management the node discovery procedure(specific connection and  discovery for the withlist devices).
  * @param  void * dicovery_config : user configuration (mandatory)
  * @retval APP_Status: Value indicating success or error code.
  */

CHADLE_Status CH_set_selective_discovery_BLE(void * dicovery_config){/*selective discovery procedure usefull in case of multinode*/
  

  tBleStatus ret;
  app_discovery_t * user_config;
  
  if(dicovery_config==NULL){
    /*it is mandatory set up a discovery configuration */
    PRINTDEBUG("you cannot set up the selective discovery with a default parameters please set up a configuration /n");
    return CHADLE_ERROR; 
  }else{
     user_config = (app_discovery_t*)dicovery_config;
     
     if(user_config->Num_whiteList_Entries == 0 || user_config->addr_array == NULL){
         /*it is wrong discovery configuration */
          PRINTDEBUG("you cannot set up the selective discovery with EMPTY WHITE_LIST  please set up correctly the discovery configuration /n");
          return CHADLE_ERROR;
     }else{
        /*correct discovery configuration */
        ret = aci_gap_start_selective_conn_establish_proc(user_config->scan_type,
                                                          user_config->sinterval,
                                                          user_config->swindows,
                                                          user_config->ownaddrtype,
                                                          user_config->fduplicates,
                                                          user_config->Num_whiteList_Entries,
                                                          user_config->addr_array
                                                            );
       
      if (ret != BLE_STATUS_SUCCESS){
          return CHADLE_ERROR;
      }   
       
   }
        
  }
 
 return CHADLE_SUCCESS;
  
}


CHADLE_Status CH_Connection_Complete_BLE(connection_t * connection, uint16_t handle, uint8_t peer_addrs[6]){
 uint8_t i;
 if(connection==NULL)return CHADLE_ERROR; 
  connection->Connection_Handle = handle;
  
  PRINTDEBUG("Connected to peripheral: ");
      for (i=0; i<5; i++) {
        PRINTDEBUG("0x%02X-", connection->device_address[i]);
      }
   PRINTDEBUG("0x%02X", connection->device_address[i]);    
  PRINTDEBUG("\n");    
      
   PRINTDEBUG("Connected to expected peripheral: ");
      for (i=0; i<5; i++) {
        PRINTDEBUG("0x%02X-",peer_addrs[i] );
      }
    PRINTDEBUG("0x%02X", *((uint8_t*)peer_addrs+i));   
   PRINTDEBUG("\n"); 
   
     HAL_Delay(5);
   return CHADLE_SUCCESS;
}


CHADLE_Status CH_Connection_Complete_perispheral_BLE(connection_t * connection, uint16_t handle, uint8_t peer_addrs[6]){
  uint8_t i; 
  if(connection==NULL)return CHADLE_ERROR; 

    PRINTDEBUG("Connected to central: ");
    
    for (i=0; i<5; i++) {
        PRINTDEBUG("0x%02X-", peer_addrs[i]);
      }
        PRINTDEBUG("0x%02X", peer_addrs[i]);    
        PRINTDEBUG("\n"); 

  connection->Connection_Handle=handle;      
  return CHADLE_SUCCESS;

}



void set_connection_wait_procedure(void){
  wait_end_procedure = 1;
}

void clean_connection_wait_procedure(void){
  wait_end_procedure = 0;
}

uint8_t get_connection_end_procedute(void){
 return wait_end_procedure;
}


CHADLE_Status CH_run_discovery_BLE(void){
CHADLE_Status ret;
ret = CH_set_discovery_BLE (DV_config);/*issue*/
return ret;

}

CHADLE_Status CH_run_selective_discovery_BLE(void){
  CHADLE_Status ret;
  ret= CH_set_selective_discovery_BLE(DV_config);
  return ret;

}

CHADLE_Status CH_run_create_connection_BLE(connection_t * connection){
 CHADLE_Status ret;

 ret = CH_create_connection_BLE(connection->cconfig,connection->device_type_addrs,connection->device_address);

return ret;
}


CHADLE_Status CH_run_advertise_BLE(void){
  CHADLE_Status ret;
  ret= CH_set_advertise_BLE(AV_config,0,NULL,0,NULL);
  return ret;
}


CHADLE_Status CH_new_device_found_BLE(connection_t * connection, void * pr){
  uint8_t i;
  le_advertising_info *info_new_device = (le_advertising_info*) pr;
  
   PRINTDEBUG("New peripheral found : ");
      for (i=0; i<5; i++) {
        PRINTDEBUG("0x%02X-", *((uint8_t*)info_new_device->bdaddr+i));
      }
     PRINTDEBUG("0x%02X", *((uint8_t*)info_new_device->bdaddr+i));     
     PRINTDEBUG("\n");


   connection->device_type_addrs = info_new_device->bdaddr_type;
     COPY_VAR(connection->device_address,info_new_device->bdaddr); 
     connection->connection_status = ST_CREATE_CONNECTION;

 return CHADLE_SUCCESS;    

}


/**
  * @brief  This function management the node advertisement procedure called by the server.
  * @param  void * advertise_conf : user configuration (optional)
  * @param  uint8_t scanres_data_size :scan respounse datasize (default 0)
  * @param  void * scanres_data:scan respounse data (default NULL)
  * @param  uint8_t serviceuuidlength:service uuid length (default 0)
  * @param  void * serviceuuidlist:service uuid list (default NULL)
  * @retval APP_Status: Value indicating success or error code.
  */

CHADLE_Status CH_set_advertise_BLE(void * advertise_conf, 
                                uint8_t scanres_data_size,
                                void * scanres_data,
                                uint8_t serviceuuidlength, 
                                void * serviceuuidlist){/*server generate advertisements to clients*/
  tBleStatus ret;
  app_advertise_t * user_config;
/*check for respouse data*/  
  if(scanres_data==NULL){
    /*not advertisement response data*/
    hci_le_set_scan_resp_data(0,NULL);
  }else{
    /*set the hci command for scan respouse data in the advertisement event*/
    hci_le_set_scan_resp_data(scanres_data_size,(uint8_t * ) scanres_data);
  }

  
   uint8_t hw_board = get_harware_version();
    const char * name = get_local_name();
    uint8_t namesize = get_local_name_size();
    
/*check for advertise config*/
  if(advertise_conf==NULL){
   
    
    if(   GET_ROLE(hw_board)!= (GAP_PERIPHERAL_ROLE_IDB04A1 || GAP_BROADCASTER_ROLE_IDB04A1 || GAP_PERIPHERAL_ROLE_IDB05A1 || GAP_BROADCASTER_ROLE_IDB05A1)){
              PRINTF("There is no possible to setup the device in adverticement mode if it is not in GAP_PERIPHERAL_ROLE or GAP_BROADCASTER_ROLE please set it in app_ble.h/n");
         }
    /*uses default configuration*/
      ret = aci_gap_set_discoverable(AV_default_config.adveventtype,
                                     AV_default_config.advintervalmin,
                                     AV_default_config.advintervalmax,
                                     AV_default_config.advaddrstype,
                                     AV_default_config.advfilterpoli,
                                     namesize,
                                     name,
                                     serviceuuidlength,
                                     (uint8_t*)serviceuuidlist,
                                     AV_default_config.slconnintervalmin,
                                     AV_default_config.slconnintervalmax
                                    );

  }else{
    /*uses user configuration*/
    user_config=(app_advertise_t *) advertise_conf;

    ret = aci_gap_set_discoverable( user_config->adveventtype,
                                    user_config->advintervalmin,
                                    user_config->advintervalmax,
                                    user_config->advaddrstype,
                                    user_config->advfilterpoli,
                                    namesize,
                                    name,
                                    serviceuuidlength,
                                    (uint8_t*)serviceuuidlist,
                                    user_config->slconnintervalmin,
                                    user_config->slconnintervalmax
                                    );
  }

if (ret != BLE_STATUS_SUCCESS){
     return CHADLE_ERROR;
   }

   return CHADLE_SUCCESS;

}


CHADLE_Status CH_finish_the_connection_BLE(void)
{
   tBleStatus ret;
   
   if(CONNECTION_MODE==GENERAL_CONNECTION)ret = aci_gap_terminate_gap_procedure(0x40);
   
   
if (ret != BLE_STATUS_SUCCESS){
    return CHADLE_ERROR;
  }

  return CHADLE_SUCCESS;   
}


/*void connection_handler_broadcast(){
}


void connection_handler_error(void){
	BSP_LED_Off(LED2);
	while(1);
}

*/

