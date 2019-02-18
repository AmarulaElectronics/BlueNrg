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
*   FILENAME        -  alertNotification_Server_events.h
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
*   Comments:    Alert Notification Profile profile events callbacks for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Alert Notification Profile profile events callbacks
* 
******************************************************************************/

#ifndef _ALERT_NOTIFICATION_SERVER_EVENTS_H_
#define _ALERT_NOTIFICATION_SERVER_EVENTS_H_

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
 * ANS_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * ANS event callback which handles Alert Notification Server actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void ANS_disconnection_complete_event(uint8_t Status);

/**
 * ANS_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * ANS event callback which handles Alert Notification Server actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void ANS_le_connection_complete_event(uint8_t Status,uint16_t Connection_Handle);

/**
 * ANS_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * ANS event callback which handles Alert Notification Server actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void ANS_gap_limited_discoverable_event(void);

/**
 * ANS_gatt_write_permit_req_event
 * 
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Data_Length: Length of Data field
 * @param[in] Data: The data that the client has requested to write
 * 
 * HTP event callback which handles Alert Notification Server actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void ANS_gatt_write_permit_req_event(uint16_t Attribute_Handle,
                                     uint8_t Data_Length,
                                     uint8_t Data[]);
/**
 * ANS_gatt_attribute_modified_event
 * 
 * @param[in] Attr_Handle: Handle of the attribute that was modified
 * @param[in] Attr_Data: The modified value
 * 
 * ANS event callback which handles Alert Notification Server actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void ANS_gatt_attribute_modified_event(uint16_t Attr_Handle,
                                       uint8_t Attr_Data[]);

#endif /* _FINDME_TARGET_EVENTS_H_ */
