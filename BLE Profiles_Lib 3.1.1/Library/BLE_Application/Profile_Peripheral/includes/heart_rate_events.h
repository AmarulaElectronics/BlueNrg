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
*   FILENAME        -  heart_rate_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      11/04/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    heart rate sensor profile events callbacks
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the heart rate sensor profile events callbacks
* 
******************************************************************************/

#ifndef _HEART_RATE_EVENTS_H_
#define _HEART_RATE_EVENTS_H_

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
 * HR_Sensor_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * HRP event callback which handles Heart Rate actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void HR_Sensor_disconnection_complete_event(uint8_t Status);

/**
 * HR_Sensor_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * HRP event callback which handles Heart Rate actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void HR_Sensor_le_connection_complete_event(uint8_t Status,
                                            uint16_t Connection_Handle);

/**
 * HR_Sensor_gap_pairing_complete_event
 * 
* @param[in] Status: Pairing status
 * 
 * HRP event callback which handles Heart Rate actions
 * when a BLE gap pairing complete event happens
 * 
 * @return None
 */
void HR_Sensor_gap_pairing_complete_event(uint8_t Status);

/**
 * HR_Sensor_gatt_write_permit_req_event
 * 
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Data_Length: Length of Data field
 * @param[in] Data: The data that the client has requested to write
 * 
 * HRP event callback which handles Heart Rate actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void HR_Sensor_gatt_write_permit_req_event(uint16_t Attribute_Handle,
                                           uint8_t Data_Length,
                                           uint8_t Data[]);

#endif /* _HEART_RATE_EVENTS_H_ */
