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
*   FILENAME        -  blood_pressure_events.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      12/04/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Blood pressure sensor profile events callbacks
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the Blood pressure sensor profile events callbacks
* 
******************************************************************************/

#ifndef _BLOOD_PRESSURE_EVENTS_H_
#define _BLOOD_PRESSURE_EVENTS_H_

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
 * BP_Sensor_disconnection_complete_event
 * 
 * @param[in] Status: BLE status related to the disconnection complete event 
 * 
 * BP event callback which handles Blood pressure actions
 * when a BLE disconnection complete event happens
 * 
 * @return None
 */ 
void BP_Sensor_disconnection_complete_event(uint8_t Status);

/**
 * BP_Sensor_le_connection_complete_event
 * 
 * @param[in] Status: BLE status related to the connection complete event
 * @param[in] Connection_Handle: Connection_Handle to be used to identify a
 * connection between two Bluetooth devices. 
 * 
 * BP event callback which handles Blood Pressure actions
 * when a BLE connection complete event happens
 * 
 * @return None
 */
void BP_Sensor_le_connection_complete_event(uint8_t Status, uint16_t Connection_Handle);

/**
 * BP_Sensor_gap_limited_discoverable_event
 * 
 * @param[in] None
 * 
 * BP event callback which handles Blood Pressure actions
 * when a BLE gap limited discoverable event happens
 * 
 * @return None
 */
void BP_Sensor_gap_limited_discoverable_event(void);


#endif /* _BLOOD_PRESSURE_EVENTS_H_ */
