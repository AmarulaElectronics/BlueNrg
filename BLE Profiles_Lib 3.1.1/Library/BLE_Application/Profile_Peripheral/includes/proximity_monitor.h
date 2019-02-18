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
*   FILENAME        -  proximity_monitor.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      13/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Proximity Monitor Profile implementation header file for BlueNRG-1.
*
********************************************************************************
*
*  File Description 
*  ---------------------
*  This file will have Declarations for the implementation of 
*  Proximity Monitor Profile. 
*
*******************************************************************************/
#ifndef _PROXIMITY_MONITOR_H_
#define _PROXIMITY_MONITOR_H_

/*******************************************************************************
* Macros
*******************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER         1


#define UNPACK_2_BYTE_PARAMETER(ptr)  \
                (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
                (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

/* GATT command Opcode */
#define HCI_CMD_NO_OPERATION				     (0x0000)
#define HCI_VENDOR_CMD_GATT_EXCHG_CONFIG                     (0xFD0B)
#define HCI_VENDOR_CMD_GATT_DISC_ALL_PRIMARY_SERVICES        (0xFD12)
#define HCI_VENDOR_CMD_GATT_DISC_ALL_CHARAC_OF_A_SERVICE     (0xFD15)
#define HCI_VENDOR_CMD_GATT_WRITE_CHARAC_VAL                 (0xFD1C)
#define HCI_VENDOR_CMD_GATT_DISC_ALL_CHAR_DESCRIPTORS        (0xFD17)
#define HCI_VENDOR_CMD_GATT_READ_CHARACTERISTIC_VAL          (0xFD18)
#define HCI_VENDOR_CMD_GATT_WRITE_CHARAC_DESCRIPTOR          (0xFD21)
#define HCI_STATUS_PARAM_CMD_READ_RSSI                       (0x1405)
#define HCI_VENDOR_CMD_GATT_WRITE_WITHOUT_RESPONSE           (0xFD23)
#define HCI_VENDOR_CMD_GAP_SET_UNDIRECTED_CONNECTABLE        (0xFC8C)
#define HCI_VENDOR_CMD_GAP_SET_NON_DISCOVERABLE              (0xFC81)

/* bit masks to maintain track of the services found */
#define LINK_LOSS_SERVICE_MASK	      (0x01)
#define IMMEDIATE_ALERT_SERVICE_MASK  (0x02)
#define TX_POWER_LEVEL_SERVICE_MASK   (0x04)

/* advertising intervals in terms of 625 micro seconds */
#define PXP_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define PXP_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define PXP_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define PXP_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

/* proximity monitor main states */
#define PROXIMITY_MONITOR_STATE_UNINITIALIZED                 (0x00)
#define PROXIMITY_MONITOR_STATE_WAITING_BLE_INIT              (0x10)
#define PROXIMITY_MONITOR_STATE_INITIALIZING                  (0x20)
#define PROXIMITY_MONITOR_STATE_INITIALIZED                   (0x30)
#define PROXIMITY_MONITOR_STATE_ADVERTIZE                     (0x40)
#define PROXIMITY_MONITOR_STATE_CONNECTED                     (0x60)
#define PROXIMITY_MONITOR_STATE_INVALID                       (0xFF)

/* Sub State for PROXIMITY_MONITOR_STATE_INITIALIZED main state */
#define PROXIMITY_MONITOR_STATE_WAITING_FOR_ADVERTISE_STATE           (0x31)

/* Sub States for PROXIMITY_MONITOR_STATE_ADVERTISE main state */
#define PROXIMITY_MONITOR_STATE_ADVERTISABLE_IDLE                            (0x41)
#define PROXIMITY_MONITOR_STATE_ADVERTISING_FOR_WHITE_LIST_DO_START          (0x42)
#define PROXIMITY_MONITOR_STATE_ADVERTISING_FOR_WHITE_LIST_STARTING          (0x43)
#define PROXIMITY_MONITOR_STATE_ADVERTISING_FOR_WHITE_LIST                   (0x44)
#define PROXIMITY_MONITOR_STATE_ADVERTISING_FOR_WHITE_LIST_TIMEOUT_DO_STOP   (0x45)
#define PROXIMITY_MONITOR_STATE_ADVERTISING_FOR_WHITE_LIST_STOPPING          (0x46)
#define PROXIMITY_MONITOR_STATE_FAST_DISCOVERABLE_DO_START                   (0x47)
#define PROXIMITY_MONITOR_STATE_FAST_DISCOVERABLE_STARTING                   (0x48)
#define PROXIMITY_MONITOR_STATE_FAST_DISCOVERABLE                            (0x49)
#define PROXIMITY_MONITOR_STATE_FAST_DISCOVERABLE_TIMEOUT_DO_STOP            (0x4A)
#define PROXIMITY_MONITOR_STATE_FAST_DISCOVERABLE_STOPPING                   (0x4B)
#define PROXIMITY_MONITOR_STATE_LOW_POWER_DISCOVERABLE_DO_START              (0x4C)
#define PROXIMITY_MONITOR_STATE_LOW_POWER_DISCOVERABLE_STARTING              (0x4D)
#define PROXIMITY_MONITOR_STATE_LOW_POWER_DISCOVERABLE                       (0x4E)
#define PROXIMITY_MONITOR_STATE_LOW_POWER_DISCOVERABLE_TIMEOUT_DO_STOP       (0x4F)
#define PROXIMITY_MONITOR_STATE_LOW_POWER_DISCOVERABLE_STOPPING              (0x50)
#define PROXIMITY_MONITOR_STATE_BLE_ADVERTISING                              (0x51)
#define PROXIMITY_MONITOR_STATE_PROCESSING_PAIRING                           (0x52)


/* Sub States for PROXIMITY_MONITOR_STATE_CONNECTED main state */
#define PROXIMITY_MONITOR_STATE_CONNECTED_IDLE                                       (0x61)
#define PROXIMITY_MONITOR_STATE_EXCHANGING_CONFIG                                    (0x62)
#define PROXIMITY_MONITOR_STATE_EXCHG_CONFIG_COMPLETE                                (0x63)
#define PROXIMITY_MONITOR_STATE_DISCOVERING_ALL_PRIMARY_SERVICES                     (0x64)
#define PROXIMITY_MONITOR_STATE_DISCOVERED_ALL_PRIMARY_SERVICES                      (0x65)
#define PROXIMITY_MONITOR_STATE_DISCOVERING_CHARACS_OF_LINK_LOSS_SERV                (0x66)
#define PROXIMITY_MONITOR_STATE_DISCOVERED_CHARACS_OF_LINK_LOSS_SERV                 (0x67)
#define PROXIMITY_MONITOR_STATE_WRITING_LL_ALERT_CHARACTERISTIC                      (0x68)
#define PROXIMITY_MONITOR_STATE_WRITTEN_LL_ALERT_CHARACTERISTIC                      (0x69)
#define PROXIMITY_MONITOR_STATE_DISCOVERING_CHARACS_OF_IMMED_ALERT_SERV              (0x6A)
#define PROXIMITY_MONITOR_STATE_DISCOVERED_CHARACS_OF_IMMED_ALERT_SERV               (0x6B)
#define PROXIMITY_MONITOR_STATE_WRITING_IMM_ALERT_CHARACTERISTIC                     (0x6C)
#define PROXIMITY_MONITOR_STATE_WRITTEN_IMM_ALERT_CHARACTERISTIC                     (0x6D)
#define PROXIMITY_MONITOR_STATE_DISCOVERING_CHARACS_OF_TX_PWR_SERV                   (0x6E)
#define PROXIMITY_MONITOR_STATE_DISCOVERED_CHARACS_OF_TX_PWR_SERV                    (0x6F)
#define PROXIMITY_MONITOR_STATE_DISCOVERING_ALL_DESCRIPTORS_OF_TX_PWR_LEVL_CHAR      (0x70)
#define PROXIMITY_MONITOR_STATE_DISCOVERED_ALL_DESCRIPTORS_OF_TX_PWR_LEVL_CHAR       (0x71)
#define PROXIMITY_MONITOR_STATE_READING_TX_POWER_LEVEL                               (0x72)
#define PROXIMITY_MONITOR_STATE_READ_TX_POWER_LEVEL_DONE                             (0x73)
#define PROXIMITY_MONITOR_STATE_ENABLING_TX_POWER_LEVEL_NOTIFICATION                 (0x74)
#define PROXIMITY_MONITOR_STATE_ENABLED_TX_POWER_LEVEL_NOTIFICATION                  (0x75)
#define PROXIMITY_MONITOR_STATE_MONITOR                                              (0x76)
#define PROXIMITY_MONITOR_STATE_PATH_LOSS_DETECTED                                   (0x77)
#define PROXIMITY_MONITOR_STATE_SENDING_IMMEDIATE_ALERT                              (0x78)
#define PROXIMITY_MONITOR_STATE_SENT_AN_IMMEDIATE_ALERT                              (0x79)
#define PROXIMITY_MONITOR_STATE_DO_DISCONNECT                                        (0x7A)
#define PROXIMITY_MONITOR_STATE_DISCONNECTING                                        (0x7B)
#define PROXIMITY_MONITOR_STATE_DISCONNECTED                                         (0x7C)
#define PROXIMITY_MONITOR_STATE_WAIT                                                 (0x77)

                  
/* alert levels */
#define NO_ALERT		          (0x00)
#define MILD_ALERT                        (0x01)
#define HIGH_ALERT                        (0x02)

#define PATH_LOSS_THRESHOLD               (0x7F)

#define LINK_LOSS_ALERT_LEVEL		  (MILD_ALERT)
#define PATH_LOSS_ALERT_LEVEL		  (MILD_ALERT)
/**
 * Error codes for EVT_PM_DISCOVERY_CMPLT
 */ 
#define PM_LINK_LOSS_SERVICE_NOT_FOUND	  (0x01)
                  
                  
/*******************************************************************************
* type definitions
*******************************************************************************/
typedef struct _tProximityMonitorContext
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
   * bit mask of the services found during the
   * discovery process on the reporter
   */ 
  uint8_t servFound;
	
  /**
   * flag to keep track of whether we had
   * previously started the alert on the
   * reporter
   */ 
  uint8_t pathLossNotify;
	
  /**
   * connection handle
   */ 
  uint16_t connHandle;
	
  /**
   * handle of link loss service
   */ 
  uint16_t linkLossServHandle;
	
  /**
   * end handle of link loss service
   */ 
  uint16_t linkLossServEndHandle;
	
  /**
   * handle of the immediate alert service
   */ 
  uint16_t immAlertServHandle;
	
  /**
   * End handle of the immediate alert service
   */ 
  uint16_t immAlertServEndHandle;
	
  /**
   * handle of the TX power level service
   */ 
  uint16_t txPwrLvlServHandle;
	
  /**
   * End handle of the TX power level service
   */ 
  uint16_t txPwrLvlServEndHandle;
	
  /**
   * characteristic handle of the alert level
   * characteristic of the link loss service
   */ 
  uint16_t linkLossAlertLevelCharHandle;
	
  /**
   * characteristic handle of the alert level
   * characteristic of immediate alert service
   */
  uint16_t immAlertLevelCharHandle;
	
  /**
   * characteristic handle of the TX power
   * level characteristic
   */ 
  uint16_t txPowerLevelCharHandle;
	
  /**
   * characteristic descriptor handle of the
   * client configuration descriptor
   */ 
  uint16_t txPwrLevlCharConfigDescHandle;
	
  /**
   * characteristic descriptor handle of the
   * TX power level presentation format
   * descriptor
   */ 
  uint16_t txPwrLevlCharPresFormatDescHandle;

  /**
   * TX power level of the reporter
   * as read from the TX power level service
   */ 
  int8_t  txPwrLvl;
	
  /**
   * profile interface context to be registered
   * with the main profile
   */ 
  tProfileInterfaceContext PMtoBLEInf;

  /**
   * BLE Procedure ongoing
   */
  uint16_t opCode;
}tProximityMonitorContext;


/******************************************************************************
 * function prototypes
 *****************************************************************************/
/**
 * 
 * @param[in] profileCallbackFunc : the callback function to be called by the
 *            proifle to notify the application of the profile specific events
 * 
 * Initializes the proximity monitor
 * 
 * @return BLE_STATUS_SUCCESS if the initialization was successful
 */ 
tBleStatus ProximityMonitor_Init(BLE_CALLBACK_FUNCTION_TYPE profileCallbackFunc);

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
tBleStatus ProximityMonitor_Make_Discoverable(uint8_t useBoundedDeviceList);

/**
 * 
 * Proximity Monitor Discovery Primary Service routine
 */ 
void ProximityMonitor_Discovery_Primary_Serv(void);

/**
 *
 * This function is added to enable/disable the alert
 * for pathLoss alerts from the command line.
 * Note : This function is only for testing using PTS as devices cannot
 * be physically moved away.
 *
 * @return BLE_STATUS_SUCCESS if the command was successfully sent 
 */
tBleStatus ProximityMonitor_Notify_Path_Loss(uint8_t enableAlert);

/**
 * 
 * Proximity Monitor profile's state machine: to be called on application main loop. 
 */ 
tBleStatus ProximityMonitorProfile_StateMachine(void);

/**
 * 
 * @param[in] None
 */ 
void ProximityMonitor_GATT_Procedure_Complete_EvtHandler (void);


#endif  /* _PROXIMITY_MONITOR_H_ */
