#ifndef _COMMON_TABLES_
#define _COMMON_TABLES_
#ifdef __cplusplus
extern "C" {

#endif
/*includes**************************/
#ifdef BLE_APP_CONFIG
#include  <blefw_conf.h>
#endif

#include "bluenrg_gap.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_gatt_server.h"	

#ifdef __cplusplus
} 
#endif

extern const uint16_t event_common_table [9];

#endif /*_COMMON_TABLES_*/