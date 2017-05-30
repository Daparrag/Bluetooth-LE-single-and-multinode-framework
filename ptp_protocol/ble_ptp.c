/*Simple Sync-PTP BLE protocol*/


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif

#include "ptp_ble.h"

ptp_uuid_t sync_uuid; /*store the Universally unique identifier*/
ptp_ProfileHandle_t handlers;

/* ptp service*/

uint16_t service_handle, dev_name_char_handle, appearance_char_handle;



static const uint8_t ptp_service_uuid[16] = { 0x66, 0x9a, 0x0c,
		0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xf0, 0xf2, 0x73,
		0xd9 };
static const uint8_t ptp_char_uuid[16] = { 0x66, 0x9a,
		0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xf1, 0xf2,0x73, 
		0xd9 };

		
tBleStatus ptp_init_service_profile(void){
	tBleStatus ret;
   /*start the FSM variables and signals*/
   /**/
   memcpy(sync_uuid.ServiceUUID,
              ptp_service_uuid,
              sizeof(ptp_service_uuid));                 
  memcpy(sync_uuid.CharTimeUUID,
              ptp_char_uuid,
              sizeof(ptp_char_uuid));
   /*ptp_sync Service creation */
  status = app_AddService(sync_uuid.ServiceUUID, &handlers.ServiceHandle);
  
  if(status != APP_SUCCESS)
  {
    app_Error_Handler();
  }
  
 /* characteristic_creation */
  status = 	app_AddChar(sync_uuid,&handlers.ServiceHandle,handlers);
  
   if(status != APP_SUCCESS)
  {
    app_Error_Handler();
  }
  
  return APP_SUCCESS;
};





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
		/*Slave have to req dealy*/
	}
}

void app_Error_Handler(void){
	
}

BV_Status app_gatt_AddService(uint8_t *service_uuid, uint16_t *service_handle)
{}