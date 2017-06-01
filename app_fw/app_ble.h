#ifndef APP_BLE_H
#define APP_BLE_H

#ifdef BLE_APP_CONFIG
#include "ble_app_conf.h" 
#endif

/*used the valid roles*/
/*valid roles
*GAP_PERIPHERAL_ROLE
*GAP_BROADCASTER_ROLE
*GAP_CENTRAL_ROLE
*GAP_PERIPHERAL_ROLE
*/

#ifndef CONCAT_AUX
#define CONCAT_AUX(_x,_y) _x##_y
#endif

#ifndef CONCAT
#define CONCAT(x,y) CONCAT_AUX(x, y)
#endif

#ifndef ROLE
#define ROLE GAP_CENTRAL_ROLE
#endif

#define GET_ROLE(arch) ((arch)==0 ? CONCAT(ROLE,_IDB04A1) : CONCAT(ROLE,_IDB05A1))



/*configuration Variables*/
#ifndef IDB04A1
#define IDB04A1 0
#endif

#ifndef IDB05A1
#define IDB05A1 1
#endif
/*connection setup default parameters*/
#ifndef SCAN_P
#define SCAN_P  		(0x0028)      		             /*!< Scan Interval 40ms.*/			
#endif
#ifndef SCAN_L
#define SCAN_L  		(0x0014)      		             /*!< Scan Window.  20ms*/
#endif
#ifndef	CONN_P1
#define CONN_P1 		((int)((10)/1.25f))  	         /*!< Min connection interval in ms.*/
#endif
#ifndef	CONN_P2
#define CONN_P2			((int)((10)/1.25f))  	         /*!< Max connection interval in ms.*/
#endif
#ifndef	SUPERV_TIMEOUT
#define SUPERV_TIMEOUT  (100)         		             /*!< Supervision timeout.*/
#endif
#ifndef	CONN_L1
#define CONN_L1         ((int)((5)/0.625f))   	         /*!< Min connection length.*/
#endif
#ifndef	CONN_L2
#define CONN_L2         ((int)((5)/0.625f))   	         /*!< Min connection length.*/
#endif
#ifndef	LATENCY
#define LATENCY			(0)
#endif

/*scan setup default parameters*/
#ifndef SLNODES
#define SLNODES		(0x0002)
#endif
#ifndef SCAN_INTV
#define SCAN_INTV   (200)
#endif
#ifndef SCAN_WIN
#define SCAN_WIN	  (5)
#endif


/*device address parameters*/
/*address setup defaut parameters*/
#ifndef	CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_OFFSET     (0x00) /**< Bluetooth public address or random*/
#endif

#ifndef CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_LEN		   (6)
#endif

/*Local_Macros*/
#define MY_HAVE_IDB0xA1(arch, func_name) ((arch) == 1 ?func_name##_IDB05A1:func_name##_IDB04A1)/*concatenation of the funtion name and the architecture*/
#define BLE_ARCH_MASK 1 /*used for reconize if we have a IDB04A1 or IDB05A1 architecture  BLE_ARCH_MASK is = #architectures -1 */

#ifndef UUID_TYPE
#define UUID_TYPE UUID_TYPE_128
#endif

#ifndef SERVICE_TYPE
#define PRIMARY_SERVICE
#endif


typedef enum /*used for return the result of and operation on the application*/
{
  APP_SUCCESS = 0x00,
  APP_ERROR = 0x10,
  APP_NOT_CONFIG= 0x02
} APP_Status; 


typedef struct{ /*structure that define any characteristic on BLE_app firmware*/
	uint8_t CharTimeUUID[16];            /*!< Control characteristic UUID.*/
	uint16_t CharHandle;             /*!< Audio characteristic handle.*/
	void * char_value;
	void * next_char_val;
	uint8_t n_val;
}app_char_t;

typedef struct{/*structure that define any service on BLE_app firmware*/
	uint8_t ServiceUUID[16];              /*!< Service UUID.*/
	uint16_t ServiceHandle;               /*!< Service handle.*/
	app_char_t * char;
	app_char_t * next_char;
	uint8_t n_service;
}app_service_t;




/**
*STATIC_PROFILE: static profile declaration:
*it is used for declare a specific BLE profile the type must be a structure that will be the first element 
*of the application profile. This implementation uses a pointer for handler ta set of diferent linked services and 
*characteristic data strctures.
*@param: name of the profile.
*/

#define STATIC_PROFILE(name)\
			static void * CONCAT(name,_profile)=NULL;\
			static app_profile_t name = (app_profile_t)&CONCAT(name,_profile)

/**
*PROFILE: global profile declaration:
*it is used for declare a specific BLE profile the type must be a structure that will be the first element 
*of the application profile. This implementation uses a pointer for handler ta set of diferent linked services and 
*characteristic data strctures.
**@param: name of the profile.
*/
#define PROFILE(name)\
			void * CONCAT(name,_profile);\
			app_profile_t name



/**
*STATIC_SERVICE: static service declaration:
*it is used for declare a static & specific BLE service the type must be a structure that will be the first service 
*of the application profile. This implementation uses a pointer for handler the services data strctures.
*@param: name of the service.
*/
#define STATIC_SERVICE(name)\
			static void * CONCAT(name,_service)=NULL;\
			static app_service_t name = (app_service_t)&CONCAT(name,_service)

/**
*SERVICE: global service declaration:
*it is used for declare a global & specific BLE service associate to one specific profile. 
*The type must be a structure that will be the first element of the service list. 
*This implementation uses a pointer for handler a set of diferent linked services
**@param: name of the service.
*/
#define SERVICE(name)\
			void * CONCAT(name,_service);\
			app_service_t name


/**
*STATIC_CHAR: static characteristic declaration:
*it is used for declare a static and specific BLE characteristic associate to  a specific service the 
*type must be a structure that will be the first element of the characteristic list. This implementation 
*uses a pointer for handler ta set of diferent linked characteristics and value data strctures.
*@param: name of the profile.
*/
#define STATIC_CHAR(name)\
			static void * CONCAT(name,_char)=NULL;\
			static app_char_t name = (app_char_t)&CONCAT(name,_char)

/**
*CHAR: global profile declaration:
*it is used for declare a specific BLE profile the type must be a structure that will be the first element 
*of the application profile. This implementation uses a pointer for handler ta set of diferent linked services and 
*characteristic data strctures.
**@param: name of the profile.
*/
#define CHAR(name)\
			void * CONCAT(name,_char);\
			app_char_t name

/**
* the profile_type
*/
typedef void ** app_profile_t;



/**
* global_func_declaration
*/


APP_Status APP_Init_BLE(void); /*init the application*/
APP_Status profile_init(app_profile_t profile);
APP_Status add_service(app_profile_t profile,app_service_t service);
APP_Status add_char(/*ok*/);


#endif /* PTP_BLE_H */