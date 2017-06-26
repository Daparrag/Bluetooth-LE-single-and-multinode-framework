/*event handler file */

/*the idea of this module is to generalize the ble events,
 thus they could be access for any other process in the system*/

/*multiqueue associate to specific type events for efficient resource management*/

/*allows management the queue according to  priorities for high efficient event management*/


#include "hal_types.h"
#include "osal.h"
#include "ble_status.h"
#include "hal.h"
#include "hci_const.h"
#include "gp_timer.h"
#include <eventhandler.h>

#include "stm32_bluenrg_ble.h"
/*****************************macros**********************************************/
#define COPY_EVENT(dest,source) memcpy(dest,source,sizeof((source)))


#define _EH_HCI_READ_PACKET_NUM_MAX 		 (5)

#define MIN(a,b)            ((a) < (b) )? (a) : (b)
#define MAX(a,b)            ((a) > (b) )? (a) : (b)

/****************** Variable Declaration **************************/


event_t _event;
tListNode EH_hciReadPktPool;
tListNode EH_hciReadPktRxQueue;
/* pool of hci read packets */
static tHciDataPacket     hciReadPacketBuffer[_EH_HCI_READ_PACKET_NUM_MAX];
static volatile uint8_t hci_timer_id;
static volatile uint8_t hci_timeout;

/********************static functions*******************/

/********************************************************/


void EH_hci_timeout_callback(void)
{
  hci_timeout = 1;
  return;
}


/**
  * @brief  Used for initialized the event handler module.
  * @param none.
  * @retval none,
  */

void HCI_Init_Event_CB(void){
  uint8_t index;
  
  /* Initialize list heads of ready and free hci data packet queues */
  list_init_head (&EH_hciReadPktPool);
  list_init_head (&EH_hciReadPktRxQueue);
  
  /* Initialize the queue of free hci data packets */
  for (index = 0; index < _EH_HCI_READ_PACKET_NUM_MAX; index++)
  {
    list_insert_tail(&EH_hciReadPktPool, (tListNode *)&hciReadPacketBuffer[index]);
  }
        
}



/**
  * @brief  This function return the number of pending events .
  * @param none.
  * @retval uint8_t: return  the number of pending events
  */
uint8_t HCI_Get_Event_Queue_Size_CB(void)
{
  
 return 0;
}


/**
  * @brief  This function retreve status of the queeue: empty, full, normal .
  * @param none.
  * @retval uint8_t: Status of the event queue. 
  */

uint8_t HCI_Get_Event_Queue_status_CB(void){
  
    return 0;
}


/**
  * @brief return a free slot in the queue for attach a new event.
  * @param none.
  * @retval uint8_t: slot index. 
  */

uint8_t HCI_Get_Entry_Index_CB(void)
{
/*insert control mechanism  to avoid overwrite slots which are not yet processed */  
  
 
  
  return 0;
}


/**
  * @brief add and event in the index queue.
  * @param struct event_entry * entry : event structure
  * @retval none. 
  */

void HCI_add_Event_CB(struct event_entry * entry){
  

  
}




/**
  * @brief event handler whitout queue.
  * @param void *pckt: pointer to an input packet
  * @retval none . 
  */
void HCI_Event_Handler_CB_(void *pckt){/*this version does not uses the event handler queue*/
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

/**
  * @brief event handler using queue.(does not works at the moment)
  * @param void *pckt: pointer to an input packet
  * @retval none . 
  */


uint8_t Packet__Get_Priority(void){
  return 0;
}


int HCI__Event_verify_CB(const tHciDataPacket_enhnalce * hciReadPacket)
{
  const uint8_t *hci_pckt = hciReadPacket->dataBuff;
  
  if(hci_pckt[_EH_HCI_PCK_TYPE_OFFSET] != HCI_EVENT_PKT)
    return 1;  /* Incorrect type. */
  
  if(hci_pckt[_EH_EVENT_PARAMETER_TOT_LEN_OFFSET] != hciReadPacket->data_len - (1+HCI_EVENT_HDR_SIZE))
    return 2; /* Wrong length (packet truncated or too long). */
  
  return 0;      
}


void HCI_Isr_Event_Handler__CB(){
  /*Here had been modified the HCI_Isr method to allows:
  *
  *1. timestamp for all imput packages.
  *2. allocate the input packets into the packet queue with priorities.
  *
  */

  tHciDataPacket_enhnalce * hciReadPacket = NULL;
  uint8_t data_len;
  tClockTime HCI_Isr_time = Clock_Time(); /*this is the current time in which the packet was transfered from the PoolQueue to the ReadRXqueue*/
  Clear_SPI_EXTI_Flag();

   while(BlueNRG_DataPresent()){ 
      if(list_is_empty (&EH_hciReadPktPool) == FALSE){

         /* enqueueing a packet for read */
        list_remove_head (&EH_hciReadPktPool, (tListNode **)&hciReadPacket);
        data_len = BlueNRG_SPI_Read_All(hciReadPacket->dataBuff, HCI_READ_PACKET_SIZE);
        if(data_len > 0){
          hciReadPacket->data_len = data_len;
          if(HCI__Event_verify_CB(hciReadPacket) == 0){
            hciReadPacket->Isr_timestamp = 0;

            if(Packet_Get_Priority()!=0){
                /*insert packet according to the priority level */

            }else{
              /*if not priority insert in the tail*/
              list_insert_tail(&EH_hciReadPktRxQueue,(tListNode *)hciReadPacket);
            }


          }else{
              /*this is a wrong packet(it is not an event_packet)*/
              /* Insert the packet back into the pool.*/
              list_insert_head(&EH_hciReadPktPool, (tListNode *)hciReadPacket);
          }

        }else{
          /*this is a wrong packet (packet without length)*/
           /* Insert the packet back into the pool.*/
              list_insert_head(&EH_hciReadPktPool, (tListNode *)hciReadPacket);
        }
   }else{
      // HCI Read Packet Pool is empty, wait for a free packet.
      Clear_SPI_EXTI_Flag();
      return;
   }

  Clear_SPI_EXTI_Flag();
  }
}



void HCI_Packet_Release__Event_CB(){
/*Here had been modified the HCI_Isr method to allows:
  *
  *1. timestamp for all imput packages.
  *2. allocate the input packets into the packet queue with priorities.
  *
  */

  tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  tClockTime HCI_Isr_time = Clock_Time(); /*this is the current time in which the packet was transfered from the PoolQueue to the ReadRXqueue*/
  Clear_SPI_EXTI_Flag();

   while(BlueNRG_DataPresent()){ 
      if(list_is_empty (&hciReadPktPool) == FALSE){

         /* enqueueing a packet for read */
        list_remove_head (&hciReadPktPool, (tListNode **)&hciReadPacket);
        data_len = BlueNRG_SPI_Read_All(hciReadPacket->dataBuff, HCI_READ_PACKET_SIZE);
        if(data_len > 0){
          hciReadPacket->data_len = data_len;
          if(HCI_verify(hciReadPacket) == 0){
            hciReadPacket->Isr_timestamp=HCI_Isr_time;

            if(Packet_Get_Priority()!=0){
                /*insert packet according to the priority level */

            }else{
              /*if not priority insert in the tail*/
              list_insert_tail(&hciReadPktRxQueue,(tListNode *)hciReadPacket);
            }


          }else{
              /*this is a wrong packet(it is not an event_packet)*/
              /* Insert the packet back into the pool.*/
              hciReadPacket->Isr_timestamp = 0;
              list_insert_head(&hciReadPktPool, (tListNode *)hciReadPacket);
          }

        }else{
          /*this is a wrong packet (packet without length)*/
           /* Insert the packet back into the pool.*/
              list_insert_head(&hciReadPktPool, (tListNode *)hciReadPacket);
        }
   }else{
      // HCI Read Packet Pool is empty, wait for a free packet.
      Clear_SPI_EXTI_Flag();
      return;
   }

  Clear_SPI_EXTI_Flag();
  }
}


void * HCI_Get__Event_CB(){
   tHciDataPacket * hciReadPacket = NULL;
   Disable_SPI_IRQ();
   uint8_t list_empty = list_is_empty(&hciReadPktRxQueue);
   
    if(list_empty==FALSE){
      list_get_head (&hciReadPktRxQueue, (tListNode **)&hciReadPacket);/**/
      Enable_SPI_IRQ();
      hci_uart_pckt * hci_pckt = ( hci_uart_pckt *)(hciReadPacket->dataBuff);
      hci_event_pckt * event_pckt = (hci_event_pckt*)(hci_pckt->data);
       
       if(hci_pckt->type != HCI_EVENT_PKT){
        /*BLUENRG only support event_packets*/
        return NULL;
        }

        switch(event_pckt->evt){

          case EVT_DISCONN_COMPLETE:
          {
                  /*stil is not defined*/
                  while(1);
      
          }
          break;

            case EVT_LE_META_EVENT:
            {

              evt_le_meta_event *evt = (void *)event_pckt->data;
              switch(evt->subevent){
                  case EVT_LE_CONN_COMPLETE:
                  {
                    evt_le_connection_complete *cc = (void *)evt->data;
                    ret_event.event_type = EVT_LE_CONN_COMPLETE;
                    ret_event.evt_data =cc;
                    ret_event.ISR_timestamp = hciReadPacket->Isr_timestamp;
                    return (void*)&ret_event;
                  }
                  break;
                  case EVT_LE_ADVERTISING_REPORT:
                  {
                    le_advertising_info *pr = (le_advertising_info*) (((uint8_t*)evt->data)+1);
                    ret_event.event_type = EVT_LE_ADVERTISING_REPORT;
                    ret_event.evt_data = pr;
                    ret_event.ISR_timestamp = hciReadPacket->Isr_timestamp;
                    return (void*)&ret_event;
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
                  evt_gatt_procedure_complete * pr = (void*)blue_evt->data;
                  ret_event.event_type=EVT_BLUE_GATT_PROCEDURE_COMPLETE;
                  ret_event.evt_data = pr;
                  ret_event.ISR_timestamp = hciReadPacket->Isr_timestamp;;
                  return (void*)&ret_event;
                }
                break;
              }
            }
            break;
        }
    }else{
    //  Enable_SPI_IRQ();
    }
return NULL;
}
