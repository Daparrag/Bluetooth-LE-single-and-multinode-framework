/*Simple Sync-PTP BLE protocol*/


#include "ptp_ble.h"

#ifndef COPY_VAR
#define COPY_VAR(source,dest) (memcpy((source),(dest),sizeof(dest)))
#endif



/**************************Service Variables*************************/
ptp_uuid_t sync_uuid; /*store the Universally unique identifier*/
ptp_ProfileHandle_t handlers; /*store the service characteristics handler*/
static ptp_ControlTypeDef handler_ptp_app;/*total control of the application handler & app status*/

app_service_t bleptp_service;
app_attr_t bleptp_tx_att;
app_attr_t bleptp_rx_att;

/* ptp service*/
static const uint8_t ptp_service_uuid[16] = { 0x66, 0x9a, 0x0c,
		0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xf0, 0xf2, 0x73,
		0xd9 };
static const uint8_t ptp_TXchar_uuid[16] = { 0x66,0x9a,0x0c,
		0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe1,0xf2,0x73,
		0xd9};
static const uint8_t ptp_RXchar_uuid[16] = { 0x66,
	0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe2,0xf2,0x73,
	0xd9};
/******************************Static Func**************************************/
static void init_ptp_profile(app_profile_t * profile);
static void ptp_error_handler(void);
static void multinode_setup_connection(void);
static void singlenode_setup_connection(void);
static void setup_adverticement(void);
/*******************************************************************************/


/**
  * @brief  This function initialize the ptp_service.
  * @ This function must be called at the begining of the application.
  * @param profile datastructure.
  * @
  */
static void init_ptp_profile(app_profile_t * profile){
/*create the ptp_service */
  COPY_VAR(bleptp_service.ServiceUUID,ptp_service_uuid);
  bleptp_service.service_uuid_type=UUID_TYPE_128;
  bleptp_service.service_type=PRIMARY_SERVICE;
  bleptp_service.max_attr_records=7;
  /*copy the and associate the service to the BLE application profile*/
  ret = APP_add_BLE_Service(profile,&bleptp_service);
  
  if(ret!=APP_SUCCESS) ptp_error_handler();
  /*create the ptp_TX_attribute*/
  COPY_VAR(bleptp_tx_att.CharUUID,ptp_TXchar_uuid);
  bleptp_tx_att.charUuidType = UUID_TYPE_128;
  bleptp_tx_att.charValueLen = 20;
  bleptp_tx_att.charProperties = CHAR_PROP_NOTIFY;
  bleptp_tx_att.secPermissions = ATTR_PERMISSION_NONE;
  bleptp_tx_att.gattEvtMask = GATT_DONT_NOTIFY_EVENTS;
  bleptp_tx_att.encryKeySize=16;
  bleptp_tx_att.isVariable=1;
  /*copy and associate the ptp_TX_attribute to the ptp service*/
   ret= APP_add_BLE_attr(&bleptp_service,&bleptp_tx_att);
    if(ret!=APP_SUCCESS)ptp_error_handler();
  /*create the ptp_RX_attribute*/
   COPY_VAR(bleptp_rx_att.CharUUID,charUuidRX);
  bleptp_rx_att.charUuidType = UUID_TYPE_128;
  bleptp_rx_att.charValueLen = 20;
  bleptp_rx_att.charProperties = CHAR_PROP_NOTIFY;
  bleptp_rx_att.secPermissions = ATTR_PERMISSION_NONE;
  bleptp_rx_att.gattEvtMask = GATT_DONT_NOTIFY_EVENTS;
  bleptp_rx_att.encryKeySize=16;
  bleptp_rx_att.isVariable=1;
  /*copy and associate the RX_attribute to a service*/
  ret= APP_add_BLE_attr(&bleptp_service,&bleptp_rx_att); 
    if(ret!=APP_SUCCESS)ptp_error_handler();
}






void ptp_Dispatch(ptp_fsm * ptp_inst){
	switch(ptp_inst->C_State){
		case INIT:
		/*slave turn on*/
		/*init service, discovery master clock, create a sync, follow req*/

		case UNSYNC:
		/*salve requiere re-sync */
		case SYNC:
		/*salve already sync*/
		case WAIT_RESP:
		/*Slave wait for delay resp*/
		case PENDING_REQ:
		/*Slave have to req delay*/
	}
}

void app_Error_Handler(void){
	
}

BV_Status app_gatt_AddService(uint8_t *service_uuid, uint16_t *service_handle)
{}