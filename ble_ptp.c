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

tBleStatus ptp_init_server(void){
	tBleStatus ret;
    uint8_t uuid[16]
	
	/**/
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

