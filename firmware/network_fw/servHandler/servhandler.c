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

/**
* @brief  This function is called for discovery a service characterized by an uuid.
* @param  connection_t * connection: specify the services for this specific connection.
* @retval SERV_Status: SERV_SUCCESS if operation is success otherwise SERV_ERROR.
*/
SERV_Status DSCV_primary_services_by_uuid(connection_t * connection){
tBleStatus ret;
uint8_t i;
uint8_t num_service; 
app_service_t * service;
sv_ctrl_flags  *serv_control_flags;

serv_control_flags = &(connection->Node_profile->svflags);
num_service = connection->Node_profile->n_service;

	if (serv_control_flags->services_to_find > 0 && num_service!=0)
	{
		service = (app_service_t *) connection->Node_profile->services;
		for(i=0;i < num_service-(serv_control_flags->services_to_find); i++ ){
                  
			service = (app_service_t *)service->next_service;
		}

		if(service==NULL)
		{
 			/*something is wrong*/
 			return SERV_ERROR;
 		}

	}else{
		/*all services have been success discovered*/
		serv_control_flags->services_success_scanned+=1;
		return SERV_SUCCESS;
	}


	/*< at this point is possible to send the service discover request >*/
	ret = aci_gatt_disc_prim_service_by_uuid(connection->Connection_Handle,
                                                 service->service_uuid_type,
                                                (uint8_t*)&(service->ServiceUUID));

	if(ret != BLE_STATUS_SUCCESS){
	/*something was wrong*/
	return SERV_ERROR;
	}
serv_control_flags->services_to_find-=1;
	/*primary services have been success discovered*/
	 return  SERV_SUCCESS;

}

/**
* @brief  This function is called for discovery a characteristic given its uuid.
* @param  connection_t * connection: contain specific characteristics and services for this connection.
* @retval SERV_Status: SERV_SUCCESS if operation is success otherwise SERV_ERROR.
*/
SERV_Status DSCV_primary_char_by_uuid(connection_t * connection)
{
    tBleStatus ret;
    uint8_t i;
    uint8_t num_char;
    app_attr_t * charac;
    app_service_t * service;
    char_flags * attr_control_flags;
    
      /*lets get the correct service*/
    service = connection->Node_profile->services;
    while(service!=NULL && service->chrflags.char_discovery_success!=0){
      service = service->next_service;
    }
    
    if(service==NULL){
        /*all the characteristics for this profile had been discovery*/
         connection->Node_profile->svflags.attr_success_scanned=1;
         return SERV_SUCCESS;
    }
    
    
    /*At this point: its where it'll discovery each characteristic */
    num_char = service->n_attr;
    attr_control_flags = &(service->chrflags);
    /*validate that this is the correct characteristic*/
    if( (num_char==0) || (attr_control_flags->char_scanned >= attr_control_flags->char_to_scan)){
    /*this profile does not have characteristics to discover*/
      return SERV_SUCCESS;
    }
    /*Retrieve the correct characteristic to scan*/
    charac = service->attrs;
    
    for(i=0; i < attr_control_flags->char_scanned; i++)
    {
      charac = charac->next_attr;
    }
    
    if(charac==NULL){
      /*something is wrong */
      return SERV_ERROR;
    }
    
    
    
   /*< at this point is possible to send the characteristic discover request >*/
    
   ret = aci_gatt_disc_charac_by_uuid(connection->Connection_Handle,
                                      0x0001,
                                      0xFFFF,
              charac->charUuidType,
                                      (uint8_t*)&(charac->CharUUID)
                                      ); 
    
    
  if(ret != BLE_STATUS_SUCCESS){
    /*something was wrong*/
      return SERV_ERROR;
  }
  
//  attr_control_flags->char_scanned+=1;
  //service->chrflags.char_to_scan-=1;
  /*a characteristic have been discovered*/ 
  return  SERV_SUCCESS;
}

SERV_Status DSCV_Enable_Notify(connection_t * connection)
{
 
  uint8_t i;
  uint8_t num_char;
  app_attr_t * charac;
  app_service_t * service;
  char_flags * attr_control_flags;
  
   uint8_t client_char_conf_data[] = {0x01, 0x00}; // Enable notifications
  
   /*lets get the correct service*/
    service = connection->Node_profile->services;
    while(service!=NULL && service->chrflags.char_notify_enabled_success!=0){
       service = service->next_service;
    }
    
    if(service==NULL){
       connection->Node_profile->svflags.notify_success_enable=1;
       return SERV_SUCCESS;
    }
    
    /*At this point: its where it'll enable the notify for each characteristic */
    num_char = service->n_attr;
    attr_control_flags = &service->chrflags;
    /*validate that this is the correct characteristic*/
    
    if( (num_char==0) || (attr_control_flags->char_scanned >= attr_control_flags->char_to_scan)){
    /*this profile does not have characteristics to enable*/
      connection->Node_profile->svflags.notify_success_enable=1;
      attr_control_flags->char_notify_enabled_success=1;
      return SERV_SUCCESS;
    }
    
    
        /*Retrieve the correct characteristic to enable*/
    charac = service->attrs;
    attr_control_flags = &service->chrflags;
    
    for(i=0; i < attr_control_flags->char_scanned; i++)
    {
      charac = charac->next_attr;
    }
    
    if(charac==NULL){
      /*something is wrong */
      return SERV_ERROR;
    }
 
    /*< at this point is possible to enable the notify using the low level command >*/
    
    if((charac->charProperties && CHAR_PROP_NOTIFY ) == 1){
      
          struct timer t;
          Timer_Set(&t,CLOCK_SECOND*10);
        /*  while(ret=(aci_gatt_write_charac_descriptor(connection->Connection_Handle, charac->CharHandle+2, 2, client_char_conf_data))==BLE_STATUS_INSUFFICIENT_RESOURCES)
          {
              if(Timer_Expired(&t))
              {
                return SERV_ERROR;
              }
    
          }*/
  
    }else{
      attr_control_flags->char_scanned+=1;
      return SERV_NOT_APPL;
    }
    
   attr_control_flags->char_scanned += 1;
  /*notify setup complete*/ 
  return  SERV_SUCCESS;
}

SERV_Status SH_Associate_att_handler_CB(connection_t * connection,uint16_t peer_attr_handler){

    uint8_t i;
    app_attr_t * charac;
    app_service_t * service;
    char_flags * attr_control_flags;
    
     /*lets get the correct service*/
    service = connection->Node_profile->services;
    
     while(service!=NULL && service->chrflags.char_discovery_success!=0){
      service = service->next_service;
    }
    
        if(service==NULL){
        /*all the characteristics for this profile had been discovery*/
        /*then this is not possible verify*/  
         return SERV_SUCCESS;
        }
     
    /*Retrieve the correct characteristic to enable*/
    charac = service->attrs;
    attr_control_flags = &service->chrflags;

    for (i=0; i < attr_control_flags->char_scanned; i++)
    {
      charac = charac->next_attr;
    }
    
    if(charac==NULL){
    /*something is get it wrong*/
      return SERV_ERROR;
    }
    
    /*associate this attribute handler to this characteristic*/
    charac->Associate_CharHandler=peer_attr_handler;
    attr_control_flags->char_scanned+=1;

    if(attr_control_flags->char_scanned == attr_control_flags->char_to_scan){

      attr_control_flags->char_discovery_success=1;
    }
    
    return SERV_SUCCESS;
}


SERV_Status SH_Associate_list_att_handler_CB(connection_t * connection, uint8_t data_length, uint8_t * buffer){

   uint8_t i;
   app_attr_t * charac;
   app_service_t * service;
   char_flags * attr_control_flags;
   
    /*lets get the correct service*/
    service = connection->Node_profile->services;
    
     while(service!=NULL && service->chrflags.char_discovery_success!=0){
      service = service->next_service;
    }
    
    if(service==NULL){
        /*all the characteristics for this profile had been discovery*/
        /*then this is not possible verify*/  
         return SERV_SUCCESS;
        }
     
  /*Retrieve the correct characteristic to enable*/
    charac = service->attrs;
    attr_control_flags = &service->chrflags;
    
  for (i=0; i < attr_control_flags->char_scanned; i++)
    {
      charac = charac->next_attr;
    }
    
    if(charac==NULL){
    /*something is get it wrong*/
      return SERV_ERROR;
    }  
    
    
    do{
      charac->Associate_CharHandler=*buffer||*(buffer+1);
      buffer+=4;
      data_length-=4;
      attr_control_flags->char_scanned+=1;
      charac=charac->next_attr;
    }while(charac!=NULL && data_length>0);


    if(attr_control_flags->char_scanned == attr_control_flags->char_to_scan){
      
      attr_control_flags->char_discovery_success=1;
    }


  return SERV_SUCCESS;  
                    
}
       




/**
* @brief  This is a control function used in case of service handler error.
* @retval none.
*/
void service_error_handler(void){
	BSP_LED_Off(LED2);
	while(1);
}
