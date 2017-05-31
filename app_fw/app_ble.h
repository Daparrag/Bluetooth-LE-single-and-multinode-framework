#ifndef APP_BLE_H
#define APP_BLE_H

#ifdef BLE_APP_CONFIG
#include "ble_app_conf.h" 
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
#define PRIMARY_SERVICE
#endif



typedef struct 
{
	uint8_t offset
	uint8_t len
	const uint8_t *val;
}addr_conf;


typedef enum 
{
  APP_SUCCESS = 0x00,
  APP_ERROR = 0x10,
  APP_NOT_CONFIG= 0x02
} APP_Status;



#endif /* PTP_BLE_H */