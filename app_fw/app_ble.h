#ifndef APP_BLE_H
#define APP_BLE_H

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hci_le.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"
#include "role_type.h"
#include "uart_support.h"
#include "stm32_bluenrg_ble.h"
#include <list.h>

#ifdef BLE_APP_CONFIG
#include "ble_app_conf.h" 
#endif

#ifdef DEBUG
#include "debug.h"
#endif

/*used the valid roles*/
/*valid roles
*GAP_PERIPHERAL_ROLE
*GAP_BROADCASTER_ROLE
*GAP_CENTRAL_ROLE
*GAP_PERIPHERAL_ROLE
*/

#ifndef CONCAT_AUX
#define CONCAT_AUX(_x,_y) _x##_y
#endif

#ifndef CONCAT
#define CONCAT(x,y) CONCAT_AUX(x, y)
#endif

#ifndef ROLE
#define ROLE GAP_CENTRAL_ROLE
#endif



#define GET_ROLE(arch) ((arch)==0 ? CONCAT(ROLE,_IDB04A1) : CONCAT(ROLE,_IDB05A1))



/*configuration Variables*/
#ifndef IDB04A1
#define IDB04A1 0
#endif

#ifndef IDB05A1
#define IDB05A1 1
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
#define SUPERV_TIMEOUT  (100)         		             /*!< Supervision timeout.*/
#endif
#ifndef	CONN_L1
#define CONN_L1         ((int)((5)/0.625f))   	         /*!< Min connection length.*/
#endif
#ifndef	CONN_L2
#define CONN_L2         ((int)((5)/0.625f))   	         /*!< Min connection length.*/
#endif
#ifndef	LATENCY
#define LATENCY			(0)
#endif

/*scan setup default parameters*/
#ifndef SLNODES
#define SLNODES		(0x0002)
#endif
#ifndef SCAN_INTV
#define SCAN_INTV   (200)
#endif
#ifndef SCAN_WIN
#define SCAN_WIN	  (5)
#endif


/*device address parameters*/
/*address setup defaut parameters*/
#ifndef	CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_OFFSET     (0x00) /**< Bluetooth public address or random*/
#endif

#ifndef CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_LEN		   (6)
#endif

/*Local_Macros*/
#define MY_HAVE_IDB0xA1(arch, func_name) ((arch) == 1 ?func_name##_IDB05A1:func_name##_IDB04A1)/*concatenation of the funtion name and the architecture*/
#define BLE_ARCH_MASK 1 /*used for reconize if we have a IDB04A1 or IDB05A1 architecture  BLE_ARCH_MASK is = #architectures -1 */

#ifndef UUID_TYPE
#define UUID_TYPE UUID_TYPE_128
#endif

#ifndef SERVICE_TYPE
#define SERVICE_TYPE PRIMARY_SERVICE
#endif



typedef enum /*used for return the result of and operation on the application*/
{
  APP_SUCCESS = 0x00,            /*!< command success applied */
  APP_ERROR = 0x01,              /*!< and error occour during the command execution*/
  APP_NOT_CONFIG= 0x02          /*!< not configuration is present before to applied a command*/
} APP_Status; 

typedef struct{
  LIST_STRUCT(_value);
  uint8_t CharUUID[16];            /*!< Control characteristic UUID.*/
  uint8_t charUuidType;            /*!< Control characteristic UUID_TYPE. 16 or 128 bits*/   
  uint8_t charValueLen;            /*!< Current length of the characteristic description*/
  uint8_t charProperties;          /*!< Characteristic properties.*/
  uint8_t secPermissions;         /*!< Permissions*/
  uint8_t gattEvtMask;            /*!< Type of event generated by GATT server*/
  uint8_t encryKeySize;           /*!< minimum encription key size requirement for this attr: 7 to 16*/
  uint8_t isVariable;             /*!< 0x00:fixed attr length 0x01:var attr length*/
  uint16_t CharHandle;             /*!< characteristic handle.*/
  uint8_t n_val;                  /*!< control counter of the number of values associate to this characteristic */
}app_attr_t;


typedef struct{
  LIST_STRUCT(_attr);
  uint8_t ServiceUUID[16];              /*!<Control service UUID.*/
  uint16_t ServiceHandle;               /*!< Service handle.*/
  uint8_t service_uuid_type;            /*!<Control service UUID_TYPE. 16 or 128 bits*/
  uint8_t service_type;                 /*!<Type of service (primary or secondary) */
  uint8_t max_attr_records;             /*!< Maximum number of att-records that can be add to this service*/
  uint8_t n_attr;                       /*!< Control counter of the number of attributes add to this service*/
}app_service_t;


typedef struct{
  LIST_STRUCT(_service);
  uint8_t n_service;                  /*!< Control counter of the number of services associate to this application*/
}app_profile_t;

/**
* global_func_declaration
*/
APP_Status APP_Init_BLE(void); /*init the BLE_arch*/
APP_Status APP_init_BLE_Profile(app_profile_t * profile);/*init the BLE app*/
APP_Status APP_add_BLE_Service(app_profile_t * profile, app_service_t * service);/*add BLE services*/
APP_Status APP_add_BLE_attr(app_service_t * service, app_attr_t *attr);/*add BLE attribute*/



#endif /* PTP_BLE_H */