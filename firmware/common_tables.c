/* This is a common constant event table used to evalue as fast as possible the arrival 
 * events in the fsm used by the  applications and/or by the BLUENRG modules  
 */

/* This list only contain few events
 * if the event that your application need to process is not 
   present */
#include "common_tables.h"

const uint16_t event_common_table[9]=
{
	EVT_DISCONN_COMPLETE,
	EVT_LE_CONN_COMPLETE,
	EVT_LE_ADVERTISING_REPORT,
	EVT_BLUE_GATT_PROCEDURE_COMPLETE,
	EVT_BLUE_GATT_ATTRIBUTE_MODIFIED,
	EVT_BLUE_GATT_WRITE_PERMIT_REQ,
	EVT_BLUE_GATT_NOTIFICATION,
	EVT_BLUE_ATT_FIND_BY_TYPE_VAL_RESP,
	EVT_BLUE_GATT_DISC_READ_CHAR_BY_UUID_RESP
};



