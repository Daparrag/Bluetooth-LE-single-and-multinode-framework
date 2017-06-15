/*event handler header*/
#ifndef _EVT_HAHDLER_H_
#define _EVT_HAHDLER_H_

#define EVENT_QUEUE_SIZE	8

#include <ble_firmware.h>


struct event_entry{
	struct * next;
	event_t event_val;
}

void HCI_Init_Event_CB(void);
void HCI_Event_Handler_CB(void *pckt);
uint8_t HCI_Get_Event_CB(event_t * event);
#endif /* _EVT_HAHDLER_H_ */
