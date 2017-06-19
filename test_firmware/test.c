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
    PRINTDEBUG("PRINTING SERVICE...\n");
    PRINTUUID(service->ServiceUUID);
    if(service->service_uuid_type == UUID_TYPE_128 ) PRINTDEBUG("service_uuid_type=UUID_TYPE_128\n");
      else PRINTDEBUG("service_uuid_type=UUID_TYPE_16\n");
      
    if(service->service_type == PRIMARY_SERVICE)PRINTDEBUG("service_type=PRIMARY_SERVICE\n");
      else PRINTDEBUG("service_type=SECUNDARY_SERVICE\n");
      
   PRINTDEBUG("Maximum attribute records= %d  \n",service->max_attr_records);
   PRINTDEBUG("number of attributes= %d  \n",service->n_attr); 
}


void print_attr_values(app_attr_t * attr){
  if(attr==NULL)return;
  PRINTDEBUG("PRINTING CHARACTERISTIC...\n");
  PRINTUUID(attr->CharUUID);
    if(attr->charUuidType==UUID_TYPE_128)PRINTDEBUG("charUuidType=UUID_TYPE_128\n");
    else if(attr->charUuidType == UUID_TYPE_16)PRINTDEBUG("charUuidType=UUID_TYPE_16\n");
    
    
    if(attr->charProperties == CHAR_PROP_BROADCAST )PRINTDEBUG("charProperties=CHAR_PROP_BROADCAST\n");
    else if(attr->charProperties == CHAR_PROP_READ )PRINTDEBUG("charProperties=CHAR_PROP_READ\n");
    else if(attr->charProperties == CHAR_PROP_WRITE_WITHOUT_RESP )PRINTDEBUG("charProperties=CHAR_PROP_WRITE_WITHOUT_RESP\n");
    else if(attr->charProperties == CHAR_PROP_WRITE )PRINTDEBUG("charProperties=CHAR_PROP_WRITE\n");
    else if(attr->charProperties == CHAR_PROP_NOTIFY )PRINTDEBUG("charProperties=CHAR_PROP_NOTIFY\n");
    else if(attr->charProperties == CHAR_PROP_INDICATE )PRINTDEBUG("charProperties=CHAR_PROP_INDICATE\n");
    else if(attr->charProperties == CHAR_PROP_SIGNED_WRITE )PRINTDEBUG("charProperties=CHAR_PROP_SIGNED_WRITE\n");
    else if(attr->charProperties == CHAR_PROP_EXT )PRINTDEBUG("charProperties=CHAR_PROP_EXT\n");
    
    
    if(attr->secPermissions==ATTR_PERMISSION_NONE)PRINTDEBUG("secPermissions=ATTR_PERMISSION_NONE\n");
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHEN_READ) PRINTDEBUG("secPermissions=ATTR_PERMISSION_AUTHEN_READ\n");
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHOR_READ) PRINTDEBUG("secPermissions=ATTR_PERMISSION_AUTHOR_READ\n");
    else if (attr->secPermissions==ATTR_PERMISSION_ENCRY_READ) PRINTDEBUG("secPermissions=ATTR_PERMISSION_ENCRY_READ\n");
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHEN_WRITE) PRINTDEBUG("secPermissions=ATTR_PERMISSION_AUTHEN_WRITE\n"); 
    else if (attr->secPermissions==ATTR_PERMISSION_AUTHOR_WRITE) PRINTDEBUG("secPermissions=ATTR_PERMISSION_AUTHOR_WRITE\n");
    else if (attr->secPermissions==ATTR_PERMISSION_ENCRY_WRITE) PRINTDEBUG("secPermissions=ATTR_PERMISSION_ENCRY_WRITE\n");
    
    if(attr->gattEvtMask==GATT_DONT_NOTIFY_EVENTS) PRINTDEBUG("gattEvtMask=GATT_DONT_NOTIFY_EVENTS\n"); 
    else if(attr->gattEvtMask==GATT_NOTIFY_ATTRIBUTE_WRITE) PRINTDEBUG("gattEvtMask=GATT_NOTIFY_ATTRIBUTE_WRITE\n");
    else if(attr->gattEvtMask==GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP) PRINTDEBUG("gattEvtMask=GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP\n");
    else if(attr->gattEvtMask==GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP) PRINTDEBUG("gattEvtMask=GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP\n"); 
    
    PRINTDEBUG("encryKeySize= %d\n",attr->encryKeySize);
    PRINTDEBUG("isVariable= %d\n",attr->isVariable);
}

void print_device_name(char * name, int namesize){

  int i;
  printf("Device name: ");
  for(i=0; i < namesize; i++)printf("%c",name[i]); 
   printf("\n");
}





Rtest_t app_module_test(){

/* 

* expected output

* BLE: address 0x55:0x11:0x07:0x01:0x16:0xE1 
* Device name: 	BLE-UNO
*
* PRINTING SERVICE...
* BLE: _UUID 0x66:0x9A:0x0C:0x20:0x00:0x08:0x96:0x9E:0xE2:0x11:0x9E:0xB1:0xE0:0xF2:0x73:0xD9 
* service_uuid_type=UUID_TYPE_128
* service_type=PRIMARY_SERVICE
* Maximum attribute records= 7  
* number of attributes= 2  
*
* PRINTING CHARACTERISTIC...
* BLE: _UUID 0x66:0x9A:0x0C:0x20:0x00:0x08:0x96:0x9E:0xE2:0x11:0x9E:0xB1:0xE1:0xF2:0x73:0xD9 
* charUuidType=UUID_TYPE_128
* charProperties=CHAR_PROP_NOTIFY
* secPermissions=ATTR_PERMISSION_NONE
* gattEvtMask=GATT_DONT_NOTIFY_EVENTS
* encryKeySize= 16
* isVariable= 1
*
* PRINTING CHARACTERISTIC...
* BLE: _UUID 0x66:0x9A:0x0C:0x20:0x00:0x08:0x96:0x9E:0xE2:0x11:0x9E:0xB1:0xE2:0xF2:0x73:0xD9 
* charUuidType=UUID_TYPE_128
* charProperties=CHAR_PROP_NOTIFY
* secPermissions=ATTR_PERMISSION_NONE
* gattEvtMask=GATT_DONT_NOTIFY_EVENTS
* encryKeySize= 16
* isVariable= 1
*
* PRINTING SERVICE...
* BLE: _UUID 0x66:0x9A:0x0C:0x20:0x00:0x08:0x96:0x9E:0xE2:0x11:0x9E:0xB1:0xE3:0xF2:0x73:0xD9 
* service_uuid_type=UUID_TYPE_128
* service_type=PRIMARY_SERVICE
* Maximum attribute records= 7  
* number of attributes= 2  
*
* PRINTING CHARACTERISTIC...
* BLE: _UUID 0x66:0x9A:0x0C:0x20:0x00:0x08:0x96:0x9E:0xE2:0x11:0x9E:0xB1:0xE4:0xF2:0x73:0xD9 
* charUuidType=UUID_TYPE_128
* charProperties=CHAR_PROP_NOTIFY
* secPermissions=ATTR_PERMISSION_NONE
* gattEvtMask=GATT_DONT_NOTIFY_EVENTS
* encryKeySize= 16
* isVariable= 1
* 
* PRINTING CHARACTERISTIC...
* BLE: _UUID 0x66:0x9A:0x0C:0x20:0x00:0x08:0x96:0x9E:0xE2:0x11:0x9E:0xB1:0xE5:0xF2:0x73:0xD9 
* charUuidType=UUID_TYPE_128
* charProperties=CHAR_PROP_NOTIFY
* secPermissions=ATTR_PERMISSION_NONE
* gattEvtMask=GATT_DONT_NOTIFY_EVENTS
* encryKeySize= 16
* isVariable= 1
*
*/


uint8_t i;
uint8_t j;
  /*this test consist in the creation of two services an two attributes using the app-firmware*/
 
  /*1. initialize the device*/ 
  APP_Status ret;
  app_service_t * RSERVICE = NULL;
  app_attr_t * RATTR = NULL;
  int address_size = 0;
  int name_size = 0;

  
  
  void * device_addrs = NULL;
  void * device_name = NULL;
  
  
  
  /*1. initialize the device*/ 
  ret = APP_Init_BLE();
    if(ret!=APP_SUCCESS)return TEST_FAIL;
    
  /*2.get the address of the device*/
     device_addrs = APP_get_direct_addrs_BLE(&address_size);
     
      if (address_size==0 || (uint8_t *)device_addrs == NULL)return TEST_FAIL;
      PRINTADDRS(device_addrs);
  /*3.get the name of the device*/ 
    device_name = APP_get_direct_name_BLE(&name_size);
      if (!name_size || (char *) device_name == NULL)return TEST_FAIL;
      print_device_name((char *)device_name,name_size);
      
    
    
  
  /*4.INIT THE PROFILE FOR THIS APPLICATION*/
  ret=APP_init_BLE_Profile(&PROFILE);
    if(ret!=APP_SUCCESS)return TEST_FAIL; 
  
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
     if(ret!=APP_SUCCESS)return TEST_FAIL;
   ret=APP_add_BLE_Service(&PROFILE, &SERVICE2);
     if(ret!=APP_SUCCESS)return TEST_FAIL;   

     
     
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
       if(ret!=APP_SUCCESS)return TEST_FAIL; 
    ret=APP_add_BLE_attr(&SERVICE1,&ATTR12);
       if(ret!=APP_SUCCESS)return TEST_FAIL; 
       
       
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
       if(ret!=APP_SUCCESS)return TEST_FAIL; 
    ret=APP_add_BLE_attr(&SERVICE2,&ATTR22);
       if(ret!=APP_SUCCESS)return TEST_FAIL; 
       
       /*GET ALL*/ 
       
       RSERVICE = NULL;
       for(i=0; i < PROFILE.n_service; i++)
       {
         RSERVICE = APP_get_service_BLE(&PROFILE,(void *)RSERVICE);
         if(RSERVICE==NULL) break;
            print_service_values(RSERVICE);
         RATTR=NULL;
         for (j=0; j < RSERVICE->n_attr; j++)
         {
            RATTR = APP_get_attribute_BLE ((app_service_t *)RSERVICE,RATTR);
             if(RATTR==NULL) break;
              print_attr_values(RATTR);  
         }
       }     
return TEST_SUCCESS;
}


Rtest_t chandler_module_test_central(void){

  
  return TEST_SUCCESS;
}


Rtest_t chandler_module_test_pherispheral(void){

  
  return TEST_SUCCESS;
}