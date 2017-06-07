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


/*connection setup default parameters*/
#ifndef SCAN_P
#define SCAN_P  		(0x0028)      		             /*!< Scan Interval 40ms.*/			
#endif
#ifndef SCAN_L
#define SCAN_L  		(0x0014)      		             /*!< Scan Window.  20ms*/
#endif
#ifndef	CONN_P1
#define CONN_P1 		((int)((10)/1.25f))  	         /*!< Min connection interval in ms.*/
#endif
#ifndef	CONN_P2
#define CONN_P2			((int)((10)/1.25f))  	         /*!< Max connection interval in ms.*/
#endif
#ifndef	SUPERV_TIMEOUT
#define SUPERV_TIMEOUT  (100)         		         /*!< Supervision timeout.*/
#endif
#ifndef	CONN_L1
#define CONN_L1         ((int)((5)/0.625f))   	   /*!< Min connection length.*/
#endif
#ifndef	CONN_L2
#define CONN_L2         ((int)((5)/0.625f))   	   /*!< Min connection length.*/
#endif
#ifndef	LATENCY
#define LATENCY			(0)
#endif

#ifndef OUR_ADDRS_TYPE
#define OUR_ADDRS_TYPE PUBLIC_ADDR
#endif

typedef enum /*used for return the result of and operation on the application*/
{
  CHADLE_SUCCESS = 0x00,            /*!< command success applied */
  CHADLE_ERROR = 0x01,              /*!< and error occour during the command execution*/
} CHADLE_Status; 




typedef struct{/*structure for connection configuration*/
uint16_t sinterval;               /*!<  Time interval between two LE scans:0x0004-0x4000(period)*/
uint16_t swindows;                /*!<  Amoung of time for the duration of the LE scan:0x0004-0x4000(length)*/
//uint8_t peer_addrtype; /*user parameter*/
//uint8_t peer_addrs [6];/*user parameter*/
uint8_t ownaddrtype;             /*!<  0x00:public address, 0x01: random device address >*/ 
uint16_t cintervalmin;           /*!<  minimum value for connection event interval shall be less or equal to  cintervalmax: 0x0006-0x0C80 >*/
uint16_t cintervalmax;           /*!<  maximum value for connection event interval shall be greater or equal to  cintervalmin: 0x0006-0x0C80>*/
uint16_t clatency;               /*!<  salve latency for connection in number of connection events:0x0000-0x01F4>*/
uint16_t stimeout;               /*!<  supervisor time out for LE link: 0x000A-0x0C80>*/
uint16_t clengthmin;             /*!<  minimum length of connection event needed for LE: 0x0000-0xFFFF time N*0.625ms>*/
uint16_t clengthmax;             /*!<  maximum length of connection event needed for LE: 0x0000-0xFFFF time N*0.625ms>*/
}config_connection_t;



CHADLE_Status CH_create_connection_BLE(void *connect_config, 
                                    uint8_t peer_addrtype, 
                                    void * peer_addrs);/*used by setup connection by the master node*/


#endif /* CONNECTION_HANDLER_BLE_H */
/**/