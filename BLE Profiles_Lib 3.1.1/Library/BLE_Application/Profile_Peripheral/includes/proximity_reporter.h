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
*   FILENAME        -  proximity_reporter.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      11/05/2016
*   $Revision$:  First version
*   $Author$:    
*   Comments:    Proximity Reporter Profile implementation header file for BlueNRG-1. 
*
********************************************************************************
*
*  File Description 
*  ---------------------
*
*******************************************************************************/
#ifndef _PROXIMITY_REPORTER_H_
#define _PROXIMITY_REPORTER_H_

/*******************************************************************************
* Macros
*******************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER                 1

#define READ_CURR_TRANSMIT_POWER_LEVEL   (0x00)

/* in terms of 625 micro seconds */
#define PXP_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define PXP_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define PXP_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define PXP_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */


/* Proximity Reporter Profile's main states */
#define PROXIMITY_REPORTER_STATE_UNINITIALIZED                 		 (0x00)
#define PROXIMITY_REPORTER_STATE_WAITING_BLE_INIT              		 (0x10)
#define PROXIMITY_REPORTER_STATE_INITIALIZING                 		 (0x20)
#define PROXIMITY_REPORTER_STATE_INITIALIZED                  		 (0x30)
#define PROXIMITY_REPORTER_STATE_ADVERTISE                    		 (0x40)
#define PROXIMITY_REPORTER_STATE_CONNECTED                    		 (0x50)
#define PROXIMITY_REPORTER_STATE_INVALID                       		 (0xFF)

/* sub states for PROXIMITY_REPORTER_STATE_INITIALIZING main state */
#define PROXIMITY_REPORTER_STATE_ADDING_LINK_LOSS_SERVICE                        (0x21)
#define PROXIMITY_REPORTER_STATE_ADDED_LINK_LOSS_SERVICE                         (0x22)
#define PROXIMITY_REPORTER_STATE_ADDING_LINK_LOSS_ALERT_LEVEL_CHAR               (0x23)
#define PROXIMITY_REPORTER_STATE_ADDED_LINK_LOSS_ALERT_LEVEL_CHAR                (0x24)
#define PROXIMITY_REPORTER_STATE_ADDING_IMMEDIATE_ALERT_SERVICE                  (0x25)
#define PROXIMITY_REPORTER_STATE_ADDED_IMMEDIATE_ALERT_SERVICE                   (0x26)
#define PROXIMITY_REPORTER_STATE_ADDING_IMMEDIATE_ALERT_LEVEL_CHAR               (0x27)
#define PROXIMITY_REPORTER_STATE_ADDED_IMMEDIATE_ALERT_LEVEL_CHAR                (0x28)
#define PROXIMITY_REPORTER_STATE_ADDING_TX_POWER_SERVICE                         (0x29)
#define PROXIMITY_REPORTER_STATE_ADDED_TX_POWER_SERVICE                          (0x2A)
#define PROXIMITY_REPORTER_STATE_ADDING_TX_POWER_LEVEL_CHAR                      (0x2B)
#define PROXIMITY_REPORTER_STATE_ADDED_TX_POWER_LEVEL_CHAR                       (0x2C)
#define PROXIMITY_REPORTER_STATE_ADDING_TX_PWR_LEVL_CHAR_PRESEN_FORMAT_DECS      (0x2D)
#define PROXIMITY_REPORTER_STATE_ADDED_TX_PWR_LEVL_CHAR_PRESEN_FORMAT_DECS       (0x2E)
#define PROXIMITY_REPORTER_STATE_READ_TX_PWR_LEVEL        			 (0x31)
#define PROXIMITY_REPORTER_STATE_UPDATE_TX_PWR_LEVEL_CHAR        		 (0x32)
#define PROXIMITY_REPORTER_STATE_INITIALIZATION_ERROR                            (0x2F)


/* Sub State for PROXIMITY_REPORTER_STATE_INITIALIZED main state */
#define PROXIMITY_REPORTER_STATE_WAITING_FOR_ADVERTISE_STATE           (0X31)

/* Sub State for PROXIMITY_REPORTER_STATE_ADVERTISE main state */
#define PROXIMITY_REPORTER_STATE_ADVERTISABLE_IDLE                     		(0x41)
#define PROXIMITY_REPORTER_STATE_ADVERTISING_FOR_WHITE_LIST_DO_START   		(0x42)
#define PROXIMITY_REPORTER_STATE_ADVERTISING_FOR_WHITE_LIST_STARTING   		(0x43)
#define PROXIMITY_REPORTER_STATE_ADVERTISING_FOR_WHITE_LIST            		(0x44)
#define PROXIMITY_REPORTER_STATE_ADVERTISING_FOR_WHITE_LIST_TIMEOUT_DO_STOP     (0x45)
#define PROXIMITY_REPORTER_STATE_ADVERTISING_FOR_WHITE_LIST_STOPPING            (0x46)
#define PROXIMITY_REPORTER_STATE_FAST_DISCOVERABLE_DO_START                     (0x47)
#define PROXIMITY_REPORTER_STATE_FAST_DISCOVERABLE_STARTING                     (0x48)
#define PROXIMITY_REPORTER_STATE_FAST_DISCOVERABLE                              (0x49)
#define PROXIMITY_REPORTER_STATE_FAST_DISCOVERABLE_TIMEOUT_DO_STOP              (0x4A)
#define PROXIMITY_REPORTER_STATE_FAST_DISCOVERABLE_STOPPING                     (0x4B)
#define PROXIMITY_REPORTER_STATE_LOW_POWER_DISCOVERABLE_DO_START                (0x4C)
#define PROXIMITY_REPORTER_STATE_LOW_POWER_DISCOVERABLE_STARTING                (0x4D)
#define PROXIMITY_REPORTER_STATE_LOW_POWER_DISCOVERABLE                         (0x4E)
#define PROXIMITY_REPORTER_STATE_LOW_POWER_DISCOVERABLE_TIMEOUT_DO_STOP         (0x4F)
#define PROXIMITY_REPORTER_STATE_LOW_POWER_DISCOVERABLE_STOPPING                (0x50)
#define PROXIMITY_REPORTER_STATE_BLE_ADVERTISING                                (0x51)
#define PROXIMITY_REPORTER_STATE_PROCESSING_PAIRING                             (0x52)

/* Sub States for PROXIMITY_REPORTER_STATE_CONNECTED main state */
#define PROXIMITY_REPORTER_STATE_CONNECTED_IDLE                        (0x51)
#define PROXIMITY_REPORTER_STATE_DO_DISCONNECT                         (0x52)
#define PROXIMITY_REPORTER_STATE_DISCONNECTING                         (0x53)
#define PROXIMITY_REPORTER_STATE_DISCONNECTED                          (0x54)

/* alert levels */
#define NO_ALERT		      (0x00)
#define MILD_ALERT                    (0x01)
#define HIGH_ALERT                    (0x02)

/******************************************************************************
* type definitions
******************************************************************************/
typedef struct _tProximityReporterContext
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
   * this is a flag set during initialization
   * based on the application's value to tell the
   * profile whether the TX power level and
   * immediate alert services have to be exposed
   */ 
  uint8_t immAlertTxPowerServSupport;
    
  /**
   * handle of link loss service
   */ 
  uint16_t linkLossServiceHandle;
	
  /**
   * handle of the alert level charecteristic of
   * link loss service
   */ 
  uint16_t linkLossAlertLevelCharHandle;
	
  /**
   * handle of the immediate alert service
   */ 
  uint16_t immdiateAlertServiceHandle;
	
  /**
   * handle of the alert level characteristic
   * in the immediate alert service
   */ 
  uint16_t immediateAlertLevelCharHandle;
	
  /**
   * handle of the TX power level service
   */ 
  uint16_t txPowerServiceHandle;
	
  /**
   * handle of the TX power level characteristic 
   */ 
  uint16_t txPowerLevelCharHandle;

  /**
   * the alert level that has to be raised
   * on detecting a link loss
   */ 
  uint8_t linkLossAlertLevel;
	
//  /**
//   * ID of the timer started by the
//   * proximity reporter
//   */ 
//  tTimerID timerID;
	
  /**
   * profile interface context to be registered
   * with the main profile
   */ 
  tProfileInterfaceContext PRtoBLEInf;
}tProximityReporterContext;


/******************************************************************************
* function prototypes
******************************************************************************/
/**
 * 
 * @param[in] immAlertTxPowerSupport : set to a non zero value if the TX power
 *            level and immediate alert services are to be exposed by the profile
 *            else set to 0
 * @param[in] profileCallbackFunc : the callback to be called to notify the
 *            application of the profile specific events
 */ 
tBleStatus ProximityReporter_Init(uint8_t immAlertTxPowerSupport, 
				  BLE_CALLBACK_FUNCTION_TYPE profileCallbackFunc);

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
tBleStatus ProximityReporter_Make_Discoverable(uint8_t useBoundedDeviceList);

/**
 * 
 * @param[in] None
 * 
 * Proximity Reporter profile's state machine: to be called on application main loop. 
 */ 
tBleStatus ProximityReporterProfile_StateMachine(void);


#endif  /* _PROXI_REPORTER_H_ */
