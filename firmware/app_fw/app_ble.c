/*suported firmware for BLE app_layer (single node specification)*/
/*application management functions*/
/*global procedures common to any application */

#include <app_ble.h>
#include <list.h>

/**********************Local Variables***********************/
uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
uint8_t  bnrg_expansion_board =  IDB04A1;
uint8_t service_list_init=0;            /*!< flag used to initialized the service attribute list */

const uint8_t DEVICE_BDADDR[] =  { 0x55, 0x11, 0x07, 0x01, 0x16, 0xE1 }; /*device addrs*/
const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','L','E','-','U','N','O'}; /*device name*/
//const app_discovery_t DV_default_config = { SCAN_INTV, SCAN_WIN, 0x00,0x01}; /*default configuration for the scan procedure*/
//const app_connection_t CN_default_config = {SCAN_P, SCAN_L, OUR_ADDRS_TYPE, CONN_P1, CONN_P2, LATENCY, SUPERV_TIMEOUT, CONN_L1, CONN_L2};/*connection default configuration*/
const app_advertise_t  AV_default_config = {ADV_EVT_TYPE, ADV_IT_MIN, ADV_IT_MAX, ADV_ADDR_TYPE, ADV_POLICY, SLAVE_INT_MIN, SLAVE_INT_MAX}; /*advertisement default configuration*/
/************************************************************/

tBleStatus(*const  GAP_INIT_FUNC [BLE_ARCH_MASK+1])(uint8_t ,uint8_t ,uint8_t ,uint16_t* ,
                                                 uint16_t* ,uint16_t* ) = {                                             
MY_HAVE_IDB0xA1(0, aci_gap_init),                                            
MY_HAVE_IDB0xA1(1, aci_gap_init)                                                    
}; /*init_gap_macro*/

#define  aci_gap_init(_role,_privacity_enable,_device_name_char_length,_service_handler,\
                      _dev_name_char_handle,_appearance_char_handle,arch) \
        ((*GAP_INIT_FUNC[arch & BLE_ARCH_MASK])(_role,_privacity_enable,_device_name_char_length,\
                                              _service_handler,_dev_name_char_handle,\
                                              _appearance_char_handle))/*init gap function*/
          
/******************************Static func************************************************/ 
static int APP_BLE_GET_VERSION(uint8_t *hwVersion, uint16_t *fwVersion);          
/*****************************************************************************************/          

/**
  * @brief  This function is called to retrieve the BLE APP version.
  * @param void.
  * @retval APP_Status: Value indicating success or error code.
  */
  
static int APP_BLE_GET_VERSION(uint8_t *hwVersion, uint16_t *fwVersion){


  uint8_t status;
  uint8_t hci_version, lmp_pal_version;
  uint16_t hci_revision, manufacturer_name, lmp_pal_subversion;

  status = hci_le_read_local_version(&hci_version, &hci_revision, &lmp_pal_version, 
				     &manufacturer_name, &lmp_pal_subversion);

  if (status == BLE_STATUS_SUCCESS) {
    *hwVersion = hci_revision >> 8;
    *fwVersion = (hci_revision & 0xFF) << 8;              // Major Version Number
    *fwVersion |= ((lmp_pal_subversion >> 4) & 0xF) << 4; // Minor Version Number
    *fwVersion |= lmp_pal_subversion & 0xF;               // Patch Version Number
  }

  HCI_Process(); // To receive the BlueNRG EVT

  return status;

}

/**
  * @brief  This function is called to init the BLE architecture.
  * @param  none
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_Init_BLE(void){/*can be used by any application*/
  uint8_t ret = 0;
  uint8_t  hwVersion;
  uint16_t fwVersion;  
/* get the BlueNRG HW and FW versions */
  APP_BLE_GET_VERSION(&hwVersion, &fwVersion);
   BlueNRG_RST();
   
    if (hwVersion > 0x30) { 
    bnrg_expansion_board = IDB05A1; 
  	}

  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, DEVICE_BDADDR);

	if (ret != BLE_STATUS_SUCCESS)
  	{
    	return APP_ERROR;
  	}
        
 
  ret = aci_gatt_init();    
         if(ret)
         {
             return APP_ERROR;
         }
  
  	ret = aci_gap_init(GET_ROLE(bnrg_expansion_board), 0, 0x07,  &service_handle, &dev_name_char_handle,  &appearance_char_handle,bnrg_expansion_board);


  if (ret != BLE_STATUS_SUCCESS)
  {
    return APP_ERROR;
  }

   /* Set auth requirement*/
  aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                               OOB_AUTH_DATA_ABSENT,
                               NULL, 7, 16,
                               USE_FIXED_PIN_FOR_PAIRING, 123456,
                               BONDING);
                               
  /* Set output power level */
  aci_hal_set_tx_power_level(1, 5);
  
  return APP_SUCCESS;
}

/**
  * @brief  This function initialize the profile list.
  * @ This function must be called at the begining of the application.
  * @param profile datastructure.
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_init_BLE_Profile(app_profile_t * profile){
   LIST_STRUCT_INIT(profile,_service);
    profile->n_service=0;
    
    return APP_SUCCESS;
}


/**
  * @brief  This function is called to add any Service.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_add_BLE_Service(app_profile_t * profile, app_service_t * service){
  tBleStatus ret;
  app_service_t * aux_service;
  if(service==NULL)return APP_ERROR;
  if(profile->services==NULL)
    {
      profile->services=service;
    } else if(profile->services!=service){
      aux_service = profile->services;
       do{
            aux_service=aux_service->next_service;
            if(aux_service==service)break;/*service already included for this profile*/
         }while(aux_service!=NULL);
      
       if(aux_service==NULL){
          aux_service=service;
       }
    }else if (profile->services == service  || aux_service == service)return APP_ERROR;
  //LIST_STRUCT_INIT(service,_attr); /*Initialized the attribute list associate to this service*/
  //list_add(profile->_service, service); /*Add the new service at the list of services*/
  ret = aci_gatt_add_serv(service->service_uuid_type,
                          service->ServiceUUID,
                          service->service_type,
                          service->max_attr_records,
                          &(service->ServiceHandle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    /*remove the service form the list list_chop*/
    return APP_ERROR;
  }
  
  if(!service->attr_list_init){
   
    service->attr_list_init=1;  
  }

  profile->n_service+=1; /*increment the control service counter*/
 return APP_SUCCESS;
}

/**
  * @brief  This function is called to add any characteristic.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval APP_Status: Value indicating success or error code.
  */
APP_Status APP_add_BLE_attr(app_service_t * service, app_attr_t *attr){
    tBleStatus ret;

    
    LIST_STRUCT_INIT(attr,_value);
   //LIST_STRUCT_INIT2(service,_attr); /*Initialized the attribute list associate to this service*/
   list_add(service->_attr,attr);
    
    ret = aci_gatt_add_char(service->ServiceHandle,
                          attr->charUuidType, 
                          attr->CharUUID, 
                          attr->charValueLen, 
                          attr->charProperties, 
                          attr->secPermissions,
                          attr->gattEvtMask,
                          attr->encryKeySize,
                          attr->isVariable,
                          &(attr->CharHandle));
    
    if (ret != BLE_STATUS_SUCCESS) {
      /*delete the attr entry in the service list*/
      return APP_ERROR;
      
    }  
    service->n_attr+=1;
    return APP_SUCCESS;
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

APP_Status APP_set_advertise_BLE(void * advertise_conf, 
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

/*check for advertise config*/
  if(advertise_conf==NULL){

    /*uses default configuration*/
      ret = aci_gap_set_discoverable(AV_default_config.adveventtype,
                                     AV_default_config.advintervalmin,
                                     AV_default_config.advintervalmax,
                                     AV_default_config.advaddrstype,
                                     AV_default_config.advfilterpoli,
                                     sizeof(local_name),
                                     local_name,
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
                                    sizeof(local_name),
                                    local_name,
                                    serviceuuidlength,
                                    (uint8_t*)serviceuuidlist,
                                    user_config->slconnintervalmin,
                                    user_config->slconnintervalmax
                                    );
  }

if (ret != BLE_STATUS_SUCCESS){
     return APP_ERROR;
   }

   return APP_SUCCESS;

}




/*user control functions*/

/**
  * @brief  This function return the default configuration for the BLE app firmware.
  * @param  void *app_discovery (used for catch the discovery config parameter)
  * @param  void * app_connection (used for catch the connection config parameter)
  * @param  void * app_advertise (used for catch the advertise config parameter)
  * @retval none.
  */

void APP_get_default_config_BLE(void *app_discovery,void * app_connection,void * app_advertise){

}



/**
  * @brief  This function return pointer to the device name.
  * @param int * size (used for catch the device name size)
  * @retval void *: void pointer to the name.
  */

void * APP_get_direct_name_BLE(int * size){
  *size = sizeof(local_name);
  return (void *)&(local_name);
}



/**
  * @brief  This function return pointer to the device address.
  * @param int * size (used for catch the device address size)
  * @retval void *: void pointer to the address. 
  */

void * APP_get_direct_addrs_BLE(int * size){
    *size = sizeof(DEVICE_BDADDR);
  return (void *)&(DEVICE_BDADDR);  
}


/**
  * @brief  This function retreve the services presents in the profile.
  * @param app_profile_t * profile: profile which contain the services
  * @param app_profile_t * service: pointer used to retreve the serivices in the profile
  * @APP_Status: Value indicating success or error code.
  */

void * APP_get_service_BLE(app_profile_t * profile, void * serv){
  app_service_t * temp_service;

  if(profile == NULL) return NULL;
  
  if(serv == NULL){
     temp_service = (app_service_t *) list_head(profile->_service);
  }else 
  {
    temp_service = list_item_next((void *)serv);
  }
  
  return temp_service;
 
}


/**
  * @brief  This function retreve the services presents in the profile.
  * @param app_service_t * service: service which contain a set of attributes
  * @param app_profile_t * service: pointer used to retreve the attributes associated to this particular service.
  * @APP_Status: Value indicating success or error code.
  */

void * APP_get_attribute_BLE(app_service_t * service, void *attr){
  app_attr_t * temp_att;
  
  if(service==NULL) return NULL;
  
  if(attr==NULL) temp_att = list_head(service->_attr);
  else temp_att = list_item_next((void *) attr);
  
  return temp_att;

}




