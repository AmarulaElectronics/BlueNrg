/*******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2014 STMicroelectronics International NV
*
*   FILENAME        -  profile_HR_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      29-August-2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Heart Rate Collector. 
*                It uses the BlueNRG-1 Master Profiles Library.
*
*******************************************************************************
*
*  File Description 
*  ---------------------
* 
*******************************************************************************/

/*******************************************************************************
 * Include Files
*******************************************************************************/
/* new from BlueNRG DK framework */

#include "osal.h"
#include "stdio.h"
#include "ble_const.h" 
#include "bluenrg1_api.h"
#include "BlueNRG1_stack.h" 
#include "host_config.h"
#include "Profile_config.h"
#include "debug.h"
#include "sleep.h"
#include "SDK_EVAL_Config.h"
#include "debug.h"
#include "master_basic_profile.h"

#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_COLLECTOR)
#include <heart_rate_service.h>
#include <heart_rate_collector_config.h>
#include <heart_rate_collector.h>

/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS PTS_PERIPHERAL_ADDRESS //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS

/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR

/* Heart_Rate Collector test configuration for some PTS tests */

///@cond BLE_CURRENT_PROFILE_ROLES & HEART_RATE_COLLECTOR
/**
 *@defgroup Heart_Rate_Collector
 *@brief PTS test application user input commands for Heart Rate Collector Central role
 *@{
 */

//General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 *  Heart Rate Collector user input command: Start Discovery procedure for detecting an Heart Rate Sensor(<b>HRC_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_HR_SENSOR                    'a' 

/** 
 *  Heart Rate Collector user input command: Start Connection procedure for connecting to the discovered Heart Rate Sensor(<b> HRC_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_HR_SENSOR                     'b' 

/** 
 *  Heart Rate Collector user input command: Terminate the current connection with the Heart Rate Sensor(<b> HRC_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_HR_SENSOR                  'c' 

/** 
 *   Heart Rate Collector user input command: Start the pairing procedure (or enable the encryption) with the connected Heart Rate Sensor (<b> HRC_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_HR_SENSOR                   'd'

/** 
 *  Heart Rate Collector user input command: Clear the security database (it removes any bonded device) (<b>HRC_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE               'e' 

//------------- Discovery

/** 
 *  Heart Rate Collector user input command: Discover All Primary Services (<b>HRC_ServicesDiscovery() </b> API)
 */
#define APPL_DISCOVER_HEART_RATE_SERVICES          'f' 
/** 
 *  NOTE: user input not currently used 
 */
#define APPL_DISCOVER_DEVICE_INFORMATION_SERVICES  'g'   

/** 
 *  Heart Rate Collector user input command: Discover All Characteristics of Heart Rate Services -->  Heart Rate Measurement Services, Body Sensor Location, Heart Rate Control Point characteristics (<b>HRC_DiscCharacServ(HEART_RATE_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVER_HR_CHARACTERISTICS                'h' 

/** 
 *  Heart Rate Collector user input command: Discover Heart Rate Measurement Client Characteristic Configuration Descriptor (<b>HRC_Start_Heart_Rate_Measurement_Characteristic_Descriptor_Discovery() </b> API)
 */                  
#define APPL_DISCOVER_HR_MEASUREMENT_CHAR_DESCRIPTOR    'i' 

//#define APPL_DISCOVER_BODY_SENSOR_LOCATION_CHAR         'j' //Discover Body Sensor Location Characteristic
//#define APPL_DISCOVER_HR_CTRL_POINT_CHAR                'k' //Discover Heart Rate Control Point Characteristic

/** 
 *  Heart Rate Collector user input command: Discover Device Information Service (DIS) Characteristics (<b>HRC_DiscCharacServ(DEVICE_INFORMATION_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVER_DIS_CHARACTERISTICS               'l' 

/** 
 *  Heart Rate Collector user input command: Read Device Information Service Manufacturer Name Characteristic (<b>HRC_ReadDISManufacturerNameChar() </b> API)
 */
#define APPL_READ_DIS_MANUFACTURER_NAME_CHAR            'm'

/** 
 *  Heart Rate Collector user input command: Configure Heart Rate Measurement for Notification (<b>HRC_Enable_HR_Measurement_Char_Notification() </b> API)
 */
#define APPL_ENABLE_HR_MEASUREMENT_CHAR_NOTIFICATION    'n' 

//----------------------------- NOTIFICATIONS
//Receive Heart Rate Measurement Notifications
//Receive Heart Rate Measurement Notifications with reserved flags
//Receive Heart Rate Measurement Notifications with additional octets not represented in the flags field
//Receive multiple Heart Rate Measurement Notifications

//----------------------------- READ BODY SENSOR LOCATION 

/** 
 *  Heart Rate Collector user input command: Read Body Sensor Location characteristic(<b>HRC_Read_Body_Sensor_Location() </b> API)
 */
#define APPL_READ_BODY_SENSOR_LOCATION_CHAR             'o' 

/** 
 *  Heart Rate Collector user input command: Write Heart Rate Control Point characteristic reset energy expended(<b>HRC_Write_HR_Control_Point() </b> API)
 */
#define APPL_WRITE_HR_CTRL_POINT_CHAR                   'p' 

//#define APPL_VERIFY_BOND_STATUS_ON_RECONNECTION         'q' //Verify Bond Status on Reconnection

/** 
 *  Heart Rate Collector user input command: Starts all the connection, service discovery, characteristic discovery, ..... procedures.
 */
#define APPL_START_FULL_CONFIGURATION                   'w' 

/** 
 *  Heart Rate Collector user input command: Display PTS user input commands list.
 */
#define DISPLAY_PTS_MENU                                'z'
/**
 * @}
 */
///@endcond

/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/******************************************************************************
 * Local Variable Declarations
******************************************************************************/
uint8_t deviceState = APPL_UNINITIALIZED;

SleepModes sleep_mode = SLEEPMODE_WAKETIMER; 

/******************************************************************************
 * Global Variable Declarations
******************************************************************************/

void *profiledbgfile;

uint8_t status;


/******************************************************************************
 * Function Declarations
******************************************************************************/
uint8_t __io_getcharNonBlocking(uint8_t *data);

/******************************************************************************
 * Function Definitions 
******************************************************************************/
/* Enable the Sleep mode: SLEEPMODE_NOTIMER or SLEEPMODE_WAKETIMER with wake up on push1 button */
void Enter_Sleep_Mode(void)
{
  uint8_t ret, wakeup_source, wakeup_level;   
  
  /* BlueNRG-1 STEVAL-IDB007V1 PUSH1 Button: WAKEUP_IO13 */
  wakeup_source = WAKEUP_IO13; 
  wakeup_level = (WAKEUP_IOx_LOW << WAKEUP_IO13_SHIFT_MASK); 
  
  //printf("\r\nEnter to Sleep(mode: %d)\r\n", sleepMode);
  ret = BlueNRG_Sleep(sleep_mode, wakeup_source, wakeup_level);
  if (ret != BLE_STATUS_SUCCESS) 
  {
    printf("BlueNRG_Sleep() error 0x%02x\r\n", ret);
    while(1);
  }
}/* end Enter_Sleep_Mode*/

void printDebugServices(uint8_t numReporterServices, uint8_t *reporterServices)
{
  uint8_t i , j;
  uint16_t handle, uuid;
  //for PTS Automating
  
  //APPL_MESG_INFO(profiledbgfile,"****    Number of Primary Service: %d\r\n", numReporterServices);
  j = 0;
  for (i=0; i<numReporterServices; i++) {
    if (reporterServices[j] == UUID_TYPE_16) {
      Osal_MemCpy(&uuid, &reporterServices[j+5], 2);
      switch (uuid) {
      case HEART_RATE_SERVICE_UUID:
#ifdef PTS_AUTOMATING 
          APPL_MESG_INFO(profiledbgfile,"*********************************************\r\n");
          APPL_MESG_INFO(profiledbgfile,"(%c%c1) ****    Heart Rate Service UUID found\r\n", PTS_CODE, HRP_PTS_CODE);  
#else
          APPL_MESG_INFO(profiledbgfile,"****    Heart Rate Service UUID found\r\n");
#endif	
        break;
      case DEVICE_INFORMATION_SERVICE_UUID:
#ifdef PTS_AUTOMATING
          APPL_MESG_INFO(profiledbgfile,"(%c%c2) ****    Device Information Service UUID found\r\n", PTS_CODE, HRP_PTS_CODE)
#else
          APPL_MESG_INFO(profiledbgfile,"****    Device Information Service UUID found\r\n");
#endif
	break;
      default:
#ifndef PTS_AUTOMATING
          APPL_MESG_INFO(profiledbgfile,"****    UUID 0x%04x\r\n", uuid);
#endif
  break;
      }
    }
    j++;
    Osal_MemCpy(&handle, &reporterServices[j], 2);

#ifdef PTS_AUTOMATING
      if ((uuid == HEART_RATE_SERVICE_UUID) || (uuid == DEVICE_INFORMATION_SERVICE_UUID))
      {  
        APPL_MESG_INFO(profiledbgfile," ****       Start Handle = 0x%04x\r\n", handle);
        j += 2;
        Osal_MemCpy(&handle, &reporterServices[j], 2);
        APPL_MESG_INFO(profiledbgfile," ****       End Handle = 0x%04x\r\n", handle);
      }
      else
      {  
        j += 2;
        Osal_MemCpy(&handle, &reporterServices[j], 2);
      }  
#else 
      APPL_MESG_INFO(profiledbgfile," ****       Start Handle = 0x%04x\r\n", handle);
      j += 2;
      Osal_MemCpy(&handle, &reporterServices[j], 2);
      APPL_MESG_INFO(profiledbgfile," ****       End Handle = 0x%04x\r\n", handle);  
#endif    

    j += 2;    
    if (reporterServices[j-5] == UUID_TYPE_16) {
      j += 2;
    } else {
      j += 16;
    }
  }
}
void printDebugCharac(uint8_t numCharac, uint8_t *charac)
{
  uint8_t i, index, sizeElement, properties;
  uint16_t value_handle, charac_handle, uuid_charac;
  uint8_t bPrint=0;

  //APPL_MESG_INFO(profiledbgfile,"****    Number of Charac of a Service: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 8;
    Osal_MemCpy(&uuid_charac, &charac[index+6], 2);
    } else {
      sizeElement = 22;
    }

    switch(uuid_charac) {
    case HEART_RATE_MEASURMENT_UUID:
#ifdef PTS_AUTOMATING 
      APPL_MESG_INFO(profiledbgfile,"(%c%c3)****      Heart Rate Measurement Characteristic found\r\n", PTS_CODE, HRP_PTS_CODE)
#else
      APPL_MESG_INFO(profiledbgfile,"****      Heart Rate Measurement Characteristic found\r\n");
#endif
      bPrint = 1;        
      break;
    case SENSOR_LOCATION_UUID:
#ifdef PTS_AUTOMATING 
      APPL_MESG_INFO(profiledbgfile,"*********************************************\r\n");
      APPL_MESG_INFO(profiledbgfile,"(%c%c4)****      Heart Rate Body Sensor Location Characteristic found\r\n", PTS_CODE, HRP_PTS_CODE);
#else
      APPL_MESG_INFO(profiledbgfile,"****       Heart Rate Body Sensor Location Characteristic found\\r\n");
#endif
      bPrint = 1;        
      break;
    case CONTROL_POINT_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"*********************************************\r\n");  
      APPL_MESG_INFO(profiledbgfile,"(%c%c5)****      Heart Rate Control Point Characteristic found\r\n", PTS_CODE, HRP_PTS_CODE)      
#else
      APPL_MESG_INFO(profiledbgfile,"****      Heart Rate Control Point Characteristic found\r\n");
#endif
      bPrint = 1; 
      break; 
    
    case MANUFACTURER_NAME_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c6)****      Device Info Manufacturer Name Charac found\r\n", PTS_CODE, HRP_PTS_CODE)
#else
      APPL_MESG_INFO(profiledbgfile,"****      Device Information Manufacturer Name Characteristic found\r\n");
#endif        
      bPrint = 1; 
      break;
        
    default:
#ifndef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic found\r\n");
      bPrint = 1;  
#else
      bPrint = 0;
#endif
    }

    Osal_MemCpy(&charac_handle, &charac[index+1], 2);
    properties = charac[index+3];
    Osal_MemCpy(&value_handle, &charac[index+4], 2);
    if (bPrint)
    {
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Handle = 0x%04x\r\n", charac_handle);
      APPL_MESG_INFO(profiledbgfile,"****      Properties = 0x%02x\r\n", properties);
      APPL_MESG_INFO(profiledbgfile,"****      ValueHandle = 0x%04x\r\n", value_handle);
      APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_charac);
    }  

    index += sizeElement;
  }
}

//void printDebugCharac(uint8_t numCharac, uint8_t *charac)
//{
//  uint8_t i, index, sizeElement, properties;
//  uint16_t value_handle, charac_handle, uuid_charac;
//
//  APPL_MESG_INFO(profiledbgfile,"****    Number of Charac of a Service: %d\r\n", numCharac);
//  index = 0;
//  for (i=0; i<numCharac; i++) { 
//    if (charac[index] == 1) {
//      sizeElement = 8;
//    Osal_MemCpy(&uuid_charac, &charac[index+6], 2);
//    } else {
//      sizeElement = 22;
//    }
//
//    switch(uuid_charac) {
//    case HEART_RATE_MEASURMENT_UUID:
//      APPL_MESG_INFO(profiledbgfile,"****      Heart Rate Measurement Characteristic found\r\n");
//      break;
//    case SENSOR_LOCATION_UUID:
//      APPL_MESG_INFO(profiledbgfile,"****      Heart Rate Body Sensor Location Characteristic found\r\n");
//      break;
//    case CONTROL_POINT_UUID:
//      APPL_MESG_INFO(profiledbgfile,"****      Heart Rate Control Point Characteristic found\r\n");
//      break; 
//    
//    case MANUFACTURER_NAME_UUID:
//      APPL_MESG_INFO(profiledbgfile,"****      Device Information Manufacturer Name Characteristic found\r\n");
//      break;
//        
//    default:
//      APPL_MESG_INFO(profiledbgfile,"****      Characteristic found\r\n");
//    }
//
//    Osal_MemCpy(&charac_handle, &charac[index+1], 2);
//    properties = charac[index+3];
//    Osal_MemCpy(&value_handle, &charac[index+4], 2);
//      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Handle = 0x%04x\r\n", charac_handle);
//      APPL_MESG_INFO(profiledbgfile,"****      Properties = 0x%02x\r\n", properties);
//      APPL_MESG_INFO(profiledbgfile,"****      ValueHandle = 0x%04x\r\n", value_handle);
//      APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_charac);
//
//    index += sizeElement;
//  }
//}

void printDebugCharacDesc(uint8_t numCharac, uint8_t *charac)
{
  uint8_t i, index, sizeElement;
  uint16_t handle_characDesc, uuid_characDesc;

  APPL_MESG_INFO(profiledbgfile,"****    Number of Charac Descriptor: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 5;
      Osal_MemCpy(&uuid_characDesc, &charac[index+3], 2);
    } else {
      sizeElement = 19;
    }

    Osal_MemCpy(&handle_characDesc, &charac[index+1], 2);

    if (CHAR_PRESENTATION_FORMAT_DESCRIPTOR_UUID) {
    }
    switch(uuid_characDesc) {
    case CHAR_PRESENTATION_FORMAT_DESCRIPTOR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Presentation Format Descriptor found\r\n");
      break;
    case CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c7)", PTS_CODE, HRP_PTS_CODE)
#endif
      APPL_MESG_INFO(profiledbgfile,"****      Client Characteristic Configuration Descriptor found\r\n");
      break;
    default:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Descriptor found\r\n");
    }

    APPL_MESG_INFO(profiledbgfile,"****      Characteristic Descriptor Handle = 0x%04x\r\n", handle_characDesc);
    APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_characDesc);

    index += sizeElement;
  }
}

void Display_Appl_Menu()
{
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_HR_SENSOR: %c\n", APPL_DISCOVER_HR_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_HR_SENSOR: %c\n",APPL_CONNECT_HR_SENSOR);  
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_HR_SENSOR: %c\n",APPL_DISCONNECT_HR_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_HR_SENSOR: %c\n",APPL_PAIR_WITH_HR_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_HEART_RATE_SERVICES: %c\n",APPL_DISCOVER_HEART_RATE_SERVICES);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_DEVICE_INFORMATION_SERVICES: %c\n",APPL_DISCOVER_DEVICE_INFORMATION_SERVICES);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_HR_CHARACTERISTICS: %c\n",APPL_DISCOVER_HR_CHARACTERISTICS);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_HR_MEASUREMENT_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_HR_MEASUREMENT_CHAR_DESCRIPTOR);
  //APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_BODY_SENSOR_LOCATION_CHAR: %c\n",APPL_DISCOVER_BODY_SENSOR_LOCATION_CHAR); //TBR???
  //APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_HR_CTRL_POINT_CHAR: %c\n",APPL_DISCOVER_HR_CTRL_POINT_CHAR); //TBR??
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_DIS_CHARACTERISTICS: %c\n",APPL_DISCOVER_DIS_CHARACTERISTICS);
  APPL_MESG_DBG(profiledbgfile,"APPL_READ_DIS_MANUFACTURER_NAME_CHAR: %c\n",APPL_READ_DIS_MANUFACTURER_NAME_CHAR);
  APPL_MESG_DBG(profiledbgfile,"APPL_ENABLE_HR_MEASUREMENT_CHAR_NOTIFICATION: %c\n",APPL_ENABLE_HR_MEASUREMENT_CHAR_NOTIFICATION);
  APPL_MESG_DBG(profiledbgfile,"APPL_READ_BODY_SENSOR_LOCATION_CHAR: %c\n",APPL_READ_BODY_SENSOR_LOCATION_CHAR);
  APPL_MESG_DBG(profiledbgfile,"APPL_WRITE_HR_CTRL_POINT_CHAR: %c\n",APPL_WRITE_HR_CTRL_POINT_CHAR);
 // APPL_MESG_DBG(profiledbgfile,"APPL_VERIFY_BOND_STATUS_ON_RECONNECTION: %u\n",APPL_VERIFY_BOND_STATUS_ON_RECONNECTION);
  
  APPL_MESG_DBG(profiledbgfile,"APPL_START_FULL_CONFIGURATION: %c\n",APPL_START_FULL_CONFIGURATION);
  APPL_MESG_DBG(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);  
  
}/* end Display_Appl_Menu() */

void Device_Init(void)
{
  hrcInitDevType InitDev; 
  uint8_t public_addr[6]= HRC_PUBLIC_ADDRESS;
  uint8_t device_name[]=HRC_DEVICE_NAME;
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      HRC_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  
  ble_status = HRC_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  hrcSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  HRC_IO_CAPABILITY;
  param.mitm_mode =     HRC_MITM_MODE;
  param.bonding_mode =  HRC_BONDING_MODE;
  param.use_fixed_pin = HRC_USE_FIXED_PIN;
  param.fixed_pin =     HRC_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
  
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = HRC_SC_SUPPORT;
  param.keypress_support = HRC_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = HRC_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    HRC_OOB_ENABLE;
#endif  
  ble_status = HRC_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  hrcDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  HRC_GEN_DISC_SCAN_INT;
  param.scanWindow =    HRC_GEN_DISC_SCAN_WIND;
  ble_status = HRC_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  hrcConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         HRC_FAST_SCAN_DURATION;
  param.fastScanInterval =         HRC_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           HRC_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = HRC_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   HRC_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        HRC_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        HRC_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             HRC_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      HRC_SUPERVISION_TIMEOUT;
  param.min_conn_length =          HRC_MIN_CONN_LENGTH;
  param.max_conn_length =          HRC_MAX_CONN_LENGTH;
  ble_status = HRC_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = HRC_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_ServicesDiscovery(void)
{
  tBleStatus ble_status;
  ble_status = HRC_ServicesDiscovery();

  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_ServicesDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_ServicesDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = HRC_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"HRC_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_CharacServ(uint16_t uuid_service)
{
  tBleStatus ble_status;
  ble_status = HRC_DiscCharacServ(uuid_service);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"Device_Discovery_CharacServ() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Device_Discovery_CharacServ() Error: 0x%02X, uuid_service: 0x%02X\n", ble_status,uuid_service);
  }
}

void Device_StartFullConfig(void)
{
  hrcConnDevType connParam;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS; 

  /* Connection Parameter */
  connParam.fastScanDuration =         HRC_FAST_SCAN_DURATION;
  connParam.fastScanInterval =         HRC_FAST_SCAN_INTERVAL;
  connParam.fastScanWindow =           HRC_FAST_SCAN_WINDOW;
  connParam.reducedPowerScanInterval = HRC_REDUCED_POWER_SCAN_INTERVAL;
  connParam.reducedPowerScanWindow =   HRC_REDUCED_POWER_SCAN_WINDOW;
  connParam.peer_addr_type =           PUBLIC_ADDR;
  connParam.peer_addr =                peer_addr;
  connParam.own_addr_type =            PUBLIC_ADDR;
  connParam.conn_min_interval =        HRC_FAST_MIN_CONNECTION_INTERVAL;
  connParam.conn_max_interval =        HRC_FAST_MAX_CONNECTION_INTERVAL;
  connParam.conn_latency =             HRC_FAST_CONNECTION_LATENCY;
  connParam.supervision_timeout =      HRC_SUPERVISION_TIMEOUT;
  connParam.min_conn_length =          HRC_MIN_CONN_LENGTH;
  connParam.max_conn_length =          HRC_MAX_CONN_LENGTH;

  /* Configuration Parameter */

  if (HRC_ConnConf(connParam) != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"Error in HRC_ConnConf()\r\n");
  } 
  //else {
    //glucoseCollectorContext.useConnConf = TRUE;
  //}
}

//TBR
void Host_Profile_Test_Application (void)
{
  uint8_t ret;
  deviceState = APPL_UNINITIALIZED;
  APPL_MESG_INFO(profiledbgfile,"\n HRC Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init Collector Profile Central Role */
  Device_Init();

  /* Init Profile Security */
  Device_SetSecurity();

  deviceState = APPL_INIT_DONE;
  
  while(1)
  {    
    /* BLE Stack Tick */
    BTLE_StackTick();
    
    /* Call the Profile Master role state machine */
    Master_Process();
    
    if (heartRateCollectorContext.fullConf) 
        HRC_StateMachine(); 
    
#ifndef PTS_AUTOMATING
    Enter_Sleep_Mode();
#endif
  
    /* if the profiles have nothing more to process, then
     * wait for application input
     */ 
    if(deviceState >= APPL_INIT_DONE) //TBR
    {      
      uint8_t input = 200;
      deviceState = input;
      if (__io_getcharNonBlocking(&input))
      {
        APPL_MESG_DBG(profiledbgfile,"io--- input: %c\n",input); 

        switch(input)
        {   
	  case DISPLAY_PTS_MENU: 
            
          case APPL_DISCOVER_HR_SENSOR :
	  case APPL_CONNECT_HR_SENSOR :
	  case APPL_DISCONNECT_HR_SENSOR :
	  case APPL_PAIR_WITH_HR_SENSOR :
	  case APPL_CLEAR_SECURITY_DATABASE :
	  case APPL_DISCOVER_HEART_RATE_SERVICES :
          case APPL_DISCOVER_DEVICE_INFORMATION_SERVICES:  
	  case APPL_DISCOVER_HR_CHARACTERISTICS :
	  case APPL_DISCOVER_HR_MEASUREMENT_CHAR_DESCRIPTOR :
          //case APPL_DISCOVER_BODY_SENSOR_LOCATION_CHAR:  //TBR??
	  //case APPL_DISCOVER_HR_CTRL_POINT_CHAR : //TBR??
	  case APPL_DISCOVER_DIS_CHARACTERISTICS :
	  case APPL_READ_DIS_MANUFACTURER_NAME_CHAR :
	  case APPL_ENABLE_HR_MEASUREMENT_CHAR_NOTIFICATION :
	  case APPL_READ_BODY_SENSOR_LOCATION_CHAR :
	  case APPL_WRITE_HR_CTRL_POINT_CHAR :
//	  case APPL_VERIFY_BOND_STATUS_ON_RECONNECTION :
          case APPL_START_FULL_CONFIGURATION:
          {
            deviceState = input;
          }
          break;
          default:
          break; //continue
        }/* end switch(input) */
      } /* end if _IO.... */
    }/* end if(deviceState >= APPL_CONNECTED) */
            
    /* application specific processing */	
    switch(deviceState)
    {
      case DISPLAY_PTS_MENU:
      {
        Display_Appl_Menu();
      }
      break;
      case APPL_DISCOVER_HR_SENSOR :
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_HR_SENSOR : call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_HR_SENSOR :
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_HR_SENSOR : call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_HR_SENSOR :
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_HR_SENSOR: call Device_Disconnection() \n"); 
        Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_HR_SENSOR :
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_HR_SENSOR: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call HRC_Clear_Security_Database()\n"); 
        status = HRC_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"HRC_Collector_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"HRC_Collector_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      case APPL_DISCOVER_HEART_RATE_SERVICES :  /* 4.3.1 TP/HRD/CO/BV-01-I */                                                   
      {
        /* It discover the  services of the connected heart rate sensor device */
        APPL_MESG_DBG(profiledbgfile,"Call Device_ServicesDiscovery() x connected Heart Rate Device \n"); 
        Device_ServicesDiscovery();
      }
      break;
      
      case APPL_DISCOVER_HR_CHARACTERISTICS: 
      {
        /* It discovers all the characteristics of the connected heart rate service */
        //APPL_MESG_DBG(profiledbgfile,"Call Device_Discovery_CharacServ() x Heart Rate Service\n"); 
        Device_Discovery_CharacServ(HEART_RATE_SERVICE_UUID);
      }
      break;
           
#if 0 //TBR???
    case APPL_DISCOVER_BODY_SENSOR_LOCATION_CHAR:
      {
        /* It discovers all the characteristics of the connected heart rate service */
        APPL_MESG_DBG(profiledbgfile,"Call HRC_Start_Sensor_Location_Characteristics_Discovery() x Heart Rate Service\n"); 
        status = HRC_Start_Sensor_Location_Characteristics_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_Start_Sensor_Location_Characteristics_Discovery() call: %02X\n", status);
        }
      }
      break;
      
      case APPL_DISCOVER_HR_CTRL_POINT_CHAR: 
      {
        /* It discovers all the characteristics of the connected heart rate service */
        APPL_MESG_DBG(profiledbgfile,"Call HRC_Start_Control_Point_Characteristics_Discovery() x Heart Rate Service\n"); 
        status = HRC_Start_Control_Point_Characteristics_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_Start_Control_Point_Characteristics_Discovery() call: %02X\n", status);
        }
      }
      break;
#endif 
      
      case APPL_DISCOVER_HR_MEASUREMENT_CHAR_DESCRIPTOR :            
      {
        /* It discovers the characteristic descriptors of the connected Heart Rate Sensor Measurement Context Characteristic */
        APPL_MESG_DBG(profiledbgfile,"Call HRC_Start_Heart_Rate_Measurement_Characteristic_Descriptor_Discovery() x Heart Rate Measurement Characteristic\n"); 
        status = HRC_Start_Heart_Rate_Measurement_Characteristic_Descriptor_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_Start_Heart_Rate_Measurement_Characteristic_Descriptor_Discovery() call: %02X\n", status);
        }
      }
      break;     

      case APPL_DISCOVER_DIS_CHARACTERISTICS: 
      {
         /* It discovers all the characteristics of the connected device information service */
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_DIS_CHARACTERISTICS: call Device_Discovery_CharacServ() x Device Information Service\n"); 
        Device_Discovery_CharacServ(DEVICE_INFORMATION_SERVICE_UUID);
      }
      break;
      
    case APPL_READ_DIS_MANUFACTURER_NAME_CHAR: /* 4.3.9 TP//HRD/CO/BV-10-I */
      {
        APPL_MESG_DBG(profiledbgfile,"Read Device Information Service: Manufacturer Name Characteristic \n");        
        /* Read Device Info Manufacturer Name Characteristic */
        status = HRC_ReadDISManufacturerNameChar();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_ReadDISManufacturerNameChar() call: 0x%02X\n", status);
        }
      }
      break;
      
      case APPL_ENABLE_HR_MEASUREMENT_CHAR_NOTIFICATION :  /* 4.4.3 TP/HRF/CO/BV-03-I */
      {        
        /* Enable Heart Rate Measurement Char Descriptor for notifications */
        APPL_MESG_DBG(profiledbgfile,"Call HRC_Enable_HR_Measurement_Char_Notification() \n");
        status = HRC_Enable_HR_Measurement_Char_Notification();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_Enable_HR_Measurement_Char_Notification() call: %02X\n", status);
        }
      }
      break;
      
      case APPL_READ_BODY_SENSOR_LOCATION_CHAR : /* 4.4.9 TP/HRF/CO/BI-03-I */
      {
        APPL_MESG_DBG(profiledbgfile,"Read Body Sensor Location Characteristic Value.\n");
        /* Read Body Sensor Location Characteristic */
        status = HRC_Read_Body_Sensor_Location();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_Read_Body_Sensor_Location() call (Body Sensor Location Characteristic): %02X\n", status);
        }
      }
      break;
      
      
      case APPL_WRITE_HR_CTRL_POINT_CHAR :
      {
        APPL_MESG_DBG(profiledbgfile,"Write Heart Rate Control Point Characteristic Value.\n");
        /* Write Heart Rate Control Point Characteristi */
        status = HRC_Write_HR_Control_Point();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the HRC_Write_HR_Control_Point() call (Heart Rate Control Point Characteristic): %02X\n", status);
        }
      }
      break;
      case APPL_START_FULL_CONFIGURATION:
        APPL_MESG_DBG(profiledbgfile,"Call Device_StartFullConfig()\n");
        Device_StartFullConfig();
      break;
    }/* end switch(devicestate) */

  } /* end while(1) */
}/* end Host_Profile_Test_Application() */

/***************  Public Function callback ******************/

void HRC_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
                                     uint8_t data_length, uint8_t *data, 
                                     uint8_t RSSI)
{
  uint8_t addr_to_find[6]=PERIPHERAL_PEER_ADDRESS;
  
  if (Osal_MemCmp(addr, addr_to_find, 6) == 0) {
    APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
    APPL_MESG_INFO(profiledbgfile,"*\r\n");
    APPL_MESG_INFO(profiledbgfile,"**** Peer Device Found\r\n");
    APPL_MESG_INFO(profiledbgfile,"**** Addr = 0x");
    for (uint8_t i=0; i<6; i++)
      APPL_MESG_INFO(profiledbgfile,"%02x", addr[i]);
    APPL_MESG_INFO(profiledbgfile,"\r\n");
    APPL_MESG_INFO(profiledbgfile,"*\r\n");
    APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  }
}

void HRC_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  switch(connection_evt) {
  case CONNECTION_ESTABLISHED_EVT:
    if (status == BLE_STATUS_SUCCESS) {
      APPL_MESG_INFO(profiledbgfile,"****  Connection Established with Success\r\n");
    }
    break;
  case CONNECTION_FAILED_EVT:
    APPL_MESG_INFO(profiledbgfile,"****  Connection Failed with Status = 0x%02x\r\n", status);
    break;
  case DISCONNECTION_EVT:
    APPL_MESG_INFO(profiledbgfile,"****  Disconnection with peer device\r\n");
    break;
  }
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
}

void HRC_ServicesDiscovery_CB(uint8_t status, uint8_t numServices, uint8_t *services)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Primary Service Discovery Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugServices(numServices, services);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void HRC_CharacOfService_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  //APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristics of a Service Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharac(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void HRC_CharacDesc_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristic Descriptor Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharacDesc(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");    
}

void HRC_DataValueRead_CB(uint8_t status, uint16_t data_len, uint8_t *data)
{
  int8_t i;

  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Data Read Value Procedure: Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_INFO(profiledbgfile,"****    Len data Read = %d\r\n", data_len);
    if (data_len == HR_READ_SIZE)
    {
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c0) **** HR Body Sensor Location Value: 0x%02x\n", PTS_CODE, HRP_PTS_CODE, heartRateCollectorContext.BodySensorLocValue)
#else
        APPL_MESG_INFO(profiledbgfile,"**** HR Body Sensor Location Value: 0x%02x\n", heartRateCollectorContext.BodySensorLocValue);
#endif
    }
    else
    {
      //////// PTS_AUTOMATING ////////
#ifdef PTS_AUTOMATING  
        APPL_MESG_INFO(profiledbgfile,"(%c%c8)", PTS_CODE, HRP_PTS_CODE);      
        APPL_MESG_INFO(profiledbgfile,"****    Data - %d: ", data_len);
        if (data_len == 13)
        {
          for (i=0; i<data_len; i++) 
            APPL_MESG_INFO(profiledbgfile,"%c", data[i]); 
        }  
        else //if (data_len == 4)
        {  
          APPL_MESG_INFO(profiledbgfile,"0x");
          for (i=data_len-1; i>=0; i--) 
            APPL_MESG_INFO(profiledbgfile,"0%0x", data[i]);
        } 
        APPL_MESG_INFO(profiledbgfile,"\n");
      ///////////////////////////////////
#else
      {
        APPL_MESG_INFO(profiledbgfile,"****    Data: ");
        for (i=0; i<data_len; i++) {
          APPL_MESG_INFO(profiledbgfile,"0x%0x(%d) ", data[i], data[i]);
        }  
      }  
      APPL_MESG_INFO(profiledbgfile,"\r\n");
#endif
    }
  }
}

void HRC_FullConfError_CB(uint8_t error_type, uint8_t code)
{
  if (heartRateCollectorContext.fullConf) {
    APPL_MESG_INFO(profiledbgfile,"***** HRC_FullConf_CB() Error Type = 0x%0x with Code = 0x%0x\r\n", error_type, code);
  }
}

void HRC_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the Heart Rate collector indication */
  //pinCode = ....

  ret = HRC_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}
                                        
void HRC_NotificationReceived_CB(uint8_t handle, uint8_t length, uint8_t * data_value)
{
#ifdef PTS_AUTOMATING 
//    APPL_MESG_INFO(profiledbgfile,"(%c%c9)NOTIFICATION RECEIVED: handle=0x%04x, data length=%d, data:\r\n", 
//                   PTS_CODE, HRP_PTS_CODE, handle, length);
    APPL_MESG_INFO(profiledbgfile,"(%c%c9)NOTIFICATION RECEIVED: handle=0x%04x, data length=%d, data: ", 
                   PTS_CODE, HRP_PTS_CODE, handle, length);
    APPL_MESG_INFO(profiledbgfile,"0x");
    for (uint8_t i=0; i<length; i++) {
      APPL_MESG_INFO(profiledbgfile,"%02x", data_value[i]);
    }
#else 
    APPL_MESG_INFO(profiledbgfile,"NOTIFICATION_DATA_RECEIVED: handle=0x%04x, data length=%d, data:\r\n",
                   handle, length);  
    for (uint8_t i=0; i<length; i++) {
      APPL_MESG_INFO(profiledbgfile,"%02x ", data_value[i]);
    }
#endif
//  for (uint8_t i=0; i<length; i++) {
//    APPL_MESG_INFO(profiledbgfile,"%02x ", data_value[i]);
//  }
//  APPL_MESG_INFO(profiledbgfile,"\r\n");  
   APPL_MESG_INFO(profiledbgfile, "\r\n");
}
  
void HRC_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
  
}

void HRC_EnableNotificationIndication_CB(uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"**** Enable Notification/Indication procedure ended with Status = 0x%0x\r\n", status);
}

#endif    

/** \endcond 
 */
