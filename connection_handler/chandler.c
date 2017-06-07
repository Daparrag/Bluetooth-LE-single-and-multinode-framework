/*connection handler fsm*/



#define SEG_DELAY 0x03E8 /*1seg delay*/
#define HALF_SEG_DELAY 0x1F4 /*1/2 seg delay*/
uint8_t  bnrg_connection_mode =  CONNECTION_ORIENTED;/*the default connection mode*/
dv_state_t bnrg_device_status = DEVICE_UNITIALIZED; /*device status initialized by default*/
app_discovery_t * DV_config = NULL;
app_advertise_t  * AV_config= NULL;
/************************************Module Flags*****************************************/



/******************************Static func************************************************/ 
static void connection_handler_coriented_single_node();
static void connection_handler_coriented_multi_node();
static void connection_handler_broadcast();
static void connection_handler_error(void);
/*****************************************************************************************/

/**
  * @brief  This function is called to setup the discovery configuration into the connection handler module.
  * @param  app_discovery_t * dv_conf: contain the discovery configuration.
  * @retval void.
  */
  
void connection_handler_set_discovery_config(app_discovery_t * dv_conf){
	DV_config = dv_conf;
};


/**
  * @brief  This function is called to setup the advertisement configuration into the connection handler module.
  * @param  app_discovery_t * dv_conf: contain the discovery configuration.
  * @retval void.
  */
  
void connection_handler_set_advertise_config(app_advertise_t  * adv_conf){
	AV_config = adv_conf;
}






void connection_handler_coriented_single_node(network_t * net){
	bnrg_device_status=get_dev_status(void);/*it ius useful since the device is out of the scope of the connection handler*/

	if(bnrg_device_status == DEVICE_UNITIALIZED){
		PRINTF("device unitialized please reinitialize \n");
		HAL_DELAY(SEG_DELAY);
		return;
	}

	switch(net->connection_status){
		case ST_DISCOVERY:
		{
			if(led_toggle_count++ > LED_TOGGLE_DISCOVERY)
			{
				led_toggle_count=0;
				BSP_LED_Toggle(LED2);
			}


		}
		break;
		case ST_ADVERTISE:
		{
			if(led_toggle_count++ > LED_TOGGLE_ADVERTICEMENT)
			{
				led_toggle_count=0;
				BSP_LED_Toggle(LED2);
			}

			if((net->device_found) && !(net->wait_end_procedure)){
				/*single connection*/

			}

		}
		break;
		case ST_NOTCONNECTED:
		{


		}
		break;
		case ST_WAITCONNECTION:
		{

		}
		break;
		case ST_CONNECTED_WAIT_CHAR_DISC:
		{

		}
		break;
		case ST_READY:
		{

		}
		break;

	}
}







connection_handler_coriented_single_node(network_t * net){
	switch(bnrg_device_status){
		case DEVICE_UNITIALIZED:
		{
			PRINTF("device unitialized please reinitialize \n");
			HAL_DELAY(SEG_DELAY);

		}
		break;
		case DEVICE_ADVERTISEMENT_MODE:
		{
			if(led_toggle_count++ > LED_TOGGLE_ADVERTICEMENT)
			{
				led_toggle_count=0;
				BSP_LED_Toggle(LED2);
			}
		}
		break;
		case DEVICE_DISCOVERY_MODE:
		{
			if(led_toggle_count++ > LED_TOGGLE_DISCOVERY)
			{
				led_toggle_count=0;
				BSP_LED_Toggle(LED2);
			}

			if((net->device_found) && !(net->wait_end_procedure)){
				/*single connection*/
				APP_create_connection_BLE(net->mMSConnection.cconfig,net->mMSConnection.device_type_addrs,(void*)&(net->mMSConnection.device_address));
				net->device_found=0;
				bnrg_device_status = ST_CONNECTED_WAIT_CHAR_DISC;
			}else if(!(net->device_found) && !(net->wait_end_procedure)){
				/*setup the discovery procedure.*/
				APP_Status ret=APP_set_discovery_BLE(DV_config);
				if(ret != APP_SUCCESS){
					PRINTF("error has been occour during the setting the discovery procedure \n");
					connection_handler_error();
				}

			}

		}
		break;
		case DEVICE_CONNECTED:
		{

		}
		break;
		case DEVICE_CONNECTED:
		{

		}
		break;	
	}

}


void connection_handler_error(void){
	BSP_LED_On(LED2);
	while(1);
}