/*header file for management the services*/
/*version 1.0*/
#ifndef SERVICE_HANDLER_BLE_H
#define SERVICE_HANDLER_BLE_H


/********************SERVICE HANDLER DEFAULT CONFIG*************/
#ifndef DISC_SP_SERVICE
#define DISC_SP_SERVICE		0x01
#endif

#ifndef DISC_SP_CHAR
#define DISC_SP_CHAR		0x01	
#endif

typedef enum{
	SERV_SUCCESS=0x00,
	SERV_ERROR=0x01;
}SERV_Status;

typedef struct{
volatile uint8_t service_discovery;               /*!< this flag is fire when a new service has been discover >*/    
volatile uint8_t char_discovery;                  /*!< this flag is fire when a new char has been discover >*/
}sv_hdler_flags;

typedef struct{
  uint8_t dsc_sp_services;
  uint8_t dsc_sp_char;
}servhandler_conf;

typedef struct{
sv_hdler_flags flags;
servhandler_conf config;
}service_t;


#endif /* SERVICE_HANDLER_BLE_H */
/**/