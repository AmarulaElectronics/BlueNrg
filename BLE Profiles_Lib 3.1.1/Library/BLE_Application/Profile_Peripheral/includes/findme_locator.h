/******************************************************************************/
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
*   FILENAME        -  findme_locator.h
*
*   COMPILER        -  EWARM
*
*******************************************************************************
*
*   CHANGE HISTORY
* ---------------------------
*
*   $Date$:      09/05/2016
*   $Revision$:  First version
*   $Author$:    
*   Comments:    Find Me Profile 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header for the find me target profile
******************************************************************************/
#ifndef _FIND_ME_LOCATOR_
#define _FIND_ME_LOCATOR_

/**********************************************************************************
* MACROS
**********************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER                 1

#define UNPACK_2_BYTE_PARAMETER(ptr)  \
                (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
                (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

/* states */
#define FML_UNINITIALIZED		(0x00)
#define FML_INIT			(0x01)
#define FML_IDLE			(0x02)
#define FML_ADV_IN_FAST_DISC_MODE	(0x03)
#define FML_START_ADV_IN_LOW_PWR_MODE   (0x04)
#define FML_ADV_IN_LOW_PWR_MODE		(0x05)
#define FML_DISCOVER_SERVICE		(0x06)
#define FML_DISCOVER_CHARAC		(0x07)
#define FML_STATE_CONNECTED		(0x08)
#define FML_CONNECTED_IDLE		(0x09)

/* advertising intervals in terms of 625 micro seconds */
#define FML_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define FML_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define FML_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define FML_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

/**
 * error codes for EVT_FML_DISCOVERY_CMPLT
 */
#define ALERT_CHARACTERISTIC_NOT_FOUND	(0x01)
#define IMM_ALERT_SERVICE_NOT_FOUND     (0x02)

/* Alert levels */
#define NO_ALERT		        (0x00)
#define MILD_ALERT                      (0x01)
#define HIGH_ALERT                      (0x02)
                  
/******************************************************************************
* Type definitions
*****************************************************************************/
typedef struct _tFMLContext
{
	/**
	 * the current state of the
	 * find me locator profile
	 */ 
	uint8_t fmlstate;
    
	/**
	 * connection handle 
	 */ 
	uint16_t connHandle;
	
	/**
	 * store the handle of the
	 * immediate alert service once
	 * it is added
	 */ 
	uint16_t immAlertServHandle;
	
	/**
	 * store the handle of the
	 * immediate alert service once
	 * it is added
	 */ 
	uint16_t immAlertServEndHandle;
	
	/**
	 * stores the handle of the
	 * alert characteristic once
	 * it is added
	 */ 
	uint16_t alertCharHandle;
	
	/**
	 * internal structure used for
	 * registering the find me target
	 * with the ble profile
	 */ 
	tProfileInterfaceContext FMLtoBLEInf;
	
	/**
	 * application callback
	 * to be supplied by the application
	 * at the time of init
	 */ 
	BLE_CALLBACK_FUNCTION_TYPE applcb;
}tFMLContext;
               
                  
/**********************************************************************************
* Function Prototypes
**********************************************************************************/
/**
 * 
 * @param[in] bleSecReq : security requirements of the profile
 * @param[in] FindMeTargetcb : callback function to be called
 *           by the profile to notify the application of events
 * 
 * Initializes the Find Me Target profile
 */ 
tBleStatus FindMeLocator_Init(tSecurityParams* bleSecReq,
			      BLE_CALLBACK_FUNCTION_TYPE FindMeTargetcb);

/**
 * 
 * Find Me Locator Discovery Primary Service routine
 */ 
void FindMeLocator_Discovery_Primary_Serv(void);
							 
/**
 * 
 * This function puts the device into
 * discoverable mode if it is in the
 * proper state to do so
 */ 
tBleStatus FML_Advertize(void);

/**
 * 
 * @param[in] bdAddr : address of the peer device
 *            that has to be added to the whitelist
 */ 
tBleStatus FML_Add_Device_To_WhiteList(uint8_t addrType,uint8_t* bdAddr);

/**
 * 
 * This function is called when the advertising
 * for fast connection times out
 */ 
void FML_LimDisc_Timeout_Handler(void);

/**
 * 
 * @param[in] alertLevel : thw alert level the target has to be
 *            configured to\n
 *            NO_ALERT	   (0x00)\n
 *            MILD_ALERT   (0x01)\n
 *            HIGH_ALERT   (0x02)
 * 
 * Starts a write without response GATT procedure
 * to write the alertvalue to the find me target
 * 
 * @return returns BLE_STATUS_SUCCESS if the parameters
 * are valid and the procedure has been started else
 * returns error
 */ 
tBleStatus FML_ALert_Target(uint8_t alertLevel);

/**
 * 
 * FML profile's state machine: to be called on application main loop. 
 */ 
tBleStatus FMLProfile_StateMachine(void);

/**
 * 
 * @param[in] state : the new state of the FML
 * 
 * Changes the state of the FML state machine atomically
 * as the states can be changed in the interrupt context 
 * as well as from the application
 */ 
void FML_SetState(uint8_t state);

/**
 * FML_Exit
 * 
 * This function unregisters the profile with
 * the main profile and resets the state machine
 */ 
void FML_Exit(void);

#endif
