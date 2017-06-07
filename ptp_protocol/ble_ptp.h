/*Simple Sync-PTP BLE protocol*/

#ifndef PTP_BLE_H
#define PTP_BLE_H

/*Messages_Type*/
#define SYNC 0x01
#define FOLLOW_UP 0x02
#define DELAY_REQ 0x04
#define DELAY_RSP 0x08


typedef enum ptp_State {
	INIT, UNSYNC, SYNC, WAIT_RESP, PENDING_REQ
}ptp_state_t;


typedef enum ptp_Signal {
	
}ptp_signal_t;



/** 
* @brief ptp packet structures.
*/
typedef struct{
	uint8_t  ptp_type; /*1 octect*/
	uint8_t  ptp_version;/*1 octect*/
	uint16_t ptp_msg_length; /*2 octect*/
	uint8_t  domain_number; /*1 octect*/
	uint16_t correction_field;/*2 octect*/
	uint16_t source_id;	/*2 octect*/
}ptp_hdr;/*total 9bytes*/

typedef struct{
	ptp_hdr header; 
	uint16_t orginTimeStamp;
}sync_, delay_req_, follow_up_;


typedef struct{
	ptp_hdr header;
	uint16_t receivedTimeStamp;
}delay_resp_msg;



typedef struct ptp_fsm_ {
	ptp_state_t ptp_cstate;
	ptp_state_t ptp_substate;
}ptp_fsm

tBleStatus ptp_init_server();
void ptp_init_client();
Signal DecodeEvents();
void ptp_Dispatch(ptp_fsm * ptp_ins);

#endif /* PTP_BLE_H */