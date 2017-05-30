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
	uint16_t LE_Timeout_;
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
	app_Master_broadcast_conf M_conf;
	app_Slave_observer_conf  S_conf;
}gatt_conf;




#endif /* _MNODE_GATT_H_ */