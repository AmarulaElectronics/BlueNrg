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
*   FILENAME        -  alertNotification_Client_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      20/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Alert Notification Client profile events callbacks for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Alert Notification Client profile events callbacks
* 
******************************************************************************/

#ifndef _ALERT_NOTIFICATION_CLIENT_EVENTS_H_
#define _ALERT_NOTIFICATION_CLIENT_EVENTS_H_

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
 * ANC_disconnection_complete_event
 * 
 * @param[in] None
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void ANC_disconnection_complete_event(void);

/**
 * ANC_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void ANC_le_connection_complete_event(uint8_t Status, uint16_t Connection_Handle);

/**
 * ANC_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void ANC_gap_limited_discoverable_event(void);

/**
 * ANC_gap_pairing_complete_event
 * 
 * @param[in] Status: BLE status related to the pairing complete event
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE pairing complete event happens
 * 
 * @return None
 */
void ANC_gap_pairing_complete_event(uint8_t Status);

/**
 * ANC_gatt_proc_timeout_event
 * 
 * @param[in] None
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE gatt procedure timeout event happens
 * 
 * @return None
 */
void ANC_gatt_proc_timeout_event(void);

/**
 * ANC_att_exchange_mtu_resp_event
 * 
 * @param[in] None
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE att exchange MTU response event happens
 * 
 * @return None
 */
void ANC_att_exchange_mtu_resp_event(void);

/**
 * ANC_att_find_info_resp_event
 * 
 * @param[in] Format: Format of the hanndle-uuid pairs
 * @param[in] Event_Data_Length: Length of Handle_UUID_Pair in octets
 * @param[in] Handle_UUID_Pair: A sequence of handle-uuid pairs. 
 *  if format=1, each pair is:[2 octets for handle, 2 octets for UUIDs], 
 *  if format=2, each pair is:[2 octets for handle, 16 octets for UUIDs] 
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE att find information response event happens
 * 
 * @return None
 */
void ANC_att_find_info_resp_event(uint8_t Format,
                                  uint8_t Event_Data_Length,
                                  uint8_t Handle_UUID_Pair[]);

/**
 * ANC_att_read_by_type_resp_event
 * 
 * @param[in] Handle_Value_Pair_Length: The size of each attribute handle-value pair
 * @param[in] Data_Length: Length of Handle_Value_Pair_Data in octets
 * @param[in] Handle_Value_Pair_Data: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 *  A sequence of handle-value pairs: [2 octets for Attribute Handle, 
 * (Handle_Value_Pair_Length - 2 octets) for Attribute Value]
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE att read by type response event happens
 * 
 * @return None
 */
void ANC_att_read_by_type_resp_event(uint8_t Handle_Value_Pair_Length,
                                     uint8_t Data_Length,
                                     uint8_t Handle_Value_Pair_Data[]);

/**
 * ANC_att_read_resp_event
 * 
 * @param[in] Event_Data_Length: Length of of following data
 * @param[in] Attribute_Value: The value of the attribute.
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE att read response event happens
 * 
 * @return None
 */
void ANC_att_read_resp_event(uint8_t Event_Data_Length,
                             uint8_t Attribute_Value[]);

/**
 * ANC_att_read_by_group_type_resp_event
 * 
 * @param[in] Attribute_Data_Length: The size of each attribute data
 * @param[in] Data_Length: Length of Attribute_Data_List in octets
 * @param[in] Attribute_Data_List: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE att read by group type response event happens
 * 
 * @return None
 */
void ANC_att_read_by_group_type_resp_event(uint8_t Attribute_Data_Length,
                                           uint8_t Data_Length,
                                           uint8_t Attribute_Data_List[]);

/**
 * ANC_gatt_notification_event
 * 
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Attribute_Value_Length: Length of Attribute_Value in octets
 * @param[in] Attribute_Value: The current value of the attribute
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE gatt notification event happens
 * 
 * @return None
 */
void ANC_gatt_notification_event(uint16_t Attribute_Handle,
                                 uint8_t Attribute_Value_Length,
                                 uint8_t Attribute_Value[]);

/**
 * ANC_gatt_proc_complete_event
 * 
 * @param[in] None
 * 
 * ANC event callback which handles Alert Notification Client actions
 * when a BLE gatt procedure complete event happens
 * 
 * @return None
 */
void ANC_gatt_proc_complete_event(void);


#endif /* _ALERT_NOTIFICATION_CLIENT_EVENTS_H_ */