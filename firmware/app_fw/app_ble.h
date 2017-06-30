#ifndef APP_BLE_H
#define APP_BLE_H

/* Includes ------------------------------------------------------------------*/
#include <ble_firmware.h>
#include <eventhandler.h>
#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif

#ifdef DEBUG
#include "debug.h"
#endif

/*used the valid roles*/
/*valid roles
*GAP_PERIPHERAL_ROLE
*GAP_BROADCASTER_ROLE
*GAP_CENTRAL_ROLE
*GAP_OBSERVER_ROLE
*/

#ifndef CONCAT_AUX
#define CONCAT_AUX(_x,_y) _x##_y
#endif

#ifndef CONCAT
#define CONCAT(x,y) CONCAT_AUX(x, y)
#endif

#ifndef ROLE
#error you have to dedefine one of the following roles GAP_PERIPHERAL_ROLE,\
       GAP_BROADCASTER_ROLE, GAP_OBSERVER_ROLE or GAP_CENTRAL_ROLE in blefw_conf.h.         
#endif



#define GET_ROLE(arch) ((arch)==0 ? CONCAT(ROLE,_IDB04A1) : CONCAT(ROLE,_IDB05A1))


/*configuration Variables*/
#ifndef IDB04A1
#define IDB04A1 0
#endif

#ifndef IDB05A1
#define IDB05A1 1
#endif




/*device address parameters*/
/*address setup defaut parameters*/
#ifndef	CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_OFFSET     (0x00) /**< Bluetooth public address or random*/
#endif

#ifndef CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_LEN		   (6)	/**< Bluetooth public address size (bytes)*/
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


/**
* global_func_declaration
*/
APP_Status APP_Init_BLE(void); /*init the BLE_arch*/
APP_Status APP_init_BLE_Profile(app_profile_t * profile);/*init the BLE profile */
APP_Status APP_add_BLE_Service(app_profile_t * profile, app_service_t * service);/*add BLE services*/
APP_Status APP_add_BLE_attr(app_service_t * service, app_attr_t *attr);/*add BLE attribute*/
void * APP_get_service_BLE(app_profile_t * profile, void * service);/*return each one of the services (one at time) associated to a profile*/
void * APP_get_attribute_BLE(app_service_t * service, void *attr);/*return each one of the attributes (one at time) associated to a service*/
void * APP_get_direct_addrs_BLE(int * size);/* used for retreive the addrs of the BLE device*/
void * APP_get_direct_name_BLE(int * size); /*used for retrieve the name of the BLE device*/
uint8_t get_harware_version(void); /*return the harware version IDB0xA1*/
const char * get_local_name (void);/*return pointer to the local name*/
uint8_t  get_local_name_size (void);/*return the size of the local name*/

#endif /* PTP_BLE_H */