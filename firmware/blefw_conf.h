#ifndef _BLEFW_CONFIG_H_
#define _BLEFW_CONFIG_H_


 
#define ROLE    GAP_CENTRAL_ROLE			/*!< Here is where is defined  the role for this device >*/

#define MULTINODE					/*!< Used only if your device support multinode connections and if it is requiered for your application >*/

#ifdef MULTINODE            				/*define MULTINODE for allows multinode network configuration */        
#define EXPECTED_NODES 2
#define EXPECTED_CENTRAL_NODES 1					
#else
#define EXPECTED_NODES 1
#define EXPECTED_CENTRAL_NODES 1 
#endif

/*****************SIMPLE_MACROS*****************************/
#ifndef MIN
#define MIN(a,b)            ((a) < (b) )? (a) : (b)
#endif

#ifndef MAX
#define MAX(a,b)            ((a) > (b) )? (a) : (b)
#endif


/*************************HARDWARE MACROS*****************************/
/*Local_Macros*/
#define MAY_HAVE_IDB0xA1(arch, func_name) ((arch) == 1 ?func_name##_IDB05A1:func_name##_IDB04A1)/*concatenation of the funtion name and the architecture*/
#define BLE_ARCH_MASK 1 /*used for reconize if we have a IDB04A1 or IDB05A1 architecture  BLE_ARCH_MASK is = #architectures -1 */


/*address setup defaut parameters*/
#define DEVICE_ADDRS_LENGTH 		        (6)		/*!< Modified according to the peer device address size (bytes)>*/
#define CONFIG_DATA_PUBADDR_OFFSET	 (0x00) 	/**< Bluetooth public address or random*/
#define CONFIG_DATA_PUBADDR_LEN		    (6)		/*!< Public address size (bytes) >*/		






#define GENERAL_DISCOVERY			(0x0)	/*!< Used for setup the general perispheral discovery  >*/
#define SELECTIVE_DISCOVERY			(0x1)	/*!< Used for setup a selective perispheral discovery  >*/
#define SPECIFIC_DISCOVERY      	(0x2)	/*!< Used for setup a specific perispheral discovery   >*/


#define GENERAL_ADVERTISEMENT		(0x0)	/*!< Used for setup a general adverticement in the perispheral devices >*/
#define SELECTIVE_ADVERTISEMENT		(0x1)	/*!< Used for setup an adverticement in the perispheral device for connect to selective central node(s)>*/
#define SPECIFIC_ADVERTISEMENT		(0x2)	/*!< Used for setup an adverticement in the perispheral device for connect to specific central node(s)>*/

#define GENERAL_CONNECTION			(0x0)	/*!< Used for stablish a general connection type (the central node will connect to any device )>*/
#define SELECTIVE_CONNECTION		(0x1)	/*!< Used for stablish a selective connection type (the central node will connect to a set of device(s) defied in the whitelist)>*/



#define DISCOVERY_MODE          SELECTIVE_DISCOVERY 	/*!< Use one of the follow: GENERAL_DISCOVERY, SELECTIVE_DISCOVERY, SPECIFIC_DISCOVERY  >*/
#define ADVERTISEMENT_MODE		GENERAL_ADVERTISEMENT	/*!< Use one of the follow: GENERAL_ADVERTISEMENT, SELECTIVE_ADVERTISEMENT, SPECIFIC_ADVERTISEMENT  >*/
#define CONNECTION_MODE         GENERAL_CONNECTION		/*!< Use one of the follow: GENERAL_CONNECTION, SELECTIVE_CONNECTION >*/

#define LED_TOGGLE_CONFIG
#define LED_TOGGLE_UNITIALIZED        100000			/*!< Used for indicate if the device is UNITIALIZED >*/
#define LED_TOGGLE_INTERMIDLE         300000			/*!< Used for indicate if the device is in : ADVERTISING, SCANNING>*/
#define LED_TOGGLE_INTERCHANGE         10000          	/*!< Used for indicate if the device is interchanging  services or characteristics >*/
#define LED_TOGGLE_READY                5000			/*!< Used for indicate if the device is able to transmit/receive data  >*/


/********************SERVICE HANDLER DEFAULT CONFIG*************/
#define DONT_FIND_SERVICE 				0x00			/*!< Used for disable the service discovery in the device>*/
#define FIND_SPE_SERVICE  				0x01			/*!< Used for enable specific service(s) discovery (services characterized by specific UUID) >*/
#define FIND_GEN_SERVICE  				0x02			/*!< Used for discovery any service on the peer device(s) >*/

#define DONT_FIND_CHAR					0x00			/*!< Used for disable the attribute(s) discovery in the device>*/
#define FIND_SPE_CHAR  					0x01			/*!< Used for enable specific attribute(s) discovery (attribute(s) characterized by specific UUID) >*/
#define FIND_GEN_CHAR  					0x02			/*!< Used for discovery any attribute(s) on the peer device(s) >*/

#ifndef DISC_SERVICE
#define DISC_SERVICE					FIND_SPE_SERVICE	/*!< Use one of the follow: DONT_FIND_SERVICE, FIND_SPE_SERVICE, FIND_GEN_SERVICE >*/
#endif


#ifndef DISC_CHAR
#define DISC_CHAR	        			FIND_SPE_CHAR		/*!< Use one of the follow: DONT_FIND_CHAR, FIND_SPE_CHAR, FIND_GEN_CHAR >*/
#endif

/********************GENERAL_ADVERTISEMENT DEFAULT CONFIGURATION *************/

#define ADV_EVT_TYPE  					ADV_IND               /*<!  advertisement event type   >*/
#define ADV_IT_MIN    					(0x0040)              /*<!  advertisement interval min >*/
#define ADV_IT_MAX    					(0x0040)              /*<!  advertisement interval max  >*/
#define ADV_ADDR_TYPE 				    PUBLIC_ADDR           	  /*<!  advertisement address type  >*/
#define ADV_POLICY  		            NO_WHITE_LIST_USE       		  /*<!  advertisement policy  >*/
#define SLAVE_INT_MIN   				(0x0020)           	  /*<!  slave connection interval min >*/
#define SLAVE_INT_MAX   				(0x0020)              /*<!  slave connection interval max  >*/
/********************CONNECTION_SETUP DEFAULT CONFIGURATION *************/

#define SCAN_P  						(0x0050)      		  /*!< Scan Interval 40ms. >*/			
#define SCAN_L  						(0x0050)      		  /*!< Scan Window.  20ms  >*/
#define CONN_P1 			 ((int)((20)/1.25f))  	          /*!< Min connection interval in ms. >*/
#define CONN_P2				 ((int)((20)/1.25f))  	          /*!< Max connection interval in ms. >*/
#define SUPERV_TIMEOUT  				  (3200)         	  /*!<  Supervision timeout.   >*/
#define CONN_L1         	((int)((8)/0.625f))   	   		  /*!<  Min connection length. >*/
#define CONN_L2         	((int)((8)/0.625f))   	   		  /*!<  Max connection length. >*/
#define LATENCY								 (0)
/********************SCAN_SETUP DEFAULT CONFIGURATION *************/
#define SCAN_INTV       				  (0x60)
#define SCAN_WIN	  			          (0x60)
#define SCAN_TYPE       				  (0x01)


#endif /* _BLEFW_CONFIG_H_ blefw_conf.h*/