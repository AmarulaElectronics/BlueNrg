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
*   FILENAME        -  findme_locator_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      10/05/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Find Me Locator profile events callbacks for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Find Me Loator profile events callbacks
* 
******************************************************************************/

#ifndef _FINDME_LOCATOR_EVENTS_H_
#define _FINDME_LOCATOR_EVENTS_H_

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
 * FML_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * FML event callback which handles Find Me Locator actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void FML_disconnection_complete_event(uint8_t Status);

/**
 * FML_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * FML event callback which handles Find Me Locator actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void FML_le_connection_complete_event(uint8_t Status, uint16_t Connection_Handle);

/**
 * FML_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * FML event callback which handles Find Me Locator actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void FML_gap_limited_discoverable_event(void);

/**
 * FML_att_read_by_group_type_resp_event
 * 
 * @param[in] Attribute_Data_Length: The size of each attribute data
 * @param[in] Data_Length: Length of Attribute_Data_List in octets
 * @param[in] Attribute_Data_List: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 * 
 * FML event callback which handles Find Me Locator actions
 * when a BLE att read by group type response event happens
 * 
 * @return None
 */
void FML_att_read_by_group_type_resp_event(uint8_t Attribute_Data_Length,
                                           uint8_t Data_Length,
                                           uint8_t Attribute_Data_List[]);
/**
 * FML_att_read_by_type_resp_event
 * 
 * @param[in] Handle_Value_Pair_Length: The size of each attribute handle-value pair
 * @param[in] Data_Length: Length of Handle_Value_Pair_Data in octets
 * @param[in] Handle_Value_Pair_Data: Attribute Data List as defined in Bluetooth Core v4.1 spec.
 * 
 * FML event callback which handles Find Me Locator actions
 * when a BLE att read by type response event happens
 * 
 * @return None
 */
void FML_att_read_by_type_resp_event(uint8_t Handle_Value_Pair_Length,
                                     uint8_t Data_Length,
                                     uint8_t Handle_Value_Pair_Data[]);

/**
 * FML_gatt_proc_complete_event
 * 
 * @param[in] None
 * 
 * FML event callback which handles Find Me Locator actions
 * when a BLE gatt procedure complete event happens
 * 
 * @return None
 */
void FML_gatt_proc_complete_event(void);


#endif /* _FINDME_LOCATOR_EVENTS_H_ */
