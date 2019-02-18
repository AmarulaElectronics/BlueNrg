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
*   FILENAME        -  findme_target_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      09/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Find Me Target profile events callbacks for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Find Me Target profile events callbacks
* 
******************************************************************************/

#ifndef _FINDME_TARGET_EVENTS_H_
#define _FINDME_TARGET_EVENTS_H_

/******************************************************************************
* Macro Declarations
******************************************************************************/


/******************************************************************************
* Type definitions
******************************************************************************/


/******************************************************************************
* Function Declarations
******************************************************************************/
/**
 * FMT_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * FMT event callback which handles Find Me Target actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void FMT_disconnection_complete_event(uint8_t Status);

/**
 * FMT_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * FMT event callback which handles Find Me Target actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void FMT_le_connection_complete_event(uint8_t Status,uint16_t Connection_Handle);

/**
 * FMT_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * FMT event callback which handles Find Me Target actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void FMT_gap_limited_discoverable_event(void);

/**
 * FMT_gatt_attribute_modified_event
 * 
 * @param[in] Attr_Handle: Handle of the attribute that was modified
 * @param[in] Attr_Data: The modified value
 * 
 * FMT event callback which handles Find Me Target actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void FMT_gatt_attribute_modified_event(uint16_t Attr_Handle,
                                              uint8_t Attr_Data[]);

#endif /* _FINDME_TARGET_EVENTS_H_ */
