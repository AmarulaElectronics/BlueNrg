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
*   FILENAME        -  findme_target.h
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

#ifndef _FIND_ME_TARGET_
#define _FIND_ME_TARGET_

/******************************************************************************
* Macros
*****************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER                 1

/* states */
#define FMT_UNINITIALIZED		(0x00)
#define FMT_INIT			(0x01)
#define FMT_IDLE			(0x02)
#define FMT_ADV_IN_FAST_DISC_MODE	(0x03)
#define FMT_START_ADV_IN_LOW_PWR_MODE   (0x04)
#define FMT_ADV_IN_LOW_PWR_MODE		(0x05)
#define FMT_STATE_CONNECTED		(0x06)

/* advertising intervals in terms of 625 micro seconds */
#define FMT_FAST_CONN_ADV_INTERVAL_MIN	(0x20)  /* 20ms */
#define FMT_FAST_CONN_ADV_INTERVAL_MAX	(0x30)  /* 30ms */
#define FMT_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define FMT_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

/* Alert levels */
#define NO_ALERT   		        (0x00)
#define MILD_ALERT                      (0x01)
#define HIGH_ALERT                      (0x02)

/******************************************************************************
* Types
*****************************************************************************/
typedef struct _tFindMeContext
{
	/**
	 * the current state of the
	 * find me target profile
	 */ 
	uint8_t fmtstate;
    
	/**
	 * store the handle of the
	 * immediate alert service once
	 * it is added
	 */ 
	uint16_t immediateAlertServHandle;
	
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
	tProfileInterfaceContext FMTtoBLEInf;
	
	/**
	 * application callback
	 * to be supplied by the application
	 * at the time of init
	 */ 
	BLE_CALLBACK_FUNCTION_TYPE applcb;
}tFindMeContext;


/******************************************************************************
* Global Functions(SAP)
*****************************************************************************/

/**
 * 
 * @param[in] FindMeTargetcb: callback function to be called
 *           by the profile to notify the application of events
 * 
 * Initializes the Find Me Target profile
 */ 
tBleStatus FindMeTarget_Init(BLE_CALLBACK_FUNCTION_TYPE FindMeTargetcb);
							 
/**
 * 
 * This function puts the device into
 * discoverable mode if it is in the
 * proper state to do so
 */ 
tBleStatus FMT_Advertize(void);

/**
 * 
 * @param[in] addrType: address type of the bdAddr
 *            to be added to the whitelist
 * @param[in] bdAddr: address of the peer device
 *            that has to be added to the whitelist
 * 
 * Adds a device to the whitelist
 */ 
tBleStatus FMT_Add_Device_To_WhiteList(uint8_t addrType,uint8_t* bdAddr);

/**
 * 
 * FMT profile's state machine: to be called on application main loop. 
 */ 
tBleStatus FMTProfile_StateMachine(void);

/**
 * 
 * @param[in] state: state to set
 * 
 * Set the FMT state
 */ 
void FMT_SetState(uint8_t state);

#endif
