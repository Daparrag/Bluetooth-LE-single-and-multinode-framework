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
servhandler_conf servhandler_config = {DISC_SERVICE,DISC_CHAR}; /*this is the default configuration of the service handler module*/
/**************************static functions******************************/
static void service_handler();
static void set_char_flags(connection_t * connection);
static void service_error_handler(void);
static SERV_Status discovery_services();
static SERV_Status discovery_specific_service();
/***********************************************************************/

/**
  * @brief  This function is called to retrieve the .
  * @param void.
  * @retval APP_Status: Value indicating success or error code.
  */
void get_sv_handler_config(void *config){
	(servhandler_conf*)config->service_discovery_mode=servhandler_config.service_discovery_mode;
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


/*connection inclide the profile services and  characteristics */
/*This wil recursively scan for all the services and attributes in the profile structure
* the user could overwrite this function acording to what it is needed.
*/
void service_handler(connection_t * connection, conflags * flags){
SERV_Status ret;	
/*input verification*/
if(connection==NULL || flags==NULL){
	PRINTF("some of the imput parameters on the connection handler is NULL  please verify\n");
	service_error_handler();
}

/*profile discovery fsm */
	switch(connection->connection_status){
		case ST_READY_TO_INTERCHANGE:
		{
			switch(connection_t->service_status){
				case ST_SERVICE_DISCOVERY:
				{
					if(!(flags->wait_end_procedure) && services_to_find){
						/*discover the remote services.*/
						if(servhandler_config->serv_disc_mode==FIND_SPE_SERVICE) ret= DSCV_primary_services_by_uuid(connection);
						if(servhandler_config->serv_disc_mode==FIND_GEN_SERVICE) ret=SERV_SUCCESS;/*not yet implemented*/
						if(servhandler_config->serv_disc_mode==FIND_GEN_SERVICE) ret=SERV_SUCCESS;/*not yet implemented*/

						if( ret != SERV_SUCCESS) service_error_handler();
						flags->wait_end_procedure=1;
					}
				}
				break;
				case ST_CHAR_DISCOVERY:
				{
					if(flags->wait_end_procedure && char_to_find){
						/*discover the remote characteristics.*/
						if(servhandler_config->char_disc_mode==FIND_SPE_CHAR)

					}

				}
				break;
			}
		}
		break;
	}

}


SERV_Status DSCV_primary_services_by_uuid(connection_t * connection){
tBleStatus ret;
uint8_t i;
uint8_t num_service; 
app_service_t * service;


	if (services_to_find!=0 && num_service!=0)
	{
		service = (app_service_t *) list_head(connection->Node_profile._service);

		for(i=0;i < services_to_find; i++ ){
			service = (app_service_t *)list_item_next((void * service ));
		}

		if(service=NULL){
	 	/*something is wrong*/
	 	return SERV_ERROR;
	 	}


	 	ret = aci_gatt_disc_prim_service_by_uuid(connection->Connection_Handle,
	 											 service->service_uuid_type,
	 											 (uint8_t*)&(service->ServiceUUID));

		if(ret != BLE_STATUS_SUCCESS){
			/*something was wrong*/
			return SERV_ERROR;
		}

		services_to_find-=1;
	}

	/*primary services have been discovered*/
	 return  SERV_SUCCESS;


}


/*for each service*/
SERV_Status DSCV_char_by_uuid(connection_t * connection){
tBleStatus ret;
uint8_t i;
uint8_t num_char; 
app_attr_t * charac;
app_service_t * service;

	/*step 1. get the correct service*/
	
	if((char_flags->services_scanned < char_flags->services_to_scan) 
		&& char_flags->services_to_scan!=0){

		service = (app_service_t *) list_head(connection->Node_profile._service);

		for(i=0; i < services_scanned; i ++){
			service = (app_service_t *)list_item_next((void * service )); 
		}

		if(service=NULL){
	 	/*something is wrong*/
	 	return SERV_ERROR;
	 	}
		/*at this point we have got the correct service*/

		/*step 2. discover the characteristics*/
		num_char = service->n_attr;
		if(char_flags->char_scanned <  num_char
			&& num_char!=0){
			
		}


	}


}



void service_error_handler(void){
	BSP_LED_Off(LED2);
	while(1);
}