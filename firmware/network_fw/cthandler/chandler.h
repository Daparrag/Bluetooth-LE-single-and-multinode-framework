/*connection handler fsm*/
#ifndef CONNECTION_HANDLER_BLE_H
#define CONNECTION_HANDLER_BLE_H


#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif

#ifdef DEBUG
#include "debug.h"
#endif

#include <ble_firmware.h>
#include <stm32f4xx_hal.h>
#include <app_ble.h>
#include <debug.h>

/*discovery modes*/
#ifndef GENERAL_DISCOVERY
#define GENERAL_DISCOVERY       (0x0)
#endif
#ifndef SELECTIVE_DISCOVERY
#define SELECTIVE_DISCOVERY     (0x1)
#endif
#ifndef SPECIFIC_DISCOVERY
#define SPECIFIC_DISCOVERY      (0x2)
#endif

#ifndef DISCOVERY_MODE
#define DISCOVERY_MODE          GENERAL_DISCOVERY
#endif 

/*connection setup default parameters*/
#ifndef SCAN_P
#define SCAN_P  		(0x0005)      		             /*!< Scan Interval 40ms.*/			
#endif
#ifndef SCAN_L
#define SCAN_L  		(0x0005)      		             /*!< Scan Window.  20ms*/
#endif
#ifndef	CONN_P1
#define CONN_P1 		((int)((50)/1.25f))  	         /*!< Min connection interval in ms.*/
#endif
#ifndef	CONN_P2
#define CONN_P2			((int)((50)/1.25f))  	         /*!< Max connection interval in ms.*/
#endif
#ifndef	SUPERV_TIMEOUT
#define SUPERV_TIMEOUT  (3200)         		         /*!< Supervision timeout.*/
#endif
#ifndef	CONN_L1
#define CONN_L1         ((int)((12)/0.625f))   	   /*!< Min connection length.*/
#endif
#ifndef	CONN_L2
#define CONN_L2         ((int)((12)/0.625f))   	   /*!< Min connection length.*/
#endif
#ifndef	LATENCY
#define LATENCY			(0)
#endif

#ifndef OUR_ADDRS_TYPE
#define OUR_ADDRS_TYPE PUBLIC_ADDR
#endif


/*scan setup default parameters*/
#ifndef SLNODES
#define SLNODES		(0x0002)
#endif
#ifndef SCAN_INTV
#define SCAN_INTV       (0x60)
#endif
#ifndef SCAN_WIN
#define SCAN_WIN	  (0x60)
#endif
/*additional parameters for selective scanning*/
#ifndef SCAN_TYPE
#define SCAN_TYPE       (0x01)
#endif

#ifndef LED_TOGGLE_CONFIG
#define LED_TOGGLE_UNESTABLISHED      80000
#define LED_TOGGLE_DISCOVERY          80000
#define LED_TOGGLE_CONNECTED          30000
#endif



//void connection_handler_coriented (connection_t * connection, event_t * event);

CHADLE_Status CH_set_discovery_BLE(void * dicovery_config);

CHADLE_Status CH_create_connection_BLE(void *connect_config, 
                                    uint8_t peer_addrtype, 
                                    void * peer_addrs);/*used by setup connection by the master node*/


CHADLE_Status CH_Connection_Complete_BLE(connection_t * connection, uint16_t handle, uint8_t  peer_addrs[6]);

CHADLE_Status CH_Connection_Complete_perispheral_BLE(connection_t * connection, uint16_t handle, uint8_t peer_addrs[6]);


CHADLE_Status CH_set_discovery_specific_BLE(void * dicovery_config);

CHADLE_Status CH_set_discovery_limited_BLE(void * dicovery_config);

CHADLE_Status CH_set_selective_discovery_BLE(void * dicovery_config);


CHADLE_Status CH_run_discovery_BLE(void);

CHADLE_Status CH_run_selective_discovery_BLE(void);

CHADLE_Status CH_run_create_connection_BLE(connection_t * connection);

CHADLE_Status CH_new_device_found_BLE(connection_t * connection, void * pr);

/*control flag function */

void set_connection_wait_procedure(void);

void clean_connection_wait_procedure(void);

uint8_t get_connection_end_procedute(void);

/*configuration functions*/
void connection_handler_set_discovery_config(app_discovery_t * dv_conf);


CHADLE_Status CH_run_advertise_BLE(void);


#endif /* CONNECTION_HANDLER_BLE_H */
/**/