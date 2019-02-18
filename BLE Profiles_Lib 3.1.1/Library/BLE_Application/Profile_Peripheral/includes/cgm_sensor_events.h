/******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2018 STMicroelectronics International NV
*
*   FILENAME        -  cgm_sensor_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      06/08/2018
*   $Revision$:  first version
*   $Author$:    
*   Comments:    cgm sensor profile events callbacks
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the cgm sensor profile events callbacks
* 
******************************************************************************/

#ifndef _CGM_SENSOR_EVENTS_H_
#define _CGM_SENSOR_EVENTS_H_

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
 * CGM_Sensor_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * CGM event callback which handles Cgm Sensor actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void CGM_Sensor_disconnection_complete_event(uint8_t Status);

/**
 * CGM_Sensor_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * CGM event callback which handles Cgm Sensor actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void CGM_Sensor_le_connection_complete_event(uint8_t Status,uint16_t Connection_Handle);

/**
 * CGM_Sensor_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * CGM event callback which handles Cgm Sensor actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void CGM_Sensor_gap_limited_discoverable_event(void);

/**
 * CGM_Sensor_gatt_write_permit_req_event
 * 
 * @param[in] Attribute_Handle: The handle of the attribute
 * @param[in] Data_Length: Length of Data field
 * @param[in] Data: The data that the client has requested to write
 * 
 * CGM event callback which handles Cgm Sensor actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void CGM_Sensor_gatt_write_permit_req_event(uint16_t Attribute_Handle,
                                            uint8_t Data_Length,
                                            uint8_t Data[]);

/**
 * CGM_Sensor_gatt_attribute_modified_event
 * 
 * @param[in] Attr_Handle: Handle of the attribute that was modified
 * @param[in] Attr_Data_Length: Length of Attr_Data in octets
 * @param[in] Attr_Data: The modified value
 * 
 * CGM event callback which handles Cgm Sensor actions
 * when a BLE gatt write permit request event happens
 * 
 * @return None
 */
void CGM_Sensor_gatt_attribute_modified_event(uint16_t Attr_Handle,
                                              uint16_t Attr_Data_Length,
                                              uint8_t Attr_Data[]);

   
void CGM_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                      uint16_t Available_Buffers);                                         
#endif /* _HEALTH_THERMOMETER_EVENTS_H_ */
