/*API_Config*/

#ifndef _MNODE_GATT_H_
#define _MNODE_GATT_H_

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "hal_types.h"
#include "hci_const.h"
#include "bluenrg_gap.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_utils.h"
#include "sm.h"
#include "stm32_bluenrg_ble.h"



typedef enum 
{
  MNODE_OPT_SUCCESS = 0x00,
  MNODE_OPT_ERROR = 0x10,
  MNODE_NOT_CONFIG= 0x02
} MNODE_GATT_Status;


typedef struct scan_adv_conf_{
	uint16_t LE_scan_interval;
	uint16_t LE_scan_windows;
}SC_adv_conf;/*scan adverticement configuration*/


typedef struct connection_setup_conf_{
	uint16_t LE_scan_interval;
	uint16_t LE_scan_windows;
	uint16_t LE_CI_max;
	uint16_t LE_CI_min;
	uint16_t Conn_Latency;
	uint16_t LE_Timeout;
	uint16_t Conn_Len_Min;
	uint16_t Conn_Len_Max;
}CN_setup_conf; /*conf Connection Setup Master to slave*/


typedef struct gatt_M_conf_{
	uint16_t num_slaves;
	SC_adv_conf scan_adv_c;
	CN_setup_conf CN_c;
}gatt_Master_Broadcast_conf;



typedef struct gatt_S_conf_{
	uint16_t num_masters;
	uint16_t LE_adv_int_min;
	uint16_t LE_adv_int_max;
}gatt_Slave_Observer_conf;


typedef struct gatt_config_{
	uint32_t role;
	gatt_Master_Broadcast_conf M_conf;
	gatt_Slave_Observer_conf  S_conf;
}gatt_conf;

MNODE_GATT_Status _GATT_MULT_NODE_SET_DEFAULT_CONFIG(gatt_conf * multinode);
MNODE_GATT_Status _GATT_MULT_NODE_SET_DEVICE_ADDR(uint8_t offset, uint8_t len,const uint8_t *val);
MNODE_GATT_Status _GATT_MULT_NODE_DISCOVERY_PROCEDURE(gatt_conf * multinode);
MNODE_GATT_Status _GATT_MULT_NODE_SETUP_CONNECTION(gatt_conf * multinode, uint32_t peer_addr_type,uint8_t *Peer_Address,uint32_t Own_Address_Type);
MNODE_GATT_Status _GATT_MULT_NODE_INIT_GATT(void);
MNODE_GATT_Status _GATT_MULT_SERVER_ADD_SERVICE(uint8_t *service_uuid,uint16_t *service_handle);
MNODE_GATT_Status _GATT_MULT_NODE_TEST_MODULE_MASTER(void);


#endif /* _MNODE_GATT_H_ */