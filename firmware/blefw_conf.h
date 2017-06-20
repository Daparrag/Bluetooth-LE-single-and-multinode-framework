#ifndef _BLEFW_CONFIG_H_
#define _BLEFW_CONFIG_H_
 
#define ROLE    GAP_CENTRAL_ROLE

#define GENERAL_DISCOVERY		(0x0)
#define SELECTIVE_DISCOVERY		(0x1)
#define SPECIFIC_DISCOVERY              (0x2)


#define GENERAL_ADVERTICEMENT		(0x0)
#define SELECTIVE_ADVERTICEMENT		(0x1)
#define SPECIFIC_ADVERTICEMENT          (0x2)



#define DISCOVERY_MODE                  SELECTIVE_DISCOVERY
#define ADVERTICEMENT_MODE		GENERAL_ADVERTICEMENT

#define LED_TOGGLE_CONFIG
#define LED_TOGGLE_UNITIALIZED        100000
#define LED_TOGGLE_INTERMIDLE         300000
#define LED_TOGGLE_INTERCHANGE         10000          
#define LED_TOGGLE_READY                5000
#define LED_TOGGLE_CONNECTED           10000

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
#define DISC_CHAR	        FIND_SPE_CHAR	
#endif


#endif /* _BLEFW_CONFIG_H_ blefw_conf.h*/