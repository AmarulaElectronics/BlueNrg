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
*   FILENAME        -  time_server_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      25/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Time Profile profile events callbacks for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Time Profile profile events callbacks
* 
******************************************************************************/

#ifndef _TIME_SERVER_EVENTS_H_
#define _TIME_SERVER_EVENTS_H_

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
 * TIS_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * TIS event callback which handles Time Server actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void TIS_disconnection_complete_event(uint8_t Status);

/**
 * TIS_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * TIS event callback which handles Time Server actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void TIS_le_connection_complete_event(uint8_t Status,uint16_t Connection_Handle);

/**
 * TIS_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * TIS event callback which handles Time Server actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void TIS_gap_limited_discoverable_event(void);

/**
 * TIS_gap_pairing_complete_event
 * 
* @param[in] Status: Pairing status
 * 
 * TIS event callback which handles Heart Rate actions
 * when a BLE gap pairing complete event happens
 * 
 * @return None
 */
void TIS_gap_pairing_complete_event(void);


/**
 * TIS_gatt_attribute_modified_event
 * 
 * @param[in] Attr_Handle: Handle of the attribute that was modified
 * @param[in] Attr_Data: The modified value
 * 
 * TIS event callback which handles Time Server actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void TIS_gatt_attribute_modified_event(uint16_t Attr_Handle,
                                       uint8_t Attr_Data[]);

/**
 * TIS_gatt_read_permit_req_event
 * 
 * @param[in] Attribute_Handle: Handle of the attribute that was read
 * 
 * TIS event callback which handles Time Server actions
 * when a BLE gatt read permit request event happens
 * 
 * @return None
 */
void TIS_gatt_read_permit_req_event(uint16_t Attribute_Handle);

#endif /* _FINDME_TARGET_EVENTS_H_ */
