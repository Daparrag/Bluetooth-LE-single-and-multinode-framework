/*connection handler fsm*/
#ifndef CONNECTION_HANDLER_BLE_H
#define CONNECTION_HANDLER_BLE_H


#ifdef BLE_APP_CONFIG
#include "ble_app_conf.h" 
#endif

#include <app_ble.h>
#include <debug.h>


/*type of connections*/
#define CONNECTION_ORIENTED 0x1	/*connection is master-slave oriented*/
#define	BROADCAST_ORIENTED	0x0 /*connection is advertisement oriented*/

/*multinode setup*/
#ifndef MULTINODE
#define MULTINODE 			0x0
#endif




typedef struct connection_handler_fsm {
	cn_state_t ptp_cstate;
}connection_handler_fsm;


void connection_handler_Dispatch(ptp_fsm * ptp_ins);

#endif /* PTP_BLE_H */
/**/