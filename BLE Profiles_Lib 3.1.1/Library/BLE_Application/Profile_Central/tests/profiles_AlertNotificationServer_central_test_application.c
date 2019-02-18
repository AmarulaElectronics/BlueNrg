/*******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2016 STMicroelectronics International 
*
*   FILENAME        -  profile_AlertNotificationClient_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      12/09/2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Alert Notification Server
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

#include "master_basic_profile.h"

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
#include <alert_notification_server.h>
#include <alert_notification_server_config.h>

/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS  PTS_PERIPHERAL_ADDRESS //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS

/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR


/* ----- User Inputs for Alert Notification Server Central PTS tests -----------------*/

///@cond BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER
/**
 *@defgroup Alert_Notification_Server
 *@brief PTS test application user input commands for Alert Notification Server Central role
 *@{
 */
//General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 *  Alert Notification Server user input command: Start Discovery procedure for detecting an alert notification server (<b>ANS_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_ALERT_NOTIFICATION_CLIENT      'a' 

/** 
 *  Alert Notification Server user input command: Start Connection procedure for connecting to the discovered alert notification server (<b>ANS_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_ALERT_NOTIFICATION_CLIENT       'b' 

/** 
 *  Alert Notification Server user input command: Terminate the current connection with the alert notification server (<b>ANS_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_ALERT_NOTIFICATION_CLIENT    'c' 

/** 
 *  Alert Notification Server user input command: Start the pairing  procedure (or enable the encryption) with the connected alert notification server (<b>ANS_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_ALERT_NOTIFICATION_CLIENT     'd' 

/** 
 *  Alert Notification Server user input command: Clear the security database (it removes any bonded device) (<b>ANS_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE                 'e' 

/* 
4.3	Service Definition 
4.3.1 	TP/SD/BV-01-C [Service Definition] 
4.4	Characteristic Declaration 
4.4.1 	TP/DEC/BV-01-C [Characteristic Declaration – Supported New Alert Category] 
4.4.2 	TP/DEC/BV-02-C [Characteristic Declaration – Alert Notification Control Point] 
4.4.3 	TP/DEC/BV-03-C [Characteristic Declaration – New Alert] 
4.4.4 	P/DEC/BV-04-C [Characteristic Declaration – Unread Alert Status] 
4.4.5 	TP/DEC/BV-05-C [Characteristic Declaration – Supported Unread Alert Category]
4.5	Characteristic Descriptors 
4.5.1 	TP/DES/BV-01-C [Client Configuration Descriptor – New Alert]
4.5.2 	TP/DES/BV-02-C [Client Configuration Descriptor –Unread Alert Status]
4.6	Characteristic Configuration Descriptors Write
4.6.1	TP/DESW/BV-01-C [Client Configuration Descriptor – New Alert]
4.6.2 	TP/DESW/BV-02-C [Client Configuration Descriptor – Unread Alert Status]
4.7	Characteristic Read 
4.7.1 	TP/CR/BV-01-C [Characteristic Read – Supported New Alert Category] 
4.7.2 	TP/CR/BV-02-C [Characteristic Read – Supported Unread Alert Category] 

4.8	Characteristic Write
4.8.1	TP/CW/BV-01-C [Characteristic Write – Alert Notification Control Point]
*/

/* 

4.9	Characteristic Notify
4.9.1 	TP/CN/BV-01-C [Characteristic Notify – New Alert]
4.9.2 	TP/CN/BV-02-C [Characteristic Notify – Unread Alert Status]

*/

/* 

4.10	Service Procedures 
4.10.1	TP/SP/BV-01-C [Service Behavior – Alert Notification Control Point for New Alert,CCCD=ON, Category=OFF]
4.10.2	TP/SP/BV-02-C [Service Behavior – Alert Notification Control Point for New Alert,CCCD=ON, Category=ON]
4.10.3	TP/SP/BV-03-C [Service Behavior – Alert Notification Control Point for New Alert,CCCD=ON, Category=ON<Non-support>]
4.10.4	TP/SP/BV-04-C [Service Behavior – Alert Notification Control Point for New Alert,CCCD=ON, Category=ON command with 0xff]
4.10.5	TP/SP/BV-05-C [Service Behavior – Alert Notification Control Point for New Alert,CCCD=OFF, Category=ON]
4.10.6	TP/SP/BV-06-C [Service Behavior – Alert Notification Control Point for New Alert, CCCD=ON, Category=ON <All>] 
4.10.7	TP/SP/BV-07-C [Service Behavior – Alert Notification Control Point for Unread Alert Status, CCCD=ON, Category=OFF]
4.10.8	TP/SP/BV-08-C [Service Behavior – Alert Notification Control Point for Unread Alert Status, CCCD=ON, Category=ON]  
4.10.9	TP/SP/BV-09-C [Service Behavior – Alert Notification Control Point for Unread Alert Status, CCCD=ON, Category=ON<Non-support>] 
4.10.10	TP/SP/BV-10-C [Service Behavior – Alert Notification Control Point for Unread Alert Status, CCCD=ON, Category=ON command with 0xff] 
4.10.11	TP/SP/BV-11-C [Service Behavior – Alert Notification Control Point for Unread Alert Status, CCCD=OFF, Category=ON]
4.10.12	TP/SP/BV-12-C [Service Behavior – Alert Notification Control Point for Unread Alert Status, CCCD=ON, Category=ON <All>] 

*/
	
/* ANP: Alert Notification  Profile test for Alert Notification Server 
4.8.5 	TP/ANPSF/BV-01-I[Verify Bond Status on Reconnection] 
*/

/** 
 *  Alert Notification Server user input command: Update New Alert  characteristic (<b>ANS_Read_New_Alert_Category();</b> API)
 */
#define APPL_UPDATE_NEW_ALERT                        'f' // 4.9.1 

/** 
 *  Alert Notification Server user input command: Update Unread Alert Status characteristic (<b>ANS_Update_Unread_Alert_Status();</b> API)
 */
#define APPL_UPDATE_UNREAD_ALERT_STATUS              'g' // 4.9.1 


/** 
 *  Alert Notification Server user input command: Display PTS user input commands list
 */
#define DISPLAY_PTS_MENU                             'z'  

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

/* alert notification server profile init parameters */
uint8_t alertCategory[SUPPORTED_NEW_ALERT_CATEGORY_CHAR_SIZE] = {0xFF,0x03};
uint8_t unreadAlertCategory[SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR_SIZE] = {0xFF,0x00};
  
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

void Display_Appl_Menu()
{
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_ALERT_NOTIFICATION_CLIENT: %c\n",APPL_DISCOVER_ALERT_NOTIFICATION_CLIENT);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_CONNECT_ALERT_NOTIFICATION_CLIENT: %c\n",APPL_CONNECT_ALERT_NOTIFICATION_CLIENT);
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCONNECT_ALERT_NOTIFICATION_CLIENT: %c\n",APPL_DISCONNECT_ALERT_NOTIFICATION_CLIENT);
  APPL_MESG_INFO(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);
  APPL_MESG_INFO(profiledbgfile,"APPL_PAIR_WITH_ALERT_NOTIFICATION_CLIENT: %c\n",APPL_PAIR_WITH_ALERT_NOTIFICATION_CLIENT);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_NEW_ALERT: %c\n",APPL_UPDATE_NEW_ALERT);
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_UNREAD_ALERT_STATUS: %c\n",APPL_UPDATE_UNREAD_ALERT_STATUS);
  APPL_MESG_INFO(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);          
    
}/* end Display_Appl_Menu() */

void Device_Init(void)
{
  ansInitDevType InitDev; 
  uint8_t public_addr[6]=ANS_PUBLIC_ADDRESS;
  uint8_t device_name[]=ANS_DEVICE_NAME;
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      ANS_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  
  /* Init Alert Notification Server Central Role parameters*/
  InitDev.alertCategory = &(alertCategory[0]);
  InitDev.unreadAlertCategory = &(unreadAlertCategory[0]); 
  
  ble_status = ANS_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  ansSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  ANS_IO_CAPABILITY;
  param.mitm_mode =     ANS_MITM_MODE;
  param.bonding_mode =  ANS_BONDING_MODE;
  param.use_fixed_pin = ANS_USE_FIXED_PIN;
  param.fixed_pin =     ANS_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
  
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = ANS_SC_SUPPORT;
  param.keypress_support = ANS_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = ANS_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    ANS_OOB_ENABLE;
#endif
    
  ble_status = ANS_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  ansDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  ANS_LIM_DISC_SCAN_INT;
  param.scanWindow =    ANS_LIM_DISC_SCAN_WIND;
  ble_status = ANS_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  ansConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         ANS_FAST_SCAN_DURATION;
  param.fastScanInterval =         ANS_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           ANS_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = ANS_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   ANS_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        ANS_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        ANS_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             ANS_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      ANS_SUPERVISION_TIMEOUT;
  param.min_conn_length =          ANS_MIN_CONN_LENGTH;
  param.max_conn_length =          ANS_MAX_CONN_LENGTH;
  ble_status = ANS_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = ANS_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = ANS_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}


//TBR
void Host_Profile_Test_Application (void)
{
  uint8_t ret;
  
  deviceState = APPL_UNINITIALIZED;
  APPL_MESG_INFO(profiledbgfile,"\n ANS Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
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
  
  /* Add device service & characteristics */
  status =  ANS_Add_Services_Characteristics();
  if (status == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_Add_Services_Characteristics() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"ANS_Add_Services_Characteristics() Error: %02X\n", status);
  }
  
  deviceState = APPL_INIT_DONE;
  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();
    
    /* Call the Profile Master role state machine */
    Master_Process();
    
    ANS_StateMachine(); 
    
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
             
          case APPL_DISCOVER_ALERT_NOTIFICATION_CLIENT:
          case APPL_CONNECT_ALERT_NOTIFICATION_CLIENT:
          case APPL_DISCONNECT_ALERT_NOTIFICATION_CLIENT:
          case APPL_PAIR_WITH_ALERT_NOTIFICATION_CLIENT:
          case APPL_CLEAR_SECURITY_DATABASE:
            
          case APPL_UPDATE_NEW_ALERT: 
          case APPL_UPDATE_UNREAD_ALERT_STATUS:
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
      
      case APPL_DISCOVER_ALERT_NOTIFICATION_CLIENT:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_DISCOVER_ALERT_NOTIFICATION_CLIENT: call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_ALERT_NOTIFICATION_CLIENT:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_CONNECT_ALERT_NOTIFICATION_CLIENT: call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_ALERT_NOTIFICATION_CLIENT:
      {
         GL_DBG_MSG(profiledbgfile,"APPL_DISCONNECT_ALERT_NOTIFICATION_CLIENT: call Device_Disconnection() \n"); 
         Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_ALERT_NOTIFICATION_CLIENT:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_ALERT_NOTIFICATION_CLIENT: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        PROFILE_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call Clear_Security_Database() \n"); 
        status = ANS_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"ANS_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"ANS_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      
      case APPL_UPDATE_NEW_ALERT:
      {
        tTextInfo textInfo;
        uint8_t str[4] = {'J','O','H','N'};
        APPL_MESG_DBG(profiledbgfile,"ALERT_NOTIFICATION_SERVER\n");
        textInfo.len = 0x04;
        textInfo.str = str;

        status = ANS_Update_New_Alert(CATEGORY_ID_EMAIL,0x01,textInfo);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"ANS_Update_New_Alert() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"ANS_Update_New_Alert() Error: %02X\n", status);
        }
      }			 	 
      break;
      case APPL_UPDATE_UNREAD_ALERT_STATUS: 
      {
        status = ANS_Update_Unread_Alert_Status(CATEGORY_ID_SMS_MMS,0x01);	
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"ANS_Update_Unread_Alert_Status() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"ANS_Update_Unread_Alert_Status() Error: %02X\n", status);
        }
      }
      break;
    }/* end switch(devicestate) */

  } /* end while(1) */
}/* end Host_Profile_Test_Application() */

/***************  Public Function callback ******************/

void ANS_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
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

void ANS_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
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

void ANS_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the glucose collector indication */
  //pinCode = ....

  ret = ANS_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

void ANS_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
  
}

void ANS_Attribute_Modification_Received_CB(uint8_t handle, uint8_t length, uint8_t * data_value)
{
//  APPL_MESG_INFO(profiledbgfile,"ATTRIBUTE_MODIFICATION_RECEIVED: handle=0x%04x, data length=%d, data:\r\n",
//	   handle, length);
    
  for (uint8_t i=0; i<length; i++) {
    APPL_MESG_INFO(profiledbgfile,"%02x ", data_value[i]);
  }
  APPL_MESG_INFO(profiledbgfile,"\r\n");  
}

void ANS_Received_Control_Point_Command_CB(uint8_t * command_value)
{
  APPL_MESG_INFO(profiledbgfile,"Received Control Point command value (0x%02x):",command_value[0]);
  switch (command_value[0])
  {
    case ENABLE_NEW_ALERT_NOTIFICATION:     
     APPL_MESG_INFO(profiledbgfile,"ENABLE_NEW_ALERT_NOTIFICATION, ");
     break;
    case ENABLE_UNREAD_ALERT_STATUS_NOTIFICATION:
      APPL_MESG_INFO(profiledbgfile,"ENABLE_UNREAD_ALERT_STATUS_NOTIFICATION, ");
      break;
    case DISABLE_NEW_ALERT_NOTIFICATION:
      APPL_MESG_INFO(profiledbgfile,"DISABLE_NEW_ALERT_NOTIFICATION, ");
      break;
    case DISABLE_UNREAD_ALERT_STATUS_NOTIFICATION:
      APPL_MESG_INFO(profiledbgfile,"DISABLE_UNREAD_ALERT_STATUS_NOTIFICATION, ");
      break;
    case NOTIFY_NEW_ALERT_IMMEDIATELY:
      APPL_MESG_INFO(profiledbgfile,"NOTIFY_NEW_ALERT_IMMEDIATELY, ");
      break;
    case NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY:
      APPL_MESG_INFO(profiledbgfile,"NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY, ");
    break;
   default:
     APPL_MESG_INFO(profiledbgfile,"Unknown Command !, ");
    break;
  }
  APPL_MESG_INFO(profiledbgfile,"Category id: 0x%02x\n",command_value[1]);
}

#endif    

/** \endcond 
 */
