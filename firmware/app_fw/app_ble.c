/*Support firmware for BLE app_layer (single node specification)*/
/*application management functions*/
/*global procedures common to any application */

#include <app_ble.h>
#include <list.h>

/**********************Local Variables***********************/
uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
uint8_t  bnrg_expansion_board =  IDB05A1;
uint8_t service_list_init=0;            /*!< flag used to initialized the service attribute list */

static int name_size;
static int address_size;
const uint8_t * PTR_DEVICE_BDADDR=NULL;
const char * ptr_local_name=NULL;
/************************************************************/
tBleStatus(*const  GAP_INIT_FUNC [BLE_ARCH_MASK+1])(uint8_t ,uint8_t ,uint8_t ,uint16_t* ,
                                                    uint16_t* ,uint16_t* ) = { /*architecture independent array, call the correct function according to the architecture version IDB05A1 or IDB04A1*/                                            
MAY_HAVE_IDB0xA1(0, aci_gap_init),                                            
MAY_HAVE_IDB0xA1(1, aci_gap_init)                                                    
}; /*init_gap_macro*/

#define  aci_gap_init(_role,_privacity_enable,_device_name_char_length,_service_handler,\
                      _dev_name_char_handle,_appearance_char_handle,arch) \
        ((*GAP_INIT_FUNC[arch & BLE_ARCH_MASK])(_role,_privacity_enable,_device_name_char_length,\
                                              _service_handler,_dev_name_char_handle,\
                                              _appearance_char_handle))/*init gap function*/
          
/******************************Static func************************************************/ 
static int APP_BLE_GET_VERSION(uint8_t *hwVersion, uint16_t *fwVersion);   /*retreive the version of the BLUENRG board*/       
/*****************************************************************************************/          

/**
  * @brief  This function is called to retrieve the BLE APP version.
  * @param void.
  * @retval APP_Status: Value indicating success or error code.
  */
  
int APP_BLE_GET_VERSION(uint8_t *hwVersion, uint16_t *fwVersion){


  uint8_t status;
  uint8_t hci_version, lmp_pal_version;
  uint16_t hci_revision, manufacturer_name, lmp_pal_subversion;

  status = hci_le_read_local_version(&hci_version, &hci_revision, &lmp_pal_version, 
				     &manufacturer_name, &lmp_pal_subversion);

  if (status == BLE_STATUS_SUCCESS) {
    *hwVersion = hci_revision >> 8;
    *fwVersion = (hci_revision & 0xFF) << 8;              // Major Version Number
    *fwVersion |= ((lmp_pal_subversion >> 4) & 0xF) << 4; // Minor Version Number
    *fwVersion |= lmp_pal_subversion & 0xF;               // Patch Version Number
  }

  //HCI_Isr_Event_Handler_CB();
  //HCI_Process(); // To receive the BlueNRG EVT
  HCI_Get_Event_CB();
  HCI_Packet_Release_Event_CB();
  return status;

}

/**
  * @brief  This function is called to setup the  address and the name of the device in the APP module.
  * @param  none
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_Set_Address_And_Name_BLE(const uint8_t * device_address,int device_address_size,const char * localname, int name_size)/*used for setup a device address and name*/
  {
      PTR_DEVICE_BDADDR = device_address;
      ptr_local_name = localname;
      if(PTR_DEVICE_BDADDR==NULL || ptr_local_name == NULL){
          return APP_ERROR;
      }
      name_size=name_size;
      address_size=device_address_size;
      
      return APP_SUCCESS;
  }



/**
  * @brief  This function is called to init the BLE architecture.
  * @param  none
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_Init_BLE(void){/*can be used by any application*/
  uint8_t ret = 0;
  uint8_t  hwVersion;
  uint16_t fwVersion;  
/* get the BlueNRG HW and FW versions */
  APP_BLE_GET_VERSION(&hwVersion, &fwVersion);
   BlueNRG_RST();
   
 
    if (hwVersion > 0x30) { 
    bnrg_expansion_board = IDB05A1; 
  	}

#if defined (MULTINODE)  
    uint8_t mode = 0x03;
    
    ret = aci_hal_write_config_data(CONFIG_DATA_MODE_OFFSET,
                                  0x01,
                                  &mode);    

        if (ret!= BLE_STATUS_SUCCESS)
        {
          return APP_ERROR;
        }
    
#endif  
    
    
    
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, PTR_DEVICE_BDADDR);

	if (ret != BLE_STATUS_SUCCESS)
  	{
    	return APP_ERROR;
  	}
        
 
  ret = aci_gatt_init();    
         if(ret)
         {
             return APP_ERROR;
         }
  
  	ret = aci_gap_init(GET_ROLE(bnrg_expansion_board), 0, 0x07,  &service_handle, &dev_name_char_handle,  &appearance_char_handle,bnrg_expansion_board);


  if (ret != BLE_STATUS_SUCCESS)
  {
    return APP_ERROR;
  }

   /* Set auth requirement*/
  aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                               OOB_AUTH_DATA_ABSENT,
                               NULL, 7, 16,
                               USE_FIXED_PIN_FOR_PAIRING, 123456,
                               BONDING);
                               
  /* Set output power level */
  aci_hal_set_tx_power_level(1, 7);
  
  return APP_SUCCESS;
}

/**
  * @brief  This function initialize the profile .
  * @ This function must be called at the beginning of the application.
  * @param profile data-structure.
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_init_BLE_Profile(app_profile_t * profile){
    profile->n_service=0;
    return APP_SUCCESS;
}


/**
  * @brief  This function is called to add any kind of Service.
  * @param  app_profile_t * profile: Profile structure.
  * @param  app_service_t * service: service to be included.
  * @retval APP_Status: Value indicating success or error code.
  */

APP_Status APP_add_BLE_Service(app_profile_t * profile, app_service_t * service){
  tBleStatus ret;
  app_service_t ** aux_service_addrs;
  if(profile == NULL || service==NULL)return APP_ERROR; /*it's not acepted input NULL*/
 
  if(profile->services==NULL)
    {/*if this is the first service to include included*/
      profile->services=service;
    } else if(profile->services!=service){/*if not check that it is not already included*/
        aux_service_addrs = &profile->services;
       do{/*USED to check if a service was already included to this profile*/
            aux_service_addrs = &((*aux_service_addrs)->next_service);
            if(*aux_service_addrs==service)break;
         }while(*aux_service_addrs!=NULL);
      
       
       if(*aux_service_addrs==NULL){
          *aux_service_addrs = service;/**/
       }else if(*aux_service_addrs == service)return APP_ERROR;
       
    }else if (profile->services == service)return APP_ERROR; /*if service is already included return error*/
  /*send the add service command  BULENRG-MS*/
  ret = aci_gatt_add_serv(service->service_uuid_type,
                          service->ServiceUUID,
                          service->service_type,
                          service->max_attr_records,
                          &(service->ServiceHandle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    
    /*remove the service if and error occur*/
    *aux_service_addrs =NULL;
    return APP_ERROR;
  }
  
  profile->n_service+=1; /*increment the control service counter*/
  profile->svflags.services_to_find+=1;
  profile->svflags.services_success_scanned=0;
 return APP_SUCCESS;
}

/**
  * @brief  This function is called to add any characteristic.
  * @param  app_service_t * service: Pointer to a service in which the characteristic will be included.
  * @param  service, app_attr_t *attr : Characteristic to include.
  * @retval APP_Status: Value indicating success or error code.
  */
APP_Status APP_add_BLE_attr(app_service_t * service, app_attr_t *attr){
    tBleStatus ret;
    app_attr_t ** aux_attr_addrs;
    if(service== NULL || attr == NULL  ) return APP_ERROR; /*it's not accepted input NULL*/
    
    if(service->attrs==NULL){
        service->attrs = attr; /*this is the first attribute associated to this service*/
    }else if (service->attrs != attr){
      /*if this entry is not already included*/
        aux_attr_addrs = &service->attrs;
         do{
           
            aux_attr_addrs = & ((*aux_attr_addrs)->next_attr);
            if(*aux_attr_addrs==attr)break;/*service already included for this profile*/
            
         }while(*aux_attr_addrs!=NULL);
         
       if(*aux_attr_addrs==NULL){
          *aux_attr_addrs=attr;/*if this attribute is  not already included add it to the list of attributes*/
       }else if(*aux_attr_addrs==attr) return APP_ERROR;  
         
    }else if (service->attrs ==  attr )  return APP_ERROR;
    
   /*send the add attr command  BULENRG-MS*/
    ret = aci_gatt_add_char(service->ServiceHandle,
                          attr->charUuidType, 
                          attr->CharUUID, 
                          attr->charValueLen, 
                          attr->charProperties, 
                          attr->secPermissions,
                          attr->gattEvtMask,
                          attr->encryKeySize,
                          attr->isVariable,
                          &(attr->CharHandle));
    
    if (ret != BLE_STATUS_SUCCESS) {
      /*delete the attr entry in the service list*/
      *aux_attr_addrs=NULL;
      return APP_ERROR;
      
    }  
    service->n_attr+=1;
    service->chrflags.char_to_scan+=1;
    service->chrflags.char_discovery_success=0;
    return APP_SUCCESS;
}


/**
  * @brief  This function return pointer to the device address.
  * @param int * size (used for catch the device address size)
  * @retval void *: void pointer to the address. 
  */

void * APP_get_direct_addrs_BLE(int * size){
    *size = address_size;
  return (void *)PTR_DEVICE_BDADDR;  
}


/**
  * @brief  This function retrieve the services presents in the profile.
  * @param app_profile_t * profile: profile which contain the services
  * @param app_profile_t * service: pointer used to retrieve the services in the profile
  *                                (If it is NULL the function return the first service from the list of services
                                    If it is different to NULL the function beginning form this input return the next service(if any or NULL) )
  * @retval void * : Return the service pointer as a void pointer..
  */

void * APP_get_service_BLE(app_profile_t * profile, void  * serv){
  app_service_t * temp_service = (app_service_t *) serv;

  if(profile == NULL) return NULL;
  
  if(serv == NULL){
     temp_service = (app_service_t *) profile->services;
  }else 
  {
    temp_service = (app_service_t *) temp_service->next_service;
  }
  
  return temp_service;
 
}


/**
  * @brief  This function retrieve the characteristics associated to a service.
  * @param app_service_t * service: service which contain a set of attributes
  * @param void *attr:              pointer used to retrieve the attributes associated to this particular service.
                                    (if it is NULL, return the first attribute associated to this service if any 
                                     if it is not NULL return then beginning from this attribute the function return the next
                                      attribute in the chain if any).
  * @retval void *: The function return a void * associated to the attributed retreated.
  */

void * APP_get_attribute_BLE(app_service_t * service, void *attr){
  app_attr_t * temp_att = (app_attr_t*) attr;
  
  if(service==NULL) return NULL;
  
  if(attr==NULL) temp_att = (app_attr_t *) service->attrs;
  else temp_att = (app_attr_t *) temp_att->next_attr;
  
  return temp_att;

}


/**
  * @brief  This function retrieve the BLUE-NRG Board's version.
  * @param void.
  * @retval uint8_t: Value indicating the version of the bluenrg board (IDB04A1=0 or IDB05A1=1).
  */
uint8_t get_harware_version(void){
return bnrg_expansion_board;
}

/**
  * @brief  This function retrieve pointer to the device local name .
  * @param void.
  * retval: const char *: pointer associated to the local name.
  */
const char * get_local_name (void){
  return ptr_local_name;
}

/**
  * @brief  This function the size of the local name .
  * @param void.
  * retval: uint8_t: device name size.
  */
int  get_local_name_size (void){
  return name_size;
}


