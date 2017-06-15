/*event handler file */

/*the idea of this module is to generalize the ble events,
 thus they could be access for any other process in the system*/

/*multiqueue associate to specific type events for efficient resource management*/


#include <eventhandler.h>
#include <list.h>

/*****************************macros**********************************************/
#define COPY_EVENT(dest,source) memcpy(dest,(void*)source,sizeof((hci_event_pckt)))

/****************** Variable Declaration **************************/
volatile uint8_t new_event=0; /*control flag*/
LIST(event_queue); /*definition of the network queue*/
struct event_entry _events [EVENT_QUEUE_SIZE];

/********************static functions*******************/
static void HCI_Set_NET_Event_CB(void);
static void HCI_Clean_NET_Event_CB(void);
static uint8_t HCI_Get_Event_Queue_Size_CB(void);




/********************************************************/

void HCI_Set_NET_Event_CB(void){
	new_event =1;
}


void HCI_Clean_NET_Event_CB(void){
	new_event =0;
	list_pop(event_queue);

}




void HCI_Init_Event_CB(void){

	list_init(event_queue);
}

uint8_t HCI_Get_Event_Queue_Size_CB(void)
{
	

	return list_length(event_queue);

}


void HCI_Event_Handler_CB(void *pckt){
uint8_t index_queue;
  hci_uart_pckt *hci_pckt = pckt;
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  index_queue = HCI_Get_Event_Queue_Size_CB();

	if(hci_pckt->type != HCI_EVENT_PKT)return;
	if(index_queue >= EVENT_QUEUE_SIZE ) return;

	switch(event_pckt->evt){
	 	case EVT_DISCONN_COMPLETE:
	 	{
	 		
	 		COPY_EVENT (&_events[index_queue].event_data,event_pckt);
	 		_events[index_queue].event_type = EVT_DISCONN_COMPLETE;
	 		HCI_Set_Event_CB();
	 	}
	 	break;

	 	case EVT_LE_META_EVENT:
	 	{
	 		  evt_le_meta_event *evt = (void *)event_pckt->data;
	 		   switch(evt->subevent)
	 		   {
	 		   	 case EVT_LE_CONN_COMPLETE:
	 		   	 {
	 		   	 	COPY_EVENT (&_events[index_queue].event_data,event_pckt);
	 		   	 	_events[index_queue].event_type = EVT_LE_CONN_COMPLETE;

	 		   	 }
	 		   	 break;

	 		   	 case EVT_LE_ADVERTISING_REPORT:
	 		   	 {
	 		   	 	COPY_EVENT (&_events[index_queue].event_data,event_pckt);
	 		   	 	_events[index_queue].event_type = EVT_LE_ADVERTISING_REPORT;

	 		   	 }
	 		   	 break;

	 		   	 case DEVICE_FOUND:
	 		   	 {
	 		   	 	/* X-NUCLEO-IDB04A1*/
	 		   	 }

	 		   }
	 	}
	 	break;
	}

    

}



uint8_t HCI_Get_Event_CB(event_t * event){

	if(list_length) {

		/*assing the event.*/
	return 1;	
	}

return 0;
	

}


