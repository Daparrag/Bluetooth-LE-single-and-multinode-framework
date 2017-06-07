/*service_handler:
** version 1.0
* task:
* -Handler how is interchanged the services and attributes of the application
*
* NOTE:
* Services and Attributes could be diferent for each node in a multinode topology.
* This module management those (Services and Attributes) efficiently for each node,
* allowing and create an efficient multinode application since diferent nodes shoud be access 
* to diferent services according to the application requirement. 
*/


#include <servhandler.h>
/*************************services_handler_config***********************/
servhandler_conf servhandler_config = {DISC_SP_SERVICE,DISC_SP_CHAR}; /*this is the default configuration of the service handler module*/


/**************************static functions******************************/
static void service_handler();
static SERV_Status discovery_services();
static SERV_Status discovery_specific_service();
/***********************************************************************/

/**
  * @brief  This function is called to retrieve the .
  * @param void.
  * @retval APP_Status: Value indicating success or error code.
  */
void get_sv_handler_config(void *config){
	(servhandler_conf*)config->dsc_sp_services=servhandler_config.dsc_sp_services;
	(servhandler_conf*)config->dsc_sp_char=servhandler_config.dsc_sp_char;
}

/**
  * @brief  This function is called to retrieve the BLE APP version.
  * @param void.
  * @retval APP_Status: Value indicating success or error code.
  */
void set_sv_handler_config(void * config){
	servhandler_conf * user_config = (servhandler_conf *)config;
	servhandler_config.dsc_sp_services=user_config->dsc_sp_services;
	servhandler_config.dsc_sp_char=user_config->dsc_sp_char;
} 


/*connection inclide the connection characteristics and the service haracteristics */
void service_handler(connection_t * connection){


}

