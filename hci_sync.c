/*adding some functionalities to the host controller 
 *interface HCI for synchonization*/

 
 
 /**
 * Verify if HCI packet is correctly formatted..
 *
 * @param[in] hciReadPacket    The packet that is received from HCI interface.
 * @return 0 if HCI packet is as expected
 */
 /*
 HCI Packet types 
#define HCI_COMMAND_PKT		0x01
#define HCI_ACLDATA_PKT		0x02
#define HCI_SCODATA_PKT		0x03
#define HCI_EVENT_PKT		0x04
#define HCI_VENDOR_PKT		0xff
 */
int HCI_verify_sync(const tHciDataPacket * hciReadPacket){/*Return The priority*/
	const uint8_t *hci_pckt = hciReadPacket->dataBuff;
	
}
 
 
 
/*include the priority queue declaration*/ 
 
void HCI_Init_sync(void){
/*init the priority_queue*/

}


void HCI_Process_sync(void){
	/*process the imput packects*/
}

BOOL HCI_Queue_Empty_sync(void){
	/*process if the queue is empty*/
	
}


void HCI_Isr_sync(void){
	/*Packet-Reception-Interrupt-Handler*/
  tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  
  Clear_SPI_EXTI_Flag();
  while(BlueNRG_DataPresent()){
	  if (/*queue_not_full*/){
		  /*get_the arrival_clock*/
		  data_len = BlueNRG_SPI_Read_All(&SpiHandle, hciReadPacket->dataBuff, HCI_READ_PACKET_SIZE);
		  if(data_len>0){
			  hciReadPacket->data_len = data_len;
		  }
	  }
  }
}


