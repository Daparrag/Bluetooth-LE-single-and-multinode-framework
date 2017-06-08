/*header file for management the services*/
/*version 1.0*/
#ifndef SERVICE_HANDLER_BLE_H
#define SERVICE_HANDLER_BLE_H


/********************SERVICE HANDLER DEFAULT CONFIG*************/
#define DONT_FIND_SERVICE 	0x00
#define FIND_SPE_SERVICE  	0x01	
#define FIND_GEN_SERVICE  	0x02	

#define DONT_FIND_CHAR		0x01
#define FIND_SPE_CHAR  		0x02	
#define FIND_GEN_CHAR  		0x04	

#ifndef DISC_SERVICE
#define DISC_SERVICE		FIND_SPE_SERVICE
#endif

#ifndef DISC_CHAR
#define DISC_CHAR			FIND_SPE_CHAR	
#endif

typedef enum{
	SERV_SUCCESS=0x00,
	SERV_ERROR=0x01;
}SERV_Status;


typedef struct{

  uint8_t serv_disc_mode;
  uint8_t char_disc_mode;
}servhandler_conf;


typedef struct{
servhandler_conf config;
}service_t;


void get_sv_handler_config(void *config);/*return the service handler configuration*/
void set_sv_handler_config(void * config);/*set up the service handler configuration*/
void service_handler(connection_t * connection, net * flags);/*finite state machine which
															 * describe the behavior of the service handler module
															 * you can overwrite this function according to what your application needs.
															 */
#endif /* SERVICE_HANDLER_BLE_H */
/**/