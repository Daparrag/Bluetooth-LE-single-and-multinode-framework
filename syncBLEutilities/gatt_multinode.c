/*GATT Multinode connections functions*/

/**/
#include "gatt_multinode.h"

// DEBUG Macros
#define DEBUG 1
#if DEBUG
#include <stdio.h>
#include "sample_service.h"
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTDEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTDEBUG(...)
#endif



static uint8_t cfg_server_scan = 0;
static uint8_t cfg_server_connect = 0;
static uint8_t config_client = 0;


/*scan setup default parameters*/
#define SLNODES		(0x0002)
#define SCAN_INTV       (200)
#define SCAN_WIN	(5)

/*connection setup default parameters*/
#define SCAN_P                             (0x0028)      		             /*!< Scan Interval 40ms.*/
#define SCAN_L                             (0x0014)      		             /*!< Scan Window.  20ms*/
#define CONN_P1                            ((int)((10)/1.25f))  	             /*!< Min connection interval in ms.*/
#define CONN_P2                            ((int)((10)/1.25f))  	             /*!< Max connection interval in ms.*/
#define SUPERV_TIMEOUT                     (100)         		             /*!< Supervision timeout.*/
#define CONN_L1                            ((int)((5)/0.625f))   	             /*!< Min connection length.*/
#define CONN_L2                            ((int)((5)/0.625f))   	             /*!< Min connection length.*/
#define LATENCY								(0)
/*address setup defaut parameters*/
#ifndef CONFIG_DATA_PUBADDR_OFFSET
#define CONFIG_DATA_PUBADDR_OFFSET     (0x00) /**< Bluetooth public address */
#endif

#ifndef CONFIG_DATA_PUBADDR_LEN
#define CONFIG_DATA_PUBADDR_LEN		   (6)
#endif

/*test variables*/
#ifndef IDB04A1
#define IDB04A1 0
#endif

#ifndef IDB05A1
#define IDB05A1 1
#endif

/**/
/*-----------------------static functions-------------------------------*/

static MNODE_GATT_Status _GATT_MULT_CONNECTION_SETUP_CONFIG(gatt_conf * multinode, int16_t Sinterval, 
								  uint16_t Intv_windows, uint16_t ConnInter_max, uint16_t ConnInter_min,
								  uint16_t C_Latency,uint16_t Conn_Len_Min, uint16_t Conn_Len_Max,
								  uint16_t LE_Timeout_min);
								  
								  
static MNODE_GATT_Status _GATT_MULT_NODE_SCAN_CONFIG(gatt_conf * multinode, uint16_t n_slaves, uint16_t Sinterval, 
								  uint16_t Intv_windows);								  

/*----------------------------------------------------------------------*/



static MNODE_GATT_Status _GATT_MULT_CONNECTION_SETUP_CONFIG(gatt_conf * multinode, int16_t Sinterval, 
								  uint16_t Intv_windows, uint16_t ConnInter_max, uint16_t ConnInter_min,
								  uint16_t C_Latency,uint16_t Conn_Len_Min, uint16_t Conn_Len_Max,
								  uint16_t LE_Timeout_min){
									  
	multinode->M_conf.CN_c.LE_scan_interval=Sinterval;/*Scan_Interval*/
	multinode->M_conf.CN_c.LE_scan_windows=Intv_windows;/*Scan_Window*/
	multinode->M_conf.CN_c.LE_CI_max=ConnInter_max;
	multinode->M_conf.CN_c.LE_CI_min=ConnInter_min;
	multinode->M_conf.CN_c.Conn_Latency=C_Latency;
	multinode->M_conf.CN_c.Conn_Len_Min=Conn_Len_Min;
	multinode->M_conf.CN_c.Conn_Len_Max=Conn_Len_Max;
	multinode->M_conf.CN_c.LE_Timeout=LE_Timeout_min;
	cfg_server_connect=1;
	return MNODE_OPT_SUCCESS;
								  
}

/**
* @brief  This function is called from the server for setup the configuration.
* @param  application config structure, number of slaves, Devices Role, Scaning iterval, and the interval windows.
* @retval _APP_Status: Value indicating success or error code
*/

static MNODE_GATT_Status _GATT_MULT_NODE_SCAN_CONFIG(gatt_conf * multinode, uint16_t n_slaves, uint16_t Sinterval, 
								  uint16_t Intv_windows){
	
	multinode->M_conf.num_slaves = n_slaves; 				/*number of slaves*/
	multinode->M_conf.scan_adv_c.LE_scan_interval=Sinterval;		/*scan interval*/
	multinode->M_conf.scan_adv_c.LE_scan_windows=Intv_windows;		/*scan windows*/
	/*used length=num_slaves*Intv_windows+10ms*/
	cfg_server_scan=1;
	return MNODE_OPT_SUCCESS;
	
}


/**
  * @brief  Simple setup configuration for two slaves
  * @param offset, length, pointer to the val
  * @retval _APP_Status: Value indicating success or error code.
  */
  

MNODE_GATT_Status _GATT_MULT_NODE_SET_DEFAULT_CONFIG(gatt_conf * multinode){
	
	_GATT_MULT_NODE_SCAN_CONFIG(multinode,SLNODES,SCAN_INTV,SCAN_WIN);
	
	_GATT_MULT_CONNECTION_SETUP_CONFIG(multinode,SCAN_P,SCAN_L,CONN_P1,CONN_P2,LATENCY,CONN_L1,CONN_L2,SUPERV_TIMEOUT);
	return MNODE_OPT_SUCCESS;
	
		
}


/**
  * @brief  allows setup the devices address
  * @param offset, length, pointer to the val
  * @retval _APP_Status: Value indicating success or error code.
  */
  
/*THIS IS NOT HERE*/
MNODE_GATT_Status _GATT_MULT_NODE_SET_DEVICE_ADDR(uint8_t offset, 
											 uint8_t len,
											 const uint8_t *val){
	int ret;
	ret= aci_hal_write_config_data(offset,len,val);
	
	if (ret != BLE_STATUS_SUCCESS){
		PRINTF("error: while setup device address, error value %d\n",ret);
		return MNODE_OPT_ERROR;
	}
	
	return MNODE_OPT_SUCCESS;
	
	
						  
}

/**
  * @brief  setup the devices as dicovery mode for a set of slaves.
  * @param APP configuration
  * @retval _APP_Status: Value indicating success or error code.
  */


MNODE_GATT_Status _GATT_MULT_NODE_DISCOVERY_PROCEDURE(gatt_conf * multinode){
	uint8_t ret;
	int i;
	if(!cfg_server_scan){
		
		PRINTF("error: Not configuration in the discovery procedure\n");
		return MNODE_NOT_CONFIG;	
	}
	//for(i=1;i < multinode->M_conf.num_slaves; i++){
		 ret = aci_gap_start_general_discovery_proc(multinode->M_conf.scan_adv_c.LE_scan_interval,/*scan interval*/
		 	                                        multinode->M_conf.scan_adv_c.LE_scan_windows, /*scan windows*/
													0x00, /**/
													0x01/**/
													); 
		 if (ret != BLE_STATUS_SUCCESS)
			{
				PRINTF("error: while add server discovery procedure error num %d \n",ret);
				return MNODE_OPT_ERROR;
			}	
	//}
	return MNODE_OPT_SUCCESS;
}


/**
  * @brief  This function is used for setup multiple active scanning for connection request
  * @param APP configuration, peer address type, peer address, Own_Address_Type.    
  * @retval _APP_Status: Value indicating success or error code.
  */

MNODE_GATT_Status _GATT_MULT_NODE_SETUP_CONNECTION(gatt_conf * multinode,uint32_t peer_addr_type,uint8_t *Peer_Address, 
											  uint32_t Own_Address_Type){
												  
	 /*
   Scan_Interval, Scan_Window, Peer_Address_Type, Peer_Address, Own_Address_Type, Conn_Interval_Min,
   Conn_Interval_Max, Conn_Latency, Supervision_Timeout, Conn_Len_Min, Conn_Len_Max
   */
   uint8_t ret;
   if(!cfg_server_connect){
		
		PRINTF("error: Not configuration in the setup connection \n");
		return MNODE_NOT_CONFIG;	
	}
   
   ret = aci_gap_create_connection(multinode->M_conf.CN_c.LE_scan_interval, /*Scan_Interval*/
							 multinode->M_conf.CN_c.LE_scan_windows,  /*Scan_Window*/
							 Own_Address_Type,								  /*Peer_Address_Type*/
                             Peer_Address, 									  /*Peer_Address*/
							 Own_Address_Type, 								  /*Own_Address_Type*/
							 multinode->M_conf.CN_c.LE_CI_min,		  /*Conn_Interval_Min*/	
                             multinode->M_conf.CN_c.LE_CI_max,		  /*Conn_Interval_Max*/
							 multinode->M_conf.CN_c.Conn_Latency,	  /*Conn_Latency*/
							 multinode->M_conf.CN_c.LE_Timeout,	  /*Supervision_Timeout*/	
							 multinode->M_conf.CN_c.Conn_Len_Min, 	  /*Conn_Len_Min*/
							 multinode->M_conf.CN_c.Conn_Len_Max		  /*Conn_Len_Max*/
							);
							
	 if (ret != BLE_STATUS_SUCCESS)
			{
				PRINTF("error: while connection setup, error value: %d \n",ret);
				return MNODE_OPT_ERROR;
			}												
                            
  return MNODE_OPT_SUCCESS;
   
   
}

/**
  * @brief  This function is called init the gatt module from the application.
  * @param null 
  * @retval _APP_Status: Value indicating success or error code.
  */


MNODE_GATT_Status _GATT_MULT_NODE_INIT_GATT(void){
	int ret;
	ret = aci_gatt_init();
	if(ret!= BLE_STATUS_SUCCESS){
		PRINTF("error: while init GATT, error value: %d \n",ret);
		return MNODE_OPT_ERROR;
	}
	return MNODE_OPT_SUCCESS;
}




/**
  * @brief  This function is called to add Service at the app level.
  * @param  service_uuid: Service uuid value.
  * @param  service_handle: Pointer to a variable in which the service handle will be saved.
  * @retval BV_Status: Value indicating success or error code.
  */

MNODE_GATT_Status _GATT_MULT_SERVER_ADD_SERVICE(uint8_t *service_uuid,uint16_t *service_handle){
		/*include BLUEVOICE + SYNC + CTRL services*/
  return MNODE_OPT_SUCCESS;
		
}


/**
  * @brief  This function is for test the GATT_MULTINODE module.
  * @param  void
  * @retval MNODE_GATT_Status: Value indicating success or error code.
  */

MNODE_GATT_Status _GATT_MULT_NODE_TEST_MODULE_MASTER(void){
   
  /*test 1: using default configuration 1 master 2 slave nodes*/


const uint8_t CENTRAL_BDADDR[] = { 0x55, 0x11, 0x07, 0x01, 0x16, 0xE1 };/*central MAC address*/
uint8_t bnrg_expansion_board = IDB05A1; /* at startup, suppose the X-NUCLEO-IDB04A1 is used */   
gatt_conf default_multinode; /*Master multinode default config*/
int ret; /*aux variable for catch error*/
uint16_t service_handle, dev_name_char_handle, appearance_char_handle;/*default handlers*/
uint8_t  hwVersion; /*harware version aux_var*/
uint16_t fwVersion; /*firmwall version aux_var*/


  
/* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);
/* 
   * Reset BlueNRG again otherwise we won't
   * be able to change its MAC address.
   * aci_hal_write_config_data() must be the first
   * command after reset otherwise it will fail.
   */
  BlueNRG_RST();
  
  PRINTF("HWver %d, FWver %d\n", hwVersion, fwVersion);
  
  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    bnrg_expansion_board = IDB05A1; 
  }
  
    BlueNRG_RST();
  /*setup the server address */
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, CENTRAL_BDADDR);
  
  if (ret != BLE_STATUS_SUCCESS)
  {
    return MNODE_OPT_ERROR;
  }
  
  ret = _GATT_MULT_NODE_INIT_GATT();
  
  if (ret!=MNODE_OPT_SUCCESS){
        return MNODE_OPT_ERROR;
  }
  
   
  ret = _GATT_MULT_NODE_SET_DEFAULT_CONFIG(&default_multinode);

    if (ret!=MNODE_OPT_SUCCESS){
        return MNODE_OPT_ERROR;
  }

if (bnrg_expansion_board == IDB05A1) 
  {
    ret = aci_gap_init_IDB05A1(GAP_CENTRAL_ROLE_IDB05A1, 0, 0x07,  &service_handle, &dev_name_char_handle,  &appearance_char_handle);
  }     
  else
  {
    ret = aci_gap_init_IDB04A1(GAP_CENTRAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  
  if (ret != BLE_STATUS_SUCCESS)
  {
    return MNODE_OPT_ERROR;
  }
  
  ret = Add_Sample_Service();
    
    if(ret == BLE_STATUS_SUCCESS)
      PRINTF("Service added successfully.\n");
    else
      PRINTF("Error while adding service.\n");
    
     /* Set output power level */
  aci_hal_set_tx_power_level(1, 5);
  
  /*setup the discovery procedure*/
  ret = _GATT_MULT_NODE_DISCOVERY_PROCEDURE(&default_multinode);

    if (ret!=MNODE_OPT_SUCCESS){
        return MNODE_OPT_ERROR;
    }
    
    return MNODE_OPT_SUCCESS;
}
