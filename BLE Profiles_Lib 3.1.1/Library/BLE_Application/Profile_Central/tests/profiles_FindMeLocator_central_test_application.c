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
*   FILENAME        -  profile_FindMeLocator_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      13/09/2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Find Me Locator.
*                It uses the BlueNRG-1 Host Profiles Library 
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

#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
#include <find_me_locator.h>
#include <find_me_locator_config.h>


/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS PTS_PERIPHERAL_ADDRESS //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS

/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR

/* ---------- User Inputs for Profile Central role PTS tests -----------------*/

///@cond BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR
/**
 *@defgroup Find_Me_Locator
 *@brief PTS test application user input commands for Find Me Locator Central role
 *@{
 */

// General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 * Find Me Locator user input command: Start Discovery procedure for detecting a find me target(<b>FML_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_FIND_ME_TARGET      'a' 

/** 
 * Find Me Locator user input command: Start Connection procedure for connecting to the discovered find me target(<b>FML_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_FIND_ME_TARGET       'b' 

/** 
 * Find Me Locator user input command: Terminate the current connection with the find me target(<b>FML_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_FIND_ME_TARGET    'c' 

/** 
 *  Find Me Locator user input command: Start the pairing procedure (or enable the encryption) with the connected find me target (<b>FML_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_FIND_ME_TARGET     'd' 

/** 
 * Find Me Locator user input command: Clear the security database (it removes any bonded device) (<b>FML_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE      'e'

// ---------- 4.3: Discover Services and Characteristics tests 

/** 
 *  Find Me Locator user input command: Discover Immediate Alert Service using GA for LE (<b>FML_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_IAS_SERVICE                 'f' // 4.3.2 TP/FMS/FL/BV-02-I 

/** 
 *  Find Me Locator user input command: Discover Alert Level Characteristic for Immediate Alert(<b>FML_DiscCharacServ(IMMEDIATE_ALERT_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVER_ALERT_LEVEL_CHARACTERISTIC  'g' // 4.3.3 	TP/FMS/FL/BV-03-I 


// 4.4	Find Me Profile Features 

/** 
 *  Find Me Locator user input command: cause an alert on the Find Me Target device(<b>FML_ALert_Target(SET_ALERT) </b> API)
 */
#define APPL_SET_ALERT                            'h' // 4.4.1 	TP/FMF/FL/BV-01-I

/** 
 *  Find Me Locator user input command: cancel alert on the Find Me Target device(<b>FML_ALert_Target(CANCEL_ALERT) </b> API)
 */
#define APPL_CANCEL_ALERT                         'k' // 4.4.2 	TP/FMF/FL/BV-02-I 
// 4.4.3 	TP/FMF/FL/BV-03-I [Verify Bond Status on Reconnection] 

/** 
 *  Find Me Locator user input command: Starts all the connection, service discovery, characteristic discovery, ..... procedures.
 */
#define APPL_START_FULL_CONFIGURATION             'w' 

/** 
 *  Find Me Locator user input command: Display PTS user input commands list.
 */
#define DISPLAY_PTS_MENU                          'z'

/**
 * @}
 */
///@endcond

/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/* --------------- Define values required for PTS test cases ------------------ */
/* Alert level values */ 
#define SET_ALERT    0x01 
#define CANCEL_ALERT 0x00

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

  APPL_MESG_INFO(profiledbgfile,"****    Number of Primary Service: %d\r\n", numReporterServices);
  j = 0;
  for (i=0; i<numReporterServices; i++) {
    if (reporterServices[j] == UUID_TYPE_16) {
      Osal_MemCpy(&uuid, &reporterServices[j+5], 2);
      switch (uuid) {
      case IMMEDIATE_ALERT_SERVICE_UUID:
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c1)", PTS_CODE, FML_PTS_CODE);
#endif          
	APPL_MESG_INFO(profiledbgfile,"****    Immediate Service UUID found\r\n");
	break;
      default:
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
    case ALERT_LEVEL_CHARACTERISTIC_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c2)", PTS_CODE, FML_PTS_CODE);
#endif        
      APPL_MESG_INFO(profiledbgfile,"****      Alert Level Characteristic found\r\n");
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


void Display_Appl_Menu()
{
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_FIND_ME_TARGET: %c\n",APPL_DISCOVER_FIND_ME_TARGET);
  APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_FIND_ME_TARGET: %c\n",APPL_CONNECT_FIND_ME_TARGET);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_FIND_ME_TARGET: %c\n",APPL_DISCONNECT_FIND_ME_TARGET);
  APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_FIND_ME_TARGET: %c\n",APPL_PAIR_WITH_FIND_ME_TARGET);
  APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_IAS_SERVICE: %c\n",APPL_DISCOVER_IAS_SERVICE);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GL_MEASUREMENT_CONTEXT_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_ALERT_LEVEL_CHARACTERISTIC);
  APPL_MESG_DBG(profiledbgfile,"APPL_SET_ALERT: %c\n",APPL_SET_ALERT);
  APPL_MESG_DBG(profiledbgfile,"APPL_CANCEL_ALERT: %c\n",APPL_CANCEL_ALERT);
   
  APPL_MESG_DBG(profiledbgfile,"APPL_START_FULL_CONFIGURATION: %c\n",APPL_START_FULL_CONFIGURATION);
  APPL_MESG_DBG(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);          
}/* end Display_Appl_Menu() */
 
void Device_Init(void)
{
  fmlInitDevType InitDev; 
  uint8_t public_addr[6]=FML_PUBLIC_ADDRESS;
  uint8_t device_name[]=FML_DEVICE_NAME;
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      FML_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  
  ble_status = FML_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  fmlSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  FML_IO_CAPABILITY;
  param.mitm_mode =     FML_MITM_MODE;
  param.bonding_mode =  FML_BONDING_MODE;
  param.use_fixed_pin = FML_USE_FIXED_PIN;
  param.fixed_pin =     FML_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
    
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = FML_SC_SUPPORT;
  param.keypress_support = FML_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = FML_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    FML_OOB_ENABLE;
#endif
    
  ble_status = FML_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  fmlDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  FML_LIM_DISC_SCAN_INT;
  param.scanWindow =    FML_LIM_DISC_SCAN_WIND;
  ble_status = FML_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  fmlConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         FML_FAST_SCAN_DURATION;
  param.fastScanInterval =         FML_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           FML_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = FML_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   FML_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        FML_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        FML_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             FML_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      FML_SUPERVISION_TIMEOUT;
  param.min_conn_length =          FML_MIN_CONN_LENGTH;
  param.max_conn_length =          FML_MAX_CONN_LENGTH;
  ble_status = FML_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = FML_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_ServicesDiscovery(void)
{
  tBleStatus ble_status;
  ble_status = FML_ServicesDiscovery();

  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_ServicesDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_ServicesDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = FML_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"FML_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"FML_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_CharacServ(uint16_t uuid_service)
{
  tBleStatus ble_status;
  ble_status = FML_DiscCharacServ(uuid_service);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"Device_Discovery_CharacServ() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Device_Discovery_CharacServ() Error: %02X, uuid_service: 0x%02X\n", ble_status,uuid_service);
  }
}

void Device_StartFullConfig(void)
{
  fmlConnDevType connParam;
  //fmlConfDevType confParam;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS; 

  /* Connection Parameter */
  connParam.fastScanDuration =         FML_FAST_SCAN_DURATION;
  connParam.fastScanInterval =         FML_FAST_SCAN_INTERVAL;
  connParam.fastScanWindow =           FML_FAST_SCAN_WINDOW;
  connParam.reducedPowerScanInterval = FML_REDUCED_POWER_SCAN_INTERVAL;
  connParam.reducedPowerScanWindow =   FML_REDUCED_POWER_SCAN_WINDOW;
  connParam.peer_addr_type =           PUBLIC_ADDR;
  connParam.peer_addr =                peer_addr;
  connParam.own_addr_type =            PUBLIC_ADDR;
  connParam.conn_min_interval =        FML_FAST_MIN_CONNECTION_INTERVAL;
  connParam.conn_max_interval =        FML_FAST_MAX_CONNECTION_INTERVAL;
  connParam.conn_latency =             FML_FAST_CONNECTION_LATENCY;
  connParam.supervision_timeout =      FML_SUPERVISION_TIMEOUT;
  connParam.min_conn_length =          FML_MIN_CONN_LENGTH;
  connParam.max_conn_length =          FML_MAX_CONN_LENGTH;

  /* Configuration Parameter */

  if (FML_ConnConf(connParam) != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"Error in FML_ConnConf()\r\n");
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
  
  APPL_MESG_INFO(profiledbgfile,"\n FML Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init Profile Central Role */
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
    
    if (findMeLocatorContext.fullConf) 
       /* Profile state machine */
       FML_StateMachine(); 
    
#ifndef PTS_AUTOMATING    
    Enter_Sleep_Mode();
#endif
    
    /* if the profiles have nothing more to process, then
     * wait for application input
     */ 
    if(deviceState >= APPL_INIT_DONE) 
    {      
      uint8_t input = 200;
      deviceState = input;
      if (__io_getcharNonBlocking(&input))
      {
        APPL_MESG_DBG(profiledbgfile,"io--- input: %c\n",input); 

        switch(input)
        {           
          case DISPLAY_PTS_MENU: 
          case APPL_DISCOVER_FIND_ME_TARGET:
          case APPL_CONNECT_FIND_ME_TARGET:
          case APPL_DISCONNECT_FIND_ME_TARGET:
          case APPL_PAIR_WITH_FIND_ME_TARGET:
          case APPL_CLEAR_SECURITY_DATABASE:
          case APPL_DISCOVER_IAS_SERVICE:
          case APPL_DISCOVER_ALERT_LEVEL_CHARACTERISTIC:
          case APPL_SET_ALERT:
          case APPL_CANCEL_ALERT:
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
      case APPL_DISCOVER_FIND_ME_TARGET:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_FIND_ME_TARGET: call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_FIND_ME_TARGET:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_FIND_ME_TARGET: call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_FIND_ME_TARGET:
      {
         FML_DBG_MSG(profiledbgfile,"APPL_DISCONNECT_FIND_ME_TARGET: call Device_Disconnection() \n"); 
         Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_FIND_ME_TARGET:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_FIND_ME_TARGET: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call Clear_Security_Database() \n"); 
        status = FML_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          FML_DBG_MSG(profiledbgfile,"FML_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          FML_DBG_MSG(profiledbgfile,"FML_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      case APPL_DISCOVER_IAS_SERVICE: /* 4.3.2 */
      {
        /* It discover all the primary services of the connected find me target device */
        APPL_MESG_DBG(profiledbgfile,"Call Device_ServicesDiscovery() x IAS Service\n"); 
        Device_ServicesDiscovery();
      }
      break;
      
      case APPL_DISCOVER_ALERT_LEVEL_CHARACTERISTIC: /* 4.3.3 */ 
      {
        /* It discovers all the characteristics of the connected device  */
        APPL_MESG_DBG(profiledbgfile,"Call Device_Discovery_CharacServ() x Immediate Alert Service\n"); 
        Device_Discovery_CharacServ(IMMEDIATE_ALERT_SERVICE_UUID);
      }
      break;
      
      case APPL_SET_ALERT: /* 4.4.1 */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_ALERT (%d)\n",SET_ALERT);
        /* Set alert  */
        status = FML_ALert_Target(SET_ALERT);
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the FML_ALert_Target(%d) call: %02X\n", SET_ALERT, status);
        }
      }
      break;
      case APPL_CANCEL_ALERT: /* 4.4.2 */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CANCEL_ALERT (%d)\n",CANCEL_ALERT);
        /* Cancel alert  */
        status = FML_ALert_Target(CANCEL_ALERT);
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the FML_ALert_Target(%d) call: %02X\n", CANCEL_ALERT, status);
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

void FML_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
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

void FML_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
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

void FML_ServicesDiscovery_CB(uint8_t status, uint8_t numServices, uint8_t *services)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Primary Service Discovery Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugServices(numServices, services);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void FML_CharacOfService_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristics of a Service Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharac(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void FML_FullConfError_CB(uint8_t error_type, uint8_t code)
{
  if (findMeLocatorContext.fullConf) {
    APPL_MESG_INFO(profiledbgfile,"***** FML_FullConf_CB() Error Type = 0x%0x with Code = 0x%0x\r\n", error_type, code);
  }
}

void FML_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the glucose collector indication */
  //pinCode = ....

  ret = FML_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

void FML_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
  
}
#endif    


/** \endcond 
 */
