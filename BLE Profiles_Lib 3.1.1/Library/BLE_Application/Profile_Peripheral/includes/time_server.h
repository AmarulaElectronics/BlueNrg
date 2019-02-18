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
*   FILENAME        -  time_server.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      24/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Time Profile's implementation header file
*
********************************************************************************
*
*  File Description 
*  ---------------------
*  This file have Time Profile's Server Role's declarations. 
*
*******************************************************************************/
#ifndef _TIME_SERVER_H_
#define _TIME_SERVER_H_

#include "time_profile_types.h"
#include "hal_types.h"
/*******************************************************************************
* Macros
*******************************************************************************/
/* Number of virtual timer used for advertising timeout or delay handling */
#define VIRTUAL_TIMER                   1
/* number of virtual timer started for the delay between current time updates */
#define DELAY_TIMER                     2

/* in terms of 625 micro seconds */
#define TIP_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define TIP_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define TIP_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define TIP_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

/* Time server profile main states */
#define TIME_SERVER_STATE_UNINITIALIZED                 (0x00)
#define TIME_SERVER_STATE_WAITING_BLE_INIT              (0x10)
#define TIME_SERVER_STATE_INITIALIZED                   (0x20)
#define TIME_SERVER_STATE_ADVERTIZE                     (0x30)
#define TIME_SERVER_STATE_CONNECTED                     (0x40)
#define TIME_SERVER_STATE_INVALID                       (0xFF)

/* substates for TIME_SERVER_STATE_ADVERTIZE main state*/
#define TIME_SERVER_STATE_ADVERTISABLE_IDLE                            (0x31)
#define TIME_SERVER_STATE_ADVERTISING_FOR_WHITE_LIST                   (0x32)
#define TIME_SERVER_STATE_ADVERTISING_FOR_WHITE_LIST_TIMEOUT_DO_STOP   (0x33)
#define TIME_SERVER_STATE_FAST_DISCOVERABLE_DO_START                   (0x34)
#define TIME_SERVER_STATE_FAST_DISCOVERABLE                            (0x35)
#define TIME_SERVER_STATE_FAST_DISCOVERABLE_TIMEOUT_DO_STOP            (0x36)
#define TIME_SERVER_STATE_LOW_POWER_DISCOVERABLE                       (0x37)
#define TIME_SERVER_STATE_BLE_ADVERTISING                              (0x38)
#define TIME_SERVER_STATE_PROCESSING_PAIRING                           (0x39)

/* substates under TIME_SERVER_STATE_CONNECTED main state */
#define TIME_SERVER_STATE_CONNECTED_IDLE                               (0x41)
#define TIME_SERVER_STATE_CONNECTED_BUSY                               (0x42)
#define TIME_SERVER_STATE_UPDATING_NEW_TIME_VALUE                      (0x43)
#define TIME_SERVER_STATE_DO_DISCONNECT                                (0x44)
#define TIME_SERVER_STATE_DISCONNECTING                                (0x45)
#define TIME_SERVER_STATE_DISCONNECTED                                 (0x46)

#define NEXT_DST_CHANGE_SERVICE_BITMASK		(0x01)
#define REFERENCE_TIME_UPDATE_SERVICE_BITMASK   (0x02)

#define REFERENCE_UPDATE_TIMEOUT		(300) /* 5 minutes */

/******************************************************************************
* Type Definitions
******************************************************************************/
typedef struct _tTimeServerContext
{
  /**
       * main state of the heart rate
       * sensor state machine
       */ 
  tProfileState mainState;
      
      /**
       * sub state of the heart rate
       * sensor state machine
       */ 
  tProfileState subState;
 
      /**
       * callback to notify the applciation with
       * the profile specific events
       */ 
  BLE_CALLBACK_FUNCTION_TYPE applicationNotifyFunc;

     /**
       * flag which is set at the time of initialization
       * by the application. It indicates whether the optional
       * services of Next DST Change and Reference Update Time 
       * are supported
       */ 
  uint8_t servicesSupported;
  
      /**
       * connection handle
       */
  uint16_t connHandle;

      /**
       * handle of current time service
       */ 
  uint16_t currentTimeServiceHandle;
      
      /**
       * handle of current time characteristic
       */ 
  uint16_t currentTimeCharHandle;
      
      /**
       * handle of local time characteristic
       */ 
  uint16_t localTimeInfoCharHandle;
      
      /**
       * handle of reference time characteristic
       */ 
  uint16_t referenceTimeInfoCharHandle;
      
      /**
       * handle of next DST change service
       */ 
  uint16_t nextDSTChangeServiceHandle;
      
      /**
       * handle of DST characteristic
       */ 
  uint16_t timeWithDSTCharHandle;
      
      /**
       * handle of Reference Time Update service
       */ 
  uint16_t referenceUpdateTimeServiceHandle;
      
      /**
       * handle of time update control point characteristic
       */ 
  uint16_t timeUpdateControlPointCharHandle;
      
      /**
       * handle of time update state characteristic
       */ 
  uint16_t timeUpdateStateCharHandle;
      
      /**
       * time update state
       */
  uint8_t timeUpdateState;

//      /**
//       * ID of the timer started for
//       * advertising timeout or delay
//       * handling
//       */ 
//  tTimerID timerID;
      
//      /**
//       * ID of the timer started for
//       * the delay between current time updates
//       */
//   tTimerID delayTimerID; 
  BOOL delayTimerStart; 
      /**
       * profile interface context to be registered
       * with the main profile
       */ 
  tProfileInterfaceContext TStoBLEInf;
      
      /**
       * this flag is used to send the allow
       * read command
       */ 
  uint8_t timeServerAllowRead;
	
}tTimeServerContext;


/******************************************************************************
* function prototypes
*****************************************************************************/
/**
 * 
 * @param[in] servicesToBeSupported: bit mask of the optional services 
 *            to be supported
 * @param[in] timeServerCallbackFunc : function to be called by the profile
 *            to notify the application of profile specific events
 * 
 * Initializes the time profile in server role
 * 
 * @return BLE_STATUS_SUCCESS if the time server is successfully initialized
 */ 
tBleStatus TimeServer_Init(uint8_t servicesToBeSupported, 
                           BLE_CALLBACK_FUNCTION_TYPE timeServerCallbackFunc);

/**
 * 
 * @param[in] useBoundedDeviceList: advertize with whitelist which contains 
 *             devices that were previously bonded
 * 
 * puts the device into discoverable mode
 * 
 * @return BLE_STATUS_SUCCESS if the command to put the device into
 * discoverable mode was successfully sent
 */ 
tBleStatus TimeServer_Make_Discoverable(uint8_t useBoundedDeviceList);

/**
 * 
 * @param[in] timeValue : the time with which the current time
 *            characteristic has to be updated
 * 
 * updates the current time characteristic with the timeValue
 * specified
 */ 
tBleStatus TimeServer_Update_Current_Time_Value(tCurrentTime timeValue);

/**
 * 
 * @param[in] localTimeInfo : the local time information with which the local time
 *            information characteristic has to be updated
 * 
 * updates the localTimeInfo characteristic with the value specified
 */ 
tBleStatus TimeServer_Update_Local_Time_Information(tLocalTimeInformation localTimeInfo);

/**
 * 
 * @param[in] refTimeInfo : the new reference time information
 * 
 * updates the reference time information characteristic with 
 * the value specified
 */ 
tBleStatus TimeServer_Update_Reference_Time_Information(tReferenceTimeInformation refTimeInfo);

/**
 * 
 * @param[in] timeDST : the new DST information
 * 
 * updates the next DST change characteristic with 
 * the value specified
 */ 
tBleStatus TimeServer_Update_Next_DST_Change(tTimeWithDST timeDST);

/**
 * 
 * @param[in] errCode: the error code with which the update
 *            completed
 * @param[in] newTime : new time to update the current time
 */ 
void Update_Reference_Time(uint8_t errCode,tCurrentTime* newTime);

/**
 * 
 * sends the allow read command to the controller
 */ 
tBleStatus Allow_Curtime_Char_Read(void);

/**
 * 
 * @param[in] pckt : Pointer to the ACI packet
 * 
 * It parses the events received from ACI according to the profile's state machine.
 * Inside this function each event must be identified and correctly parsed.
 * NOTE: It is the event handler to be called inside HCI_Event_CB().
 */ 
void TimeServer_Event_Handler(void *pckt);

/**
 * 
 * TimeServer profile's state machine: to be called on application main loop. 
 */ 
tBleStatus TimeServer_StateMachine(void);


#endif  /* _TIME_SERVER_H_ */
