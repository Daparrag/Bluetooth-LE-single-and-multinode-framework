#ifndef APP_BLE_H
#define APP_BLE_H

/* Includes ------------------------------------------------------------------*/
#include <ble_firmware.h>

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



/*advertise default configuration*/
#ifndef ADV_EVT_TYPE
#define ADV_EVT_TYPE  ADV_IND               /*<!  advertisement event type  >*/
#endif

#ifndef ADV_IT_MIN
#define ADV_IT_MIN    (0x00)                /*<!  advertisement interval min  >*/
#endif

#ifndef ADV_IT_MAX
#define ADV_IT_MAX    (0x00)                /*<!  advertisement interval max  >*/  
#endif

#ifndef ADV_ADDR_TYPE
#define ADV_ADDR_TYPE PUBLIC_ADDR           /*<!  advertisement address type  >*/        
#endif

#ifndef ADV_POLICY
#define ADV_POLICY  NO_WHITE_LIST_USE       /*<!  advertisement policy  >*/
#endif

#ifndef SLAVE_INT_MIN
#define SLAVE_INT_MIN   (0x00)            /*<!  slave connection interval min  >*/
#endif

#ifndef SLAVE_INT_MAX
#define SLAVE_INT_MAX   (0x00)            /*<!  slave connection interval max  >*/
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





/*could be removed since the network has to be independent of the node*/
typedef struct{                     /*central node type structure*/
  network_t net;
  dv_state_t central_status;
}central_node_t;


typedef struct{                    /* simple pherisperal node type structure*/
  dv_state_t pherispheral_status;
}spherispheral_node_t;

typedef central_node_t broadcast_node_t; /*broadcast node type structure*/
typedef spherispheral_node_t observer_node_t;  /*observer node type structure*/
typedef central_node_t cpherispheral_node_t; /*complex pherispheral node*/


/**
* global_func_declaration
*/
APP_Status APP_Init_BLE(void); /*init the BLE_arch*/
APP_Status APP_init_BLE_Profile(app_profile_t * profile);/*init the BLE app*/
APP_Status APP_add_BLE_Service(app_profile_t * profile, app_service_t * service);/*add BLE services*/
APP_Status APP_add_BLE_attr(app_service_t * service, app_attr_t *attr);/*add BLE attribute*/
//APP_Status APP_set_discovery_BLE(void * dicovery_config);/*this is used for receive advertisements called by clients*/
APP_Status APP_set_advertise_BLE(void * advertise_conf, 
                                uint8_t scanres_data_size,
                                void * scanres_data,
                                uint8_t serviceuuidlength, 
                                void * serviceuuidlist);/*server generate advertisements to clients*/
//APP_Status APP_create_connection_BLE(void *connect_config, 
//                                    uint8_t peer_addrtype, 
//                                    void * peer_addrs);/*used by setup connection by the master node*/


void * APP_get_service_BLE(app_profile_t * profile, void * service);

void * APP_get_attribute_BLE(app_service_t * service, void *attr);


void APP_get_default_config_BLE(void *app_discovery,void * app_connection,void * app_advertise); /*used for retrieve the default config*/


void * APP_get_direct_addrs_BLE(int * size);/* used for retreive the addrs of the BLE device*/

void * APP_get_direct_name_BLE(int * size); /*used for retrieve the name of the BLE device*/




#endif /* PTP_BLE_H */