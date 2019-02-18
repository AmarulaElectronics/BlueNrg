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
*   FILENAME        -  time_client.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      25/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Time Profile's implementation header file
*
********************************************************************************
*
*  File Description 
*  ---------------------
* This file have all Time Client's declarations. 
* 
*******************************************************************************/
#ifndef _TIME_CLIENT_H_
#define _TIME_CLIENT_H_

#include "time_profile_types.h"
/*******************************************************************************
* Macros
*******************************************************************************/


/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER         1

/* advertising intervals in terms of 
 * 625 micro seconds
 */
#define TIP_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define TIP_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define TIP_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define TIP_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */


/* time client profile main states */
#define TIME_CLIENT_STATE_UNINITIALIZED                 (0x00)
#define TIME_CLIENT_STATE_WAITING_BLE_INIT              (0x10)
#define TIME_CLIENT_STATE_INITIALIZED                   (0x30)
#define TIME_CLIENT_STATE_ADVERTIZE                     (0x40)
#define TIME_CLIENT_STATE_CONNECTED                     (0x60)
#define TIME_CLIENT_STATE_INVALID                       (0xFF)

/* Sub State for TIME_CLIENT_STATE_INITIALIZED main state */
#define TIME_CLIENT_STATE_WAITING_FOR_ADVERTISE_STATE                  (0X31)

/* Sub States for TIME_CLIENT_STATE_ADVERTIZE main state */
#define TIME_CLIENT_STATE_ADVERTISABLE_IDLE                            (0x41)
#define TIME_CLIENT_STATE_ADVERTISING_FOR_WHITE_LIST                   (0x42)
#define TIME_CLIENT_STATE_ADVERTISING_FOR_WHITE_LIST_TIMEOUT_DO_STOP   (0x43)
#define TIME_CLIENT_STATE_FAST_DISCOVERABLE_DO_START                   (0x44)
#define TIME_CLIENT_STATE_FAST_DISCOVERABLE                            (0x45)
#define TIME_CLIENT_STATE_FAST_DISCOVERABLE_TIMEOUT_DO_STOP            (0x46)
#define TIME_CLIENT_STATE_LOW_POWER_DISCOVERABLE_DO_START              (0x47)
#define TIME_CLIENT_STATE_LOW_POWER_DISCOVERABLE                       (0x48)
#define TIME_CLIENT_STATE_LOW_POWER_DISCOVERABLE_TIMEOUT_DO_STOP       (0x49)
#define TIME_CLIENT_STATE_BLE_ADVERTISING                              (0x4A)
#define TIME_CLIENT_STATE_PROCESSING_PAIRING                           (0x4B)

/* Sub States for TIME_CLIENT_STATE_CONNECTED main state */
#define TIME_CLIENT_STATE_CONNECTED_IDLE                        	          (0x61)
#define TIME_CLIENT_STATE_DO_DISCONNECT                         		  (0x62)
#define TIME_CLIENT_STATE_DISCONNECTING                         		  (0x63)
#define TIME_CLIENT_STATE_DISCONNECTED                          		  (0x64)
#define TIME_CLIENT_STATE_EXCHANGING_CONFIG                                       (0x65)
#define TIME_CLIENT_STATE_EXCHG_CONFIG_COMPLETE                                   (0x66)
#define TIME_CLIENT_STATE_DISCOVERING_ALL_PRIMARY_SERVICES                        (0x67)
#define TIME_CLIENT_STATE_DISCOVERED_ALL_PRIMARY_SERVICES                         (0x68)
#define TIME_CLIENT_STATE_DISCOVERING_CHARS_OF_CURRENT_TIME_SERVICE               (0x69)
#define TIME_CLIENT_STATE_DISCOVERED_CHARS_OF_CURRENT_TIME_SERVICE                (0x6A)
#define TIME_CLIENT_STATE_DISCOVERING_ALL_DESCRIPTORS_OF_CURRENT_TIME_CHAR        (0x6B)
#define TIME_CLIENT_STATE_DISCOVERED_ALL_DESCRIPTORS_OF_CURRENT_TIME_CHAR         (0x6C)
#define TIME_CLIENT_STATE_DISCOVERING_CHARS_OF_NEXT_DST_CHANGE_SERVICE            (0x6D)
#define TIME_CLIENT_STATE_DISCOVERED_CHARS_OF_NEXT_DST_CHANGE_SERVICE             (0x6E)
#define TIME_CLIENT_STATE_DISCOVERING_CHARS_OF_REFERENCE_UPDATE_TIME_SERVICE      (0x6F)
#define TIME_CLIENT_STATE_DISCOVERED_CHARS_OF_REFERENCE_UPDATE_TIME_SERVICE       (0x70)
#define TIME_CLIENT_STATE_DISABLING_CURRENT_TIME_UPDATE_NOTIFICATION              (0x71)
#define TIME_CLIENT_STATE_DISABLED_CURRENT_TIME_UPDATE_NOTIFICATION               (0x72)
#define TIME_CLIENT_STATE_READING_CURRENT_TIME_VALUE                              (0x73)
#define TIME_CLIENT_STATE_READING_LOCAL_TIME_INFO                                 (0x74)
#define TIME_CLIENT_STATE_READING_REFERENCE_TIME_INFO                             (0x75)
#define TIME_CLIENT_STATE_READING_NEXT_DST_CHANGE_TIME                            (0x76)
#define TIME_CLIENT_STATE_READING_SERVER_TIME_UPDATE_STATUS                       (0x77)
#define TIME_CLIENT_STATE_WRITING_TIME_UPDATE_CTL_POINT                           (0x78)
#define TIME_CLIENT_STATE_WRITING_CURRENT_TIME_UPDATE_NOTIFICATION                (0x79)


/** masks for the services 
 * and characteristics discovered
 */
#define CURRENT_TIME_SERVICE_MASK                         (0x0001)
#define NEXT_DST_CHANGE_SERVICE_MASK                      (0x0002)
#define REFERENCE_UPDATE_TIME_SERVICE_MASK                (0x0004)
#define CURRENT_TIME_CHAR_MASK                            (0x0008)
#define LOCAL_TIME_INFORMATION_CHAR_MASK                  (0x0010)
#define REFERENCE_TIME_INFORMATION_CHAR_MASK              (0x0020)
#define TIME_WITH_DST_CHAR_MASK                           (0x0040)
#define TIME_UPDATE_CONTROL_POINT_CHAR_MASK               (0x0080)
#define TIME_UPDATE_STATE_CHAR_MASK                       (0x0100)

/* error codes */
#define CT_CHARACTERISTIC_NOT_FOUND							(0x01)
#define CT_SERVICE_NOT_FOUND								(0x02)

/******************************************************************************
* Type definitions
******************************************************************************/
typedef struct _tTimeClientContext
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
   * connection handle 
   */ 
  uint16_t connHandle;
  
  /**
   * callback to notify the applciation with
   * the profile specific events
   */ 
  BLE_CALLBACK_FUNCTION_TYPE applicationNotifyFunc;
  
  /**
   * bit mask of the services found during the
   * discovery process on the reporter
   */ 
  uint16_t servicesFound;

  /**
   * handle of the current time service
   */ 
  uint16_t CurrentTimeServHandle;
      
  /**
   * End handle of the current time service
   */ 
  uint16_t CurrentTimeServEndHandle;
  
  /**
   * handle of the Next DST change service
   */ 
  uint16_t NextDSTServHandle;
  
  /**
   * End handle of the Next DST change service
   */ 
  uint16_t NextDSTServEndHandle;
  
  /**
   * handle of the Reference Time Update service
   */ 
  uint16_t RefUpdateTimeServHandle;
  
  /**
   * End handle of the Reference Time Update service
   */ 
  uint16_t RefUpdateTimeServEndHandle;
  
  /**
   * handle of the current time characteristic
   */ 
  uint16_t currentTimeCharHandle;
      
  /**
   * handle of the local time information characteristic
   */ 
  uint16_t localTimeInfoCharHandle;
      
  /**
   * handle of the reference time information characteristic
   */ 
  uint16_t refTimeInfoCharHandle;
      
  /**
   * handle of the time with DST characteristic
   */ 
  uint16_t timeWithDSTCharHandle;
      
  /**
   * handle of the time update control point characteristic
   */ 
  uint16_t timeUpdateCtlPointCharHandle;
      
  /**
   * handle of the time update state characteristic
   */ 
  uint16_t timeUpdateStateCharHandle;
      
  /**
   * handle of the client configuration descriptor 
   * of the current time characteristic
   */ 
  uint16_t currentTimeCharConfigDescHandle;

  /**
   * profile interface context to be registered
   * with the main profile
   */ 
  tProfileInterfaceContext TCtoBLEInf;
}tTimeClientContext;


/*******************************************************************************
* function prototypes
*******************************************************************************/
/**
 * 
 * @param[in] profileCallbackFunc : callback to be called by the profile to
 *            notify the application of the profile specific events
 * 
 * Initializes the time client and registers the time client profile with
 * the main profile
 */ 
tBleStatus TimeClient_Init(BLE_CALLBACK_FUNCTION_TYPE profileCallbackFunc);

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
tBleStatus TimeClient_Make_Discoverable(uint8_t useBoundedDeviceList);

/**
 * 
 * Time Client Discovery Primary Service routine
 */ 
void TimeClient_Discovery_Primary_Serv(void);

/**
 * 
 * starts a GATT procedure to read the current time
 * characteristic. Once the read is complete the read value
 * is given to the application through an event\n
 * 
 * the event data will have the following format:\n
 * byte --  content\n
 * 0    --  year\n
 * 2    --  month\n
 * 3	--  date\n
 * 4	--	hours\n
 * 5	--	minutes\n
 * 6	--	seconds\n
 * 7	--	day_of_week\n
 * 8	--	fractions256\n
 * 9	--	adjust_reason
 */ 
tBleStatus TimeClient_Get_Current_Time(void);

/**
 * 
 * Starts a gatt read procedure to read the local time
 * information characteristic on the server. The information read
 * will be passed to the application in the form of an event
 * 
 */ 
tBleStatus TimeClient_Get_Local_Time_Information(void);

/**
 * 
 * Starts a gatt read procedure to read the reference time
 * information characteristic on the server. the information read
 * will be passed to the application in the form of an event
 * 
 */ 
tBleStatus TimeClient_Get_Time_Accuracy_Info_Of_Server(void);

/**
 * 
 * Starts a gatt read procedure to read the time with DST
 * characteristic on the server. The information read
 * will be passed to the application in the form of an event
 * 
 */ 
tBleStatus TimeClient_Get_Next_DST_Change_Time(void);

/**
 * 
 * Starts a gatt read procedure to read the time update state
 * characteristic on the server. The information read
 * will be passed to the application in the form of an event
 * 
 */ 
tBleStatus TimeClient_Get_Server_Time_Update_State(void);

/**
 * 
 * @param[in] ctlValue: the value passed starts
 * a procedure to cancel or start a update process
 * 
 * Starts a gatt write without response procedure to 
 * write the time update control point characteristic  
 * on the server. The information read will be passed 
 * to the application in the form of an event
 * 
 */ 
tBleStatus TimeClient_Update_Reference_Time_On_Server(uint8_t ctlValue);

/**
 * 
 * @param[in] None
 * 
 * TimeClient profile's state machine: to be called on application main loop. 
 */ 
tBleStatus TimeClient_StateMachine(void);

/**
 *
 * @param[in] onoff : A value of 1 enables notifictions
 *            and 1 value of 0 disables notifications
 * 
 * By default notification of any change in Time on Time server 
 * will remain disabled. If the application on Time client need
 * the time update notification, it need to enable the notification
 * using this function.Once enabled, the notification of time 
 * update on time server will remain enabled. if the application 
 * dont want to receive the notification any more, it need to 
 * disable the notification using this function. 
 */
tBleStatus TimeClient_SetReset_Time_Update_Notification(uint8_t onoff);

/**
 * 
 * @param[in] procedureCode : opcode of the gatt procedure that
 *            has completed
 * 
 * Changes the profile substate according to the result of the procedure
 * and also starts the next action that needs to be taken 
 */
void TimeClient_GATT_Procedure_Complete_EvtHandler(uint8_t procedureCode);

#endif  /* _TIME_CLIENT_H_ */
