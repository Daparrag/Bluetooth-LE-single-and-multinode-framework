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
volatile uint8_t service_discovery;               /*!< this flag is fire when a new service has been discover >*/    
volatile uint8_t char_discovery;                  /*!< this flag is fire when a new char has been discover >*/
}sv_hdler_flags;

typedef struct{

  uint8_t serv_disc_mode;
  uint8_t char_disc_mode;
}servhandler_conf;

typedef struct{
uint8_t services_to_scan;
uint8_t services_scanned;
uint8_t char_to_scan;
uint8_t char_scanned;
}char_flags;


typedef struct{
sv_hdler_flags flags;
servhandler_conf config;
}service_t;


#endif /* SERVICE_HANDLER_BLE_H */
/**/