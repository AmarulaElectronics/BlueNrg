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
*   FILENAME        -  proximity_monitor_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      13/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Proximity Monitor profile events callbacks for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Proximity Monitor profile events callbacks
* 
******************************************************************************/

#ifndef _PROXIMITY_MONITOR_EVENTS_H_
#define _PROXIMITY_MONITOR_EVENTS_H_

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
 * PXM_disconnection_complete_event
 * 
 * @param[in] None
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void PXM_disconnection_complete_event(void);

/**
 * PXM_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void PXM_le_connection_complete_event(uint8_t Status, uint16_t Connection_Handle);

/**
 * PXM_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void PXM_gap_limited_discoverable_event(void);

/**
 * PXM_gap_pairing_complete_event
 * 
 * @param[in] Status: BLE status related to the pairing complete event
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE pairing complete event happens
 * 
 * @return None
 */
void PXM_gap_pairing_complete_event(uint8_t Status);

/**
 * PXM_gatt_proc_timeout_event
 * 
 * @param[in] None
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE gatt procedure timeout event happens
 * 
 * @return None
 */
void PXM_gatt_proc_timeout_event(void);

/**
 * PXM_att_exchange_mtu_resp_event
 * 
 * @param[in] None
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE att exchange MTU response event happens
 * 
 * @return None
 */
void PXM_att_exchange_mtu_resp_event(void);

/**
 * PXM_att_find_info_resp_event
 * 
 * @param[in] Format: Format of the hanndle-uuid pairs
 * @param[in] Event_Data_Length: Length of Handle_UUID_Pair in octets
 * @param[in] Handle_UUID_Pair: A sequence of handle-uuid pairs. 
 *  if format=1, each pair is:[2 octets for handle, 2 octets for UUIDs], 
 *  if format=2, each pair is:[2 octets for handle, 16 octets for UUIDs] 
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE att find information response event happens
 * 
 * @return None
 */
void PXM_att_find_info_resp_event(uint8_t Format,
                                  uint8_t Event_Data_Length,
                                  uint8_t Handle_UUID_Pair[]);

/**
 * PXM_att_read_by_type_resp_event
 * 
 * @param[in] Handle_Value_Pair_Length: The size of each attribute handle-value pair
 * @param[in] Data_Length: Length of Handle_Value_Pair_Data in octets
 * @param[in] Handle_Value_Pair_Data: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 *  A sequence of handle-value pairs: [2 octets for Attribute Handle, 
 * (Handle_Value_Pair_Length - 2 octets) for Attribute Value]
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE att read by type response event happens
 * 
 * @return None
 */
void PXM_att_read_by_type_resp_event(uint8_t Handle_Value_Pair_Length,
                                     uint8_t Data_Length,
                                     uint8_t Handle_Value_Pair_Data[]);

/**
 * PXM_att_read_resp_event
 * 
 * @param[in] Event_Data_Length: Length of of following data
 * @param[in] Attribute_Value: The value of the attribute.
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE att read response event happens
 * 
 * @return None
 */
void PXM_att_read_resp_event(uint8_t Event_Data_Length,
                             uint8_t Attribute_Value[]);

/**
 * PXM_att_read_by_group_type_resp_event
 * 
 * @param[in] Attribute_Data_Length: The size of each attribute data
 * @param[in] Data_Length: Length of Attribute_Data_List in octets
 * @param[in] Attribute_Data_List: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE att read by group type response event happens
 * 
 * @return None
 */
void PXM_att_read_by_group_type_resp_event(uint8_t Attribute_Data_Length,
                                           uint8_t Data_Length,
                                           uint8_t Attribute_Data_List[]);

/**
 * PXM_gatt_notification_event
 * 
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Attribute_Value_Length: Length of Attribute_Value in octets
 * @param[in] Attribute_Value: The current value of the attribute
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE gatt notification event happens
 * 
 * @return None
 */
void PXM_gatt_notification_event(uint16_t Attribute_Handle,
                                 uint8_t Attribute_Value_Length,
                                 uint8_t Attribute_Value[]);

/**
 * PXM_gatt_proc_complete_event
 * 
 * @param[in] None
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE gatt procedure complete event happens
 * 
 * @return None
 */
void PXM_gatt_proc_complete_event(void);

/**
 * PXM_gatt_error_resp_event
 * 
 * @param[in] None
 * 
 * PXM event callback which handles Proximity Monitor actions
 * when a BLE gatt error response event happens
 * 
 * @return None
 */
void PXM_gatt_error_resp_event(void);


#endif /* _PROXIMITY_MONITOR_EVENTS_H_ */
