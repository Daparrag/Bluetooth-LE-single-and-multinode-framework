/*event handler header*/
#ifndef _EVT_HAHDLER_H_
#define _EVT_HAHDLER_H_

#define EVENT_QUEUE_SIZE	8

#ifdef BLE_APP_CONFIG
#include "blefw_conf.h" 
#endif


#include <ble_firmware.h>
#include <network.h>

#ifdef DEBUG
#include "debug.h"
#endif


#define QUEUE_FULL      0x1
#define QUEUE_EMPTY     0x0
#define QUEUE_NORMAL    0x2

struct event_entry{
	struct event_entry * next;
	event_t event_val;
};

void HCI_Init_Event_CB(void);
void HCI_Event_Handler_CB(void *pckt);
uint8_t HCI_new_Event_CB(void);
event_t * HCI_Get_Event_CB(void);
void HCI_clean_last_Event_CB(void);
void HCI_Event_Handler_CB_(void *pckt);

#endif /* _EVT_HAHDLER_H_ */
