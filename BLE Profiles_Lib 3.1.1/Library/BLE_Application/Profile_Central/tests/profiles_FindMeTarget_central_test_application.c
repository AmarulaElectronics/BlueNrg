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
*   FILENAME        -  profile_FindMeTarget_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      14/09/2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Find Me Target 
*                Central role.
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
#include "SDK_EVAL_Config.h"
#include <host_config.h>
#include "master_basic_profile.h"

#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
#include <find_me_target.h>
#include <find_me_target_config.h>

/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS PTS_PERIPHERAL_ADDRESS //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS
   
/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR


/* --------------------- User Inputs for Profile Central PTS tests -----------------*/

///@cond BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET
/**
 *@defgroup Find_Me_Target
 *@brief PTS test application user input commands for Find Me Target Central role
 *@{
 */

// General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 * Find Me Target user input command: Start Discovery procedure for detecting a find me locator(<b>FMT_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_FIND_ME_LOCATOR      'a' 

/** 
 * Find Me Target user input command: Start Connection procedure for connecting to the discovered find me locator(<b>FMT_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_FIND_ME_LOCATOR       'b' 

/** 
 * Find Me Target user input command: Terminate the current connection with the find me locator (<b>FMT_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_FIND_ME_LOCATOR    'c' 

/** 
 *  Find Me Target user input command: Start the pairing procedure (or enable the encryption) with the connected find me locator (<b>FMT_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_FIND_ME_LOCATOR     'd' 

/** 
 * Find Me Target user input command: Clear the security database (it removes any bonded device) (<b>FMT_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE       'e' 

/* 4.3	Service Definition 
   4.3.1 TP/SD/BV-01-C [Service Definition] 
*/

/* 4.4	Characteristic Declaration
   4.4.1 TP/DEC/BV-01-C [Characteristic Declaration – Alert Level] 
*/

/* 4.5	Characteristic Write without Response 
   4.5.1 TP/CW/BV-01-C [Characteristic Write without Response – Alert Level] 
*/

/* 4.6	Service Procedures
   4.6.1 TP/SP/BV-01-C [Writing Alert Level Behavior]
*/

/** 
 * Find Me Target user input command: utility for getting the currente alert level value (<b>FMT_Get_Alert_Level_Value() </b> API)
 */
#define APPL_GET_ALERT_LEVEL               'f'

/** 
 * Find Me Target user input command: Display PTS user input commands list.
 */
#define DISPLAY_PTS_MENU                   'z' 

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


void Display_Appl_Menu()
{
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_FIND_ME_LOCATOR: %c\n",APPL_DISCOVER_FIND_ME_LOCATOR);
  
  APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_FIND_ME_LOCATOR: %c\n",APPL_CONNECT_FIND_ME_LOCATOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_FIND_ME_LOCATOR: %c\n",APPL_DISCONNECT_FIND_ME_LOCATOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_FIND_ME_LOCATOR: %c\n",APPL_PAIR_WITH_FIND_ME_LOCATOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);
  
  APPL_MESG_DBG(profiledbgfile,"APPL_GET_ALERT_LEVEL: %c\n",APPL_GET_ALERT_LEVEL);
  
  APPL_MESG_DBG(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);          
    
}/* end Display_Appl_Menu() */
 
void Device_Init(void)
{
  fmtInitDevType InitDev; 
  uint8_t public_addr[6]=FMT_PUBLIC_ADDRESS;
  uint8_t device_name[]=FMT_DEVICE_NAME;
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      FMT_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  
  /* Init Profile Central Role */ 
  ble_status = FMT_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  fmtSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  FMT_IO_CAPABILITY;
  param.mitm_mode =     FMT_MITM_MODE;
  param.bonding_mode =  FMT_BONDING_MODE;
  param.use_fixed_pin = FMT_USE_FIXED_PIN;
  param.fixed_pin =     FMT_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
      
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = FMT_SC_SUPPORT;
  param.keypress_support = FMT_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = FMT_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    FMT_OOB_ENABLE;
#endif
  
  ble_status = FMT_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  fmtDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  FMT_LIM_DISC_SCAN_INT;
  param.scanWindow =    FMT_LIM_DISC_SCAN_WIND;
  ble_status = FMT_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  fmtConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         FMT_FAST_SCAN_DURATION;
  param.fastScanInterval =         FMT_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           FMT_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = FMT_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   FMT_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        FMT_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        FMT_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             FMT_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      FMT_SUPERVISION_TIMEOUT;
  param.min_conn_length =          FMT_MIN_CONN_LENGTH;
  param.max_conn_length =          FMT_MAX_CONN_LENGTH;
  ble_status = FMT_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = FMT_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = FMT_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FMT_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}


//TBR
void Host_Profile_Test_Application (void)
{
  uint8_t ret;
  
  deviceState = APPL_UNINITIALIZED;
  
  APPL_MESG_INFO(profiledbgfile,"\n FMT Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init Profile Central */
  Device_Init();
  /* Init Profile Security */
  Device_SetSecurity();
  
  /* Add device service & characteristics */
  status =  FMT_Add_Services_Characteristics();
  if (status == BLE_STATUS_SUCCESS)
  {
    FMT_DBG_MSG(profiledbgfile,"FMT_Add_Services_Characteristics() Call: OK\n" );
  }
  else
  {
    FMT_DBG_MSG(profiledbgfile,"FMT_Add_Services_Characteristics() Error: %02X\n", status);
  }
  
  /* Clear Security database: is it needed ?TBR */
  status = FMT_Clear_Security_Database(); 
  if (status == BLE_STATUS_SUCCESS)
  {
    FMT_DBG_MSG(profiledbgfile,"FMT_Clear_Security_Database() Call: OK\n" );
  }
  else
  {
    FMT_DBG_MSG(profiledbgfile,"FMT_Clear_Security_Database() Error: %02X\n", status);
  }
  
  deviceState = APPL_INIT_DONE;
  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();
        
    /* Call the Profile Master role state machine */
    Master_Process();
    
    //FMT_StateMachine(); //TBR Is is needed?
  
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
          case APPL_DISCOVER_FIND_ME_LOCATOR: 
          case APPL_CONNECT_FIND_ME_LOCATOR:   
          case APPL_DISCONNECT_FIND_ME_LOCATOR:   
          case APPL_PAIR_WITH_FIND_ME_LOCATOR: 
          case APPL_CLEAR_SECURITY_DATABASE:
          case APPL_GET_ALERT_LEVEL:
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
      
      case APPL_DISCOVER_FIND_ME_LOCATOR:
      {
        APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_FIND_ME_LOCATOR: call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_FIND_ME_LOCATOR:
      {
        APPL_MESG_INFO(profiledbgfile,"APPL_CONNECT_FIND_ME_LOCATOR: call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_FIND_ME_LOCATOR:
      {
         APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_FIND_ME_LOCATOR: call Device_Disconnection() \n"); 
         Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_FIND_ME_LOCATOR:
      {
        APPL_MESG_INFO(profiledbgfile,"APPL_PAIR_WITH_FIND_ME_LOCATOR: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        APPL_MESG_INFO(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call Clear_Security_Database() \n"); 
        status = FMT_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          FMT_DBG_MSG(profiledbgfile,"FMT_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          FMT_DBG_MSG(profiledbgfile,"FMT_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      
      case APPL_GET_ALERT_LEVEL: /* user utility: it could be used for checking current alert level value */
      {
        uint8_t level; 
        
        level = FMT_Get_Alert_Level_Value();
        APPL_MESG_INFO(profiledbgfile,"APPL_GET_ALERT_LEVEL: call FMT_Get_Alert_Level_Value() 0x%0X\n", level);
        
      }
      break;
    
    }/* end switch(devicestate) */

  } /* end while(1) */
}/* end Host_Profile_Test_Application() */

/***************  Public Function callback ******************/

void FMT_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
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

void FMT_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
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

void FMT_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the glucose collector indication */
  //pinCode = ....

  ret = FMT_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

void FMT_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
}

/**
  * @brief  User Callback which is called each time alert level 
  *         value is received: user specific implementation 
  *         action should be done accordingly.
  * @param  None
  * @retval None
  */
void FMT_Alert_Level_Value_CB(uint8_t alert_level)
{
  switch(alert_level)
  {
    case (NO_ALERT):
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"\n(%c%c3) stops alerting\n", PTS_CODE, FMT_PTS_CODE);
#else       
      APPL_MESG_INFO(profiledbgfile,"Current Alert Level is NO_ALERT: 0x%0X\n",alert_level);
#endif
      /* do user specific implementation action ..... */
    break;
    case(MILD_ALERT):
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"\n(%c%c1) Mild Alert\n", PTS_CODE, FMT_PTS_CODE);
#else      
      APPL_MESG_INFO(profiledbgfile,"Current Alert Level is MILD_ALERT: 0x%0X\n",alert_level);
#endif    
      /* do user specific implementation action ..... */
    break;
        
    case(HIGH_ALERT):
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"\n(%c%c2) High Alert\n", PTS_CODE, FMT_PTS_CODE);
#else      
      APPL_MESG_INFO(profiledbgfile,"Current Alert Level is HIGH_ALERT: 0x%0X\n",alert_level);
#endif        
      /* do user specific implementation action ..... */
    break;
    default:
      APPL_MESG_INFO(profiledbgfile,"Current Alert Level is Unknown: 0x%0X\n",alert_level);
      /* do nothing ..... */
    break;
  }
}/* end FMT_Alert_Level_Value_CB() */

#endif    


/** \endcond 
 */
