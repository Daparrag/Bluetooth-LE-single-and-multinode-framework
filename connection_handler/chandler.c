/*connection handler fsm*/



#define SEG_DELAY 0x03E8 /*1seg delay*/
#define HALF_SEG_DELAY 0x1F4 /*1/2 seg delay*/
uint8_t  bnrg_connection_mode =  CONNECTION_ORIENTED;/*the default connection mode*/
/**/

/******************************Static func************************************************/ 
static void connection_handler_coriented_single_node();
static void connection_handler_coriented_multi_node();
static void connection_handler_broadcast();
/*****************************************************************************************/



connection_handler_coriented_single_node(){
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

		}
		break;
		case DEVICE_CONNECTED:
		{

		}
		break;	
	}

}