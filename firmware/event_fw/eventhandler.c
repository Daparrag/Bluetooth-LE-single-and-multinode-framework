/*event handler file */

/*the idea of this module is to generalize the ble events,
 thus they could be access for any other process in the system*/

/*multiqueue associate to specific type events for efficient resource management*/

/*allows management the queue according to  priorities for high efficient event management*/


#include <eventhandler.h>
#include <list.h>

/*****************************macros**********************************************/
#define COPY_EVENT(dest,source) memcpy(dest,source,sizeof((source)))

/****************** Variable Declaration **************************/
LIST(event_queue); /*definition of the network queue*/
struct event_entry _events [EVENT_QUEUE_SIZE];

event_t _event;
uint8_t start_slot;
uint8_t end_slot;
static const uint8_t max_slots = EVENT_QUEUE_SIZE-1; 

/********************static functions*******************/
static uint8_t HCI_Get_Event_Queue_Size_CB(void);
/********************************************************/

uint8_t HCI_new_Event_CB(void){
  if(list_length(event_queue)) return 1;
  return 0;
}


void HCI_Init_Event_CB(void){
        start_slot = 0;
        end_slot = max_slots;
	list_init(event_queue);
}


uint8_t HCI_Get_Event_Queue_Size_CB(void)
{
  
  return list_length(event_queue);
}


uint8_t HCI_Get_Event_Queue_status_CB(void){
  
    if (list_length(event_queue) == EVENT_QUEUE_SIZE-1) return QUEUE_FULL;
    if (list_length(event_queue) == 0)return QUEUE_EMPTY;
    return QUEUE_NORMAL;
}

uint8_t HCI_Get_Entry_Index_CB(void)
{
/*insert control mechanism  to avoid overwrite slots which are not yet processed */  
  
  uint8_t free_slots;
  uint8_t queue_size = HCI_Get_Event_Queue_Size_CB();

  if(start_slot == end_slot){
    if(HCI_Get_Event_Queue_status_CB() != QUEUE_FULL)
    {
      free_slots = max_slots - queue_size;
        if(end_slot == max_slots){
          start_slot = 0; end_slot = free_slots;
        }else if(end_slot + free_slots > max_slots){
          start_slot = end_slot; end_slot = max_slots;
        }else {
          start_slot = end_slot; end_slot += free_slots;  
        }
      return start_slot;   
    }
       
  }
  
  return start_slot;
}

void HCI_add_Event_CB(struct event_entry * entry){
  
  list_add (event_queue,entry );
  start_slot+=1;
  
}


event_t * HCI_Get_Event_CB(void){
  struct event_entry * top_entry;
  event_t * event;
  top_entry = (struct event_entry *) list_head(event_queue);
  event = &top_entry->event_val;

  return event;
  
    
}

void HCI_clean_last_Event_CB(void){
list_pop (event_queue);
}


void HCI_Event_Handler_CB_(void *pckt){
  hci_uart_pckt *hci_pckt = pckt;
  hci_event_pckt * event_pckt = (hci_event_pckt*)hci_pckt->data;
  if(hci_pckt->type != HCI_EVENT_PKT)return;
  
  switch(event_pckt->evt){
    
    case EVT_DISCONN_COMPLETE:
	 	{
	 		
                  while(1);
	 		
	 	}
	 	break;

	 	case EVT_LE_META_EVENT:
	 	{
	 		  evt_le_meta_event *evt = (void *)event_pckt->data;
	 		   switch(evt->subevent)
	 		   {
	 		   	 case EVT_LE_CONN_COMPLETE:
	 		   	 {
                                   //PRINTDEBUG("EVT_LE_CONN_COMPLETE");
                                    evt_le_connection_complete *cc = (void *)evt->data;
                                    _event.event_type = EVT_LE_CONN_COMPLETE;
                                    _event.evt_data =cc;
                                     network_process(&_event);
                                   

	 		   	 }
	 		   	 break;

	 		   	 case EVT_LE_ADVERTISING_REPORT:
	 		   	 {
	 		   	 	
                                    le_advertising_info *pr = (le_advertising_info*) (((uint8_t*)evt->data)+1);
                                   _event.event_type = EVT_LE_ADVERTISING_REPORT;
                                    _event.evt_data = pr;
                                    network_process(&_event);
                                    // BSP_LED_On(LED2);
                                    // while(1);
	 		   	 }
	 		   	 break;
                                 
                                 case EVT_BLUE_GAP_DEVICE_FOUND:
                                   {
                                   /*IDB04A1*/
                                   }
                                   break;
	 		   }
	 	}
	 	break;
                case EVT_VENDOR:
                {
                  evt_blue_aci * blue_evt = (void*)event_pckt->data;
                    switch(blue_evt->ecode)
                    {
                        case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
                          {
                            _event.event_type=EVT_BLUE_GATT_PROCEDURE_COMPLETE;
                            evt_gatt_procedure_complete * pr = (void*)blue_evt->data;
                              _event.evt_data = pr;
                              network_process(&_event);
                          }
                          break;
                      
                    
                    }
                    
                }
                break;
                
  }
}


void HCI_Event_Handler_CB(void *pckt){
uint8_t index_queue;
  hci_uart_pckt *hci_pckt = pckt;
  hci_event_pckt * event_pckt = (hci_event_pckt*)hci_pckt->data;
  index_queue = HCI_Get_Entry_Index_CB();

	if(hci_pckt->type != HCI_EVENT_PKT)return;
	if(HCI_Get_Event_Queue_status_CB() == QUEUE_FULL) return;

	switch(event_pckt->evt){
	 	case EVT_DISCONN_COMPLETE:
	 	{
	 		
	 		//COPY_EVENT (&_events[index_queue].event_val.event_data, event_pckt);
	 		_events[index_queue].event_val.event_type = EVT_DISCONN_COMPLETE;
	 		
	 	}
	 	break;

	 	case EVT_LE_META_EVENT:
	 	{
	 		  evt_le_meta_event *evt = (void *)event_pckt->data;
	 		   switch(evt->subevent)
	 		   {
	 		   	 case EVT_LE_CONN_COMPLETE:
	 		   	 {
	 		   	 	//COPY_EVENT (&_events[index_queue].event_val.event_data,event_pckt);
	 		   	 	_events[index_queue].event_val.event_type = EVT_LE_CONN_COMPLETE;
                                        HCI_add_Event_CB(&_events[index_queue]);
                                        

	 		   	 }
	 		   	 break;

	 		   	 case EVT_LE_ADVERTISING_REPORT:
	 		   	 {
	 		   	 	
                                 //   le_advertising_info *pr = (le_advertising_info*) (((uint8_t*)evt->data)+1);
                                 //   _events[index_queue].event_val.event_type = EVT_LE_ADVERTISING_REPORT;
                             
                                    // BSP_LED_On(LED2);
                                    // while(1);
	 		   	 }
	 		   	 break;
                                 
                                 case EVT_BLUE_GAP_DEVICE_FOUND:
                                   {
                                   /*IDB04A1*/
                                   }
                                   break;

	 		   

	 		   }
	 	}
	 	break;
	}

    

}





