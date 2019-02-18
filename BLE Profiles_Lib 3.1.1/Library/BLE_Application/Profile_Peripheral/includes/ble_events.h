/**
 * @file    ble_events.c
 * @author  AMG RF application team
 * @version V1.0.1
 * @date    October 26, 2016
 * @brief   This file provides all BlueNRG-1 Profiles Peripheral specific events.
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */
#ifndef _BLE_EVENTS_H_
#define _BLE_EVENTS_H_

/* Application events from ble main profile. 
   The error codes for the profile specific events are defined in the profile specific header files
*/

/** 
 *  This event is sent to the application by the main profile
 *  when the controller has been initialized.
 */
#define EVT_MP_BLUE_INITIALIZED           		(0x01)

/** 
 *  This event is sent to the application by the main
 *  profile when a connection has been successfully established with the peer.
 */
#define EVT_MP_CONNECTION_COMPLETE                      (0x02)

/** 
 *  This even is sent to the application by the main profile
 *  when there is a request for passkey during the pairing process from the controller. This
 *  event has no parameters. The application must call the function
 *  BLE_Profile_Send_Pass_Key() and send the passkey to the controller.
 */
#define EVT_MP_PASSKEY_REQUEST				(0x03)

/** 
 *  This event is sent to the application by the main
 *  profile when the device is successfully paired with the peer.
 */
#define EVT_MP_PAIRING_COMPLETE				(0x04)

/** 
 *  This event is sent to the application by the main
 *  profile to notify the result of a disconnection procedure initiated by master/slave.
 */
#define EVT_MP_DISCONNECTION_COMPLETE			(0x05)

/** 
 *  This event is sent by any of the child profiles when
 *  enabling of advertising fails. It is the application's responsibility to restart advertising.
 */
#define EVT_MP_ADVERTISE_ERROR				(0x06)

/** 
 *  This event is sent by the child profiles when the
 *  limited discoverable mode times out or the profile-specific advertising timeout occurs. It
 *  is the application's responsibility to restart advertising.
 */
#define EVT_MP_ADVERTISING_TIMEOUT			(0x07)

/* events to the application from heart rate sensor(HRS)
 */ 
#define HRS_EVT_BASE						(0x08)

/** 
 *  This event is sent to the application when the heart rate profile
 *  has completed its initialization sequence and is ready to enable advertising or the
 *  initialization sequence failed. The evtData parameter contains the error code; 0X00
 *  means the initialization was successful.
 */
#define EVT_HRS_INITIALIZED					(HRS_EVT_BASE)
/** 
 *  This event is sent to the application whenever it
 *  has started a characteristic update procedure to update the heart rate measurement or
 *  body sensor location. The evtData contains the status, service handle, and
 *  characteristic handle. This has to be changed to give different events for each update
 *  since the application is not aware of the handles.
 */

#define EVT_HRS_CHAR_UPDATE_CMPLT			        (HRS_EVT_BASE+1) /* error code indicating success or failure */

#define EVT_HRS_MANUFAC_STRING_UPD_CMPLT		        (HRS_EVT_BASE+2)


/** 
 *  This event is sent to the application when
 *  the peer writes a value of 0x01 to the control point characteristic. This event has no
 *  parameters. The application must restart accumulating the energy expended values
 *  from 0.
 */
#define EVT_HRS_RESET_ENERGY_EXPENDED			        (HRS_EVT_BASE+3)

/* events to the application from Proximity Reporter(PR)
 */ 
#define PR_EVT_BASE						(HRS_EVT_BASE+4)

/** 
 *  This  event is sent to the application when the proximity reporter
 *  has completed its initialization sequence and is ready to enable advertising, or the
 *  initialization sequence failed. The evtData parameter contains the error code; 0X00
 *  means the initialization was successful.
 */
#define EVT_PR_INITIALIZED					(PR_EVT_BASE)

/** 
 *  This event is sent to the application when a link loss is
 *  detected. The evtData contains the alert level. The application must start an alert for
 *  the level specified. The type of alert is decided by the application: the alert can continue
 *  for an application-specific duration or until another connection is established. The
 *  application must re-enable advertising to establish a new connection.
 */
#define EVT_PR_LINK_LOSS_ALERT					(PR_EVT_BASE+1) /* different alert values */

/** 
 *  This  event is sent to the application by the proximity
 *  reporter when a path loss is detected. The evtData contains the alert level. When a
 *  path loss is detected, an alert of any type must be started – the desired user action
 *  would be to move the device closer to its connected peer. The alert should continue
 *  until another event with alert level 0 is issued.
 */
#define EVT_PR_PATH_LOSS_ALERT					(PR_EVT_BASE+2) /* different alert values */

/* events to the application from Proximity Monitor(PM)
 */ 
#define PM_EVT_BASE						(PR_EVT_BASE+3)

/** 
 *  This event is sent by the proximity monitor to the application
 *  when the initialization sequence is completed and the device is ready to start
 *  advertising.
 */
#define EVT_PM_INITIALIZED					(PM_EVT_BASE)

/** 
 *  This  event is sent by the proximity monitor after a
 *  connection is established. The evtData contains the error code.
 *  – 0x00: all the mandatory services, characteristics and descriptors as specified in the
 *  profile specification were discovered successfully.
 *  – 0x01: link loss service not found.
 */
#define EVT_PM_DISCOVERY_CMPLT				        (PM_EVT_BASE+1)

/** 
 *  This  event is sent to the application when a link loss is
 *  detected. The evtData contains the alert level. The application must start an alert for the
 *  level specified. The type of alert is decided by the application: the alert can continue for
 *  an application-specific duration or until another connection is established. The
 *  application must re-enable advertising to establish a new connection.
 */
#define EVT_PM_LINK_LOSS_ALERT					(PM_EVT_BASE+2) /* different alert values */

/** 
 *  This event is sent to the application by the proximity
 *  monitor when a path loss is detected. The evtData contains the alert level. When a path
 *  loss is detected, the application can start an alert of any type for the alert level specified.
 */
#define EVT_PM_PATH_LOSS_ALERT					(PM_EVT_BASE+3) /* different alert values */

/* events to the application from Time Server(TS)
 */ 
#define TS_EVT_BASE						(PM_EVT_BASE+4)

/** 
 *  This event is sent by the time server to the application when the
 *  initialization sequence has completed and the device is ready to start advertising.
 */
#define EVT_TS_INITIALIZED					(TS_EVT_BASE)

/** 
 *  This event is sent to the application when an
 *  update previously started by the application completes. The status indicates whether
 *  the update was successful or it failed. The evtData also contains the service handle
 *  and the characteristic handle.
 */
#define EVT_TS_CHAR_UPDATE_CMPLT				(TS_EVT_BASE+1)

/** 
 *  This  event is sent to the application when the
 *  GET_REFERENCE_UPDATE(0x01) command is written to the updateState
 *  characteristic by the time client.
 */
#define EVT_TS_START_REFTIME_UPDATE				(TS_EVT_BASE+2)

/** 
 *  This  event is sent to the application when the
 *  CANCEL_REFERENCE_UPDATE(0x02) command is written to the updateState
 *  characteristic by the time client.
 */
#define EVT_TS_STOP_REFTIME_UPDATE				(TS_EVT_BASE+3)

#define EVT_TS_CURTIME_READ_REQ					(TS_EVT_BASE+4)

/* events to the application from Time Client(TC)
 */ 
#define TC_EVT_BASE					(TS_EVT_BASE+5)


#define EVT_TC_REQUIREMENTS_NOT_MET			(TC_EVT_BASE) /* error codes CT_CHARACTERISTIC_NOT_FOUND, CT_SERVICE_NOT_FOUND */

/** 
 *  This event is sent by the time client after a connection
 *  is established and all the mandatory services, characteristics and descriptors as
 *  specified in the profile specification were discovered successfully.
 */
#define EVT_TC_DISCOVERY_CMPLT				(TC_EVT_BASE+1)

/** 
 *  This event is sent to the application when a
 *  notification for the current time characteristic is received by the time client. The event
 *  data contains all the fields of the current time characteristic.
 */
#define EVT_TC_CUR_TIME_VAL_RECEIVED			(TC_EVT_BASE+2)

/** 
 *  This event is sent by the time client to the application when the
 *  initialization sequence is completed and the device is ready to start advertising.
 */
#define EVT_TC_INITIALIZED				(TC_EVT_BASE+3)

/** 
 *  The application can read the current time
 *  characteristic on the server by using the function TimeClient_Get_Current_Time(). The
 *  response to this function call is returned in this event. The evtData contains the
 *  response received from the server.
 */
#define EVT_TC_READ_CUR_TIME_CHAR			(TC_EVT_BASE+4)

/** 
 *  The application can read the reference
 *  time characteristic on the server by using the function
 *  TimeClient_Get_Time_Accuracy_Info_Of_Server(). The response to this function call
 *  is returned in this event. The evtData contains the response received from the server.
 */
#define EVT_TC_READ_REF_TIME_INFO_CHAR			(TC_EVT_BASE+5)

/** 
 *  The application can read the local time
 *  information characteristic on the server by using the function
 *  TimeClient_Get_Local_Time_Information(). The response to this function call is
 *  returned in this event. The evtData contains the response received from the server.
 */
#define EVT_TC_READ_LOCAL_TIME_INFO_CHAR	        (TC_EVT_BASE+6)

/** 
 *  The application can read the time with dst
 *  change characteristic on the server by using the function
 *  TimeClient_Get_Next_DST_Change_Time(). The response to this function call is
 *  returned in this event. The evtData contains the response received from the server.
 */
#define EVT_TC_READ_TIME_WITH_DST_CHAR			(TC_EVT_BASE+7)

/** 
 *  The application can read the time
 *  update state characteristic on the server by using the function
 *  TimeClient_Get_Server_Time_Update_State(). The response to this function call is
 *  returned in this event. The evtData contains the response received from the server.
 */
#define EVT_TC_READ_TIME_UPDATE_STATE_CHAR		(TC_EVT_BASE+8)


/* events to the application related to Device Information service(DIS)
 */ 
#define DIS_EVT_BASE					  (TC_EVT_BASE+9)

#define EVT_DIS_SYSTEM_ID_UPDATE_CMPLT                    (DIS_EVT_BASE)
#define EVT_DIS_MODEL_NUM_UPDATE_CMPLT                    (DIS_EVT_BASE+1)
#define EVT_DIS_SERIAL_NUM_UPDATE_CMPLT                   (DIS_EVT_BASE+2)
#define EVT_DIS_FIRMWARE_STRING_UPDATE_CMPLT              (DIS_EVT_BASE+3)
#define EVT_DIS_HARDWARE_STRING_UPDATE_CMPLT              (DIS_EVT_BASE+4)
#define EVT_DIS_SOFTWARE_STRING_UPDATE_CMPLT              (DIS_EVT_BASE+5)
#define EVT_DIS_MANUFACTURER_NAME_STRING_UPDATE_CMPLT     (DIS_EVT_BASE+6)
#define EVT_DIS_IEEE_CERTIFICATION_UPDATE_CMPLT           (DIS_EVT_BASE+7)
#define EVT_DIS_PNP_ID_UPDATE_CMPLT                       (DIS_EVT_BASE+8)


/* events to the application from Find Me target profile(FMT)
 */ 
#define FMT_EVT_BASE				(DIS_EVT_BASE+9)

/** 
 *  This event is sent to the application when the find me target
 *  has completed its initialization sequence and is ready to enable advertising, or the
 *  initialization sequence failed. The evtData parameter contains the error code; 0X00
 *  means the initialization was successful.
 */
#define EVT_FMT_INITIALIZED			(FMT_EVT_BASE)

/** 
 *  This event is sent to the application when the client writes to the alert
 *  level characteristic with a valid alert level. The application must start alerting if the alert
 *  level is 0x01 or 0x02, and stop when the alert level is 0x00.
 */
#define EVT_FMT_ALERT				(FMT_EVT_BASE+1)

/* events to the application from Find Me Locator Profile(FML)
 */ 
#define FML_EVT_BASE			(FMT_EVT_BASE+2)

/** 
 *  This event is sent by the find me locator to the application
 *  when the initialization sequence is completed and the device is ready to start
 *  advertising.
 */
#define EVT_FML_INITIALIZED		(FML_EVT_BASE)

/** 
 *  This  event is sent by the find me locator after a
 *  connection is established. The evtData contains the error code:
 *  - 0x00: all the mandatory services, characteristics and descriptors as specified in the profile specification were discovered successfully.
 *  - 0x01: alert characteristic not found.
 *  - 0x02: immediate alert service not found.
 */
#define EVT_FML_DISCOVERY_CMPLT	        (FML_EVT_BASE+1) /* error codes ALERT_CHARACTERISTIC_NOT_FOUND, IMM_ALERT_SERVICE_NOT_FOUND, BLE_STATUS_SUCCESS */

/* events to the application from Phone Alert Status Client
 */ 
#define PAC_EVT_BASE			(FML_EVT_BASE+2)

/** 
 *  This  event is sent by the phone alert client to the application
 *  when the initialization sequence is completed and the device is ready to start
 *  advertising.
 */
#define EVT_PAC_INITIALIZED		(PAC_EVT_BASE)

/** 
 *  This event is sent by the phone alert client after a
 *  connection is established. The evtData contains the error code:
 *  0x00: all the mandatory services, characteristics and descriptors as specified in the
 *  profile specification were discovered successfully.
 *  0x01: PHONE_ALERT_SERVICE_NOT_FOUND.
 *  0x02: PHONE_ALERT_STATUS_CHARAC_NOT_FOUND.
 *  0x03: RINGER_CNTRL_POINT_CHARAC_NOT_FOUND.
 *  0x04: RINGER_SETTING_CHARAC_NOT_FOUND.
 *  0x05: PHONE_ALERT_STATUS_DESC_NOT_FOUND.
 *  0x06: RINGER_CNTRL_POINT_DESC_NOT_FOUND.
 *  0x07: RINGER_SETTING_DESC_NOT_FOUND.
 */
#define EVT_PAC_DISCOVERY_CMPLT	        (PAC_EVT_BASE+1)

/** 
 *  The application can start a procedure to read the alert
 *  status characteristic on the peer server using the function PAC_Read_AlertStatus().
 *  The response to this function call is returned in this event. The evtData contains the
 *  response received from the server.
 */ 
#define EVT_PAC_ALERT_STATUS			(PAC_EVT_BASE+2)

/** 
 *  The application can read the ringer setting on the server
 *  by using the function PAC_Read_RingerSetting(). The response to this function call is
 *  returned in this event. The evtData contains the response received from the server.
 */
#define EVT_PAC_RINGER_SETTING			(PAC_EVT_BASE+3)

/* events to the application from blood pressure profile
 */
#define BPS_EVT_BASE			(PAC_EVT_BASE+4)

/** 
 *  This event is sent by the blood pressure sensor to the
 *  application when the initialization sequence is completed and the device is ready to
 *  start advertising.
 */
#define EVT_BPS_INITIALIZED		(BPS_EVT_BASE)

/** 
 *  This event is sent to the application when an
 *  update to the blood pressure measurement characteristic previously started by the
 *  application completes. The status indicates whether the update was successful or not.
 */
#define EVT_BPS_BPM_CHAR_UPDATE_CMPLT 	(BPS_EVT_BASE+1)

/** 
 *  This event is sent to the application when an
 *  update to the intermediate cuff pressure characteristic previously started by the
 *  application completes. The status indicates whether the update was successful or not.
 */
#define EVT_BPS_ICP_CHAR_UPDATE_CMPLT 	(BPS_EVT_BASE+2)

/** 
 *  This event is sent to the application when there is no measurements to be sent to the collector for more than five seconds.
 */
#define EVT_BPS_IDLE_CONNECTION_TIMEOUT (BPS_EVT_BASE+3)

/* events to the application from the health thermometer profile
 */
#define HT_EVT_BASE					(BPS_EVT_BASE+4)

/** 
 *  This event is sent by the thermometer to the application when
 *  the initialization sequence is completed and the device is ready to start advertising.
 */
#define EVT_HT_INITIALIZED				(HT_EVT_BASE)

/** 
 *  This event is sent to the application when an update to the temperature measurement characteristic previously
 *  started by the application completes. The status indicates whether the update was
 *  successful or it failed.
 */
#define EVT_HT_TEMPERATURE_CHAR_UPDATE_CMPLT 		(HT_EVT_BASE+1)

/** 
 *  This event is sent to the application when an update to the intermediate temperature measurement
 *  characteristic previously started by the application completes. The status indicates
 *  whether the update was successful or it failed.
 */
#define EVT_HT_INTERMEDIATE_TEMP_CHAR_UPDATE_CMPLT      (HT_EVT_BASE+2)


#define EVT_HT_TEMP_TYPE_CHAR_UPDATE_CMPLT 		(HT_EVT_BASE+3)

/** 
 *  This  event is sent to the application when an update to the measurement interval characteristic previously
 *  started by the application completes. The status indicates whether the update was
 *  successful or it failed.
 */
#define EVT_HT_MEASUREMENT_INTERVAL_UPDATE_CMPLT	(HT_EVT_BASE+4)

/** 
 *  This  event is sent to the application when the collector writes to the measurement interval characteristic.
 */
#define EVT_HT_MEASUREMENT_INTERVAL_RECEIVED		(HT_EVT_BASE+5)

/** 
 *  This event is sent to the application when there are no measurements to be sent to the collector for more than five seconds.
 */
#define EVT_HT_IDLE_CONNECTION_TIMEOUT 			(HT_EVT_BASE+6)

/* events to the application from the alert notification profile - client
 */ 
#define ANC_EVT_BASE					(HT_EVT_BASE+7)

/** 
 *  This event is sent by the alert notification client to the
 *  application when the initialization sequence has completed and the device is ready to
 *  start advertising.
 */
#define EVT_ANC_INITIALIZED				(ANC_EVT_BASE)

/** 
 *  This event is sent by the alert notification client after
 *  a connection is established and all the mandatory services, characteristics and
 *  descriptors as specified in the profile specification were discovered successfully.
 */
#define EVT_ANC_DISCOVERY_CMPLT				(ANC_EVT_BASE+1) /* with error codes */

/** 
 *  This event is sent to the application when a
 *  notification for the new alert is received by the alert notification client.
 */
#define EVT_ANC_NEW_ALERT_RECEIVED			(ANC_EVT_BASE+2)

/** 
 *  This  event is sent to the application when a notification for an unread alert is received by the alert notification
 *  client.
 */
#define EVT_ANC_UNREAD_ALERT_STATUS_RECEIVED		(ANC_EVT_BASE+3)

/* events to the application from the alert notification profile - server
 */ 
#define ANS_EVT_BASE					(ANC_EVT_BASE+4)

/** 
 *  This event is sent by the alert notification server to the
 *  application when the initialization sequence has completed and the device is ready to
 *  start advertising.
 */
#define EVT_ANS_INITIALIZED				(ANS_EVT_BASE)


/* events to the application from the glucose profile.
 */
#define GL_EVT_BASE					(ANS_EVT_BASE+1)

/** 
 *  This event is sent by the glucose sensor to the application when
 *  the initialization sequence has completed and the device is ready to start advertising.
 */
#define EVT_GL_INITIALIZED			        (GL_EVT_BASE)

/** 
 *  This event is sent to the application when the connection is idle for more than five seconds.
 */
#define EVT_GL_IDLE_CONNECTION_TIMEOUT 			(GL_EVT_BASE+1)


/* events to the application from the cgm profile.
 */
#define CGM_EVT_BASE					(GL_EVT_BASE+2)

/** 
 *  This event is sent by the glucose sensor to the application when
 *  the initialization sequence has completed and the device is ready to start advertising.
 */
#define EVT_CGM_INITIALIZED			        (CGM_EVT_BASE)

/** 
 *  This event is sent to the application when the connection is idle for more than five seconds.
 */
#define EVT_CGM_IDLE_CONNECTION_TIMEOUT 			(CGM_EVT_BASE+1)

#endif
