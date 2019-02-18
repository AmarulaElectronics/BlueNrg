/*******************************************************************************/
/*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2016 STMicroelectronics International NV
*
*   FILENAME        -  alertNotification_Client.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      20/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Alert Notification Profile Header File 
*
********************************************************************************
*
*  File Description 
*  ---------------------
*  
*
*******************************************************************************/
#ifndef _ALERT_NOTIFICATION_CLIENT_H_
#define _ALERT_NOTIFICATION_CLIENT_H_

/*******************************************************************************
* Macro
*******************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER                 1

#define UNPACK_2_BYTE_PARAMETER(ptr)  \
                (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
                (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

/* states */
#define ANC_UNINITIALIZED		        (0x00)
#define ANC_INITIALIZED			        (0x01)
#define ANC_IDLE 			        (0x02)
#define ANC_UNDIRECTED_ADV_MODE 	        (0x03)
#define ANC_ADV_IN_FAST_DISC_MODE	        (0x04)
#define ANC_START_ADV_IN_LOW_PWR_MODE           (0x05)
#define ANC_ADV_IN_LOW_PWR_MODE		        (0x06)
#define ANC_DISCOVER_SERVICES		        (0x07)
#define ANC_DISCOVER_CHARACS		        (0x08)
#define ANC_DISCOVER_ALERT_CHAR_DESC		(0x09)
#define ANC_DISCOVER_UNREAD_ALERT_STATUS_DESC	(0x0A)
#define ANC_READ_NEW_ALERT_CATEGORY		(0x0B)
#define ANC_READ_UNREAD_ALERT_STATUS_CATEGORY	(0x0C)
#define ANC_CONNECTED				(0x0D)

/* advertising intervals in terms of 625 micro seconds */
#define ANC_FAST_CONN_ADV_INTERVAL_MIN	(0x20)  /* 20ms */
#define ANC_FAST_CONN_ADV_INTERVAL_MAX	(0x30)  /* 30ms */
#define ANC_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define ANC_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */
                  
/* error codes for ANC_DISCOVERY_CMPLT
 * event
 */
#define ALERT_NOTIFICATION_SERVICE_NOT_FOUND		(0x01)
#define SUPPORTED_NEW_ALERT_CATEGORY_CHAR_NOT_FOUND 	(0x02)
#define NEW_ALERT_CHAR_NOT_FOUND 			(0x04)
#define SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR_NOT_FOUND 	(0x08)
#define UNREAD_ALERT_STATUS_CHAR_NOT_FOUND 		(0x10)
#define CONTROL_POINT_CHAR_NOT_FOUND 			(0x20)
#define NEW_ALERT_CHAR_DESC_NOT_FOUND 			(0x40)
#define UNREAD_ALERT_STATUS_DESC_NOT_FOUND 		(0x41)
#define NO_CATEGORY_SUPPORTED_IN_ALERT_CHAR 		(0x42)
#define NO_CATEGORY_SUPPORTED_IN_UNREAD_ALERT_CHAR 	(0x43)

/* bitmasks for the characteristics discovered */
#define ALERT_NOTIFICATION_SERVICE_FOUND		(0x01)
#define SUPPORTED_NEW_ALERT_CATEGORY_CHAR_FOUND 	(0x02)
#define NEW_ALERT_CHAR_FOUND 				(0x04)
#define SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR_FOUND 	(0x08)
#define UNREAD_ALERT_STATUS_CHAR_FOUND 			(0x10)
#define CONTROL_POINT_CHAR_FOUND 			(0x20)

#define ANC_FOUND_ALL_CHARACS				(SUPPORTED_NEW_ALERT_CATEGORY_CHAR_FOUND|\
							 NEW_ALERT_CHAR_FOUND|\
							 SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR_FOUND|\
							 UNREAD_ALERT_STATUS_CHAR_FOUND|\
							 CONTROL_POINT_CHAR_FOUND)

/* alert category bitmasks first octet */
#define CATEGORY_SIMPLE_ALERT 				(0x01)
#define CATEGORY_EMAIL 					(0x02)
#define CATEGORY_NEWS 					(0x04)
#define CATEGORY_CALL 					(0x08)
#define CATEGORY_MISSED_CALL 				(0x10)
#define CATEGORY_SMS_MMS 				(0x20)
#define CATEGORY_VOICE_MAIL 				(0x40)
#define CATEGORY_SCHEDULE 				(0x80)

/* alert category bitmasks second octet */
#define CATEGORY_HIGH_PRIORITIZED_ALERT		        (0x01)
#define CATEGORY_INSTANT_MESSAGE			(0x02)

/* category ID to be written to the control point
 * characteristic
 */
typedef uint8_t tCategoryID;
#define CATEGORY_ID_SIMPLE_ALERT 			(0x00)
#define CATEGORY_ID_EMAIL 				(0x01)
#define CATEGORY_ID_NEWS 				(0x02)
#define CATEGORY_ID_CALL 				(0x03)
#define CATEGORY_ID_MISSED_CALL 			(0x04)
#define CATEGORY_ID_SMS_MMS 				(0x05)
#define CATEGORY_ID_VOICE_MAIL 				(0x06)
#define CATEGORY_ID_SCHEDULE 				(0x07)
#define CATEGORY_ID_HIGH_PRIORITIZED_ALERT		(0x08)
#define CATEGORY_ID_INSTANT_MESSAGE			(0x09)
#define CATEGORY_ID_ALL                                 (0xFF)

/* command ID to be written to the control point
 * characteristic
 */
typedef uint8_t tCommandID;
#define ENABLE_NEW_ALERT_NOTIFICATION			(0x00)
#define ENABLE_UNREAD_ALERT_STATUS_NOTIFICATION	        (0x01)
#define DISABLE_NEW_ALERT_NOTIFICATION			(0x02)
#define DISABLE_UNREAD_ALERT_STATUS_NOTIFICATION        (0x03)
#define NOTIFY_NEW_ALERT_IMMEDIATELY			(0x04)
#define NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY	        (0x05)


/******************************************************************************
* type definitions
******************************************************************************/
typedef struct _tANCContext
{
  /**
   * state of the blood pressure
   * sensor state machine
   */ 
  tProfileState state;
  
  /**
   * connection handle 
   */ 
  uint16_t connHandle;
  
  /**
   * bitmask of the attributes
   * discovered
   */ 
  uint8_t featuresFound;
  
  /**
   * flag which maintains
   * the connection status
   */
  uint8_t disconnected;
  
  /**
   * callback to notify the applciation with
   * the profile specific events
   */ 
  BLE_CALLBACK_FUNCTION_TYPE applcb;
  
  /**
   * handle of the alert notification service
   */ 
  uint16_t alertServiceHandle;
  
  /**
   * end handle of the alert notification service
   */ 
  uint16_t alertServiceEndHandle;
  
  /**
   * handle of the supported new alert category
   * characteristic
   */ 
  uint16_t newAlertCategoryHandle;
  
  /**
   * handle of the new alert characteristic
   */ 
  uint16_t newAlertHandle;
  
  /**
   * handle of the client configuration
   * descriptor of new alert characteristic
   */ 
  uint16_t newAlertDescHandle;
  
  /**
   * handle of the supported unread alert
   * category characteristic
   */ 
  uint16_t UnreadAlertCategoryHandle;
  
  /**
   * handle of the unread alert status
   * characteristic
   */ 
  uint16_t unreadAlertStatusHandle;
  
  /**
   * handle of the client configuration
   * descriptor of unread alert status characteristic
   */ 
  uint16_t unreadAlertStatusDescHandle;
  
  /**
   * handle of the alert notification
   * control point characteristic
   */ 
  uint16_t controlPointHandle;
  
  /**
   * alert category supported by the
   * peer server
   */ 
  uint8_t alertCategory[2];
  
  /**
   * unread alert status category
   * supported by the peer server
   */ 
  uint8_t unreadAlertCategory[2];

  /**
   * profile interface context. This is
   * registered with the main profile
   */ 
  tProfileInterfaceContext ANCtoBLEInf;
}tANCContext;

/*******************************************************************************
* Functions
*******************************************************************************/
/**
 * 
 * @param[in] applcb : the callback function to be called by the
 *            proifle to notify the application of the profile specific events
 * 
 * Initializes the alert notification profile in the client role
 * 
 * @return BLE_STATUS_SUCCESS if the initialization was successful
 */ 
tBleStatus ANC_Client_Init(BLE_CALLBACK_FUNCTION_TYPE applcb);

/**
 * 
 * @param[in] useWhiteList : This should be set to 
 *            true if the user is trying to establish
 *            a connection with a previously bonded
 *            device. 
 * 
 * If the useWhiteList is set to TRUE, then the device
 * is configured to use the whitelist which is configured
 * with bonded devices at the time of initialization else
 * limited discoverable mode is entered to connect to any
 * of the avialable devices
 */ 
tBleStatus ANC_Advertize(uint8_t useWhitelist);

/**
 * 
 * Alert Notification Client Discovery Primary Service routine
 */ 
void ANC_Discovery_Primary_Serv(void);

/**
 * 
 * @param[in] command : ID of the command to be sent
 * @param[in] category : ID of the category which has to be
 *            affected by the command. A value of 0xFF 
 *            will affect all categories 
 * 
 * Writes the control point characteristic with the command
 * and category specified. 
 * 
 * @return if the category ID specified is a category not
 * supported by the peer server or the categry ID or command ID
 * is out of range, then BLE_STATUS_INVALID_PARAMS is returned.
 * On successful write, BLE_STATUS_SUCCESS is returned
 */ 
tBleStatus ANC_Write_Control_Point(tCommandID command,tCategoryID category);

/**
 * 
 * @param[in] enable : enables the notifications for the
 * new alert characteristic. After enabling this the control
 * point characteristic should also be written with the command
 * and category in order to get alerts from the peer
 * 
 * @return BLE_STATUS_SUCCESS if the input parameter is valid
 * and the command is sent else BLE_STATUS_INVALID_PARAMS
 */         
tBleStatus ANC_Enable_Disable_New_Alert_Notification(uint16_t enable);

/**
 * 
 * @param[in] enable : enables the notifications for the
 * unread alert status characteristic. After enabling this the control
 * point characteristic should also be written with the command
 * and category in order to get alerts from the peer
 * 
 * @return BLE_STATUS_SUCCESS if the input parameter is valid
 * and the command is sent else BLE_STATUS_INVALID_PARAMS
 */         
tBleStatus ANC_Enable_Disable_Unread_Alert_Status_Notification(uint16_t enable);

/**
 * 
 * ANC profile's state machine: to be called on application main loop. 
 */ 
tBleStatus ANCProfile_StateMachine(void);

void ANC_Exit(uint8_t errCode);

#endif
