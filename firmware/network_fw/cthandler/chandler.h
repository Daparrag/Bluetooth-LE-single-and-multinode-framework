/*connection handler fsm*/
#ifndef CONNECTION_HANDLER_BLE_H
#define CONNECTION_HANDLER_BLE_H


#ifdef BLE_APP_CONFIG
#include "ble_app_conf.h" 
#endif

#include <ble_firmware.h>
#include <debug.h>




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


void connection_handler_coriented (connection_t * connection, net_flags * flags);

CHADLE_Status CH_create_connection_BLE(void *connect_config, 
                                    uint8_t peer_addrtype, 
                                    void * peer_addrs);/*used by setup connection by the master node*/


#endif /* CONNECTION_HANDLER_BLE_H */
/**/