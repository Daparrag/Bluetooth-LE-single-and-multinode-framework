/*test file*/

#include <test.h>

#ifndef DEBUG
#define DEBUG 1;
#endif    

#ifdef DEBUG
#include "debug.h"
#endif
    
/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
#ifndef COPY_VAR
#define COPY_VAR(dest,source) memcpy(dest,(void*)source,sizeof((source)))
#endif



const uint8_t service_uuid1[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe0,0xf2,0x73,0xd9};
const uint8_t charUuidTX1[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe1,0xf2,0x73,0xd9};
const uint8_t charUuidRX1[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe2,0xf2,0x73,0xd9};


const uint8_t service_uuid2[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe3,0xf2,0x73,0xd9};
const uint8_t charUuidTX2[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe4,0xf2,0x73,0xd9};
const uint8_t charUuidRX2[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe5,0xf2,0x73,0xd9};



app_profile_t PROFILE;

app_service_t SERVICE1;
app_service_t SERVICE2;

app_attr_t ATTR11;
app_attr_t ATTR12;

app_attr_t ATTR21;
app_attr_t ATTR22;




void print_service_values(app_service_t * service){
  if(service==NULL)return;
    PRINTUUID(service->ServiceUUID);
    if(service->service_uuid_type == UUID_TYPE_128 ) PRINTF("service_uuid_type=UUID_TYPE_128\n");
      else PRINTF("service_uuid_type=UUID_TYPE_16\n");
      
    if(service->service_type == PRIMARY_SERVICE)PRINTF("service_type=PRIMARY_SERVICE\n");
      else PRINTF("service_type=SECUNDARY_SERVICE\n");
      
   PRINTF("Maximum attribute records= %d  \n",service->max_attr_records);
   PRINTF("number of attributes= %d  \n",service->max_attr_records); 
}


void print_attr_values(app_attr_t * attr){
  if(attr==NULL)return;
  PRINTUUID(attr->CharUUID);
    if(attr->charUuidType==UUID_TYPE_128)PRINTF("charUuidType=UUID_TYPE_128\n");
    else if(attr->charUuidType == UUID_TYPE_16)PRINTF("charUuidType=UUID_TYPE_16\n");
    
    
    if(attr->charProperties == CHAR_PROP_BROADCAST )PRINTF("charProperties=CHAR_PROP_BROADCAST\n");
    else if(attr->charProperties == CHAR_PROP_READ )PRINTF("charProperties=CHAR_PROP_READ\n");
    else if(attr->charProperties == CHAR_PROP_WRITE_WITHOUT_RESP )PRINTF("charProperties=CHAR_PROP_WRITE_WITHOUT_RESP\n");
    else if(attr->charProperties == CHAR_PROP_WRITE )PRINTF("charProperties=CHAR_PROP_WRITE\n");
    else if(attr->charProperties == CHAR_PROP_NOTIFY )PRINTF("charProperties=CHAR_PROP_NOTIFY\n");
    else if(attr->charProperties == CHAR_PROP_INDICATE )PRINTF("charProperties=CHAR_PROP_INDICATE\n");
    else if(attr->charProperties == CHAR_PROP_SIGNED_WRITE )PRINTF("charProperties=CHAR_PROP_SIGNED_WRITE\n");
    else if(attr->charProperties == CHAR_PROP_EXT )PRINTF("charProperties=CHAR_PROP_EXT\n");
    
    
    if(attr->secPermissions==ATTR_PERMISSION_NONE)PRINTF("secPermissions=ATTR_PERMISSION_NONE\n");
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHEN_READ) PRINTF("secPermissions=ATTR_PERMISSION_AUTHEN_READ\n");
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHOR_READ) PRINTF("secPermissions=ATTR_PERMISSION_AUTHOR_READ\n");
    else if (attr->secPermissions==ATTR_PERMISSION_ENCRY_READ) PRINTF("secPermissions=ATTR_PERMISSION_ENCRY_READ\n");
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHEN_WRITE) PRINTF("secPermissions=ATTR_PERMISSION_AUTHEN_WRITE\n"); 
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHOR_WRITE) PRINTF("secPermissions=ATTR_PERMISSION_AUTHOR_WRITE\n");
    else if (attr->secPermissions==ATTR_PERMISSION_ENCRY_WRITE) PRINTF("secPermissions=ATTR_PERMISSION_ENCRY_WRITE\n");
    
    if(attr->gattEvtMask==GATT_DONT_NOTIFY_EVENTS) PRINTF("gattEvtMask=GATT_DONT_NOTIFY_EVENTS\n"); 
    else if(attr->gattEvtMask==GATT_NOTIFY_ATTRIBUTE_WRITE) PRINTF("gattEvtMask=GATT_NOTIFY_ATTRIBUTE_WRITE\n");
    else if(attr->gattEvtMask==GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP) PRINTF("gattEvtMask=GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP\n");
    else if(attr->gattEvtMask==GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP) PRINTF("gattEvtMask=GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP\n"); 
    
    PRINTF("encryKeySize=\n",attr->encryKeySize);
    PRINTF("isVariable=\n",attr->isVariable);
}





Rtest_t app_module_test(){
/*this test consist in the creation of two services an two attributes using the app-firmware*/
  /*1. initialize the device*/ 
  APP_Status ret;
  app_service_t * RSERVICE=NULL;
  app_attr_t * RATTR=NULL;

  
  
  void * device_addrs=NULL;
  void * device_name = NULL;
  
  
  
  /*1. initialize the device*/ 
  ret = APP_Init_BLE();
    if(ret!=APP_SUCCESS)Error_Handler();
    
  /*2.get the address of the device*/
    int addrs_size = APP_get_addrs_BLE((uint8_t*)device_addrs);
      if (!addrs_size)Error_Handler();
      PRINTADDRS(device_addrs);
  /*3.get the name of the device*/ 
    int name_size = APP_get_name_BLE((char *)device_name);
      if (!name_size)Error_Handler(); 
    
    
    
  
  /*4.INIT THE PROFILE FOR THIS APPLICATION*/
  ret=APP_init_BLE_Profile(&PROFILE);
    if(ret!=APP_SUCCESS)Error_Handler(); 
  
  /*CREATE SERVICE 1*/
  COPY_VAR(SERVICE1.ServiceUUID,service_uuid1); 
  SERVICE1.service_uuid_type=UUID_TYPE_128;
  SERVICE1.service_type=PRIMARY_SERVICE;
  SERVICE1.max_attr_records=7;
  
   /*CREATE SERVICE 2*/
  COPY_VAR(SERVICE2.ServiceUUID,service_uuid2); 
  SERVICE2.service_uuid_type=UUID_TYPE_128;
  SERVICE2.service_type=PRIMARY_SERVICE;
  SERVICE2.max_attr_records=7;
  
  /*ADD SERVICE 1 AND 2 TO THE PROFILE*/
  ret=APP_add_BLE_Service(&PROFILE, &SERVICE1);
     if(ret!=APP_SUCCESS)Error_Handler(); 
  ret=APP_add_BLE_Service(&PROFILE, &SERVICE2);
     if(ret!=APP_SUCCESS)Error_Handler(); 
  
  /*CREATE ATT11 & ATT12*/
     /*ATT11*/
   COPY_VAR(ATTR11.CharUUID,charUuidTX1);
   ATTR11.charUuidType = UUID_TYPE_128;
   ATTR11.charValueLen=20;
   ATTR11.charProperties = CHAR_PROP_NOTIFY;
   ATTR11.secPermissions = ATTR_PERMISSION_NONE;
   ATTR11.gattEvtMask = GATT_DONT_NOTIFY_EVENTS;
   ATTR11.encryKeySize=16;
   ATTR11.isVariable=1;
     /*ATT12*/
    COPY_VAR(ATTR12.CharUUID,charUuidRX1);
   ATTR12.charUuidType = UUID_TYPE_128;
   ATTR12.charValueLen=20;
   ATTR12.charProperties = CHAR_PROP_NOTIFY;
   ATTR12.secPermissions = ATTR_PERMISSION_NONE;
   ATTR12.gattEvtMask = GATT_DONT_NOTIFY_EVENTS;
   ATTR12.encryKeySize=16;
   ATTR12.isVariable=1;
   
   /*ASOCCIATE ATT11 and ATT12 to the SERVICE1*/
    ret=APP_add_BLE_attr(&SERVICE1,&ATTR11);
       if(ret!=APP_SUCCESS)Error_Handler(); 
    ret=APP_add_BLE_attr(&SERVICE1,&ATTR12);
       if(ret!=APP_SUCCESS)Error_Handler(); 
   
     /*CREATE ATT21 & ATT22*/ 
    COPY_VAR(ATTR21.CharUUID,charUuidTX2);
   ATTR21.charUuidType = UUID_TYPE_128;
   ATTR21.charValueLen=20;
   ATTR21.charProperties = CHAR_PROP_NOTIFY;
   ATTR21.secPermissions = ATTR_PERMISSION_NONE;
   ATTR21.gattEvtMask = GATT_DONT_NOTIFY_EVENTS;
   ATTR21.encryKeySize=16;
   ATTR21.isVariable=1;
   
    COPY_VAR(ATTR22.CharUUID,charUuidRX2);
   ATTR22.charUuidType = UUID_TYPE_128;
   ATTR22.charValueLen=20;
   ATTR22.charProperties = CHAR_PROP_NOTIFY;
   ATTR22.secPermissions = ATTR_PERMISSION_NONE;
   ATTR22.gattEvtMask = GATT_DONT_NOTIFY_EVENTS;
   ATTR22.encryKeySize=16;
   ATTR22.isVariable=1;
   
 
   /*ASOCCIATE ATT11 and ATT12 to the SERVICE1*/
    ret=APP_add_BLE_attr(&SERVICE2,&ATTR21);
       if(ret!=APP_SUCCESS)Error_Handler(); 
    ret=APP_add_BLE_attr(&SERVICE2,&ATTR22);
       if(ret!=APP_SUCCESS)Error_Handler(); 
       
       
   /*GET SERVICE1 AND SERVICE 2*/
       do{
           ret=APP_get_service_BLE(&PROFILE,RSERVICE);
            if(ret!=APP_SUCCESS)Error_Handler();
            print_service_values(RSERVICE);
            
            
       }while(RSERVICE!=NULL);
       
       
        /*GET SERVICE1 ATTR1 ATTR2*/
       do{
          ret=APP_get_attribute_BLE(&SERVICE1,RATTR);
          if(ret!=APP_SUCCESS)Error_Handler();
          print_attr_values(RATTR);
       }while(RATTR!=NULL)
         
         
        /*GET SERVICE2 ATTR1 ATTR2*/
          do{
          ret=APP_get_attribute_BLE(&SERVICE2,RATTR);
          if(ret!=APP_SUCCESS)Error_Handler();
          print_attr_values(RATTR);
       }while(RATTR!=NULL)
         
       
       
        
return TEST_SUCCESS;
}