/*event handler header*/
#ifndef _EVT_HAHDLER_H_
#define _EVT_HAHDLER_H_

#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif


#ifndef	EVENT_QUEUE_SIZE
#define EVENT_QUEUE_SIZE	8		/*for enhance propouse*/
#endif


#include <ble_firmware.h>
#include <network.h>

#ifdef DEBUG
#include "debug.h"
#endif


#define QUEUE_FULL      0x1			/*for enhance propouse*/		
#define QUEUE_EMPTY     0x0			/*for enhance propouse*/
#define QUEUE_NORMAL    0x2			/*for enhance propouse*/

/*we could overwrite this structure to include some advanced charactersitics */
typedef struct _tHciDataPacket
{
  tClockTime Isr_timestamp; /*received package timestamp*/	
  tListNode currentNode;
  uint8_t dataBuff[HCI_READ_PACKET_SIZE];
  uint8_t data_len;
} tHciDataPacket;





struct event_entry{					/*for enhance propouse*/
	struct event_entry * next;
	event_t event_val;
};

void HCI_Init_Event_CB(void); /*<! Used for initialize the event handler module >*/
void HCI_Event_Handler_CB(void *pckt);	/*does not work*/
uint8_t HCI_new_Event_CB(void);			/*for enhance propouse*/
event_t * HCI_Get_Event_CB(void);		/*for enhance propouse*/
void HCI_clean_last_Event_CB(void);		/*for enhance propouse*/
void HCI_Event_Handler_CB_(void *pckt);	/*current connection handler implementation.*/

#endif /* _EVT_HAHDLER_H_ */
