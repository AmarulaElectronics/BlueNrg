/******************************************************************************
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
*   FILENAME        -  phonealert_client.h
*
*   COMPILER        -  EWARM
*
*******************************************************************************
*
*   CHANGE HISTORY
* ---------------------------
*
*   $Date$:      31/05/2016
*   $Revision$:  First version
*   $Author$:    
*   Comments:    Phone Alert Client Profile 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header for the phone alert client profile
******************************************************************************/
#ifndef _PHONE_ALERT_CLIENT_H_
#define _PHONE_ALERT_CLIENT_H_

/**********************************************************************************
* Macros
**********************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER                 1


#define UNPACK_2_BYTE_PARAMETER(ptr)  \
                (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
                (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))
                  

/* PAC states */
#define PAC_UNINITIALIZED				(0x00)
#define PAC_INIT			         	(0x01)
#define PAC_IDLE			    		(0x02)
#define PAC_ADV_IN_FAST_DISC_MODE			(0x03)
#define PAC_START_ADV_IN_LOW_PWR_MODE                   (0x04)
#define PAC_ADV_IN_LOW_PWR_MODE				(0x05)
#define PAC_DISCOVER_SERVICE				(0x06)
#define PAC_DISC_ALL_CHARACS				(0x07)
#define PAC_DISC_DESC_OF_ALERT_STATUS			(0x08)
#define PAC_DISC_DESC_OF_RINGER_SETTING			(0x09)
#define PAC_ENABLE_NOTIFICATION_FOR_ALERT_STATUS	(0x0A)
#define PAC_ENABLE_NOTIFICATION_FOR_RINGER_SETTING	(0x0B)
#define PAC_CONNECTED					(0x0C)
#define PAC_CONNECTED_IDLE				(0x0D)
#define PAC_READ_ALERT_STATUS				(0x0E)
#define PAC_READ_RINGER_SETTING				(0x0F)

/* advertising intervals in terms of 625 micro seconds */
#define PAC_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define PAC_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define PAC_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define PAC_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

/* value definitions for ringer setting characteristic */
#define RINGER_SILENT		(0x00)
#define RINGER_NORMAL		(0x01)

/* value definitions for ringer control point characteristic */
#define SILENT_MODE		(0x01)
#define MUTE_ONCE		(0x02)
#define CANCEL_SILENT_MODE	(0x03)

/* value definitions for alert status characteristic 
 * If the bits specified below are not set, it
 * implies that the corresponding state is not
 * active
 */
#define RINGER_STATE_ACTIVE		(0x01)
#define VIBRATOR_STATE_ACTIVE		(0x02)
#define DISPLAY_ALERT_ACTIVE		(0x04)

/* error codes for phone client */
#define PHONE_ALERT_SERVICE_NOT_FOUND		(0x01)
#define PHONE_ALERT_STATUS_CHARAC_NOT_FOUND     (0x02)
#define RINGER_CNTRL_POINT_CHARAC_NOT_FOUND     (0x03)
#define RINGER_SETTING_CHARAC_NOT_FOUND         (0x04)
#define PHONE_ALERT_STATUS_DESC_NOT_FOUND       (0x05)
#define RINGER_CNTRL_POINT_DESC_NOT_FOUND       (0x06)
#define RINGER_SETTING_DESC_NOT_FOUND           (0x07)

/******************************************************************************
* types and globals
*****************************************************************************/
typedef struct _tPhoneClientContext
{
	/**
	 * the current state of the
	 * phone alert status client profile
	 */ 
	uint8_t pacState;
    
	/**
	 * connection handle 
	 */ 
	uint16_t connHandle;
	
	/**
	 * store the handle of the
	 * phone alert status service once
	 * found
	 */ 
	uint16_t phoneAlertServHandle;
	
	/**
	 * store the end handle of the
	 * phone alert status service once
	 * found
	 */ 
	uint16_t phoneAlertServEndHandle;
	
	/**
	 * store the handle of the
	 * alert status characteristic once
	 * found
	 */ 
	uint16_t alertStatusCharHandle;
	
	/**
	 * store the handle of the
	 * ringer control point characteristic once
	 * found
	 */ 
	uint16_t ringerCntrlPntCharHandle;
	
	/**
	 * store the handle of the
	 * ringer setting characteristic once
	 * found
	 */ 
	uint16_t ringerSettingCharHandle;
	
	/**
	 * store the handle of the
	 * client configuration descriptor of 
	 * alert status characteristic once
	 * found
	 */ 
	uint16_t alertStatusCharDescHandle;
	
	/**
	 * store the handle of the
	 * client configuration descriptor of 
	 * ringer setting characteristic once
	 * found
	 */ 
	uint16_t ringerSettingCharDescHandle;
	
	/**
	 * internal structure used for
	 * registering the phone alert client
	 * with the ble profile
	 */ 
	tProfileInterfaceContext PACtoBLEInf;
	
	/**
	 * application callback
	 * to be supplied by the application
	 * at the time of init
	 */ 
	BLE_CALLBACK_FUNCTION_TYPE applcb;

}tPhoneClient;


/**********************************************************************************
* Function Prototypes
**********************************************************************************/
/**
 * 
 * @param[in] state : the new state of the PAC
 * 
 * Changes the state of the PAC state machine atomically
 * as the states can be changed in the interrupt context 
 * as well as from the application
 */ 
void PAC_SetState(uint8_t state);

/**
 * 
 * @param[in] FindMeTargetcb : callback function to be called
 *           by the profile to notify the application of events
 * 
 * Initializes the Phone Alert Status profile for client role
 */ 
tBleStatus PAC_Init(BLE_CALLBACK_FUNCTION_TYPE phoneAlertClientcb);

/**
 * 
 * @param[in] bdAddr : address of the peer device
 *            that has to be added to the whitelist
 */ 
tBleStatus PAC_Add_Device_To_WhiteList(uint8_t* bdAddr);
					
/**
 * 
 * This function puts the device into
 * discoverable mode if it is in the
 * proper state to do so
 */ 
tBleStatus PAC_Advertize(void);

/**
 * 
 * @param[in] ringerMode : the ringer mode to be set\n
 *            SILENT_MODE	    (0x01)\n
 *            MUTE_ONCE   	    (0x02)\n
 *            CANCEL_SILENT_MODE    (0x03)
 * 
 * Starts a write without response GATT procedure
 * to write the ringer mode command to the phone alert server
 * 
 * @return returns BLE_STATUS_SUCCESS if the parameters
 * are valid and the procedure has been started else
 * returns error codes
 */ 
tBleStatus PAC_Configure_Ringer(uint8_t ringerMode);

/**
 * 
 * When this function is called by the application,
 * the profile starts a gatt procedure to read the
 * characteristic value. The value read will be retuned
 * via the event to the application.
 * 
 * @return returns BLE_STATUS_SUCCESS if the procedure
 *         was started successfully
 */ 
tBleStatus PAC_Read_AlertStatus(void);

/**
 * 
 * When this function is called by the application,
 * the profile starts a gatt procedure to read the
 * characteristic value. The value read will be returned
 * via the event to the application.
 * 
 * @return returns BLE_STATUS_SUCCESS if the procedure
 *         was started successfully
 */ 
tBleStatus PAC_Read_RingerSetting(void);

/**
 * 
 * PAC profile's state machine: to be called on application main loop. 
 */ 
tBleStatus PACProfile_StateMachine(void);

/**
 * 
 * @param[in] None
 * 
 * Disable the Alert Status Notification
 */ 
void PAC_Disable_ALert_Status_Notification(void);

/**
 * 
 * @param[in] None
 * 
 * Disable the Ringer Status Notification
 */ 
void PAC_Disable_Ringer_Status_Notification(void);

/**
 * 
 * @param[in] deschandle : handle of the descriptor for
 *            which the notification has to be enabled
 * 
 * starts a GATT procedure to enable notification 
 * on the peer device
 */ 
void Enable_Notification(uint16_t deschandle);

#endif
