/*******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2016 STMicroelectronics International NV
*   FILENAME        -  profile_AlertNotificationClient_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      07/09/2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Alert Notification Client
*                Central role
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
#include "string.h"
#include "osal.h"
#include "stdio.h"
#include "ble_const.h" 
#include "bluenrg1_api.h"
#include "BlueNRG1_stack.h" 
#include "host_config.h"
#include "Profile_config.h"
#include "debug.h"
#include "sleep.h"
#include "uuid.h"
#include "SDK_EVAL_Config.h"

#include <host_config.h>
#if (PLATFORM_WINDOWS == 1)
#include <Windows.h>
#endif

#include "master_basic_profile.h"

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
#include <alert_notification_client.h>
#include <alert_notification_client_config.h>

/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS PTS_PERIPHERAL_ADDRESS    //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS

/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR


/* ----- User Inputs for Alert Notification Server Central PTS tests -----------------*/

///@cond BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT
/**
 *@defgroup Alert_Notification_Client
 *@brief PTS test application user input commands for Alert Notification Client Central role
 *@{
 */

// GENERAL OPERATIONS: discovery, connection, disconnection, bonding, clear security database 

/** 
 *  Alert Notification Client user input command: Start Discovery procedure for detecting an alert notification server (<b>ANC_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_ALERT_NOTIFICATION_SERVER      'a' 

/** 
 *  Alert Notification Client user input command: Start Connection procedure for connecting to the discovered alert notification server (<b>ANC_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_ALERT_NOTIFICATION_SERVER       'b' // Start Connection procedure for connecting to the discovered phone alert status server

/** 
 *  Alert Notification Client user input command: Terminate the current connection with the alert notification server (<b>ANC_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_ALERT_NOTIFICATION_SERVER    'c' 

/** 
 *  Alert Notification Client user input command: Start the pairing  procedure (or enable the encryption) with the connected alert notification server (<b>ANC_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_ALERT_NOTIFICATION_SERVER     'd' // Start the pairing  procedure (or enable the encryption) with the connected phone alert status server

/** 
 *  Alert Notification Client user input command: Clear the security database (it removes any bonded device) (<b>ANC_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE                 'e' // 

//---------------------- DISCOVERY SERVICES AND CHARACTERISTICS

/** 
 *  Alert Notification Client user input command: Discover Alert Notification Service (<b>ANC_ServicesDiscovery() </b> API)
 */
#define APPL_DISCOVER_ALERT_NOTIFICATION_SERVICE         'f' //Discover Alert Notification Service

/** 
 *  Alert Notification Client user input command: Discover All Characteristics of Alert Notification Services --> Supported New Alert Category, New Alert,Unread Alert Category, Unread Alert Status, Alert Notification Control Point characteristics. (<b>ANC_DiscCharacServ() </b> API)
 */
#define APPL_DISCOVER_ALL_CHARACTERISTICS                'g' 

/** 
 *  Alert Notification Client user input command: Discover New Alert Client Characteristic Configuration Descriptor (<b>ANC_Start_New_Alert_Client_Char_Descriptor_Discovery() </b> API)
 */
#define APPL_DISCOVER_NEW_ALERT_CHAR_DESCRIPTOR           'h' 

/** 
 *  Alert Notification Client user input command: Discover Unread Alert Status Client Characteristic Configuration Descriptor (<b>ANC_Start_Unread_Alert_Status_Client_Char_Descriptor_Discovery() </b> API)
 */
#define APPL_DISCOVER_UNREAD_ALERT_STATUS_CHAR_DESCRIPTOR 'i'

//-------------------------- CLIENT CHARACTERISTIC CONFIGURATION

/** 
 *  Alert Notification Client user input command: Configure New Alert Client Characteristic Configuration Descriptor with 0x0001 (<b>ANC_Enable_Disable_New_Alert_Notification(1)</b> API)
 */
#define APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_1           'l' 

/** 
 *  Alert Notification Client user input command: Configure New Alert Client Characteristic Configuration Descriptor with 0x0000 (<b>ANC_Enable_Disable_New_Alert_Notification(0)</b> API)
 */
#define APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_0           'm' 

/** 
 *  Alert Notification Client user input command: Configure Unread Alert Status Client Characteristic Configuration Descriptor with 0x0001(<b>ANC_Enable_Disable_New_Alert_Notification(1);</b> API)
 */
#define APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_1 'n' 

/** 
 *  Alert Notification Client user input command: Configure Unread Alert Status Client Characteristic Configuration Descriptor with 0x0000(<b>ANC_Enable_Disable_New_Alert_Notification(0);</b> API)
 */
#define APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_0 'o' //Configure Unread Alert Status Client Characteristic Configuration Descriptor with 0x0000

//--------------------- READ AND WRITE ALERT NOTIFICATION SERVICE CHARACTERISTICS 

/** 
 *  Alert Notification Client user input command: Read Supported New Alert Category characteristic (<b>ANC_Read_New_Alert_Category();</b> API)
 */
#define APPL_READ_SUPPORTED_NEW_ALERT_CAT_CHAR             'p' 

/** 
 *  Alert Notification Client user input command: Read Supported Unread Alert Category characteristic(<b>ANC_Read_Unread_Alert_Status_Category();</b> API)
 */
#define APPL_READ_SUPPORTED_UNREAD_ALERT_CAT_CHAR          'q' 

/** 
 *  Alert Notification Client user input command: Write Control Point characteristic (<b>ANC_Write_Control_Point(ENABLE_NEW_ALERT_NOTIFICATION, CATEGORY_ID_EMAIL) </b> API)
 */
#define APPL_WRITE_AN_CTRL_POINT_CHAR_DEFAULT              'r' 

/** 
 *  Alert Notification Client user input command: Write Control Point characteristic (<b>ANC_Write_Control_Point(NOTIFY_NEW_ALERT_IMMEDIATELY ,CATEGORY_ID_ALL) </b> API)
 */
#define APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_NEW_ALERT        's' 

/** 
 *  Alert Notification Client user input command: Write Control Point characteristic (<b>ANC_Write_ContrANC_Write_Control_Point(NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY ,CATEGORY_ID_ALL) </b> API)
 */
#define APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_UNREAD_ALERT     't' 

/** 
 *  Alert Notification Client user input command:  Starts all the connection, service discovery, characteristic discovery, ..... procedures.
 */
#define APPL_START_FULL_CONFIGURATION                   'w' 

/** 
 *  Alert Notification Client user input command: Display PTS user input commands list
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
BOOL bReadSupportedUnreadAlert = FALSE; 

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
  /* BlueNRG-1 UART RX: WAKEUP_IO11 */
  wakeup_source = WAKEUP_IO13; //|WAKEUP_IO11;
  wakeup_level = (WAKEUP_IOx_LOW << WAKEUP_IO13_SHIFT_MASK); //| (WAKEUP_IO_LOW << WAKEUP_IO11);
  
  //printf("\r\nEnter to Sleep(mode: %d)\r\n", sleepMode);
  ret = BlueNRG_Sleep(sleep_mode, wakeup_source, wakeup_level);
  if (ret != BLE_STATUS_SUCCESS) 
  {
    printf("BlueNRG_Sleep() error 0x%02x\r\n", ret);
    while(1);
  }
  
#if 1 //???
  switch(BlueNRG_WakeupSource()) {
 
  case WAKEUP_IO11:
    printf("WAKEUP Reason = IO-11  (UART RX)\r\n");
    break;
  case WAKEUP_IO13:
//    printf("WAKEUP Reason = IO-13 (PUSH1 button)\r\n");
    break;
  case WAKEUP_SLEEP_TIMER1:
  case WAKEUP_SLEEP_TIMER2:
    //printf("SLEEP TIMER\r\n");
    break;
  default:
    //printf("Unknown source\r\n");
    break;
  }
#endif
}/* end Enter_Sleep_Mode*/

void printDebugServices(uint8_t numReporterServices, uint8_t *reporterServices)
{
  uint8_t i , j;
  uint16_t handle, uuid;

  APPL_MESG_INFO(profiledbgfile,"****    Number of Primary Service: %d\r\n", numReporterServices);
  j = 0;
  for (i=0; i<numReporterServices; i++) {
    if (reporterServices[j] == UUID_TYPE_16) {
      Osal_MemCpy(&uuid, &reporterServices[j+5], 2);
      if (uuid == ALERT_NOTIFICATION_SERVICE_UUID)
      {
	APPL_MESG_INFO(profiledbgfile,"****    Alert Notification Service UUID found\r\n");
      }
      else
      {
	APPL_MESG_INFO(profiledbgfile,"****    UUID 0x%04x\r\n", uuid);
      }
    }
    j++;
    Osal_MemCpy(&handle, &reporterServices[j], 2);
    APPL_MESG_INFO(profiledbgfile,"****       Start Handle = 0x%04x\r\n", handle);
    j += 2;
    Osal_MemCpy(&handle, &reporterServices[j], 2);
    APPL_MESG_INFO(profiledbgfile,"****       End Handle = 0x%04x\r\n", handle);
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

  APPL_MESG_INFO(profiledbgfile,"****    Number of Charac of a Service: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 8;
    Osal_MemCpy(&uuid_charac, &charac[index+6], 2);
    } else {
      sizeElement = 22;
    }

    switch(uuid_charac) {
    case SUPPORTED_NEW_ALERT_CATEGORY_CHAR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Supported New Alert Category Characteristic found\r\n");
      break;
    case NEW_ALERT_CHAR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      New Alert Characteristic found\r\n");
      break;
    case SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Supported Unread Alert Category Characteristic found\r\n");
      break;     
    case UNREAD_ALERT_STATUS_CHAR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Unread Alert Status Characteristic found\r\n");
      break;
    case ALERT_NOTIFICATION_CONTROL_POINT_CHAR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Alert Notification Control Point Characteristic found\r\n");
      break;
   
    default:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic found\r\n");
    }

    Osal_MemCpy(&charac_handle, &charac[index+1], 2);
    properties = charac[index+3];
    Osal_MemCpy(&value_handle, &charac[index+4], 2);
    APPL_MESG_INFO(profiledbgfile,"****      Characteristic Handle = 0x%04x\r\n", charac_handle);
    APPL_MESG_INFO(profiledbgfile,"****      Properties = 0x%02x\r\n", properties);
    APPL_MESG_INFO(profiledbgfile,"****      ValueHandle = 0x%04x\r\n", value_handle);
    APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_charac);

    index += sizeElement;
  }
}

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
      APPL_MESG_INFO(profiledbgfile,"****      Client Characteristic Configuration Descriptor found\r\n");
      break;
    case VALID_RANGE_DESCRIPTOR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Valid Range Descriptor Characteristic found\r\n");
      break; 
    default:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Descriptor found\r\n");
      break;
    }

    APPL_MESG_INFO(profiledbgfile,"****      Characteristic Descriptor Handle = 0x%04x\r\n", handle_characDesc);
    APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_characDesc);

    index += sizeElement;
  }
}

void Display_Appl_Menu()
{
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_ALERT_NOTIFICATION_SERVER: %c\n",APPL_DISCOVER_ALERT_NOTIFICATION_SERVER); 
  APPL_MESG_INFO(profiledbgfile,"APPL_CONNECT_ALERT_NOTIFICATION_SERVER: %c\n",APPL_CONNECT_ALERT_NOTIFICATION_SERVER);
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCONNECT_ALERT_NOTIFICATION_SERVER: %c\n",APPL_DISCONNECT_ALERT_NOTIFICATION_SERVER);  
  APPL_MESG_INFO(profiledbgfile,"APPL_PAIR_WITH_ALERT_NOTIFICATION_SERVER: %c\n",APPL_PAIR_WITH_ALERT_NOTIFICATION_SERVER);
  APPL_MESG_INFO(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);

  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_ALERT_NOTIFICATION_SERVICE: %c\n",APPL_DISCOVER_ALERT_NOTIFICATION_SERVICE);
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_ALL_CHARACTERISTICS: %c\n",APPL_DISCOVER_ALL_CHARACTERISTICS);
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_NEW_ALERT_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_NEW_ALERT_CHAR_DESCRIPTOR);
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_UNREAD_ALERT_STATUS_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_UNREAD_ALERT_STATUS_CHAR_DESCRIPTOR);

  APPL_MESG_INFO(profiledbgfile,"APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_1: %c\n",APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_1);  
  APPL_MESG_INFO(profiledbgfile,"APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_0: %c\n",APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_0);
  APPL_MESG_INFO(profiledbgfile,"APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_1: %c\n",APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_1);
  APPL_MESG_INFO(profiledbgfile,"APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_0: %c\n",APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_0);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_READ_SUPPORTED_NEW_ALERT_CAT_CHAR: %c\n",APPL_READ_SUPPORTED_NEW_ALERT_CAT_CHAR);
  APPL_MESG_INFO(profiledbgfile,"APPL_READ_SUPPORTED_UNREAD_ALERT_CAT_CHAR: %c\n",APPL_READ_SUPPORTED_UNREAD_ALERT_CAT_CHAR);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_WRITE_AN_CTRL_POINT_CHAR_DEFAULT: %c\n",APPL_WRITE_AN_CTRL_POINT_CHAR_DEFAULT);
  APPL_MESG_INFO(profiledbgfile,"APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_NEW_ALERT: %c\n",APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_NEW_ALERT);
  APPL_MESG_INFO(profiledbgfile,"APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_UNREAD_ALERT: %c\n",APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_UNREAD_ALERT);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_START_FULL_CONFIGURATION: %c\n", APPL_START_FULL_CONFIGURATION);
  
  APPL_MESG_INFO(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);          
    
}/* end Display_Appl_Menu() */


void Device_Init(void)
{
  ancInitDevType InitDev; 
  uint8_t public_addr[6]= ANC_PUBLIC_ADDRESS;
  uint8_t device_name[]=ANC_DEVICE_NAME;
  
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      ANC_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  
  ble_status = ANC_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  ancSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  ANC_IO_CAPABILITY;
  param.mitm_mode =     ANC_MITM_MODE;
  param.bonding_mode =  ANC_BONDING_MODE;
  param.use_fixed_pin = ANC_USE_FIXED_PIN;
  param.fixed_pin =     ANC_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
  
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = ANC_SC_SUPPORT;
  param.keypress_support = ANC_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = ANC_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    ANC_OOB_ENABLE;
#endif
  
  ble_status = ANC_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  ancDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  ANC_LIM_DISC_SCAN_INT;
  param.scanWindow =    ANC_LIM_DISC_SCAN_WIND;
  ble_status = ANC_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  ancConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         ANC_FAST_SCAN_DURATION;
  param.fastScanInterval =         ANC_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           ANC_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = ANC_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   ANC_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        ANC_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        ANC_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             ANC_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      ANC_SUPERVISION_TIMEOUT;
  param.min_conn_length =          ANC_MIN_CONN_LENGTH;
  param.max_conn_length =          ANC_MAX_CONN_LENGTH;
  ble_status = ANC_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = ANC_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = ANC_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}

void Device_ServicesDiscovery(void)
{
  tBleStatus ble_status;
  ble_status = ANC_ServicesDiscovery();

  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_ServicesDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_ServicesDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_CharacServ(uint16_t uuid_service)
{
  tBleStatus ble_status;
  ble_status = ANC_DiscCharacServ(uuid_service);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DiscCharacServ() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANC_DiscCharacServ() Error: %02X, uuid_service: 0x%02X\n", ble_status,uuid_service);
  }
}

void Device_StartFullConfig(void)
{
  ancConnDevType connParam;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS; 

  /* Connection Parameter */
  connParam.fastScanDuration =         ANC_FAST_SCAN_DURATION;
  connParam.fastScanInterval =         ANC_FAST_SCAN_INTERVAL;
  connParam.fastScanWindow =           ANC_FAST_SCAN_WINDOW;
  connParam.reducedPowerScanInterval = ANC_REDUCED_POWER_SCAN_INTERVAL;
  connParam.reducedPowerScanWindow =   ANC_REDUCED_POWER_SCAN_WINDOW;
  connParam.peer_addr_type =           PUBLIC_ADDR;
  connParam.peer_addr =                peer_addr;
  connParam.own_addr_type =            PUBLIC_ADDR;
  connParam.conn_min_interval =        ANC_FAST_MIN_CONNECTION_INTERVAL;
  connParam.conn_max_interval =        ANC_FAST_MAX_CONNECTION_INTERVAL;
  connParam.conn_latency =             ANC_FAST_CONNECTION_LATENCY;
  connParam.supervision_timeout =      ANC_SUPERVISION_TIMEOUT;
  connParam.min_conn_length =          ANC_MIN_CONN_LENGTH;
  connParam.max_conn_length =          ANC_MAX_CONN_LENGTH;

  /* Configuration Parameter */

  if (ANC_ConnConf(connParam) != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"Error in ANC_ConnConf()\r\n");
  } 
  //else {
    //alertNotificationClientContext.useConnConf = TRUE;
  //}
}


//TBR
void Host_Profile_Test_Application (void)
{
  uint8_t ret;
   
  deviceState = APPL_UNINITIALIZED;
  APPL_MESG_INFO(profiledbgfile,"\n ANC Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init Alert Notification Server Profile Central Role */
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
    
    if (alertNotificationClientContext.fullConf) 
      ANC_StateMachine(); 
    
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
        APPL_MESG_INFO(profiledbgfile,"io--- input: %c\n",input); 

        switch(input)
        {   
          case DISPLAY_PTS_MENU:
            
          case APPL_DISCOVER_ALERT_NOTIFICATION_SERVER:
          case APPL_CONNECT_ALERT_NOTIFICATION_SERVER:
          case APPL_DISCONNECT_ALERT_NOTIFICATION_SERVER:
          case APPL_PAIR_WITH_ALERT_NOTIFICATION_SERVER:
          case APPL_CLEAR_SECURITY_DATABASE:
            
          case APPL_DISCOVER_ALERT_NOTIFICATION_SERVICE:           
          case APPL_DISCOVER_ALL_CHARACTERISTICS:
          case APPL_DISCOVER_NEW_ALERT_CHAR_DESCRIPTOR:
          case APPL_DISCOVER_UNREAD_ALERT_STATUS_CHAR_DESCRIPTOR:

          case APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_1: 
          case APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_0:
          case APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_1:
          case APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_0:
            
          case APPL_READ_SUPPORTED_NEW_ALERT_CAT_CHAR:
          case APPL_READ_SUPPORTED_UNREAD_ALERT_CAT_CHAR:    
            
          case APPL_WRITE_AN_CTRL_POINT_CHAR_DEFAULT:  
          case APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_NEW_ALERT:
          case APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_UNREAD_ALERT:
            
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
      
      case APPL_DISCOVER_ALERT_NOTIFICATION_SERVER:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_DISCOVER_ALERT_NOTIFICATION_SERVER: call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_ALERT_NOTIFICATION_SERVER:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_CONNECT_ALERT_NOTIFICATION_SERVER: call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_ALERT_NOTIFICATION_SERVER:
      {
         GL_DBG_MSG(profiledbgfile,"APPL_DISCONNECT_ALERT_NOTIFICATION_SERVER: call Device_Disconnection() \n"); 
         Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_ALERT_NOTIFICATION_SERVER:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_ALERT_NOTIFICATION_SERVER: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call Clear_Security_Database() \n"); 
        status = ANC_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"ANC_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"ANC_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      
      case APPL_DISCOVER_ALERT_NOTIFICATION_SERVICE:    /* 4.3.1 TP/ANPD/ANPC/BV-02-I */   
      {
        /* It discover the Alert Notication services of the connected Alert Notification Server device */
        APPL_MESG_DBG(profiledbgfile,"Call Device_ServicesDiscovery() x Alert Notication Service\n"); 
        Device_ServicesDiscovery();
      }
      break; 
      
      case APPL_DISCOVER_ALL_CHARACTERISTICS:  /* 4.3.2 TP/ANPD/ANPC/BV-03-I   -   4.3.3 TP/ANPD/ANPC/BV-04-I
                                                * 4.3.4 TP/ANPD/ANPC/BV-05-I   -   4.3.6 TP/ANPD/ANPC/BV-07-I 
                                                * 4.3.8 TP/ANPD/ANPC/BV-09-I  */   
      {
        /* It discovers all the characteristics of the connected Alert Notification Server  service */
        APPL_MESG_DBG(profiledbgfile,"Call Device_Discovery_CharacServ(ALERT_NOTIFICATION_SERVICE_UUID) x Alert Notification Service\n"); 
        Device_Discovery_CharacServ(ALERT_NOTIFICATION_SERVICE_UUID);
      }
      break;
      
      case APPL_DISCOVER_NEW_ALERT_CHAR_DESCRIPTOR:  /* 4.3.5 TP/ANPD/ANPC/BV-06-I  */
      {
        /* It discovers the characteristic descriptors of the connected New Alert Context Characteristic */
        APPL_MESG_DBG(profiledbgfile,"Call ANC_Start_New_Alert_Client_Char_Descriptor_Discovery() x Blood Pressure Measurement Characteristic\n"); 
        status = ANC_Start_New_Alert_Client_Char_Descriptor_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Start_New_Alert_Client_Char_Descriptor_Discovery() call: %02X\n", status);
        }
      }
      break;  
        
      case APPL_DISCOVER_UNREAD_ALERT_STATUS_CHAR_DESCRIPTOR:    /* 4.3.7 TP/ANPD/ANPC/BV-08-I  */
      {
        /* It discovers the characteristic descriptors of the connected Unread Alert Status Context Characteristic */
        APPL_MESG_DBG(profiledbgfile,"Call ANC_Start_Unread_Alert_Status_Client_Char_Descriptor_Discovery() x Blood Pressure Measurement Characteristic\n"); 
        status = ANC_Start_Unread_Alert_Status_Client_Char_Descriptor_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Start_Unread_Alert_Status_Client_Char_Descriptor_Discovery() call: %02X\n", status);
        }
      }
      break;  

      case APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_1:  /* 4.4.1 TP/ANPCF/ANPC/BV-01-I  */
      {        
        /* Enable New Alert Char Descriptor for notification */
        APPL_MESG_DBG(profiledbgfile,"Call ANC_Enable_Disable_New_Alert_Notification(1) \n");
        status = ANC_Enable_Disable_New_Alert_Notification(1);
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Enable_Disable_New_Alert_Notification(1) call: %02X\n", status);
        }
      }
      break;
         
      case APPL_NEW_ALERT_CHAR_DESC_CONFIGURATION_0:   /* 4.4.2 TP/ANPCF/ANPC/BV-02-I  */
      {        
        /* Disable New Alert Char Descriptor for notification */
        APPL_MESG_DBG(profiledbgfile,"Call ANC_Enable_Disable_New_Alert_Notification(0) \n");
        status = ANC_Enable_Disable_New_Alert_Notification(0);
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Enable_Disable_New_Alert_Notification(0) call: %02X\n", status);
        }
      }
      break;
      
      case APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_1:  /* 4.4.3 TP/ANPCF/ANPC/BV-03-I  */
      {        
        /* Enable Unread Alert Status Char Descriptor for notification */
        APPL_MESG_DBG(profiledbgfile,"Call ANC_Enable_Disable_New_Alert_Notification(1) \n");
        status = ANC_Enable_Disable_Unread_Alert_Status_Notification(1);
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Enable_Disable_New_Alert_Notification(1) call: %02X\n", status);
        }
      }
      break;        
        
      case APPL_UNREAD_ALERT_STATUS_CHAR_DESC_CONFIGURATION_0:   /* 4.4.4 TP/ANPCF/ANPC/BV-04-I  */
      {        
        /* Disable Unread Alert Status Char Descriptor for notification */
        APPL_MESG_DBG(profiledbgfile,"Call ANC_Enable_Disable_New_Alert_Notification(0) \n");
        status = ANC_Enable_Disable_Unread_Alert_Status_Notification(0);
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Enable_Disable_New_Alert_Notification(0) call: %02X\n", status);
        }
      }
      break;
        
      case APPL_READ_SUPPORTED_NEW_ALERT_CAT_CHAR:   /* 4.5.1 TP/ANPRF/ANPC/BV-01-I  */
      {
        APPL_MESG_DBG(profiledbgfile,"Read Supported New Alert Category Characteristic Value.\n");
        status = ANC_Read_New_Alert_Category();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Read_New_Alert_Category() call (Supported New Alert Category Characteristic): %02X\n", status);
        }
      }
      break;
            
      case APPL_READ_SUPPORTED_UNREAD_ALERT_CAT_CHAR:  /* 4.5.2 TP/ANPRF/ANPC/BV-02-I  */
      {
        bReadSupportedUnreadAlert = TRUE;
        APPL_MESG_DBG(profiledbgfile,"Read Supported Unread Alert Category Characteristic Value.\n");
        status = ANC_Read_Unread_Alert_Status_Category();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Read_Unread_Alert_Status_Category() call (Supported Unread Alert Category Characteristic): %02X\n", status);
        }        
      }  
      break;
      
      case APPL_WRITE_AN_CTRL_POINT_CHAR_DEFAULT:       /* 4.6.1 TP/ANPWF/ANPC/BV-01-I  */
      {
        APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point Characteristic Value.\n");
        /* Write Alert Notification Control Point Characteristic */
        status = ANC_Write_Control_Point(ENABLE_NEW_ALERT_NOTIFICATION, CATEGORY_ID_EMAIL); //default
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Write_Control_Point() call (Alert Notification Control Point Characteristic): %02X\n", status);
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point - Command ID:  0x%02x\n", ENABLE_NEW_ALERT_NOTIFICATION);
          APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point - Category:    0x%02x\n", CATEGORY_ID_EMAIL);
        }  
      }
      break;

      case APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_NEW_ALERT:   /* 4.8.1 TP/ANPSF/ANPC/BV-01-I */
      {
        APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point Characteristic Value.\n");
        /* Write Alert Notification Control Point Characteristic */        
        status = ANC_Write_Control_Point(NOTIFY_NEW_ALERT_IMMEDIATELY, CATEGORY_ID_ALL); 
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Write_Control_Point() call (Alert Notification Control Point Characteristic): %02X\n", status);
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point - Command ID:  0x%02x\n", NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY);
          APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point - Category:    0x%02x\n", CATEGORY_ID_ALL);
        } 
      }
      break;
      
      case APPL_WRITE_AN_CTRL_POINT_CHAR_ALL_UNREAD_ALERT:  /* 4.8.2 TP/ANPSF/ANPC/BV-02-I */
      {
        APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point Characteristic Value.\n");
        /* Write Alert Notification Control Point Characteristic */
        status = ANC_Write_Control_Point(NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY, CATEGORY_ID_ALL); 
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the ANC_Write_Control_Point() call (Alert Notification Control Point Characteristic): %02X\n", status);
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point - Command ID:  0x%02x\n", NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY);
          APPL_MESG_DBG(profiledbgfile,"Write Alert Notification Control Point - Category:    0x%02x\n", CATEGORY_ID_ALL);
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

void ANC_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
                                     uint8_t data_length, uint8_t *data, 
                                     uint8_t RSSI)
{
  uint8_t addr_to_find[6]=PERIPHERAL_PEER_ADDRESS;
  
  if (memcmp(addr, addr_to_find, 6) == 0) {
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

void ANC_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
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

void ANC_ServicesDiscovery_CB(uint8_t status, uint8_t numServices, uint8_t *services)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Primary Service Discovery Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugServices(numServices, services);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void ANC_CharacOfService_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristics of a Service Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharac(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void ANC_CharacDesc_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristic Descriptor Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharacDesc(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");    
}

void ANC_DataValueRead_CB(uint8_t status, uint16_t data_len, uint8_t *data)
{
  uint8_t i;

  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Data Read Value Procedure: Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_INFO(profiledbgfile,"****    Data: ");
    for (i=0; i<data_len; i++) {
      APPL_MESG_INFO(profiledbgfile,"0x%0x (%d)", data[i], data[i]); 
    }
    APPL_MESG_INFO(profiledbgfile,"\r\n");
  }
}

void ANC_FullConfError_CB(uint8_t error_type, uint8_t code)
{
  if (alertNotificationClientContext.fullConf) {
    APPL_MESG_INFO(profiledbgfile,"***** ANC_FullConf_CB() Error Type = 0x%0x with Code = 0x%0x\r\n", error_type, code);
  }
}


void ANC_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the glucose collector indication */
  //pinCode = ....

  ret = ANC_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

void ANC_NotificationReceived_CB(uint8_t handle, uint8_t length, uint8_t * data_value)
{
  APPL_MESG_DBG(profiledbgfile,"NOTIFICATION_DATA_RECEIVED: handle=0x%04x, data length=%d, \ndata:\r\n", handle, length);
    
  for (uint8_t i=0; i<length; i++) {
    APPL_MESG_DBG(profiledbgfile,"%02x ", data_value[i]);
  }
  APPL_MESG_DBG(profiledbgfile,"\r\n");  
}

void ANC_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
  
}

void ANC_EnableNotification_CB(uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"**** Enable Notification procedure ended with Status = 0x%0x\r\n", status);
}


#endif    

/** \endcond 
 */
