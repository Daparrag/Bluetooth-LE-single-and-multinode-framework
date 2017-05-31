/*suported firmware for BLE app_layer*/
/*application management functions*/
/*global procedures common to any application */

#include "app_ble.h"




/**********************Local Variables***********************/
uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
volatile uint8_t  bnrg_expansion_board = IDB04A1;
const uint8_t DEVICE_BDADDR[] = { 0x55, 0x11, 0x07, 0x01, 0x16, 0xE1 }; /*central device addrs*/
/************************************************************/


tBleStatus (*const  GAP_INIT_FUNC[BLE_ARCH_MASK+1])(uint8_t ,uint8_t ,uint8_t ,uint16_t* ,
                                                 uint16_t* ,uint16_t* ) = {                                             
MY_HAVE_IDB0xA1(0, aci_gap_init),                                            
MY_HAVE_IDB0xA1(1, aci_gap_init)                                                    
}; /*init_gap_macro*/

#define  aci_gap_init(_role,_privacity_enable,_device_name_char_length,_service_handler,\
                      _dev_name_char_handle,_appearance_char_handle,arch) \
        ((*GAP_INIT_FUNC[arch&BLE_ARCH_MASK])(_role,_privacity_enable,_device_name_char_length,\
                                              _service_handler,_dev_name_char_handle,\
                                              _appearance_char_handle))/*init gap function*/



APP_Status APP_Init_BLE(void){

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
    return BV_APP_ERROR;
  }

   /* Set auth requirement*/
  aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                               OOB_AUTH_DATA_ABSENT,
                               NULL, 7, 16,
                               USE_FIXED_PIN_FOR_PAIRING, 123456,
                               BONDING);
                               
  /* Set output power level */
  aci_hal_set_tx_power_level(1, 5);
  
  return BV_APP_SUCCESS;

}











void app_Error_Handler(void){
	
}



APP_Status app_AddService (sync_uuid.ServiceUUID, &handlers.ServiceHandle){

}


APP_Status app_AddChar (){


}