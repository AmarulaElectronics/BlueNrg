/*******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2016 STMicroelectronics International NV
*
*   FILENAME        -  profile_TimeServer_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      23/09/2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Time Server
*                Central role.
*                It uses the BlueNRG-1 Host Profiles Library.
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

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
#include <time_server.h>
#include <time_server_config.h>

/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS PTS_PERIPHERAL_ADDRESS //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS

/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR

/* Time Server test configuration for some PTS tests */

/* --------------------- User Inputs for Time Server PTS tests -----------------*/


///@cond BLE_CURRENT_PROFILE_ROLES & TIME_SERVER
/**
 *@defgroup Time_Server
 *@brief PTS test application user input commands for Time Server Central role
 *@{
 */

//General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 * Time Server user input command: Start Discovery procedure for detecting a time client(<b>TimeServer_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_TIME_CLIENT      'a' 

/** 
 *  Time Server user input command: Start Connection procedure for connecting to the discovered  time client (<b>TimeServer_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_TIME_CLIENT       'b' // Start Connection procedure for connecting to the discovered time client

/** 
 *  Time Server user input command: Terminate the current connection with the time client (<b>TimeServer_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_TIME_CLIENT    'c' 

/** 
 *  Time Server user input command: Start the pairing  procedure (or enable the encryption) with the connected time client (<b>TimeServer_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_TIME_CLIENT     'd' 

/** 
 *  Time Server user input command: Clear the security database (it removes any bonded device) (<b>TimeServer_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE   'e' 

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to MANUAL TIME UPDATE.
 */
#define APPL_ADJUST_REASON_MANUAL_TIME_UPDATE               'f'

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to CHANGE OF TIME ZONE.
 */
#define APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE              'g' 

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to CHANGE OF DST.
 */
#define APPL_ADJUST_REASON_CHANGE_OF_DST                    'h'

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to EXTERNAL REFERENCE TIME UPDATE.
 */
#define APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE   'k'

/** 
 *  Time Server user input command: update current time to + 2 minutes (<b>TimeServer_Update_Current_Time_Value(time) </b> API)
 */
#define APPL_UPDATE_CURRENT_TIME                'i' 

/** 
 *  Time Server user input command: update current time less than 1 minute (<b>TimeServer_Update_Current_Time_Value(time) </b> API)
 */
#define APPL_DECREASE_CURRENT_TIME              'j'  // TC_CSP_BV_06_C

/** 
 *  Time Server user input command: update current time more than 15 minutes (<b>TimeServer_Update_Current_Time_Value(time) </b> API)
 */
#define APPL_UPDATE_CURRENT_TIME_MORE_THAN_15   'l'  //TC_CSP_BV_06_C  

/** 
 *  Time Server user input command: update local time information characteristic (<b>TimeServer_Update_Local_Time_Information() </b> API)
 */
#define APPL_UPDATE_LOCAL_TIME			'm' 

/** 
 *  Time Server user input command: currently not used. 
 */
#define APPL_UPDATE_REFERENCE_TIME		'n' 

/** 
 *  Time Server user input command: update next DST change characteristic (<b>TimeServer_Update_Next_DST_Change() </b> API)
 */
#define APPL_UPDATE_NEXT_DSTCHANGE_TIME		'o' 

/** 
 *  Time Server user input command: update reference time characteristic (<b>TimeServer_Update_Reference_Time_Information() </b> API)
 */
#define APPL_UPDATE_REFERENCE_TIME_INFO		'p'

/** 
 *  Time Server user input command: Display PTS user input commands list
 */
#define DISPLAY_PTS_MENU                        'z' 

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

tCurrentTime time;

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

void Init_Current_Time()
{
  time.date = 26;
  time.day_of_week = 01;
  time.hours = 4;
  time.minutes = 1; /* keep it to 1 as default value x PTS tests */
  time.seconds = 4;
  
  time.adjustReason = 0x01; /* ADJUST_REASON_MANUAL_TIME_UPDATE: CTS/SR/CSP/BV-02-C */
  
  time.fractions256 = 0x00;
  time.year = 2016;
  time.month = 9;

}

void Display_Appl_Menu()
{
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCOVER_TIME_CLIENT: %c\n",APPL_DISCOVER_TIME_CLIENT);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_CONNECT_TIME_CLIENT: %c\n",APPL_CONNECT_TIME_CLIENT);
  APPL_MESG_INFO(profiledbgfile,"APPL_DISCONNECT_TIME_CLIENT: %c\n",APPL_DISCONNECT_TIME_CLIENT);
  APPL_MESG_INFO(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);
  APPL_MESG_INFO(profiledbgfile,"APPL_PAIR_WITH_TIME_CLIENT: %c\n",APPL_PAIR_WITH_TIME_CLIENT);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_ADJUST_REASON_MANUAL_TIME_UPDATE: %c\n",APPL_ADJUST_REASON_MANUAL_TIME_UPDATE);
  APPL_MESG_INFO(profiledbgfile,"APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE: %c\n",APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE);
  APPL_MESG_INFO(profiledbgfile,"APPL_ADJUST_REASON_CHANGE_OF_DST: %c\n",APPL_ADJUST_REASON_CHANGE_OF_DST);
  APPL_MESG_INFO(profiledbgfile,"APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: %c\n",APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE);
  
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_CURRENT_TIME: %c\n",APPL_UPDATE_CURRENT_TIME);
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_LOCAL_TIME: %c\n",APPL_UPDATE_LOCAL_TIME);
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_REFERENCE_TIME: %c\n",APPL_UPDATE_REFERENCE_TIME);
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_NEXT_DSTCHANGE_TIME: %c\n",APPL_UPDATE_NEXT_DSTCHANGE_TIME);
  APPL_MESG_INFO(profiledbgfile,"APPL_UPDATE_REFERENCE_TIME_INFO: %c\n",APPL_UPDATE_REFERENCE_TIME_INFO);	 
  
  APPL_MESG_INFO(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);          
    
}/* end Display_Appl_Menu() */

void Device_Init(void)
{
  tipsInitDevType InitDev; 
  uint8_t public_addr[6]=TIME_SERVER_PUBLIC_ADDRESS;
  uint8_t device_name[]=TIME_SERVER_DEVICE_NAME;
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      TIME_SERVER_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  
  /* Init Time Server Profile Central Role with optional supported services */
  InitDev.servicesToBeSupported = NEXT_DST_CHANGE_SERVICE_BITMASK|REFERENCE_TIME_UPDATE_SERVICE_BITMASK;
  
  ble_status = TimeServer_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  tipsSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  TIME_SERVER_IO_CAPABILITY;
  param.mitm_mode =     TIME_SERVER_MITM_MODE;
  param.bonding_mode =  TIME_SERVER_BONDING_MODE;
  param.use_fixed_pin = TIME_SERVER_USE_FIXED_PIN;
  param.fixed_pin =     TIME_SERVER_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
    
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = TIME_SERVER_SC_SUPPORT;
  param.keypress_support = TIME_SERVER_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = TIME_SERVER_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    TIME_SERVER_OOB_ENABLE;
#endif
      
  ble_status = TimeServer_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  tipsDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  TIME_SERVER_LIM_DISC_SCAN_INT;
  param.scanWindow =    TIME_SERVER_LIM_DISC_SCAN_WIND;
  ble_status = TimeServer_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  tipsConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         TIME_SERVER_FAST_SCAN_DURATION;
  param.fastScanInterval =         TIME_SERVER_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           TIME_SERVER_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = TIME_SERVER_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   TIME_SERVER_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        TIME_SERVER_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        TIME_SERVER_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             TIME_SERVER_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      TIME_SERVER_SUPERVISION_TIMEOUT;
  param.min_conn_length =          TIME_SERVER_MIN_CONN_LENGTH;
  param.max_conn_length =          TIME_SERVER_MAX_CONN_LENGTH;
  ble_status = TimeServer_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = TimeServer_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = TimeServer_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"TimeServer_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}

void Host_Profile_Test_Application (void)
{
  uint8_t ret;
  
  deviceState = APPL_UNINITIALIZED;
  
  APPL_MESG_INFO(profiledbgfile,"\n TIS Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init user time */
  Init_Current_Time();
  
  /* Init Time Server Profile Central Role */
  Device_Init();

  /* Init Time Server Profile Security */
  Device_SetSecurity();
  
  /* Add device service & characteristics */
  status =  TimeServer_Add_Services_Characteristics();
  if (status == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Time_Server_Add_Services_Characteristics() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Time_Server_Add_Services_Characteristics() Error: %02X\n", status);
  }
  
  deviceState = APPL_INIT_DONE;
  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();
    
    /* Call the Profile Master role state machine */
    Master_Process();
    
    TimeServer_StateMachine(); 

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
             
          case APPL_DISCOVER_TIME_CLIENT:
          case APPL_CONNECT_TIME_CLIENT:
          case APPL_DISCONNECT_TIME_CLIENT:
          case APPL_PAIR_WITH_TIME_CLIENT:
          case APPL_CLEAR_SECURITY_DATABASE:
            
          case APPL_ADJUST_REASON_MANUAL_TIME_UPDATE:
          case APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE: 
          case APPL_ADJUST_REASON_CHANGE_OF_DST:     
          case APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: 
            
          case APPL_UPDATE_CURRENT_TIME:            
          case APPL_UPDATE_LOCAL_TIME:				 
          case APPL_UPDATE_REFERENCE_TIME:			 
          case APPL_UPDATE_NEXT_DSTCHANGE_TIME:		 
          case APPL_UPDATE_REFERENCE_TIME_INFO:	
          case APPL_DECREASE_CURRENT_TIME:
          case APPL_UPDATE_CURRENT_TIME_MORE_THAN_15:
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
      
      case APPL_DISCOVER_TIME_CLIENT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_TIME_CLIENT: call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_TIME_CLIENT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_TIME_CLIENT: call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_TIME_CLIENT:
      {
         APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_TIME_CLIENT: call Device_Disconnection() \n"); 
         Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_TIME_CLIENT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_TIME_CLIENT: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call Clear_Security_Database() \n"); 
        status = TimeServer_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      
      case APPL_ADJUST_REASON_MANUAL_TIME_UPDATE:
      {
        time.adjustReason = 0x01; /* ADJUST_REASON_MANUAL_TIME_UPDATE: CTS/SR/CSP/BV-02-C */
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_MANUAL_TIME_UPDATE (time.adjustReason = 0x01)\n"); 
      }
      break;
      
      case APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE:
      {
        time.adjustReason = 0x04; /* ADJUST_REASON_CHANGE_OF_TIME_ZONE: CTS/SR/CSP/BV-03-C */
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_CHANGE_OF_TIME_ZONE (time.adjustReason = 0x04)\n"); 
      }
      break;
      case APPL_ADJUST_REASON_CHANGE_OF_DST:
      {
        time.adjustReason = 0x08; /* ADJUST_REASON_CHANGE_OF_DST:     CTS/SR/CSP/BV-04-C */
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_CHANGE_OF_DST (time.adjustReason = 0x08)\n"); 
      }
      break;
      case APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE:
      {
        time.adjustReason = 0x02; /* ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: CTS/SR/CSP/BV-05-C, CTS/SR/CSP/BV-06-C*/
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE (time.adjustReason = 0x02)\n"); 
      }
      break;
      case APPL_UPDATE_CURRENT_TIME:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_UPDATE_CURRENT_TIME (minute: %d) as +2 minutes\n", time.minutes); 
        
        //time.minutes += 1;
        //if(time.minutes == 60)
        //{
          //time.minutes = 0;
          //time.hours+=1;
        //}
        
        //TBR: new code for PTS tests including also PTS TEST TC_CSP_BV_06_C */
        if ((time.minutes + 2) >= 60)
        {
          time.minutes = (time.minutes + 2) - 60;
          time.hours+=1;
        }
        else
          time.minutes += 2;

        status = TimeServer_Update_Current_Time_Value(time);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Current_Time_Value() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Current_Time_Value() Error: %02X\n", status);
        }
      }
      break;
      
      case APPL_DECREASE_CURRENT_TIME: //New code only for PTS TEST TC_CSP_BV_06_C 
      {
        /* Adjusting time less than 1 minute from previous one */
        APPL_MESG_DBG(profiledbgfile,"APPL_DECREASE_CURRENT_TIME (minute: %d) less than 1 minute (decrease 1 second)\n",time.minutes);
        /* Just decrease current time of 1 second: required for PTS test TC_CSP_BV_06_C */
        time.seconds -= 1;

        status = TimeServer_Update_Current_Time_Value(time);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Current_Time_Value() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Current_Time_Value() Error: %02X\n", status);
        }
      }
      break;
      case APPL_UPDATE_CURRENT_TIME_MORE_THAN_15://New code (only for PTS TEST TC_CSP_BV_06_C?  TBR if needed)
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_UPDATE_CURRENT_TIME_MORE_THAN_15 (minute: %d) as +15 minutes\n", time.minutes);
        if ((time.minutes + 15) >= 60)
        {
          time.minutes = (time.minutes + 15) - 60;
          time.hours+=1;
        }
        else
          time.minutes += 15;
        status = TimeServer_Update_Current_Time_Value(time);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Current_Time_Value() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Current_Time_Value() Error: %02X\n", status);
        }
      }
      break;
        
      case APPL_UPDATE_LOCAL_TIME:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_UPDATE_LOCAL_TIME\n");
        tLocalTimeInformation localTimeInfo;
        localTimeInfo.timeZone = 0x03;
        localTimeInfo.dstOffset = 0x12;
        TimeServer_Update_Local_Time_Information(localTimeInfo);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Local_Time_Information() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Local_Time_Information() Error: %02X\n", status);
        }
      }
      break;
      case APPL_UPDATE_NEXT_DSTCHANGE_TIME:
      {
        tTimeWithDST timeDST;
        timeDST.date = 06;
        timeDST.year = 2000;
        timeDST.month = 04;
        timeDST.hours = 1;
        timeDST.minutes = 1;
        timeDST.seconds = 4;
               
        APPL_MESG_DBG(profiledbgfile,"APPL_UPDATE_NEXT_DSTCHANGE_TIME\n");
        TimeServer_Update_Next_DST_Change(timeDST);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Next_DST_Change() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Next_DST_Change() Error: %02X\n", status);
        }
      }
      break;
      case APPL_UPDATE_REFERENCE_TIME_INFO:
      {
        tReferenceTimeInformation refTimeInfo;
        refTimeInfo.source = 0x01;	
        refTimeInfo.accuracy = 0x02;	 
        refTimeInfo.daysSinceUpdate = 0x05;	 
        refTimeInfo.hoursSinceUpdate = 0x03;	 
        APPL_MESG_DBG(profiledbgfile,"APPL_UPDATE_REFERENCE_TIME_INFO\n");
        TimeServer_Update_Reference_Time_Information(refTimeInfo);
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Reference_Time_Information() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"TimeServer_Update_Reference_Time_Information() Error: %02X\n", status);
        }    
      }
      break;

    }/* end switch(devicestate) */

  } /* end while(1) */
}/* end Host_Profile_Test_Application() */

/***************  Public Function callback ******************/

void TimeServer_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
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

void TimeServer_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
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

void TimeServer_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the glucose collector indication */
  //pinCode = ....

  ret = TimeServer_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

void TimeServer_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
  
}

void TimeServer_Notify_State_To_User_Application_CB(uint8_t event_value)
{
  switch (event_value)
  {
   case EVT_TS_START_REFTIME_UPDATE:
    {
      APPL_MESG_INFO(profiledbgfile,"EVT_TS_START_REFTIME_UPDATE: call TimeServer_Update_Current_Time_Value()\n");
      TimeServer_Update_Current_Time_Value(time);
    }
    break;
    case EVT_TS_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_INFO(profiledbgfile,"EVT_TS_CHAR_UPDATE_CMPLT\n");		
    }
    break;
    case EVT_TS_CURTIME_READ_REQ:
    {
      APPL_MESG_INFO(profiledbgfile,"EVT_TS_CURTIME_READ_REQ: call TimeServer_Allow_Curtime_Char_Read()\n");
      TimeServer_Allow_Curtime_Char_Read();
    }
    break;
    case EVT_TS_STOP_REFTIME_UPDATE:
    {
      APPL_MESG_INFO(profiledbgfile,"EVT_TS_STOP_REFTIME_UPDATE\n");
    }
    break;
  }
}
#endif    

/** \endcond 
 */
