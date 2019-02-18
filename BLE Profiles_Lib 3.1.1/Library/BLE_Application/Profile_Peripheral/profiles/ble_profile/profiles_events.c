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
*   FILENAME        -  profiles_events.c
*
*   COMPILER        -  EWARM
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      11/04/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    BlueNRG-1 Peripheral Profiles  Events File 
*         
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  This file will have implementation of BlueNRG-1 Peripheral Profiles Events
* 
******************************************************************************/

/******************************************************************************
* Include Files
******************************************************************************/
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "ble_profile.h"
#include "host_config.h"
#include "debug.h"
#include "ble_events.h"
#include "osal.h"
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
#include "health_thermometer_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
#include "heart_rate_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
#include "blood_pressure_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
#include "glucose_sensor_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
#include "cgm_sensor_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
#include "findme_target_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
#include "findme_locator_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
#include "proximity_reporter_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
#include "proximity_monitor_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
#include "alertNotification_Server_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
#include "alertNotification_Client_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
#include "time_server_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
#include "time_client_events.h"
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
#include "phoneAlert_Client_events.h"
#endif
/******************************************************************************
* Macro Declarations
******************************************************************************/

/******************************************************************************
* type definitions
******************************************************************************/

/******************************************************************************
* Variable Declarations
******************************************************************************/


/******************************************************************************
* Imported Variable
******************************************************************************/
extern void *profiledbgfile;

/******************************************************************************
* Function Prototypes
******************************************************************************/

/*******************************************************************************
* SAP
*******************************************************************************/

/******************************************************************************
* Local Functions
******************************************************************************/

/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event occurs when a connection is terminated.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
  profile_disconnection_complete_event(Status,
                                       Connection_Handle,
                                       Reason);
  
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  HT_Sensor_disconnection_complete_event(Status);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)  
  HR_Sensor_disconnection_complete_event(Status);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)  
  BP_Sensor_disconnection_complete_event(Status);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  GL_Sensor_disconnection_complete_event(Status);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)  
  CGM_Sensor_disconnection_complete_event(Status);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  FMT_disconnection_complete_event(Status);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  FML_disconnection_complete_event(Status);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)  
  PXR_disconnection_complete_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_disconnection_complete_event();
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)  
  ANS_disconnection_complete_event(Status);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_disconnection_complete_event();
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  TIS_disconnection_complete_event(Status);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_disconnection_complete_event(Status);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)  
  PAC_disconnection_complete_event(Status);
#endif  
}/* end hci_disconnection_complete_event() */


/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates that a new connection has been created.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
                                          uint16_t Connection_Handle,
                                          uint8_t Role,
                                          uint8_t Peer_Address_Type,
                                          uint8_t Peer_Address[6],
                                          uint16_t Conn_Interval,
                                          uint16_t Conn_Latency,
                                          uint16_t Supervision_Timeout,
                                          uint8_t Master_Clock_Accuracy)
{ 
  
  profile_le_connection_complete_event(Status,
                                       Connection_Handle,
                                       Role,
                                       Peer_Address_Type,
                                       Peer_Address,
                                       Conn_Interval,
                                       Conn_Latency, 
                                       Supervision_Timeout,
                                       Master_Clock_Accuracy);

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)  
  HT_Sensor_le_connection_complete_event(Status, Connection_Handle);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)  
  HR_Sensor_le_connection_complete_event(Status, Connection_Handle);
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)  
  BP_Sensor_le_connection_complete_event(Status, Connection_Handle);
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)  
  GL_Sensor_le_connection_complete_event(Status, Connection_Handle);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)  
  CGM_Sensor_le_connection_complete_event(Status, Connection_Handle);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  FMT_le_connection_complete_event(Status, Connection_Handle);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  FML_le_connection_complete_event(Status, Connection_Handle);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  PXR_le_connection_complete_event(Status);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_le_connection_complete_event(Status, Connection_Handle);
#endif            
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)  
  ANS_le_connection_complete_event(Status, Connection_Handle);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_le_connection_complete_event(Status, Connection_Handle);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  TIS_le_connection_complete_event(Status, Connection_Handle);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)  
  TIC_le_connection_complete_event(Status, Connection_Handle);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_le_connection_complete_event(Status, Connection_Handle);
#endif 
}/* end hci_le_connection_complete_event() */

/*******************************************************************************
 * Function Name  : aci_gap_limited_discoverable_event.
 * Description    : This event is generated by the controller when the limited 
 *                  discoverable mode ends due to timeout (180 s).
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gap_limited_discoverable_event(void)
{
  profile_gap_limited_discoverable_event();

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)  
  HT_Sensor_gap_limited_discoverable_event();
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)   
  BP_Sensor_gap_limited_discoverable_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)  
  GL_Sensor_gap_limited_discoverable_event();
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  FMT_gap_limited_discoverable_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  FML_gap_limited_discoverable_event();
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  PXR_gap_limited_discoverable_event();
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_gap_limited_discoverable_event();
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  ANS_gap_limited_discoverable_event();
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  ANC_gap_limited_discoverable_event();
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  TIS_gap_limited_discoverable_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_gap_limited_discoverable_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)  
  PAC_gap_limited_discoverable_event();
#endif 
}/* end aci_gap_limited_discoverable_event() */


/*******************************************************************************
 * Function Name  : aci_gap_pairing_complete_event.
 * Description    : This event is generated when the pairing process has completed.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */
void aci_gap_pairing_complete_event(uint16_t Connection_Handle,
                                    uint8_t Status,
                                    uint8_t Reason)
{
  profile_gap_pairing_complete_event(Connection_Handle, Status, Reason);
#else
void aci_gap_pairing_complete_event(uint16_t Connection_Handle,
                                    uint8_t Status)
{
  profile_gap_pairing_complete_event(Connection_Handle, Status);    
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)  
  HR_Sensor_gap_pairing_complete_event(Status);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)  
  PXR_Sensor_gap_pairing_complete_event(Status);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_gap_pairing_complete_event(Status);
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_gap_pairing_complete_event(Status);
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  TIS_gap_pairing_complete_event();
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_gap_pairing_complete_event(Status);
#endif 
}/* end aci_gap_pairing_complete_event() */

/*******************************************************************************
 * Function Name  : aci_gatt_write_permit_req_event.
 * Description    : This event is given to the application when a write request, 
 *                  write command or signed write command is received by the server 
 *                  from the client.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_write_permit_req_event(uint16_t Connection_Handle,
                                     uint16_t Attribute_Handle,
                                     uint8_t Data_Length,
                                     uint8_t Data[])
{
  profile_gatt_write_permit_req_event(Connection_Handle,
                                      Attribute_Handle,
                                      Data_Length,
                                      Data);

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)  
  HT_Sensor_gatt_write_permit_req_event(Attribute_Handle,
                                        Data_Length,
                                        &(Data[0]));
#endif
   
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR) 
   HR_Sensor_gatt_write_permit_req_event(Attribute_Handle,
                                         Data_Length,
                                         Data);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)   
  GL_Sensor_gatt_write_permit_req_event(Attribute_Handle,
                                        Data_Length,
                                        Data);
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)   
  CGM_Sensor_gatt_write_permit_req_event(Attribute_Handle,
                                        Data_Length,
                                        Data);
#endif   
  
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)  
  ANS_gatt_write_permit_req_event(Attribute_Handle,
                                        Data_Length,
                                        Data);
#endif   
  
}/* end aci_gatt_write_permit_req_event() */

/*******************************************************************************
 * Function Name  : aci_gatt_attribute_modified_event.
 * Description    : This event is generated to the application by the GATT server 
 *                  when a client modifies any attribute on the server.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/

#if BLE_2_X /* BLE stack FW version >= 2.x */
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
#else
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint8_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
#endif
{

  profile_gatt_attribute_modified_event(Connection_Handle,
                                        Attr_Handle,
                                        Offset,
                                        Attr_Data_Length,
                                        Attr_Data);
  
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)    
  HT_Sensor_gatt_attribute_modified_event(Attr_Handle,
                                          Attr_Data_Length,
                                          &(Attr_Data[0]));
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)  
  GL_Sensor_gatt_attribute_modified_event(Attr_Handle,
                                          Attr_Data_Length,
                                          Attr_Data);
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)  
  CGM_Sensor_gatt_attribute_modified_event(Attr_Handle,
                                           Attr_Data_Length,
                                           Attr_Data);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  FMT_gatt_attribute_modified_event(Attr_Handle,
                                    Attr_Data);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  PXR_gatt_attribute_modified_event(Attr_Handle,
                                    Attr_Data);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)  
  ANS_gatt_attribute_modified_event(Attr_Handle,
                                    Attr_Data);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  TIS_gatt_attribute_modified_event(Attr_Handle,
                                    Attr_Data);
#endif 
}/* end aci_gatt_attribute_modified_event() */


/*******************************************************************************
 * Function Name  : aci_gatt_proc_complete_event.
 * Description    : This event is generated when a GATT client procedure
                    completes either with error or successfully.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_proc_complete_event(uint16_t Connection_Handle,
                                  uint8_t Error_Code)
{
  profile_gatt_proc_complete_event(Connection_Handle, Error_Code);
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  FML_gatt_proc_complete_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)  
  PXM_gatt_proc_complete_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_gatt_proc_complete_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_gatt_proc_complete_event(Error_Code);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_gatt_proc_complete_event();
#endif  
}/* end aci_gatt_proc_complete_event() */

  
/*******************************************************************************
 * Function Name  : aci_att_read_by_group_type_resp_event.
 * Description    : This event is generated in response to a Read By Group  
 *                  Type Request.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/  
void aci_att_read_by_group_type_resp_event(uint16_t Connection_Handle,
                                           uint8_t Attribute_Data_Length,
                                           uint8_t Data_Length,
                                           uint8_t Attribute_Data_List[])
{
  profile_aci_att_read_by_group_type_resp_event(Connection_Handle,
                                                Attribute_Data_Length,
                                                Data_Length,
                                                Attribute_Data_List);
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  FML_att_read_by_group_type_resp_event(Attribute_Data_Length,
                                        Data_Length,
                                        Attribute_Data_List);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_att_read_by_group_type_resp_event(Attribute_Data_Length,
                                        Data_Length,
                                        Attribute_Data_List);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  ANC_att_read_by_group_type_resp_event(Attribute_Data_Length,
                                        Data_Length,
                                        Attribute_Data_List);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_att_read_by_group_type_resp_event(Attribute_Data_Length,
                                        Data_Length,
                                        Attribute_Data_List);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_att_read_by_group_type_resp_event(Attribute_Data_Length,
                                        Data_Length,
                                        Attribute_Data_List);
#endif  
} /* end aci_att_read_by_group_type_resp_event */


/*******************************************************************************
 * Function Name  : aci_att_read_by_group_type_resp_event.
 * Description    : This event is generated in response to a Read By Group  
 *                  Type Request.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/  
 void aci_att_read_by_type_resp_event(uint16_t Connection_Handle,
                                      uint8_t Handle_Value_Pair_Length,
                                      uint8_t Data_Length,
                                      uint8_t Handle_Value_Pair_Data[])
{
  profile_aci_att_read_by_type_resp_event(Connection_Handle,
                                          Handle_Value_Pair_Length,
                                          Data_Length,
                                          Handle_Value_Pair_Data);
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)  
  FML_att_read_by_type_resp_event(Handle_Value_Pair_Length,
                                  Data_Length,
                                  Handle_Value_Pair_Data);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_att_read_by_type_resp_event(Handle_Value_Pair_Length,
                                  Data_Length,
                                  Handle_Value_Pair_Data);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_att_read_by_type_resp_event(Handle_Value_Pair_Length,
                                  Data_Length,
                                  Handle_Value_Pair_Data);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_att_read_by_type_resp_event(Handle_Value_Pair_Length,
                                  Data_Length,
                                  Handle_Value_Pair_Data);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_att_read_by_type_resp_event(Handle_Value_Pair_Length,
                                  Data_Length,
                                  Handle_Value_Pair_Data);
#endif    
}  

/*******************************************************************************
 * Function Name  : aci_gatt_proc_timeout_event.
 * Description    : This event is generated by the client/server to the 
 *                  application on a GATT timeout (30 seconds).
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/  
void aci_gatt_proc_timeout_event(uint16_t Connection_Handle)
{
  profile_gatt_proc_timeout_event(Connection_Handle);  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_gatt_proc_timeout_event();
#endif
}

/*******************************************************************************
 * Function Name  : aci_att_exchange_mtu_resp_event.
 * Description    : This event is generated in response to an Exchange  
 *                  MTU request.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/  
void aci_att_exchange_mtu_resp_event(uint16_t Connection_Handle,
                                     uint16_t Server_RX_MTU)
{
  profile_aci_att_exchange_mtu_resp_event(Connection_Handle,
                                          Server_RX_MTU);
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_att_exchange_mtu_resp_event();
#endif
}

/*******************************************************************************
 * Function Name  : aci_att_find_info_resp_event.
 * Description    : This event is generated in response to a Find  
 *                  Information Request.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/ 
void aci_att_find_info_resp_event(uint16_t Connection_Handle,
                                  uint8_t Format,
                                  uint8_t Event_Data_Length,
                                  uint8_t Handle_UUID_Pair[])
{
  profile_aci_att_find_info_resp_event(Connection_Handle,
                                       Format,
                                       Event_Data_Length,
                                       Handle_UUID_Pair);
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_att_find_info_resp_event(Format,
                               Event_Data_Length,
                               Handle_UUID_Pair);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  ANC_att_find_info_resp_event(Format,
                               Event_Data_Length,
                               Handle_UUID_Pair);
#endif  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_att_find_info_resp_event(Format,
                               Event_Data_Length,
                               Handle_UUID_Pair);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_att_find_info_resp_event(Format,
                               Event_Data_Length,
                               Handle_UUID_Pair);
#endif
}

/*******************************************************************************
 * Function Name  : aci_att_read_resp_event.
 * Description    : This event is generated in response to a Read Request.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/ 
void aci_att_read_resp_event(uint16_t Connection_Handle,
                             uint8_t Event_Data_Length,
                             uint8_t Attribute_Value[])
{
  profile_aci_att_read_resp_event(Connection_Handle,
                                  Event_Data_Length,
                                  Attribute_Value);
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_att_read_resp_event(Event_Data_Length,
                          Attribute_Value);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_att_read_resp_event(Event_Data_Length,
                          Attribute_Value);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_att_read_resp_event(Event_Data_Length,
                          Attribute_Value);
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_att_read_resp_event(Event_Data_Length,
                          Attribute_Value);  
#endif 
}

/*******************************************************************************
 * Function Name  : aci_gatt_notification_event.
 * Description    : This event is generated when a notification is received 
 *                  from the server.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/ 
void aci_gatt_notification_event(uint16_t Connection_Handle,
                                 uint16_t Attribute_Handle,
                                 uint8_t Attribute_Value_Length,
                                 uint8_t Attribute_Value[])
{
  profile_aci_gatt_notification_event(Connection_Handle,
                                      Attribute_Handle,
                                      Attribute_Value_Length,
                                      Attribute_Value);
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_gatt_notification_event(Attribute_Handle,
                              Attribute_Value_Length,
                              Attribute_Value);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)  
  ANC_gatt_notification_event(Attribute_Handle,
                              Attribute_Value_Length,
                              Attribute_Value);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_gatt_notification_event(Attribute_Handle,
                              Attribute_Value_Length,
                              Attribute_Value);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  PAC_gatt_notification_event(Attribute_Handle,
                              Attribute_Value_Length,
                              Attribute_Value);
#endif  
}

/*******************************************************************************
 * Function Name  : aci_gatt_error_resp_event.
 * Description    : This event is generated when when an Error Response is 
 *                  received from the server.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/ 
void aci_gatt_error_resp_event(uint16_t Connection_Handle,
                               uint8_t Req_Opcode,
                               uint16_t Attribute_Handle,
                               uint8_t Error_Code)
{
  profile_aci_gatt_error_resp_event(Connection_Handle,
                                    Req_Opcode,
                                    Attribute_Handle,
                                    Error_Code);
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  PXM_gatt_error_resp_event();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  TIC_gatt_error_resp_event();
#endif
}

/*******************************************************************************
 * Function Name  : aci_gatt_error_resp_event.
 * Description    : This event is generated when when an Error Response is 
 *                  received from the server.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/ 
void aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                    uint16_t Attribute_Handle,
                                    uint16_t Offset)
{
  profile_aci_gatt_read_permit_req_event(Connection_Handle,
                                         Attribute_Handle,
                                         Offset);
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  TIS_gatt_read_permit_req_event(Attribute_Handle);
#endif
}  

void aci_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                      uint16_t Available_Buffers)
{       
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)  
  CGM_gatt_tx_pool_available_event(Connection_Handle, Available_Buffers);
#endif
} 
