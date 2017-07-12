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
#define GENERAL_DISCOVERY       (0x0)                           /*!< Used for setup the general perispheral discovery  >*/
#endif
#ifndef SELECTIVE_DISCOVERY
#define SELECTIVE_DISCOVERY     (0x1)                           /*!< Used for setup a selective perispheral discovery  >*/
#endif
#ifndef SPECIFIC_DISCOVERY
#define SPECIFIC_DISCOVERY      (0x2)                           /*!< Used for setup a specific perispheral discovery   >*/
#endif

#ifndef DISCOVERY_MODE
#define DISCOVERY_MODE          GENERAL_DISCOVERY               /*!< Use one of the follow: GENERAL_DISCOVERY, SELECTIVE_DISCOVERY, SPECIFIC_DISCOVERY  >*/
#endif 

/*connection setup default parameters*/
#ifndef SCAN_P
#define SCAN_P  		(0x0005)      		             /*!< Scan Interval 40ms.*/			
#endif
#ifndef SCAN_L
#define SCAN_L  		(0x0005)      		             /*!< Scan Window.  20ms*/
#endif
#ifndef	CONN_P1
#define CONN_P1 		((int)((20)/1.25f))  	         /*!< Min connection interval in ms.*/
#endif
#ifndef	CONN_P2
#define CONN_P2			((int)((20)/1.25f))  	         /*!< Max connection interval in ms.*/
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
#define OUR_ADDRS_TYPE PUBLIC_ADDR              /*!< Address Type.*/           
#endif




/*scan setup default parameters*/

#ifndef SCAN_INTV
#define SCAN_INTV       (0x28)                  /*<!  scanning interval  >*/
#endif
#ifndef SCAN_WIN
#define SCAN_WIN	(0x28)                 /*<!  scanning windows  >*/
#endif
/*additional parameters for selective scanning*/
#ifndef SCAN_TYPE
#define SCAN_TYPE       (0x01)                /*<!  scanning type: pasive or active  >*/
#endif


/*advertise default configuration*/
#ifndef ADV_EVT_TYPE
#define ADV_EVT_TYPE  ADV_IND               /*<!  advertisement event type  >*/
#endif

#ifndef ADV_IT_MIN
#define ADV_IT_MIN    (0x0040)                /*<!  advertisement interval min  >*/
#endif

#ifndef ADV_IT_MAX
#define ADV_IT_MAX    (0x0040)                /*<!  advertisement interval max  >*/  
#endif

#ifndef ADV_ADDR_TYPE
#define ADV_ADDR_TYPE PUBLIC_ADDR           /*<!  advertisement address type  >*/        
#endif

#ifndef ADV_POLICY
#define ADV_POLICY  NO_WHITE_LIST_USE       /*<!  advertisement policy  >*/
#endif

#ifndef SLAVE_INT_MIN
#define SLAVE_INT_MIN   (0x0020)            /*<!  slave connection interval min  >*/
#endif

#ifndef SLAVE_INT_MAX
#define SLAVE_INT_MAX   (0x0020)            /*<!  slave connection interval max  >*/
#endif



/******************************************CONFIGURATION FUNTIONS***************************/
void connection_handler_set_discovery_config(app_discovery_t * dv_conf); /*used for setup a specific discovery configuration */

void connection_handler_set_advertise_config(app_advertise_t  * adv_conf);/*used for setup a specific adverticement configuration */
/******************************************EVENT DEPENDENT FUNTIONS***************************/
CHADLE_Status CH_Connection_Complete_BLE(connection_t * connection, uint16_t handle, uint8_t  peer_addrs[6]);/*<! Used for the central node once a connection with a peer device is completed >*/

CHADLE_Status CH_Connection_Complete_perispheral_BLE(connection_t * connection, uint16_t handle, uint8_t peer_addrs[6]);/*<! Used by a perispheral  once a connection with a cntral node is completed >*/

CHADLE_Status CH_new_device_found_BLE(connection_t * connection, void * pr);	/*<! Fire for the central node when a new peer device has been found>*/

CHADLE_Status CH_finish_the_connection_BLE(void); /*<! Used by any device to finish the connection setup procedure >*/

void connection_handler_error(void);

/******************************************GENERAL PURPOUSE FUNTIONS***************************/

CHADLE_Status CH_run_discovery_BLE(void);	/*<! fire the discovery procedure >*/

CHADLE_Status CH_run_selective_discovery_BLE(void);	/*<! fire the selective discovery procedure >*/

CHADLE_Status CH_run_create_connection_BLE(connection_t * connection);	/*<! fire the connection procedure >*/

CHADLE_Status CH_finish_the_connection_BLE(void); /*<! Used by any device to finish the connection setup procedure >*/

CHADLE_Status CH_run_advertise_BLE(void);/*<! Used for enable the device to be advertise by other peer devices >*/ 



#endif /* CONNECTION_HANDLER_BLE_H */
/**/