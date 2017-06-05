/*suported firmware for BLE app_layer*/
/*application management functions*/
/*global procedures common to any application */

#include <app_ble.h>
#include <list.h>



/**********************Local Variables***********************/
uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
volatile uint8_t  bnrg_expansion_board = IDB04A1;
const uint8_t DEVICE_BDADDR[] = { 0x55, 0x11, 0x07, 0x01, 0x16, 0xE1 }; /*central device addrs*/

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
/**
  * @brief  This function initialize the profile list.
  * @ This function must be called at the begining of the application.
  * @param profile datastructure.
  * @
  */
void profile_control_init(app_profile_t * profile){
    LIST_STRUCT_INIT(profile, _service);
    profile->n_service=0;
}
/**
  * @brief  This function is called to add Start the BLE APP firware.
  * @param void.
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_Init_BLE(void){/*can be used by any application*/

  uint8_t ret = 0;
  uint8_t  hwVersion;
  uint16_t fwVersion;  
/* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);
   BlueNRG_RST();
   
    if (hwVersion > 0x30) { 
    bnrg_expansion_board = IDB05A1; 
  	}

  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, DEVICE_BDADDR);

	if (ret != BLE_STATUS_SUCCESS)
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
  * @brief  This function is called to add any Service.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_add_BLE_Service(app_profile_t * profile, app_service_t * service){
  tBleStatus ret;
  LIST_STRUCT_INIT(service,_attr); /*Initialized the attribute list associate to this service*/
  list_add(profile->_service,service); /*Add the new service at the list of services*/
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
  profile->n_service+=1; /*increment the control service counter*/
 return APP_SUCCESS;
}



/**
  * @brief  This function is called to add any characteristic.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval APP_Status: Value indicating success or error code.
  */

static app_service_t * get_tail_service(app_profile_t profile){
	struct app_service_t * temp_p;
	if(*profile==NULL){
		return NULL;
	}

	for(l=*profile.service;l->next_service != NULL;l->next_service);
		return l;
}


static app_service_t * get_tail_characteristic(app_profile_t profile, app_service_t service){

}

static app_char_t * get_tail_value(app_profile_t profile, app_service_t service, app_char_t chart){

}


add_service(app_profile_t profile, app_service_t* service){
	/*declare the service name*/

}