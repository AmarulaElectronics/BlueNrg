/*******************************************************************************
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
*   FILENAME        -  profile_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      06/08/2018
*   $Revision$:  1.0.0
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application using the BlueNRG-1,2 Profiles 
*                Library 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
* 
*******************************************************************************/

/*******************************************************************************
 * Include Files
*******************************************************************************/

#include "stdio.h"
#include "ble_const.h" 
#include "bluenrg1_api.h"
#include "BlueNRG1_stack.h" 
#include "host_config.h"
#include "Profile_config.h"
#include "ble_profile.h"
#include "ble_events.h"
#include "debug.h"
#include "sleep.h"

#include "SDK_EVAL_Config.h"

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
#include <alertNotification_Client.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
#include <alertNotification_Server.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
#include <blood_pressure.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
#include <findme_locator.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
#include <findme_target.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
#include <glucose_service.h>
#include <glucose_sensor.h>
#include <glucose_racp.h>
/* Include record measurement database record */
#include <glucose_database.h>
#endif 

#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
#include <cgm_service.h>
#include <cgm_sensor.h>
#include <cgm_racp.h>
/* Include record measurement database record */
#include <cgm_database.h>
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
#include <health_thermometer.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
#include <heart_rate.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
#include <phoneAlert_client.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
#include <proximity_monitor.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
#include <proximity_reporter.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
#include <time_client.h>
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
#include <time_server.h>
#include <time_profile_types.h>
#endif

/*******************************************************************************
 * Macros
*******************************************************************************/

/** 
 *  Peripheral public address: it is the address to be set on PTS SW tool
 *  on IUT Device Address field (i.e. 0280E1000223)
 */
#define PERIPHERAL_PUBLIC_ADDRESS {0x23, 0x02, 0x00, 0xE1, 0x80, 0x02}

/**
 *  IO capability: display only
 */
#define SPI_HOST_MAIN_IO_CAPABILITY_DISPLAY_ONLY              (0x00)
/**
 *  IO capability: display yes/no
 */
#define SPI_HOST_MAIN_IO_CAPABILITY_DISPLAY_YES_NO            (0x01)
#define SPI_HOST_MAIN_IO_CAPABILITY_KEYBOARD_ONLY             (0x02)
#define SPI_HOST_MAIN_IO_CAPABILITY_NO_INPUT_NO_OUTPUT        (0x03)
#define SPI_HOST_MAIN_IO_CAPABILITY_KEYBOARD_DISPLAY          (0x04)

/**
 *  MITM mode not required
 */
#define MITM_PROTECTION_NOT_REQUIRED	(0x00)
/**
 *  MITM mode required
 */
#define MITM_PROTECTION_REQUIRED        (0x01)

/** 
 * PTS test application states 
 */
#define APPL_UNINITIALIZED		(0x00)
#define APPL_INIT_DONE			(0x01) /* it moves to APPL_ADVERTISING */
#define APPL_ADVERTISING		(0x02)
#define APPL_CONNECTED			(0x03)
#define APPL_READ			(0x04)
#define APPL_WAIT			(0x05)
#define APPL_START_TIMER	        (0x06)
#define APPL_UPDATE			(0x07)
#define APPL_ADVERTISING_TIMEOUT        (0x08)  

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
///@cond BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT
/**
 *@defgroup Alert_Notification_Client
 *@brief PTS test application user input commands for Alert Notification Client Peripheral role
 *@{
 */
/** 
 *  Alert Notification Client user input command: enable new alert characteristic notification (<b>ANC_Enable_Disable_New_Alert_Notification() </b> API)
 */
#define APPL_ENABLED_NEW_ALERT_NOTIF        	'a'

/** 
 *  Alert Notification Client user input command: disable new alert characteristic notification (<b>ANC_Enable_Disable_New_Alert_Notification() </b> API)
 */
#define APPL_DISABLED_NEW_ALERT_NOTIF        	'b'

/** 
 *  Alert Notification Client user input command: enable unread alert status characteristic notification (<b>ANC_Enable_Disable_Unread_Alert_Status_Notification() </b> API)
 */
#define APPL_ENABLED_UNREAD_ALERT_STATUS_NOTIF   'c' 

/** 
 *  Alert Notification Client user input command: disable unread alert status characteristic notification (<b>ANC_Enable_Disable_Unread_Alert_Status_Notification() </b> API)
 */
#define APPL_DISABLED_UNREAD_ALERT_STATUS_NOTIF   'd' 

/** 
 *  Alert Notification Client user input command: write control point  characteristic (<b>ANC_Write_Control_Point() </b>API)
 */
#define APPL_WRITE_CTRL_POINT 			  'e'

/** 
 *  Alert Notification Client user input command: write control point  characteristic for test ANP/CL/ANPSF/BV-01-I (<b>ANC_Write_Control_Point() </b>API)
 */
#define APPL_WRITE_CTRL_POINT_ANPSF_1 		  'f'

/** 
 *  Alert Notification Client user input command: write control point  characteristic for test ANP/CL/ANPSF/BV-02-I (<b>ANC_Write_Control_Point() </b>API)
 */
#define APPL_WRITE_CTRL_POINT_ANPSF_2 		  'g'


#define APPL_DISCOVERY_PRIMARY_SERVICE            'h'

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
///@cond BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER
/**
 *@defgroup Alert_Notification_Server
 *@brief PTS test application user input commands for Alert Notification Server Peripheral role
 *@{
 */

/** 
 *  Alert Notification Server user input command: update new alert characteristic (<b>ANS_Update_New_Alert() </b> API)
 */
#define APPL_UPDATE_NEW_ALERT 			'a' 

/** 
 *  Alert Notification Server user input command: update unread alert status characteristic (<b>ANS_Update_Unread_Alert_Status() </b> API)
 */
#define APPL_UPDATE_UNREAD_ALERT_STATUS 	'b' 

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
///@cond BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR
/**
 *@defgroup Blood_Pressure_Sensor
 *@brief PTS test application user input commands for Blood Pressure Sensor Peripheral role
 *@{
 */
/** 
 *  Blood Pressure Sensor user input command: update the blood pressure measurement characteristic (<b>BPS_Send_Blood_Pressure_Measurement() </b> API)
 */
#define APPL_SEND_BP_MEASUREMENT                'a'  

/** 
 *  Blood Pressure Sensor user input command: send the intermediate cuff pressure values during the measurement process until a stable value is obtained (<b>BPS_Send_Intermediate_Cuff_Pressure () </b> API)
 */
#define APPL_SEND_INTERMEDIATE_CUFF_PRESSURE    'b'   

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
///@cond BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR
/**
 *@defgroup Find_Me_Locator
 *@brief PTS test application user input commands for Find Me Locator Peripheral role
 *@{
 */
/** 
 *  Find Me Locator user input command: start a write without response GATT procedure to write the alert value (no alert) to the find me target (<b>FML_ALert_Target(0) </b> API)
 */
#define APPL_LOCATOR_NO_ALERT_TARGET		'a' 

/** 
 *  Find Me Locator user input command: start a write without response GATT procedure to write the alert value to the find me target (<b>FML_ALert_Target(1) </b> API)
 */
#define APPL_LOCATOR_ALERT_TARGET		'b' 
   
#define APPL_DISCOVERY_PRIMARY_SERVICE          'c'   

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
///@cond BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR
/**
 *@defgroup Glucose_Sensor
 *@brief PTS test application user input commands for Glucose Sensor Peripheral role
 *@{
 */
/** 
 *  Glucose Sensor user input command: change database [store new sequence number and reset flags field on current database measurement for simulating new records measurements; <b>glucoseDatabase_Change_Database_Manually(1, SET_FLAG) </b> API]
 */
#define APPL_GLUCOSE_CHANGE_DB_1	           'a' 

/** 
 *  Glucose Sensor user input command: change database [store new sequence number and reset flags field on current database measurement for simulating new records measurements; <b>glucoseDatabase_Change_Database_Manually(3, UNSET_FLAG) </b> API]
 */
#define APPL_GLUCOSE_CHANGE_DB_2	           'b' 

/** 
 *  Glucose Sensor user input command: set  GLUCOSE_PTS_NUMBER_OF_RECORDS to 1 (<b>SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1 </b> )
 */
#define APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1   'c'

/** 
 *  Glucose Sensor user input command: set  GLUCOSE_PTS_NUMBER_OF_RECORDS to 3 (<b>SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3 </b> )
 */
#define APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3   'd'

/** 
 *  Glucose Sensor user input command: set  GLUCOSE_PTS_NUMBER_OF_RECORDS to 4 (<b>SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4 </b> )
 */
#define APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4   'e'

/** 
 *  Glucose Sensor user input command: set  GLUCOSE_PTS_NUMBER_OF_RECORDS to 5 (<b>SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5 </b> )
 */
#define APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5   'f'

/** 
 *  Glucose Sensor user input command: set  GLUCOSE_PTS_NUMBER_OF_RECORDS to 9 (<b>SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9 </b> )
 */
#define APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9   'g'

/** 
 *  Glucose Sensor user input command: set  GLUCOSE_PTS_NUMBER_OF_RECORDS to 200 (<b>SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200 </b> )
 */
#define APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200 'h'

/** 
 *  Glucose Sensor user input command: set  Medication Units to KG (<b>APPL_SET_MEDICATION_UNITS_KG </b> )
 */
#define APPL_SET_MEDICATION_UNITS_KG                'i'
   
/** 
 *  Glucose Sensor user input command: set  Medication Units to Liter (<b>APPL_SET_MEDICATION_UNITS_LITER </b> )
 */
#define APPL_SET_MEDICATION_UNITS_LITER             'l'  

/** 
 *  Glucose Sensor user input command: set  Multiple Bond feature (<b>APPL_SET_MULTIPLE_BOND </b> )
 */
#define APPL_SET_MULTIPLE_BOND                      'n'    

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
///@cond BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR
/**
 *@defgroup Cgm_Sensor
 *@brief PTS test application user input commands for CGM Sensor Peripheral role
 *@{
 */
/** 
 *  CGM Sensor user input command: set  CGM_PTS_NUMBER_OF_RECORDS to 3 (<b>SET_CGM_PTS_NUMBER_OF_RECORDS_3 </b> )
 */
#define APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_3   'a'

/** 
 *  CGM Sensor user input command: set  CGM_PTS_NUMBER_OF_RECORDS to 5 (<b>SET_CGM_PTS_NUMBER_OF_RECORDS_5 </b> )
 */
#define APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_5   'b'

/** 
 *  CGM Sensor user input command: set  CGM_PTS_NUMBER_OF_RECORDS to 10 (<b>SET_CGM_PTS_NUMBER_OF_RECORDS_10 </b> )
 */
#define APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_10   'c'

/** 
 *  CGM Sensor user input command: set  CGM_PTS_NUMBER_OF_RECORDS to 20 (<b>SET_CGM_PTS_NUMBER_OF_RECORDS_20 </b> )
 */
#define APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_20   'd'

/** 
 *  CGM Sensor user input command: set  bStartSession to FALSE (<b>APPL_SET_NO_START_SESSION </b> )
 */
#define APPL_SET_NO_START_SESSION               'e'

/** 
 *  CGM Sensor user input command: set  bStartSession to FALSE (<b>APPL_SET_START_SESSION </b> )
 */
#define APPL_SET_START_SESSION                  'f'
   
/** 
 *  CGM Sensor user input command: set  bMultipleSession to FALSE (<b>APPL_SET_NO_MULTIPLE_SESSION </b> )
 */
#define APPL_SET_NO_MULTIPLE_SESSION            'g'   
   
/** 
 *  CGM Sensor user input command: set  bMultipleSession to TRUE (<b>APPL_SET_MULTIPLE_SESSION </b> )
 */
#define APPL_SET_MULTIPLE_SESSION               'h'   

/** 
 *  CGM Sensor user input command: set  bE2E_CRC_Support to FALSE (<b>APPL_SET_NO_E2E_CRC_SUPPORT </b> )
 */
#define APPL_SET_NO_E2E_CRC_SUPPORT             'i'   
   
/** 
 *  CGM Sensor user input command: set  bE2E_CRC_Support to TRUE (<b>APPL_SET_E2E_CRC_SUPPORT </b> )
 */
#define APPL_SET_E2E_CRC_SUPPORT                'l'  

/** 
 *  CGM Sensor user input command: set bPatientHighLowAlert_Support to FALSE (<b>APPL_SET_NO_PATIENT_ALERT_SUPPORT </b> )
 */
#define APPL_SET_NO_PATIENT_ALERT_SUPPORT       'n'   
   
/** 
 *  CGM Sensor user input command: set  bPatientHighLowAlert_Support to TRUE (<b>APPL_SET_PATIENT_ALERT_SUPPORT </b> )
 */
#define APPL_SET_PATIENT_ALERT_SUPPORT          'o' 
   
/** 
 *  CGM Sensor user input command: set bHypoAlert_Support to FALSE (<b>APPL_SET_NO_HYPO_ALERT_SUPPORT </b> )
 */
#define APPL_SET_NO_HYPO_ALERT_SUPPORT          'p'   
   
/** 
 *  CGM Sensor user input command: set bHypoAlert_Support to TRUE (<b>APPL_SET_HYPO_ALERT_SUPPORT </b> )
 */
#define APPL_SET_HYPO_ALERT_SUPPORT             'q'   

/** 
 *  CGM Sensor user input command: set bHyperAlert_Support to FALSE (<b>APPL_SET_NO_HYPER_ALERT_SUPPORT </b> )
 */
#define APPL_SET_NO_HYPER_ALERT_SUPPORT         'A'   
   
/** 
 *  CGM Sensor user input command: set bHyperAlert_Support to TRUE (<b>APPL_SET_HYPER_ALERT_SUPPORT </b> )
 */
#define APPL_SET_HYPER_ALERT_SUPPORT            'B'   

/** 
 *  CGM Sensor user input command: set bRateIncreaseDecreaseAlert_Support to FALSE (<b>APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT </b> )
 */
#define APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT 'C'   
   
/** 
 *  CGM Sensor user input command: set bRateIncreaseDecreaseAlert_Support to TRUE (<b>APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT </b> )
 */
#define APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT    'D'

/** 
 *  CGM Sensor user input command: set bDeviceSpecificAlert_Support to FALSE (<b>APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT </b> )
 */
#define APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT 'E'   
   
/** 
 *  CGM Sensor user input command: set bDeviceSpecificAlert_Support to TRUE (<b>APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT </b> )
 */
#define APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT    'F'

/** 
 *  CGM Sensor user input command: set bCalibration_Support to FALSE (<b>APPL_SET_NO_CALIBRATION_SUPPORT </b> )
 */
#define APPL_SET_NO_CALIBRATION_SUPPORT           'G'   
   
/** 
 *  CGM Sensor user input command: set bCalibration_Support to TRUE (<b>APPL_SET_CALIBRATION_SUPPORT </b> )
 */
#define APPL_SET_CALIBRATION_SUPPORT              'H'  

/** 
 *  CGM Sensor user input command: set bAllFeatureSupport_Support to FALSE (<b>APPL_SET_NO_ALL_FEATURE_SUPPORT </b> )
 */
#define APPL_SET_NO_ALL_FEATURE_SUPPORT           'I'   
   
/** 
 *  CGM Sensor user input command: set bAllFeatureSupport_Support to TRUE (<b>APPL_SET_ALL_FEATURE_SUPPORT </b> )
 */
#define APPL_SET_ALL_FEATURE_SUPPORT              'L'
 
/** 
 *  CGM Sensor user input command: set bMultipleSession to FALSE (<b>APPL_SET_NO_CGM_MULTIPLE_BOND </b> )
 */
#define APPL_SET_NO_CGM_MULTIPLE_BOND             'M'   
   
/** 
 *  CGM Sensor user input command: set bMultipleSession to TRUE (<b>APPL_SET_CGM_MULTIPLE_BOND </b> )
 */
#define APPL_SET_CGM_MULTIPLE_BOND                'N'
/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
///@cond BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER
/**
 *@defgroup Health_Thermometer 
 *@brief PTS test application user input commands for Health Thermometer Peripheral role
 *@{
 */
/** 
 *  Health Thermometer user input command: update the temperature measurement characteristic (<b>HT_Send_Temperature_Measurement() </b> API)
 */
#define APPL_SEND_HT_MEASUREMENT                'a' 
/** 
 *  Health Thermometer user input command: update the intermediate temperature measurement characteristic (<b>HT_Send_Intermediate_Temperature() </b> API)
 */
#define APPL_SEND_HT_INTERMEDIATE_MEASUREMENT   'b' 
/** 
 *  Health Thermometer user input command: update the  measurement interval characteristic (<b>HT_Update_Measurement_Interval() </b> API)
 */
#define APPL_UPDATE_MEASUREMENT_INTERVAL        'c' 
/** 
 *  Health Thermometer user input command: update the temperature type (<b>HT_Update_Temperature_Type() </b> API)
 */
#define APPL_UPDATE_TEMPERATURE_TYPE            'd' 
/** 
 *  Health Thermometer user input command: to run HTS/SEN/SP/BV-01-C PTS test (<b>HT_Update_Temperature_Type() </b> API)
 */
#define APPL_SET_HTS_SEN_SP_BV_01_C             'e' 
/** 
 *  Health Thermometer user input command: after to run HTS/SEN/SP/BV-01-C PTS test (<b>HT_Update_Temperature_Type() </b> API)
 */
#define APPL_UNSET_HTS_SEN_SP_BV_01_C           'f' 

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
///@cond BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR
/**
 *@defgroup Heart_Rate_Sensor
 *@brief PTS test application user input commands for Heart Rate Sensor Peripheral role
 *@{
 */

/** 
 *  Heart Rate Sensor user input command: send heart rate measurement characteristic with no sensor contact (<b>HRProfile_Send_HRM_Value() </b> API)
 */
#define APPL_SEND_HEART_RATE_MEASUREMENT_WITHOUT_SENSOR_CONTACT  'a' 

/** 
 *  Heart Rate Sensor user input command: send heart rate measurement characteristic with sensor contact (<b>HRProfile_Send_HRM_Value() </b> API)
 */
#define APPL_SEND_HEART_RATE_MEASUREMENT_WITH_SENSOR_CONTACT     'b' 

/** 
 *  Heart Rate Sensor user input command: set sensor body location (<b>HRProfile_Set_Body_Sensor_Location() </b> API)
 */
#define APPL_SET_BODY_SENSOR_LOCATION                            'c' 

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
///@cond BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT
/**
 *@defgroup Phone_Alert_Client
 *@brief PTS test application user input commands for Phone Alert Client Peripheral role
 *@{
 */

/** 
 *  Phone Alert Client user input command: read the alert status characteristic value (<b>PAC_Read_AlertStatus() </b> API)
 */
#define APPL_PHONE_READ_ALERT_STATUS		'a'

/** 
 *  Phone Alert Client user input command: read the ringer setting characteristic value (<b>PAC_Read_RingerSetting() </b> API)
 */
#define APPL_PHONE_READ_RINGER_SETTING	        'b' 

/** 
 *  Phone Alert Client user input command: disable the alert status notification (<b>PAC_Disable_ALert_Status_Notification() </b> API)
 */
#define APPL_DISABLE_ALERT_STATUS_NOTIF	        'c' 

/** 
 *  Phone Alert Client user input command: disable the ringer setting notification (<b>PAC_Disable_Ringer_Status_Notification() </b> API)
 */
#define APPL_DISABLE_RINGER_SETTING_NOTIF	'd' 

/** 
 *  Phone Alert Client user input command: start a write without response GATT procedure to write the ringer mode command silence to the phone alert server (<b>PAC_Configure_Ringer() </b> API)
 */
#define APPL_CONFIGURE_RINGER_SILENCE		'e'

/** 
 *  Phone Alert Client user input command: start a write without response GATT procedure to write the ringer mode command mute to the phone alert server (<b>PAC_Configure_Ringer() </b> API)
 */
#define APPL_CONFIGURE_RINGER_MUTE		'f' 

/** 
 *  Phone Alert Client user input command: start a write without response GATT procedure to write the ringer mode command cancel silence to the phone alert server (<b>PAC_Configure_Ringer() </b> API)
 */
#define APPL_CONFIGURE_RINGER_CANCEL_SILENCE	'g'

#define APPL_DISCOVERY_PRIMARY_SERVICE          'h'

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
///@cond BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR
/**
 *@defgroup Proximity_Monitor
 *@brief PTS test application user input commands for Proximity Monitor Peripheral role
 *@{
 */

/** 
 *  Proximity Monitor user input command: enable the alert for pathLoss alerts (<b>ProximityMonitor_Notify_Path_Loss(1) </b> API)
 */
#define APPL_ENABLE_ALERT_PATH_LOSS            'a'

/** 
 *  Proximity Monitor user input command: disable the alert for pathLoss alerts (<b>ProximityMonitor_Notify_Path_Loss() </b> API)
 */
#define APPL_DISABLE_ALERT_PATH_LOSS           'b'


#define APPL_DISCOVERY_PRIMARY_SERVICE         'c'

/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
///@cond BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT
/**
 *@defgroup Time_Client 
 *@brief PTS test application user input commands for Time Client Peripheral role
 *@{
 */

/** 
 *  Time Client user input command: start a gatt write without response procedure to write the time update control point characteristic on the server (<b>TimeClient_Update_Reference_Time_On_Server(0x01) </b> API)
 */
#define APPL_GET_REF_TIME_UPDATE		'a' 

/** 
 *  Time Client user input command: enable notification of time update characteristic (<b>TimeClient_SetReset_Time_Update_Notification(0x01) </b> API)
 */
#define APPL_TIME_UPDATE_NOTIFICATION		'b'  

/** 
 *  Time Client user input command: start a gatt read procedure to read the time with DST characteristic on the server (<b>TimeClient_Get_Next_DST_Change_Time() </b> API)
 */
#define APPL_READ_NEXT_DST_CHANGE_TIME 	        'c'

/** 
 *  NOTE: user input not currently used 
 */
#define APPL_READ_SERV_TIME_UPDATE_STATE	'd' 

/** 
 *  Time Client user input command: start a gatt read procedure to read the local time information characteristic on the server (<b>TimeClient_Get_Local_Time_Information() </b> API)
 */
#define APPL_READ_LOCAL_TIME_INFORM		'e'

/** 
 *  Time Client user input command: start a GATT procedure to read the current time characteristic (<b>TimeClient_Get_Current_Time() </b> API)
 */
#define APPL_READ_CURRENT_TIME		        'f' 

/** 
 *  Time Client user input command: start a gatt read procedure to read the time update state characteristic on the server (<b>TimeClient_Get_Server_Time_Update_State() </b> API)
 */
#define APPL_GET_SERV_TIME_UPDATE_STATE	        'g'

/** 
 *   NOTE: user input not currently used 
 */
#define APPL_GET_REF_TIME_INFO_ON_SERVER	'h' 

/** 
 *  Time Client user input command: start a gatt write without response procedure to write the time update control point characteristic on the server (<b>TimeClient_Update_Reference_Time_On_Server() </b> API)
 */
#define APPL_CANCEL_REF_TIME_UPDATE		'k' 

/** 
 *  Time Client user input command: start a gatt read procedure to read the reference time information characteristic on the server (<b>TimeClient_Get_Time_Accuracy_Info_Of_Server() </b> API)
 */
#define APPL_GET_TIME_ACCU_INFO_SERVER	        'i'

#define APPL_DISCOVERY_PRIMARY_SERVICE          'l'   //PTS Automating for Time Client     

#define APPL_DISCOVERY_CHARS_OF_CURRENT_TIME_SERVICE     'n'   //PTS Automating for Time Client  
#define APPL_DISCOVERY_CHARS_OF_NEXT_DS_SERVICE          'o'   //PTS Automating for Time Client
#define APPL_DISCOVERY_CHARS_OF_REFERENCE_TIME_SERVICE   'p'   //PTS Automating for Time Client    
#define APPL_DISCOVERY_CHARS_DESCR_OF_CURRENT_TIME       'q'   //PTS Automating for Time Client 
/**
 * @}
 */
///@endcond
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
///@cond BLE_CURRENT_PROFILE_ROLES & TIME_SERVER
/**
 *@defgroup Time_Server
 *@brief PTS test application user input commands for Time Server  Peripheral role
 *@{
 */

/** 
 *  Time Server user input command: update the current time characteristic with the timeValue specified (<b>TimeServer_Update_Current_Time_Value() </b> API)
 */
#define APPL_UPDATE_CURRENT_TIME                'a'

/** 
 *  Time Server user input command: decrease the current time characteristic with the timeValue specified.It is used for PTS test TC_CSP_BV_06_C. (<b>TimeServer_Update_Current_Time_Value() </b> API)
 */
#define APPL_DECREASE_CURRENT_TIME              'b'   
  
/** 
 *  Time Server user input command: update the current time characteristic 0f + 15 minutes.It is used for PTS test TC_CSP_BV_06_C.(<b>TimeServer_Update_Current_Time_Value() </b> API)
 */
#define APPL_UPDATE_CURRENT_TIME_MORE_THAN_15   'c'

/** 
 *  Time Server user input command: update the localTimeInfo characteristic with the value specified (<b>TimeServer_Update_Local_Time_Information() </b> API)
 */
#define APPL_UPDATE_LOCAL_TIME			'd'

/** 
 *  NOTE: user input not currently used 
 */
#define APPL_UPDATE_REFERENCE_TIME		'e' 

/** 
 *  Time Server user input command: update the next DST change characteristic with the value specified (<b>TimeServer_Update_Next_DST_Change() </b> API)
 */
#define APPL_UPDATE_NEXT_DSTCHANGE_TIME		'f'

/** 
 *  Time Server user input command: update the reference time information characteristic with the value specified (<b>TimeServer_Update_Reference_Time_Information() </b> API)
 */
#define APPL_UPDATE_REFERENCE_TIME_INFO		'g' 

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to MANUAL TIME UPDATE.
 */
#define APPL_ADJUST_REASON_MANUAL_TIME_UPDATE    'h'

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to CHANGE OF TIME ZONE.
 */
#define APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE    'i' 

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to CHANGE OF DST.
 */
#define APPL_ADJUST_REASON_CHANGE_OF_DST          'l'

/** 
 *  Time Server user input command: utility command for setting time.adjustReason to EXTERNAL REFERENCE TIME UPDATE.
 */
#define APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE   'n'

/**
 * @}
 */
///@endcond
#endif


/************************************* General User Input Commands: valid for all PTS tests */

/**
 *@defgroup General_Profiles
 *@brief PTS test application user input commands valid for all the Profiles Peripheral Roles
 *@{
 */

/** 
 * Display PTS user input command menu
 */
#define DISPLAY_PTS_MENU                       'm' //Display PTS application user input command menu

/** 
 * Clear security database
 */
#define APPL_SEND_SLAVE_SECURITY_REQUEST       'r'           

//Macro for start advertising after ADVERTISING TIMEOUT
//#define APPL_START_ADVERTISE		       's'  

/** 
 * Terminate connection 
 */
#define APPL_TERMINATE_LINK                    't' // It moves deviceState to APPL_UNINITIALIZED

/** 
 * Clear security database
 */
#define APPL_CLEAR_SECURITY_DATABASE           'z' // Clear the security database (it removes any bonded device) 

#define PROFILE_RESET                          'v' //reset the device
/**
 * @}
 */

#define DUMMY_INPUT_VALUE                      200

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
/* HTS_SEN_SP_BV_01_C to be enabled only for running PTS test 4.10.1: HTS_SEN_SP_BV_01_C */   
uint8_t HTS_SEN_SP_BV_01_C = 0;

#define HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT       (0xA0)
#define HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT_STOP  (0xA1)
volatile uint8_t ht_check_next_send_temperature_measurement_flag = 0;
uint8_t ht_check_next_send_temperature_measurement_state = 0; 

#endif /* BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER */

#define PROFILE_SECURITY        NO_SECURITY_REQUIRED


/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/******************************************************************************
 * Local Variable Declarations
******************************************************************************/

uint8_t deviceState = APPL_UNINITIALIZED;

SleepModes sleep_mode = SLEEPMODE_WAKETIMER;

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
tTempMeasurementVal tempMeasurementVal;
BOOL HTIntervalTimer_expired = FALSE;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
tHeartRateMeasure heartRateMeasurmentValue;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
tCurrentTime time;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
extern tGlucoseContext glucose;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
extern tCgmContext cgm;
extern BOOL bStartSession;  //if it is FALSE, indicates that a session mustn't start after connection
extern BOOL bMultipleSession; 
extern BOOL bMultipleBond;
extern BOOL bE2E_CRC_Support;
extern BOOL bPatientHighLowAlert_Support;
extern BOOL bHypoAlert_Support;
extern BOOL bHyperAlert_Support;
extern BOOL bRateIncreaseDecreaseAlert_Support;
extern BOOL bDeviceSpecificAlert_Support;
extern BOOL bCalibration_Support;
extern BOOL bAllFeatureSupport_Support;
#endif

static tSecurityParams Security_Param;

/******************************************************************************
 * Global Variable Declarations
******************************************************************************/
void *profiledbgfile;

/* Peripheral public address */
uint8_t public_address[6] = PERIPHERAL_PUBLIC_ADDRESS;

/******************************************************************************
 * External Variable Declarations
******************************************************************************/
//PTS Automating for Time Client
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
extern BOOL b_appl_discovery_primary_serv; 
extern tTimeClientContext timeClient;     //for discovery Chars 
#endif
/******************************************************************************
 * Function Declarations
******************************************************************************/
uint8_t __io_getcharNonBlocking(uint8_t *data);
/******************************************************************************
 * Function Definitions 
******************************************************************************/
void Profile_Reset(void)
{
  NVIC_SystemReset();
}

/* Enable the Sleep mode: SLEEPMODE_NOTIMER or SLEEPMODE_WAKETIMER with wake up on push1 button */
void Enter_Sleep_Mode(void)
{
  uint8_t ret, wakeup_source, wakeup_level;   
  
  /* BlueNRG-1 STEVAL-IDB007V1 PUSH1 Button: WAKEUP_IO13 */
  wakeup_source = WAKEUP_IO13; 
  wakeup_level = (WAKEUP_IOx_LOW << WAKEUP_IO13_SHIFT_MASK); 
  
  ret = BlueNRG_Sleep(sleep_mode, wakeup_source, wakeup_level);
  
  //ret = BlueNRG_Sleep(sleep_mode, 0, 0);
  if (ret != BLE_STATUS_SUCCESS) 
  {
    printf("BlueNRG_Sleep() error 0x%02x\r\n", ret);
    while(1);
  }
}/* end Enter_Sleep_Mode*/

/**
 * @brief 
 * @param event
 * @param evtLen
 * @param evtData
 */
void BLE_Profiles_Evt_Notify_Cb(tNotificationEvent event,uint8_t evtLen,uint8_t* evtData)
{
  
  switch(event)
  {
    case EVT_MP_BLUE_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BLUE_INITIALIZED\n");
    }
    break;
    case EVT_MP_ADVERTISING_TIMEOUT:
    {
     APPL_MESG_DBG(profiledbgfile,"EVT_MP_ADVERTISING_TIMEOUT\n");
     
     deviceState = APPL_ADVERTISING_TIMEOUT; 
     sleep_mode = SLEEPMODE_NOTIMER;
     
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
     APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
     TimeClient_Make_Discoverable(0x00);
#endif
    }
    break;
    case EVT_MP_CONNECTION_COMPLETE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_CONNECTION_COMPLETE\n");
      deviceState = APPL_CONNECTED;//uncommented for the time test case csp_bv_02_c(cts).
      
#if ((BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER) || (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR))          
      SdkEvalLedOff(LED1);
      SdkEvalLedOff(LED2);
#endif
    }
    break;
    case EVT_MP_DISCONNECTION_COMPLETE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_DISCONNECTION_COMPLETE %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_MP_PASSKEY_REQUEST:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_PASSKEY_REQUEST\n");
      BLE_Profile_Send_Pass_Key(111111);
    }
    break;
    case EVT_MP_PAIRING_COMPLETE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_MP_PAIRING_COMPLETE\n");
      deviceState = APPL_CONNECTED;
    }
    break;

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
    case EVT_ANC_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_ANC_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_DISCOVERY_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_ANC_NEW_ALERT_RECEIVED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_NEW_ALERT_RECEIVED %x %x %x\n",evtLen,evtData[0],evtData[1]);
    }
    break;
    case EVT_ANC_UNREAD_ALERT_STATUS_RECEIVED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANC_UNREAD_ALERT_STATUS_RECEIVED %x %x %x\n",evtLen,evtData[0],evtData[1]);
    }
    break;
  #endif
    
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
    case EVT_ANS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_ANS_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
    case EVT_BPS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BPS_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_BPS_BPM_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BPS_BPM_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_BPS_ICP_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_BPS_ICP_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
#endif		
    
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
    case EVT_FML_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_FML_DISCOVERY_CMPLT: reason %x\n",evtData[0]);
      deviceState = APPL_LOCATOR_ALERT_TARGET;    
    }
    break;
    case EVT_FML_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_FML_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
    case EVT_FMT_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_FMT_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
    case EVT_GL_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_GL_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_GL_INITIALIZED  AND DEVICE STATE IS %d\n",deviceState);
    }
    break;         
#endif /* BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR */


#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
    case EVT_CGM_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_CGM_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_GL_INITIALIZED  AND DEVICE STATE IS %d\n",deviceState);
    }
    break;         
#endif /* BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR */
    
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
    case EVT_HT_INITIALIZED:
    {
         
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_INITIALIZED %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_INITIALIZED  AND DEVICE STATE IS %d\n",deviceState);
    }
    break;
    case EVT_HT_TEMPERATURE_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_TEMPERATURE_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_HT_INTERMEDIATE_TEMP_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_INTERMEDIATE_TEMP_CHAR_UPDATE_CMPLT %x\n",evtData[0]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_HT_MEASUREMENT_INTERVAL_RECEIVED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_MEASUREMENT_INTERVAL_RECEIVED %x %x\n",evtData[0],evtData[1]);
      deviceState = APPL_CONNECTED;
    }
    break;
    case EVT_HT_MEASUREMENT_INTERVAL_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HT_MEASUREMENT_INTERVAL_UPDATE_CMPLT\n");
      deviceState = APPL_CONNECTED;
    }
    break;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
    case EVT_HRS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HRS_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_HRS_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HRS_CHAR_UPDATE_CMPLT %x:%x:%x:%x:%x\n",evtData[0],evtData[1],evtData[2],evtData[3],evtData[4]);
    }
    break;
    case EVT_HRS_RESET_ENERGY_EXPENDED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_HRS_RESET_ENERGY_EXPENDED\n");
    }
    break;
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
    case EVT_PAC_INITIALIZED:
    {
      deviceState = APPL_INIT_DONE;
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_INITIALIZED\n");
    }
    break;
    case EVT_PAC_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_DISCOVERY_CMPLT %x\n",evtData[0]);
    }
    break;
    case EVT_PAC_ALERT_STATUS:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_ALERT_STATUS %x\n",evtData[0]);
      deviceState = APPL_PHONE_READ_ALERT_STATUS;
    }
    break;
    case EVT_PAC_RINGER_SETTING:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PAC_RINGER_SETTING %x\n",evtData[0]);
      deviceState = APPL_PHONE_READ_RINGER_SETTING;
    }
    break;
#endif    
    
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
   case EVT_PM_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PM_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PM_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PM_DISCOVERY_CMPLT %x\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PM_LINK_LOSS_ALERT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PM_LINK_LOSS_ALERT %d\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      switch(evtData[0])
      {
      case 0: // no alert (D1, D2 off)
          SdkEvalLedOff(LED1);
          SdkEvalLedOff(LED2);
          break;
      case 1: //mid alert: D1 on
          SdkEvalLedOn(LED1);
          SdkEvalLedOff(LED2);
          break;
      case 2: //high alert: D2 on
          SdkEvalLedOff(LED1);
          SdkEvalLedOn(LED2);
          break;
      default:
          SdkEvalLedOff(LED1);
          SdkEvalLedOff(LED2);
      }
    }
    break;
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
    case EVT_PR_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PR_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PR_PATH_LOSS_ALERT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PR_PATH_LOSS_ALERT %d\n",evtData[0]);
      //deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_PR_LINK_LOSS_ALERT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_PR_LINK_LOSS_ALERT %d\n",evtData[0]);
      deviceState = APPL_INIT_DONE;
      switch(evtData[0])
      {
      case 0: // no alert (D1, D2 off)
          SdkEvalLedOff(LED1);
          SdkEvalLedOff(LED2);
          break;
      case 1: //mid alert: D1 on
          SdkEvalLedOn(LED1);
          SdkEvalLedOff(LED2);
          break;
      case 2: //high alert: D2 on
          SdkEvalLedOff(LED1);
          SdkEvalLedOn(LED2);
          break;
      default:
          SdkEvalLedOff(LED1);
          SdkEvalLedOff(LED2);
      }
    }
    break;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
    case EVT_TC_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TC_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_TC_DISCOVERY_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TC_DISCOVERY_CMPLT\n");
      deviceState = APPL_UPDATE;
    }
    break;
    case EVT_TC_READ_REF_TIME_INFO_CHAR:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TC_READ_REF_TIME_INFO_CHAR\n");
      APPL_MESG_DATA(profiledbgfile,evtData,evtLen);

      APPL_MESG_DBG(profiledbgfile,"\n");
    }
    break;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
    case EVT_TS_INITIALIZED:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_INITIALIZED\n");
      deviceState = APPL_INIT_DONE;
    }
    break;
    case EVT_TS_START_REFTIME_UPDATE:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_START_REFTIME_UPDATE\n");
      TimeServer_Update_Current_Time_Value(time);
    }
    break;
    case EVT_TS_CHAR_UPDATE_CMPLT:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_CHAR_UPDATE_CMPLT\n");
      deviceState = APPL_CONNECTED;			
    }
    break;
    case EVT_TS_CURTIME_READ_REQ:
    {
      APPL_MESG_DBG(profiledbgfile,"EVT_TS_CURTIME_READ_REQ\n");
      Allow_Curtime_Char_Read();
    }
    break;
#endif
    
  }/* end switch() */
}/* end BLE_Profiles_Evt_Notify_Cb() */

/* Profiles Initialization function */
void Init_Profile()
{  
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  APPL_MESG_DBG(profiledbgfile,"Initializing alert notification client profile\n");
  if (ANC_Client_Init(BLE_Profiles_Evt_Notify_Cb)== BLE_STATUS_SUCCESS)
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }  
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  uint8_t alertCategory[2] = {0xFF,0x03};
  uint8_t unreadAlertCategory[2] = {0xFF,0x00};
  APPL_MESG_DBG(profiledbgfile,"Initializing alert notification server profile\n");
  if (ANS_Init(alertCategory,unreadAlertCategory,BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }  
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  //APPL_MESG_DBG(profiledbgfile,"Initializing blood pressure sensor profile\n");
  printf ("Test Application: Initializing blood pressure sensor profile\n" );
  if (BPS_Init(0x01,
               BODY_MOVEMENT_DETECTION_SUPPORT_BIT|CUFF_FIT_DETECTION_SUPPORT_BIT|MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT,
               BLE_Profiles_Evt_Notify_Cb)== BLE_STATUS_SUCCESS)
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }  
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  APPL_MESG_DBG(profiledbgfile,"Initializing Find Me Locator profile\n");
  printf ("Test Application: Initializing Find me profile\n" );
  if (FindMeLocator_Init(&Security_Param,BLE_Profiles_Evt_Notify_Cb) == 
      BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Find Me Locator profile\n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }  
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
//  APPL_MESG_DBG(profiledbgfile,"Initializing Find me profile\n");
  printf ("Test Application: Initializing Find Me Target profile\n" );
  if (FindMeTarget_Init(BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Find Me Target profile \n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }    
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"Initializing glucose sensor profile\n");
  printf ("Test Application: Initializing glucose sensor profile with user database\n" );
  /* Number of records  on glucose database + pointer to glucose measurement and context database */
  if (GL_Init(GLUCOSE_NUMBER_OF_STORED_RECORDS, 
              &glucoseDatabase_MeasurementRecords[0], 
              &glucoseDataBase_MeasurementContextRecords[0], 
              BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS )
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
#endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */


#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"Initializing CGM sensor profile\n");
  printf ("Test Application: Initializing CGM sensor profile with user database\n" );
  /* Number of records  on cgm database + pointer to cgm measurement database */
  if (CGM_Init(CGM_NUMBER_OF_STORED_RECORDS, 
              &cgmDatabase_MeasurementRecords[0], 
              BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS )
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
#endif /* (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR) */
  
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  APPL_MESG_DBG(profiledbgfile,"Initializing health thermometer profile\n");
  printf ("Test Application: Initializing health thermometer profile\n" );
  if (HT_Init(INTERMEDIATE_TEMPERATURE_CHAR|MEASUREMENT_INTERVAL_CHAR|TEMPERATURE_TYPE,
             2, /* minValidInterval 2 */
             60, /* maxValidInterval */
             BLE_Profiles_Evt_Notify_Cb)== BLE_STATUS_SUCCESS)
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  printf ("Test Application: Initializing Heart Rate Profile \n" ); 
  if (HRProfile_Init((uint8_t)0xFF,
                     BLE_Profiles_Evt_Notify_Cb,
                     0x04) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Heart Rate Profile \n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Heart Rate Profile initialization \n" );
  }
#endif  

#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  printf ("Test Application: Initializing Phone Alert Status profile\n" );
  if (PAC_Init(BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  } 
#endif
  
  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  printf ("Test Application: Initializing Proximity Monitor Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Proximity Monitor Profile \n" );

  if (ProximityMonitor_Init(BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Proximity Monitor Profile \n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Proximity Monitor Profile \n" );
  }
  SdkEvalLedInit(LED1);
  SdkEvalLedInit(LED2);
  SdkEvalLedOff(LED1);
  SdkEvalLedOff(LED2);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  printf ("Test Application: Initializing Proximity Reporter Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Proximity Reporter Profile \n" );

  if (ProximityReporter_Init(0xff,BLE_Profiles_Evt_Notify_Cb) == 
                             BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Proximity Reporter Profile \n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Proximity Reporter Profile init failure\n" );
  }
  
  SdkEvalLedInit(LED1);
  SdkEvalLedInit(LED2);
  SdkEvalLedOff(LED1);
  SdkEvalLedOff(LED2);
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  printf ("Test Application: Initializing Time Client Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Time Client Profile \n" );

  if (TimeClient_Init(BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Time Client Profile \n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Time Client Profile \n" );
  }
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  printf ("Test Application: Initializing Time Server Profile \n" );
  APPL_MESG_DBG(profiledbgfile,"Initializing Time Server Profile \n" );

  if (TimeServer_Init (NEXT_DST_CHANGE_SERVICE_BITMASK|REFERENCE_TIME_UPDATE_SERVICE_BITMASK, 
                       BLE_Profiles_Evt_Notify_Cb) == BLE_STATUS_SUCCESS)
  {
    APPL_MESG_DBG(profiledbgfile,"Initialized Time Server Profile \n" );
    /* Set profileProcess and state of ble  */
    setProfileProcess();
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Failed Time Server Profile \n" );
  }
#endif

}/* end Init_Profile() */

/* Profile Advertize function */
void Advertize()
{
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ANC_Advertize(0x00);
#endif
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ANS_Advertize(0x00);
#endif	

#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  printf("Test Application: advertize\n");
  BPS_Advertize(0x00);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  printf("Test Application: add to whitelist\n");
  //FML_Add_Device_To_WhiteList(bdAddr);
  printf("Test Application: advertize\n");
  FML_Advertize();
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
  printf("Test Application: add to whitelist\n");
  //FMT_Add_Device_To_WhiteList(0x00,bdAddr);
  printf("Test Application: advertize\n");
  FMT_Advertize();
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  /* Set Glucose Feature characteristic value: all features except multi bond */
  GL_Set_Glucose_Feature_Value (GLUCOSE_FEATURE_ALL_SUPPORTED_NO_MULTI_BOND); //(GLUCOSE_FEATURE_ALL_SUPPORTED); //for multiple bonds
  /* Start advertising (no whitelist, fast) */
  APPL_MESG_DBG(profiledbgfile,"glucose enable advertising\n");  
  GL_Advertize(0x00);
#endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */

#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
  /* Start advertising (no whitelist, fast) */
  APPL_MESG_DBG(profiledbgfile,"cgm enable advertising\n");
  CGM_Advertize(0x00);
#endif /* (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR) */
  
#if (BLE_CURRENT_PROFILE_ROLES & (HEART_RATE_SENSOR|TIME_CLIENT|TIME_SERVER|PROXIMITY_REPORTER|PROXIMITY_MONITOR))
  uint8_t useBoundedDeviceList = 0;
#endif 
#if (BLE_CURRENT_PROFILE_ROLES & (FIND_ME_TARGET|FIND_ME_LOCATOR|PHONE_ALERT_CLIENT))
 
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
//  char systemId[8]={0x00,0x01,'.',0x00,0x00,0x00,0x00,0x01};
//  char pnpid[7]={0x01,0x01,0x01,0x01,0x01,0x01,0x01};
  uint8_t pnpid[7]={0x01,0x01,0x01,0x01,0x01,0x01,0x01};
  //BLE_Profile_Update_DIS_SystemID(8,systemId);
  //BLE_Profile_Update_DIS_ModelNum(8,systemId);
  //BLE_Profile_Update_DIS_SerialNum(8,systemId);
  //BLE_Profile_Update_DIS_FirmwareRev(8,systemId);
  //BLE_Profile_Update_DIS_HardwareRev(8,systemId);
  //BLE_Profile_Update_DIS_SoftwareRev(8,systemId);
  //BLE_Profile_Update_DIS_manufacturerName(8,systemId);
  //BLE_Profile_Update_DIS_IEEECertification(8,systemId);
  BLE_Profile_Update_DIS_pnpId(7,pnpid);
  HT_Update_Temperature_Type(0x00);
  HT_Update_Measurement_Interval(0x0000);  //TBR???
  HT_Advertize(0x00);
#endif  
  
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  APPL_MESG_DBG(profiledbgfile,"set sensor location\n");
  HRProfile_Set_Body_Sensor_Location(BODY_SENSOR_LOCATION_HAND);
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  HR_Sensor_Make_Discoverable( useBoundedDeviceList );
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)		
  //PAC_Add_Device_To_WhiteList(bdAddr);
  printf("Test Application: advertize\n");
  APPL_MESG_DBG(profiledbgfile,"writing the ringer command\n");
  PAC_Advertize();
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ProximityMonitor_Make_Discoverable(useBoundedDeviceList);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  ProximityReporter_Make_Discoverable(useBoundedDeviceList);
#endif


#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  TimeClient_Make_Discoverable(useBoundedDeviceList);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  APPL_MESG_DBG(profiledbgfile,"enable advertising\n");
  TimeServer_Make_Discoverable(useBoundedDeviceList);
#endif
  
}/* end Advertize() */

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
/* Health Thermometer Profile: Init thermometer value for PTS tests */
void Init_temperature()
{
  tempMeasurementVal.flags = (FLAG_TEMPERATURE_UNITS_FARENHEIT|FLAG_TIMESTAMP_PRESENT|FLAG_TEMPERATURE_TYPE);
  tempMeasurementVal.temperatureType = TEMP_MEASURED_FOR_BODY;
  tempMeasurementVal.timeStamp.year = 2012;
  tempMeasurementVal.timeStamp.month = 10;
  tempMeasurementVal.timeStamp.day = 26;
  tempMeasurementVal.timeStamp.hours = 19;
  tempMeasurementVal.timeStamp.minutes = 42;
  tempMeasurementVal.timeStamp.seconds = 50;
}/* end Init_temperature() */
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
/* Time Server Profile: Init current time for PTS tests */
void Init_Current_Time()
{
  time.date = 06;
  time.day_of_week = 04;
  time.hours = 1;
  time.minutes = 1;
  time.seconds = 4;
  
  /* NOTES: Values to be set for running PTS tests CTS, CSP */
  time.adjustReason = 0x01; /* ADJUST_REASON_MANUAL_TIME_UPDATE: CTS/SR/CSP/BV-02-C  */
//  time.adjustReason = 0x04; /* ADJUST_REASON_CHANGE_OF_TIME_ZONE: CTS/SR/CSP/BV-03-C */
//  time.adjustReason = 0x08; /* ADJUST_REASON_CHANGE_OF_DST:     CTS/SR/CSP/BV-04-C */
//  time.adjustReason = 0x02; /* ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: CTS/SR/CSP/BV-05-C, CTS/SR/CSP/BV-06-C*/
  
  time.fractions256 = 0x00;
  time.year = 2013;

}/* end Init_Current_Time() */
#endif

/* Display PTS test commands menu for each profile */
void Display_Appl_Menu()
{
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  printf("APPL_ENABLED_NEW_ALERT_NOTIF: %c\n",APPL_ENABLED_NEW_ALERT_NOTIF);
  printf("APPL_DISABLED_NEW_ALERT_NOTIF: %c\n",APPL_DISABLED_NEW_ALERT_NOTIF);
  printf("APPL_ENABLED_UNREAD_ALERT_STATUS_NOTIF: %c\n",APPL_ENABLED_UNREAD_ALERT_STATUS_NOTIF);
  printf("APPL_DISABLED_UNREAD_ALERT_STATUS_NOTIF: %c\n",APPL_DISABLED_UNREAD_ALERT_STATUS_NOTIF);
  printf("APPL_WRITE_CTRL_POINT: %c\n",APPL_WRITE_CTRL_POINT);
  printf("APPL_WRITE_CTRL_POINT_ANPSF_1: %c\n",APPL_WRITE_CTRL_POINT_ANPSF_1);
  printf("APPL_WRITE_CTRL_POINT_ANPSF_2: %c\n",APPL_WRITE_CTRL_POINT_ANPSF_2);
  printf("APPL_DISCOVERY_PRIMARY_SERVICE: %c\n",APPL_DISCOVERY_PRIMARY_SERVICE);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
  printf("APPL_UPDATE_NEW_ALERT: %c\n",APPL_UPDATE_NEW_ALERT);
  printf("APPL_UPDATE_UNREAD_ALERT_STATUS: %c\n",APPL_UPDATE_UNREAD_ALERT_STATUS);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  printf("APPL_SEND_BP_MEASUREMENT: %c\n",APPL_SEND_BP_MEASUREMENT);
  printf("APPL_SEND_INTERMEDIATE_CUFF_PRESSURE: %c\n",APPL_SEND_INTERMEDIATE_CUFF_PRESSURE);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  printf("APPL_LOCATOR_NO_ALERT_TARGET: %c\n",APPL_LOCATOR_NO_ALERT_TARGET);
  printf("APPL_LOCATOR_ALERT_TARGET: %c\n",APPL_LOCATOR_ALERT_TARGET);
  printf("APPL_DISCOVERY_PRIMARY_SERVICE: %c\n",APPL_DISCOVERY_PRIMARY_SERVICE);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  printf("APPL_GLUCOSE_CHANGE_DB_1: %c\n",APPL_GLUCOSE_CHANGE_DB_1);
  printf("APPL_GLUCOSE_CHANGE_DB_2: %c\n",APPL_GLUCOSE_CHANGE_DB_2);
  printf("APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1: %c\n",APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1);  
  printf("APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3: %c\n",APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3);
  printf("APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4: %c\n",APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4);
  printf("APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5: %c\n",APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5);
  printf("APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9: %c\n",APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9);
  printf("APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200: %c\n",APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200);
  printf("APPL_SET_MEDICATION_UNITS_KG: %c\n",APPL_SET_MEDICATION_UNITS_KG);
  printf("APPL_SET_MEDICATION_UNITS_LITER: %c\n",APPL_SET_MEDICATION_UNITS_LITER);  
  printf("APPL_SET_MULTIPLE_BOND: %c\n",APPL_SET_MULTIPLE_BOND);   
#endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */

#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
  printf("APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_3: %c\n",APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_3);  
  printf("APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_5: %c\n",APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_5);
  printf("APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_10: %c\n",APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_10);
  printf("APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_20: %c\n",APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_20);
  printf("APPL_SET_NO_START_SESSION: %c\n",APPL_SET_NO_START_SESSION);
  printf("APPL_SET_START_SESSION: %c\n",APPL_SET_START_SESSION);
  printf("APPL_SET_NO_MULTIPLE_SESSION: %c\n",APPL_SET_NO_MULTIPLE_SESSION);
  printf("APPL_SET_MULTIPLE_SESSION: %c\n",APPL_SET_MULTIPLE_SESSION);
  printf("APPL_SET_NO_CGM_MULTIPLE_BOND: %c\n",APPL_SET_NO_CGM_MULTIPLE_BOND);
  printf("APPL_SET_CGM_MULTIPLE_BOND: %c\n",APPL_SET_CGM_MULTIPLE_BOND);
  printf("APPL_SET_NO_E2E_CRC_SUPPORT: %c\n",APPL_SET_NO_E2E_CRC_SUPPORT);
  printf("APPL_SET_E2E_CRC_SUPPORT: %c\n",APPL_SET_E2E_CRC_SUPPORT);
  printf("APPL_SET_NO_PATIENT_ALERT_SUPPORT: %c\n",APPL_SET_NO_PATIENT_ALERT_SUPPORT);
  printf("APPL_SET_PATIENT_ALERT_SUPPORT: %c\n",APPL_SET_PATIENT_ALERT_SUPPORT);  
  printf("APPL_SET_NO_HYPO_ALERT_SUPPORT: %c\n",APPL_SET_NO_HYPO_ALERT_SUPPORT);
  printf("APPL_SET_HYPO_ALERT_SUPPORT: %c\n",APPL_SET_HYPO_ALERT_SUPPORT);
  printf("APPL_SET_NO_HYPER_ALERT_SUPPORT: %c\n",APPL_SET_NO_HYPER_ALERT_SUPPORT);
  printf("APPL_SET_HYPER_ALERT_SUPPORT: %c\n",APPL_SET_HYPER_ALERT_SUPPORT);
  printf("APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT: %c\n",APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT);
  printf("APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT: %c\n",APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT);  
  printf("APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT: %c\n",APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT);
  printf("APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT: %c\n",APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT);  
  printf("APPL_SET_NO_CALIBRATION_SUPPORT: %c\n",APPL_SET_NO_CALIBRATION_SUPPORT);
  printf("APPL_SET_CALIBRATION_SUPPORT: %c\n",APPL_SET_CALIBRATION_SUPPORT);
  printf("APPL_SET_NO_ALL_FEATURE_SUPPORT: %c\n",APPL_SET_NO_ALL_FEATURE_SUPPORT);
  printf("APPL_SET_ALL_FEATURE_SUPPORT: %c\n",APPL_SET_ALL_FEATURE_SUPPORT);  
#endif /* (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR) */
 
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  printf("APPL_SEND_HT_MEASUREMENT: %c\n",APPL_SEND_HT_MEASUREMENT);
  printf("APPL_SEND_HT_INTERMEDIATE_MEASUREMENT: %c\n",APPL_SEND_HT_INTERMEDIATE_MEASUREMENT);
  printf("APPL_UPDATE_MEASUREMENT_INTERVAL: %c\n",APPL_UPDATE_MEASUREMENT_INTERVAL);
  printf("APPL_SET_HTS_SEN_SP_BV_01_C: %c\n",APPL_SET_HTS_SEN_SP_BV_01_C);  
  printf("APPL_UNSET_HTS_SEN_SP_BV_01_C: %c\n",APPL_UNSET_HTS_SEN_SP_BV_01_C);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  printf("APPL_SEND_HEART_RATE_MEASUREMENT_WITHOUT_SENSOR_CONTACT: %c\n",APPL_SEND_HEART_RATE_MEASUREMENT_WITHOUT_SENSOR_CONTACT);
  printf("APPL_SEND_HEART_RATE_MEASUREMENT_WITH_SENSOR_CONTACT: %c\n",APPL_SEND_HEART_RATE_MEASUREMENT_WITH_SENSOR_CONTACT);
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)  
  printf("APPL_PHONE_READ_ALERT_STATUS: %c\n",APPL_PHONE_READ_ALERT_STATUS);
  printf("APPL_PHONE_READ_RINGER_SETTING: %c\n",APPL_PHONE_READ_RINGER_SETTING);
  printf("APPL_DISABLE_ALERT_STATUS_NOTIF: %c\n",APPL_DISABLE_ALERT_STATUS_NOTIF);
  printf("APPL_DISABLE_RINGER_SETTING_NOTIF: %c\n",APPL_DISABLE_RINGER_SETTING_NOTIF);
  printf("APPL_CONFIGURE_RINGER_SILENCE: %c\n",APPL_CONFIGURE_RINGER_SILENCE);
  printf("APPL_CONFIGURE_RINGER_MUTE: %c\n",APPL_CONFIGURE_RINGER_MUTE);
  printf("APPL_CONFIGURE_RINGER_CANCEL_SILENCE: %c\n",APPL_CONFIGURE_RINGER_CANCEL_SILENCE);
  printf("APPL_DISCOVERY_PRIMARY_SERVICE: %c\n",APPL_DISCOVERY_PRIMARY_SERVICE);
#endif       
                                
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  printf("APPL_ENABLE_ALERT_PATH_LOSS: %c\n",APPL_ENABLE_ALERT_PATH_LOSS);
  printf("APPL_DISABLE_ALERT_PATH_LOSS: %c\n",APPL_DISABLE_ALERT_PATH_LOSS);
  printf("APPL_DISCOVERY_PRIMARY_SERVICE: %c\n",APPL_DISCOVERY_PRIMARY_SERVICE);
#endif	  
  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)	
  printf("APPL_GET_REF_TIME_UPDATE: %c\n",APPL_GET_REF_TIME_UPDATE);
  printf("APPL_TIME_UPDATE_NOTIFICATION: %c\n",APPL_TIME_UPDATE_NOTIFICATION);
  printf("APPL_GET_SERV_TIME_UPDATE_STATE: %c\n",APPL_GET_SERV_TIME_UPDATE_STATE);
  printf("APPL_READ_NEXT_DST_CHANGE_TIME: %c\n",APPL_READ_NEXT_DST_CHANGE_TIME);
  printf("APPL_READ_SERV_TIME_UPDATE_STATE: %c\n",APPL_READ_SERV_TIME_UPDATE_STATE);	
  printf("APPL_READ_LOCAL_TIME_INFORM: %c\n",APPL_READ_LOCAL_TIME_INFORM);
  printf("APPL_READ_CURRENT_TIME: %c\n",APPL_READ_CURRENT_TIME);
  printf("APPL_GET_TIME_ACCU_INFO_SERVER: %c\n",APPL_GET_TIME_ACCU_INFO_SERVER);  
  printf("APPL_CANCEL_REF_TIME_UPDATE: %c\n",APPL_CANCEL_REF_TIME_UPDATE);
  printf("APPL_DISCOVERY_PRIMARY_SERVICE: %c\n",APPL_DISCOVERY_PRIMARY_SERVICE); //PTS Automating for Time Client
  printf("APPL_DISCOVERY_CHARS_OF_CURRENT_TIME_SERVICE: %c\n",APPL_DISCOVERY_CHARS_OF_CURRENT_TIME_SERVICE); //PTS Automating for Time Client
  printf("APPL_DISCOVERY_CHARS_OF_NEXT_DS_SERVICE: %c\n",APPL_DISCOVERY_CHARS_OF_NEXT_DS_SERVICE); //PTS Automating for Time Client
  printf("APPL_DISCOVERY_CHARS_OF_REFERENCE_TIME_SERVICE: %c\n",APPL_DISCOVERY_CHARS_OF_REFERENCE_TIME_SERVICE); //PTS Automating for Time Client    
  printf("APPL_DISCOVERY_CHARS_DESCR_OF_CURRENT_TIME: %c\n",APPL_DISCOVERY_CHARS_DESCR_OF_CURRENT_TIME); //PTS Automating for Time Client    
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  printf("APPL_UPDATE_CURRENT_TIME: %c\n",APPL_UPDATE_CURRENT_TIME);
  printf("APPL_UPDATE_LOCAL_TIME: %c\n",APPL_UPDATE_LOCAL_TIME);
  printf("APPL_UPDATE_REFERENCE_TIME: %c\n",APPL_UPDATE_REFERENCE_TIME);
  printf("APPL_UPDATE_NEXT_DSTCHANGE_TIME: %c\n",APPL_UPDATE_NEXT_DSTCHANGE_TIME);
  printf("APPL_UPDATE_REFERENCE_TIME_INFO: %c\n",APPL_UPDATE_REFERENCE_TIME_INFO);	 
  printf("APPL_ADJUST_REASON_MANUAL_TIME_UPDATE: %c\n",APPL_ADJUST_REASON_MANUAL_TIME_UPDATE);
  printf("APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE: %c\n",APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE);
  printf("APPL_ADJUST_REASON_CHANGE_OF_DST: %c\n",APPL_ADJUST_REASON_CHANGE_OF_DST);
  printf("APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: %c\n",APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE);
#endif
  
  APPL_MESG_DBG(profiledbgfile,"DISPLAY_PTS_MENU:  %c\n",DISPLAY_PTS_MENU);
  APPL_MESG_DBG(profiledbgfile,"APPL_SEND_SLAVE_SECURITY_REQUEST: %c\n",APPL_SEND_SLAVE_SECURITY_REQUEST);

  APPL_MESG_DBG(profiledbgfile,"APPL_TERMINATE_LINK: %c\n",APPL_TERMINATE_LINK);
  APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);  
  
    
}/* end Display_Appl_Menu() */


#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)

/* Health Thermometer PTS test case 4.10.1: HTS_SEN_SP_BV_01_C */
void HT_Send_Next_Temperature_measurement(void)
{
  /* check if the sending a second temperature measurement process should be initiated */
  if (ht_check_next_send_temperature_measurement_flag)
  {
    switch (ht_check_next_send_temperature_measurement_state)
    {
      case(HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT):
      {
        if (HTIntervalTimer_expired)
        {
          ht_check_next_send_temperature_measurement_state = HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT_STOP;
          /* send a second measurement */
          HT_Send_Temperature_Measurement(tempMeasurementVal);
        }
      }
      break;
      case(HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT_STOP):
      {
        /* reset state */
        ht_check_next_send_temperature_measurement_state = 0; 
        ht_check_next_send_temperature_measurement_flag = 0; 
      }
      break;
    }
  }
}/* end HT_Send_Next_Temperature_measurement() */

#endif /* BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER */


#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)

void setMedicationUnits_Kg(void)
{
  glucoseDataBase_MeasurementContextRecords[1].flags = GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS;
  glucoseDataBase_MeasurementContextRecords[1].medicationUnits = 0xA032;
  glucose.glucoseDataBase_MeasurementContextRecords_ptr = glucoseDataBase_MeasurementContextRecords;
}

void setMedicationUnits_Liter(void)
{
  glucoseDataBase_MeasurementContextRecords[1].flags = GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS;
  glucoseDataBase_MeasurementContextRecords[1].medicationUnits = 0xD050;
  glucose.glucoseDataBase_MeasurementContextRecords_ptr = glucoseDataBase_MeasurementContextRecords;
}

#endif /* BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR */


#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)

void CGM_Set_CGM_Measurement_Value(tCgmMeasurementVal *measurementRecord)
{
  *measurementRecord = cgmDatabase_MeasurementRecords[0];
}

#endif /* BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR */


/* PTS test application: BLE Stack Initialization + while (1) loop: BLE stack tick, App tick + Sleep mode */
void Host_Profile_Test_Application (void)
{
  uint8_t ret;
  
  /************** BLE Stack Initiliazation ***********************************/
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }

  /* Set device peripheral public address */  
  BLE_Set_Peripheral_Public_Address(public_address);
  
  /* Set tx power level */
  aci_hal_set_tx_power_level(1,4);
  
 /*************** BLE Profiles Initialization *********************************/

#if (BLE_CURRENT_PROFILE_ROLES & (FIND_ME_TARGET|FIND_ME_LOCATOR|PHONE_ALERT_CLIENT))
  //uint8_t bdAddr[6] = {0x82,0xB6,0x05,0xDC,0x1B,0x00};
#endif

  printf ("Test Application: Starting BLE Profiles tests\n" );

  /* initialize the security parameters */
  Security_Param.ioCapability = SPI_HOST_MAIN_IO_CAPABILITY_DISPLAY_ONLY;
  Security_Param.mitm_mode = MITM_PROTECTION_REQUIRED;
  Security_Param.bonding_mode = 0x01;
  Security_Param.Use_Fixed_Pin = 0x00;
  Security_Param.encryptionKeySizeMin = 8;
  Security_Param.encryptionKeySizeMax = 16;
  Security_Param.Fixed_Pin = 111111;
  Security_Param.initiateSecurity = PROFILE_SECURITY; 
  
#if BLE_2_X /* BLE stack FW version >= 2.x*/  
  /*
    - 0x00: Secure Connections Pairing not supported
    - 0x01: Secure Connections Pairing supported but optional
    - 0x02: Secure Connections Pairing supported and mandatory (SC Only Mode)
   */
  Security_Param.SC_Support = 0x01;  
  /*
    - 0x00: Keypress notification not supported
    - 0x01: Keypress notification supported
   */
  Security_Param.KeyPress_Notification_Support = 0x00;
  /*
  - 0x00: Public Identity Address
  - 0x01: Random (static) Identity Address

 */
  Security_Param.Identity_Address_Type = 0x00;
#else  
  Security_Param.OOB_Data_Present = 0x00;
  for (int indx=0;indx<16;indx++)
  {
    Security_Param.OOB_Data[indx] = (uint8_t)indx;
  }
#endif

  /* initialize main profile */
  BLE_Profile_Init(&Security_Param,BLE_Profiles_Evt_Notify_Cb);
  
  printf ("Test Application: BLE main profile initialized. \n" );

  /* initialize the individual profiles */
  Init_Profile();

#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  Init_temperature();
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
  Init_Current_Time();
#endif

  deviceState = APPL_UNINITIALIZED;
  
  /* ***********************************  MAIN LOOP  *************************/
  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();

    /* Add call to profile state machine */    
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
    ANCProfile_StateMachine();
#endif    
    
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
    ANSProfile_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)     
    BPS_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
    FMLProfile_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET)
    FMTProfile_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
    GL_StateMachine();
#endif   
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
    CGM_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
    HT_StateMachine(); 
    if (HTS_SEN_SP_BV_01_C == 1) /* to be enabled only for PTS test 4.10.1: HTS_SEN_SP_BV_01_C */
    {  
      HT_Send_Next_Temperature_measurement(); 
    }
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
    HRProfile_StateMachine();
#endif    

#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
    PACProfile_StateMachine();
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
    ProximityMonitorProfile_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER)
    ProximityReporterProfile_StateMachine();
#endif
    
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)      
    TimeClient_StateMachine();
#endif
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
    TimeServer_StateMachine();
#endif
         
    if(Profile_Process_Q() == 0x00) 
    {      
      /* if the profiles have nothing more to process, then
       * wait for application input
       */ 
      
#ifndef PTS_AUTOMATING
      Enter_Sleep_Mode();
#endif
      
      if(deviceState >= APPL_ADVERTISING) //APPL_CONNECTED
      {    
        uint8_t input = DUMMY_INPUT_VALUE;
        deviceState = input; 
        
        if (__io_getcharNonBlocking(&input))
        {
          printf("io--- input: %c\n",input); 
        
          switch(input)
          {
            case DISPLAY_PTS_MENU:
            case APPL_TERMINATE_LINK:
            case APPL_SEND_SLAVE_SECURITY_REQUEST:
//            case APPL_START_ADVERTISE:
            case APPL_CLEAR_SECURITY_DATABASE:
            case PROFILE_RESET:  
              
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)             
            case APPL_ENABLED_NEW_ALERT_NOTIF:        	
            case APPL_DISABLED_NEW_ALERT_NOTIF:
            case APPL_ENABLED_UNREAD_ALERT_STATUS_NOTIF:
            case APPL_DISABLED_UNREAD_ALERT_STATUS_NOTIF:        	
            case APPL_WRITE_CTRL_POINT:
            case APPL_WRITE_CTRL_POINT_ANPSF_1:
            case APPL_WRITE_CTRL_POINT_ANPSF_2:    
            case APPL_DISCOVERY_PRIMARY_SERVICE:  
#endif
     
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)             
            case APPL_UPDATE_NEW_ALERT:			 	 
            case APPL_UPDATE_UNREAD_ALERT_STATUS: 	
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
            case APPL_SEND_BP_MEASUREMENT:            
            case APPL_SEND_INTERMEDIATE_CUFF_PRESSURE: 
#endif
  
#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
            case APPL_LOCATOR_NO_ALERT_TARGET:  
            case APPL_LOCATOR_ALERT_TARGET:   
            case APPL_DISCOVERY_PRIMARY_SERVICE:
#endif
 
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
            case APPL_GLUCOSE_CHANGE_DB_1:
            case APPL_GLUCOSE_CHANGE_DB_2:
            case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1:
            case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3:
            case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4:
            case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5:
            case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9:  
            case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200:  
            case APPL_SET_MEDICATION_UNITS_KG:
            case APPL_SET_MEDICATION_UNITS_LITER:
            case APPL_SET_MULTIPLE_BOND:  
#endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */  
              
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
            case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_3:
            case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_5:
            case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_10:
            case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_20:
            case APPL_SET_NO_START_SESSION:  
            case APPL_SET_START_SESSION:   
            case APPL_SET_NO_MULTIPLE_SESSION:  
            case APPL_SET_MULTIPLE_SESSION: 
            case APPL_SET_NO_CGM_MULTIPLE_BOND:  
            case APPL_SET_CGM_MULTIPLE_BOND:
            case APPL_SET_NO_E2E_CRC_SUPPORT:
            case APPL_SET_E2E_CRC_SUPPORT:
            case APPL_SET_NO_PATIENT_ALERT_SUPPORT:
            case APPL_SET_PATIENT_ALERT_SUPPORT: 
            case APPL_SET_NO_HYPO_ALERT_SUPPORT:
            case APPL_SET_HYPO_ALERT_SUPPORT:
            case APPL_SET_NO_HYPER_ALERT_SUPPORT:
            case APPL_SET_HYPER_ALERT_SUPPORT:   
            case APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT:
            case APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT:
            case APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT:
            case APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT:
            case APPL_SET_NO_CALIBRATION_SUPPORT:
            case APPL_SET_CALIBRATION_SUPPORT:
            case APPL_SET_NO_ALL_FEATURE_SUPPORT:  
            case APPL_SET_ALL_FEATURE_SUPPORT: 
#endif /* (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR) */ 

              
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
            case APPL_SEND_HT_MEASUREMENT:             
            case APPL_UPDATE_MEASUREMENT_INTERVAL:  
            case APPL_SEND_HT_INTERMEDIATE_MEASUREMENT:
            case APPL_SET_HTS_SEN_SP_BV_01_C:  
            case APPL_UNSET_HTS_SEN_SP_BV_01_C:  
#endif
              
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)         
            case APPL_SEND_HEART_RATE_MEASUREMENT_WITHOUT_SENSOR_CONTACT: 
            case APPL_SEND_HEART_RATE_MEASUREMENT_WITH_SENSOR_CONTACT:
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
            case APPL_PHONE_READ_ALERT_STATUS:		 
            case APPL_PHONE_READ_RINGER_SETTING:
            case APPL_DISABLE_ALERT_STATUS_NOTIF:		 
            case APPL_DISABLE_RINGER_SETTING_NOTIF:	
            case APPL_CONFIGURE_RINGER_SILENCE:
            case APPL_CONFIGURE_RINGER_MUTE:
            case APPL_CONFIGURE_RINGER_CANCEL_SILENCE: 	 
            case APPL_DISCOVERY_PRIMARY_SERVICE:  
#endif
 
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
            case APPL_ENABLE_ALERT_PATH_LOSS:
            case APPL_DISABLE_ALERT_PATH_LOSS:
            case APPL_DISCOVERY_PRIMARY_SERVICE:
#endif	
              
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)				
            case APPL_GET_REF_TIME_UPDATE:		
            case APPL_TIME_UPDATE_NOTIFICATION:	
            case APPL_GET_SERV_TIME_UPDATE_STATE: 
            case APPL_READ_NEXT_DST_CHANGE_TIME: 		 
            case APPL_READ_LOCAL_TIME_INFORM:		 	
            case APPL_READ_CURRENT_TIME:				 
            case APPL_GET_TIME_ACCU_INFO_SERVER:	
            case APPL_CANCEL_REF_TIME_UPDATE:	 
            case APPL_DISCOVERY_PRIMARY_SERVICE:  //PTS Automating for Time Client
            case APPL_DISCOVERY_CHARS_OF_CURRENT_TIME_SERVICE:   //PTS Automating for Time Client
            case APPL_DISCOVERY_CHARS_OF_NEXT_DS_SERVICE:        //PTS Automating for Time Client
            case APPL_DISCOVERY_CHARS_OF_REFERENCE_TIME_SERVICE: //PTS Automating for Time Client
            case APPL_DISCOVERY_CHARS_DESCR_OF_CURRENT_TIME:
#endif
              
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
            case APPL_UPDATE_CURRENT_TIME:            
            case APPL_UPDATE_LOCAL_TIME:				 
            case APPL_UPDATE_REFERENCE_TIME:			 
            case APPL_UPDATE_NEXT_DSTCHANGE_TIME:		 
            case APPL_UPDATE_REFERENCE_TIME_INFO:	
            case APPL_DECREASE_CURRENT_TIME:
            case APPL_UPDATE_CURRENT_TIME_MORE_THAN_15:
            case APPL_ADJUST_REASON_MANUAL_TIME_UPDATE:
            case APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE: 
            case APPL_ADJUST_REASON_CHANGE_OF_DST:     
            case APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE:  
#endif
            {
              deviceState = input;
            }
            break;
       
            default:
            {
            }
            break; //continue
          }/* end switch(input) */
        } /* end if _IO.... */
      }/* end if(deviceState >= APPL_CONNECTED) */
    }/* end if(Profile_Process_Q() == 0x00) */
                
    /* application specific processing */
		
    switch(deviceState)
    { 
      /* General states/inputs */      
      case APPL_INIT_DONE:
      {
        sleep_mode = SLEEPMODE_WAKETIMER; 
        Advertize();
        deviceState = APPL_ADVERTISING;
      }
      break;
      case PROFILE_RESET:
      {
        Profile_Reset();
      }
      break;
      case DISPLAY_PTS_MENU:
      {
        Display_Appl_Menu();
      }
      break;
      
      case APPL_TERMINATE_LINK:
      {
        /* on disconnection complete, the state will be changed to INIT_DONE, 
           through EVT_MP_DISCONNECTION_COMPLETE raised by profile_disconnection_complete_event()
         */ 
        PROFILE_MESG_DBG(profiledbgfile,"Call BLE_Profile_Disconnect\n"); 
        BLE_Profile_Disconnect();
        deviceState = APPL_UNINITIALIZED;
      }
      break;
      
      case APPL_SEND_SLAVE_SECURITY_REQUEST:
      { 
        APPL_MESG_DBG(profiledbgfile,"profiles_test_application: call BLE_Profile_Send_Security_Request() \n");
        BLE_Profile_Send_Security_Request();
      }
      break; 
      
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call aci_gap_clear_security_database()\n"); 
        aci_gap_clear_security_db();
      }
      break;
      
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
      case APPL_ENABLED_NEW_ALERT_NOTIF:
      {
        ANC_Enable_Disable_New_Alert_Notification(0x0001);               
      }			 	 
      break;
      case APPL_DISABLED_NEW_ALERT_NOTIF:
      {
        ANC_Enable_Disable_New_Alert_Notification(0x0000); //0x0000 for test ANP/CL/ANPCF/BV-02-I              
      }			 	 
      break;      
      case APPL_ENABLED_UNREAD_ALERT_STATUS_NOTIF: 
      {
        ANC_Enable_Disable_Unread_Alert_Status_Notification(0x0001);
      }
      break;
      case APPL_DISABLED_UNREAD_ALERT_STATUS_NOTIF: 
      {
        ANC_Enable_Disable_Unread_Alert_Status_Notification(0x0000); //0x0000 for test ANP/CL/ANPCF/BV-04-I
      }
      break;      
      case APPL_WRITE_CTRL_POINT:
      {
        /*
        * Test ANP/CL/ANPNF/BV-01-C command = ENABLE_NEW_ALERT_NOTIFICATION          category = CATEGORY_ID_EMAIL
        * Test ANP/CL/ANPNF/BV-02-I command = ENABLE_NEW_ALERT_NOTIFICATION          category = CATEGORY_ID_EMAIL
        */
        ANC_Write_Control_Point(ENABLE_NEW_ALERT_NOTIFICATION, CATEGORY_ID_EMAIL);       //default        
      }
      break;     
      case APPL_WRITE_CTRL_POINT_ANPSF_1:
      {
        /*
        * Test ANP/CL/ANPSF/BV-01-I command = NOTIFY_NEW_ALERT_IMMEDIATELY           category = CATEGORY_ID_ALL
        */
        ANC_Write_Control_Point(NOTIFY_NEW_ALERT_IMMEDIATELY, CATEGORY_ID_ALL);          //Test ANP/CL/ANPSF/BV-01-I         
      }
      break; 
      case APPL_WRITE_CTRL_POINT_ANPSF_2:
      {
        /*
        * Test ANP/CL/ANPSF/BV-02-I command = NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY category = CATEGORY_ID_ALL
        */
        ANC_Write_Control_Point(NOTIFY_UNREAD_ALERT_STATUS_IMMEDIATELY, CATEGORY_ID_ALL); //Test ANP/CL/ANPSF/BV-02-I        
      }
      break;       
      case APPL_DISCOVERY_PRIMARY_SERVICE: 
      {
        ANC_Discovery_Primary_Serv();        
      }
      break;
#endif
      
#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER)
      case APPL_UPDATE_NEW_ALERT:
      {
        tTextInfo textInfo;
        uint8_t str[4] = {'J','O','H','N'};
        APPL_MESG_DBG(profiledbgfile,"ALERT_NOTIFICATION_SERVER\n");
        textInfo.len = 0x04;
        textInfo.str = str;
        ANS_Update_New_Alert(CATEGORY_ID_EMAIL,0x01,textInfo);
  
      }			 	 
      break;
      case APPL_UPDATE_UNREAD_ALERT_STATUS: 
      {
        ANS_Update_Unread_Alert_Status(CATEGORY_ID_SMS_MMS,0x01);			  
      }
      break;
#endif
      
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
      case APPL_SEND_BP_MEASUREMENT:
      {
        tBPMValue bpmval;
        
        bpmval.flags = (FLAG_TIME_STAMP_PRESENT|
                        FLAG_PULSE_RATE_PRESENT|
                        FLAG_USER_ID_PRESENT|
                        FLAG_MEASUREMENT_STATUS_PRESENT);
        bpmval.systolic = 0xAAAA;
        bpmval.diastolic = 0xBBBB;
        bpmval.map = 0xCCCC;
        bpmval.pulseRate = 80;
        bpmval.userID = 0x01;
        bpmval.timeStamp.year = 2012;
        bpmval.timeStamp.month = 10;
        bpmval.timeStamp.day = 26;
        bpmval.timeStamp.hours = 19;
        bpmval.timeStamp.minutes = 42;
        bpmval.timeStamp.seconds = 50;
        bpmval.measurementStatus.bodyMovementDetectionFlag = 0x01;
        bpmval.measurementStatus.cuffFitDetectionFlag = 0x01;
        bpmval.measurementStatus.measurementPositionDetectionFlag = 0x01;
       
        APPL_MESG_DBG(profiledbgfile,"Sending blood pressure measurment\n");
        BPS_Send_Blood_Pressure_Measurement(bpmval);
      }
      break;
      case APPL_SEND_INTERMEDIATE_CUFF_PRESSURE:
      {
        tICPValue icpval;
        APPL_MESG_DBG(profiledbgfile,"Sending intermediate cuff pressure measurment\n");
        icpval.flags = (FLAG_USER_ID_PRESENT|
                        FLAG_MEASUREMENT_STATUS_PRESENT);
                       // FLAG_TIME_STAMP_PRESENT|
                       // FLAG_PULSE_RATE_PRESENT|
                        
        icpval.icp = 0xAAAA;
        icpval.userID = 0x01;
        icpval.measurementStatus.bodyMovementDetectionFlag = 0x01;
        icpval.measurementStatus.cuffFitDetectionFlag = 0x01;
        icpval.measurementStatus.measurementPositionDetectionFlag = 0x01;
        BPS_Send_Intermediate_Cuff_Pressure(icpval);
      }
      break;
#endif

#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
      case APPL_LOCATOR_NO_ALERT_TARGET:
      {
        FML_ALert_Target(0x00);// Set to 0 for test TC_FMF_FL_BV_02_I
      }
      break;
      case APPL_LOCATOR_ALERT_TARGET:
      {
        FML_ALert_Target(0x01);// Set to 1 for test TC_FMF_FL_BV_01_I. 
      }
      break; 
      case APPL_DISCOVERY_PRIMARY_SERVICE: 
      {
        FindMeLocator_Discovery_Primary_Serv();        
      }
      break;
#endif
                    
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
      case APPL_GLUCOSE_CHANGE_DB_1: 
      {
        APPL_MESG_DBG(profiledbgfile,"PTS GLS test 4.10.1 SPD BV_01: call glucoseDatabase_Change_Database_Manually \n");
        /* Change database manually: store new sequence number and reset flags field on 
        current database measurement for simulating new records measurements */
        glucoseDatabase_Change_Database_Manually(1, SET_FLAG); /* PTS GLS test 4.10.1 SPD BV_01 */
      }
      break;
      case APPL_GLUCOSE_CHANGE_DB_2: 
      {
        APPL_MESG_DBG(profiledbgfile,"PTS GLS test 4.12.3 SPN BV_03: call glucoseDatabase_Change_Database_Manually \n");
        /* Change database manually: store new sequence number and reset flags field on 
        current database measurement for simulating new records measurements */
        glucoseDatabase_Change_Database_Manually(3, UNSET_FLAG); /* PTS GLS test 4.12.2 SPN BV_03 */
      }
      break;
    case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_1\n");
        glucose.max_number_of_used_records = 1;
        glucose.sequenceNumber = 1;
      }
      break;
      
    case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_3\n");
        glucose.max_number_of_used_records = 3;
        glucose.sequenceNumber = 3;
      }
      break;
    case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_4\n");
        glucose.max_number_of_used_records = 4;
        glucose.sequenceNumber = 4;
      }
      break;
    case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_5\n");
        glucose.max_number_of_used_records = 5;
        glucose.sequenceNumber = 5;
      }
      break;
    case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_9\n");
        glucose.max_number_of_used_records = 9;
        glucose.sequenceNumber = 9;
      }
      break;    
    case APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_GLUCOSE_PTS_NUMBER_OF_RECORDS_200\n");
        glucose.max_number_of_used_records = 200;
        glucose.sequenceNumber = 200;
      }
      break;   
      case APPL_SET_MEDICATION_UNITS_KG:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_MEDICATION_UNITS_KG\n");
        setMedicationUnits_Kg();
      }
      break;
     case APPL_SET_MEDICATION_UNITS_LITER:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_MEDICATION_UNITS_LITER\n");
        setMedicationUnits_Liter();
      }
      break;
      case APPL_SET_MULTIPLE_BOND:
      {
        APPL_MESG_DBG(profiledbgfile,"Glucose Feature set to Multiple Bond\n");  
        /* Set Glucose Feature characteristic value: all features except multi bond */
        GL_Set_Glucose_Feature_Value (GLUCOSE_FEATURE_ALL_SUPPORTED); //for multiple bonds
        /* Start advertising (no whitelist, fast) */
      }
      break;
#endif /* (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR) */
      
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
      case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_3:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_3\n");
        cgm.max_number_of_used_records = 3;
      }
      break;
    case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_5:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_5\n");
        cgm.max_number_of_used_records = 5;
      }
      break;
    case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_10:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_10\n");
        cgm.max_number_of_used_records = 10;
      }
      break;
    case APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_20:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_CGM_PTS_NUMBER_OF_RECORDS_20\n");
        cgm.max_number_of_used_records = 20;
      }
      break;
    case APPL_SET_NO_START_SESSION:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_START_SESSION\n");
        bStartSession = FALSE;
      }
      break;    
    case APPL_SET_START_SESSION:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_START_SESSION\n");
        bStartSession = TRUE;
      }
      break;
    case APPL_SET_NO_MULTIPLE_SESSION:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_MULTIPLE_SESSION\n");
        bMultipleSession = FALSE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_MULTIPLE_SESSION:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_MULTIPLE_SESSION\n");
        bMultipleSession = TRUE;
        Set_Feature_Value();
      }
      break;     
    case APPL_SET_NO_CGM_MULTIPLE_BOND:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_CGM_MULTIPLE_BOND\\n");
        bMultipleBond = FALSE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_CGM_MULTIPLE_BOND:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_CGM_MULTIPLE_BOND\n");
        bMultipleBond = TRUE;
        Set_Feature_Value();
      }
      break;       
    case APPL_SET_NO_E2E_CRC_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_E2E_CRC_SUPPORT\n");
        bE2E_CRC_Support = FALSE;
//        bMultipleSession = TRUE;      //TBR???
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_E2E_CRC_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_E2E_CRC_SUPPORT\n");
        bE2E_CRC_Support = TRUE;
//        bMultipleSession = FALSE;     //TBR???
        Set_Feature_Value();
      }
      break; 
      case APPL_SET_NO_PATIENT_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_PATIENT_ALERT_SUPPORT\n");
        bPatientHighLowAlert_Support = FALSE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_PATIENT_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_PATIENT_ALERT_SUPPORT\n");
        bPatientHighLowAlert_Support = TRUE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_NO_HYPO_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_HYPO_ALERT_SUPPORT\n");
        bHypoAlert_Support = FALSE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_HYPO_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_HYPO_ALERT_SUPPORT\n");
        bHypoAlert_Support = TRUE;
        Set_Feature_Value();
      }
      break;   
    case APPL_SET_NO_HYPER_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_HYPER_ALERT_SUPPORT\n");
        bHyperAlert_Support = FALSE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_HYPER_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_HYPER_ALERT_SUPPORT\n");
        bHyperAlert_Support = TRUE;
        Set_Feature_Value();
      }
      break;       
    case APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_RATE_INCR_DECR_ALERT_SUPPORT\n");
        bRateIncreaseDecreaseAlert_Support = FALSE;
        Set_Feature_Value();
      }
      break;    
    case APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_RATE_INCR_DECR_ALERT_SUPPORT\n");
        bRateIncreaseDecreaseAlert_Support = TRUE;
        Set_Feature_Value();
      }
      break;    

    case APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_DEVICE_SPECIFIC_ALERT_SUPPORT\n");
        bDeviceSpecificAlert_Support = FALSE;
        Set_Feature_Value();
      }
      break;  
    case APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_DEVICE_SPECIFIC_ALERT_SUPPORT\n");
        bDeviceSpecificAlert_Support = TRUE;
        Set_Feature_Value();
      }
      break;   
      
     case APPL_SET_NO_CALIBRATION_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_CALIBRATION_SUPPORT\n");
        bCalibration_Support = FALSE;
        Set_Feature_Value();
      }
      break;  
    case APPL_SET_CALIBRATION_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_CALIBRATION_SUPPORT\n");
        bCalibration_Support = TRUE;
        Set_Feature_Value();
      }
      break; 
      
    case APPL_SET_NO_ALL_FEATURE_SUPPORT:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_NO_ALL_FEATURE_SUPPORT\n");
        bAllFeatureSupport_Support = FALSE;
        Set_Feature_Value();
      }
      break;       
    case APPL_SET_ALL_FEATURE_SUPPORT:       
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_SET_ALL_FEATURE_SUPPORT\n");
        bAllFeatureSupport_Support = TRUE;
        Set_Feature_Value();
      }
      break;       
#endif /* (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR) */
      
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
      case APPL_SEND_HT_MEASUREMENT:
      {
        printf("sending ht measurement\n");
        if(tempMeasurementVal.timeStamp.minutes == 60)
        {
          tempMeasurementVal.timeStamp.minutes = 0;
          tempMeasurementVal.timeStamp.hours+=1;
        }
        HT_Send_Temperature_Measurement(tempMeasurementVal);
        if (HTS_SEN_SP_BV_01_C == 1) /* to be enabled only for PTS test 4.10.1: HTS_SEN_SP_BV_01_C */
        {
          ht_check_next_send_temperature_measurement_flag = 1;
          ht_check_next_send_temperature_measurement_state = HT_CONNECTED_DO_MEASUREMENT_INTERVAL_WAIT;
        }
      }
      break;
      case APPL_SEND_HT_INTERMEDIATE_MEASUREMENT:
      {
        printf("sending ht intermediate measurement\n");
        if(tempMeasurementVal.timeStamp.minutes == 60)
        {
          tempMeasurementVal.timeStamp.minutes = 0;
          tempMeasurementVal.timeStamp.hours+=1;
        }
        HT_Send_Intermediate_Temperature(tempMeasurementVal);
      }
      break;
      case APPL_UPDATE_MEASUREMENT_INTERVAL:
      {
        APPL_MESG_DBG(profiledbgfile,"Call HT_Update_Measurement_Interval()\n");
        HT_Update_Measurement_Interval(30); 
      }
      break;      
      case APPL_SET_HTS_SEN_SP_BV_01_C:
      {
        APPL_MESG_DBG(profiledbgfile,"SET HTS_SEN_SP_BV_01_C\n");
        HTS_SEN_SP_BV_01_C = 1; 
      }
      break;
      case APPL_UNSET_HTS_SEN_SP_BV_01_C:
      {
        APPL_MESG_DBG(profiledbgfile,"UNSET HTS_SEN_SP_BV_01_C\n");
        HTS_SEN_SP_BV_01_C = 0; 
      }
      break;
#endif
      
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
      case APPL_SEND_HEART_RATE_MEASUREMENT_WITHOUT_SENSOR_CONTACT:
      {
        heartRateMeasurmentValue.valueformat = HRM_VALUE_FORMAT_UINT16;            
        heartRateMeasurmentValue.sensorContact = HRM_SENSOR_CONTACT_NOT_PRESENT;
        heartRateMeasurmentValue.energyExpendedStatus = HRM_ENERGY_EXPENDED_PRESENT;
        heartRateMeasurmentValue.rrIntervalStatus = HRM_RR_INTERVAL_PRESENT;
        heartRateMeasurmentValue.heartRateValue = (uint8_t)0xE4;
        heartRateMeasurmentValue.enrgyExpended = 0x00;
        heartRateMeasurmentValue.numOfRRIntervalvalues = 0x05;
        for (int indx=0; indx<9; indx++)
        {
          heartRateMeasurmentValue.rrIntervalValues[indx] = (uint16_t)(indx + 0);
        }
        HRProfile_Send_HRM_Value (heartRateMeasurmentValue);
        deviceState = APPL_WAIT;
      }
      break;
      case APPL_SEND_HEART_RATE_MEASUREMENT_WITH_SENSOR_CONTACT:
      {
        HRProfile_Set_Sensor_Contact_Support_Bit(); //for test case TC_CN_BV_05
        heartRateMeasurmentValue.valueformat = HRM_VALUE_FORMAT_UINT16;
        heartRateMeasurmentValue.sensorContact = HRM_SENSOR_CONTACT_PRESENT;

        heartRateMeasurmentValue.energyExpendedStatus = HRM_ENERGY_EXPENDED_PRESENT;
        heartRateMeasurmentValue.rrIntervalStatus = HRM_RR_INTERVAL_PRESENT;
        heartRateMeasurmentValue.heartRateValue = (uint8_t)0xE4;
        heartRateMeasurmentValue.enrgyExpended = 0x00;
        heartRateMeasurmentValue.numOfRRIntervalvalues = 0x05;
        for (int indx=0; indx<9; indx++)
        {
          heartRateMeasurmentValue.rrIntervalValues[indx] = (uint16_t)(indx + 0);
        }
        HRProfile_Send_HRM_Value (heartRateMeasurmentValue);
        deviceState = APPL_WAIT;
      }
      break;
#endif
      
#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
      case APPL_PHONE_READ_ALERT_STATUS:		 
      {
        PAC_Read_AlertStatus();
      }
      break;
      case APPL_PHONE_READ_RINGER_SETTING:	
      {
        PAC_Read_RingerSetting(); 
      }	
      break;
      case APPL_DISABLE_ALERT_STATUS_NOTIF:	
      {
        PAC_Disable_ALert_Status_Notification();
      }
      break;
      case APPL_DISABLE_RINGER_SETTING_NOTIF:
      {
        PAC_Disable_Ringer_Status_Notification();
      }
      break;	
      case APPL_CONFIGURE_RINGER_SILENCE:
      {
        PAC_Configure_Ringer(0x01);
      }
      break;
      case APPL_CONFIGURE_RINGER_MUTE:
      {
        PAC_Configure_Ringer(0x02); 
      }
      break;
      case APPL_CONFIGURE_RINGER_CANCEL_SILENCE: 
      {
        PAC_Configure_Ringer(0x03);
      }
      break;
      
      case APPL_DISCOVERY_PRIMARY_SERVICE: 
      {
        PAC_Discovery_Primary_Serv();        
      }
      break;  
#endif	
    
#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
      case APPL_ENABLE_ALERT_PATH_LOSS:
      {
        printf("ProximityMonitor_Notify_Path_Loss() .......check 1........\n       ");
        ProximityMonitor_Notify_Path_Loss(0x01);
      }
      break;
      case APPL_DISABLE_ALERT_PATH_LOSS:
      {
        ProximityMonitor_Notify_Path_Loss(0x00);
      }
      break;
      case APPL_DISCOVERY_PRIMARY_SERVICE: 
      {
        ProximityMonitor_Discovery_Primary_Serv();        
      }
      break; 
#endif	
      
#if (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)	
      case APPL_GET_REF_TIME_UPDATE:	
      {
        TimeClient_Update_Reference_Time_On_Server(0x01);
      }	
      break;
      case APPL_CANCEL_REF_TIME_UPDATE:	
      {
        TimeClient_Update_Reference_Time_On_Server(0x02);
      }	
      break;
      case APPL_TIME_UPDATE_NOTIFICATION:
      {
        TimeClient_SetReset_Time_Update_Notification(0x01);
      }	
      break;
      case APPL_GET_SERV_TIME_UPDATE_STATE: 
      {
        TimeClient_Get_Server_Time_Update_State();
      }
      break;
      case APPL_READ_NEXT_DST_CHANGE_TIME: 		 
      {
        TimeClient_Get_Next_DST_Change_Time();
      }
      break;
      case APPL_READ_LOCAL_TIME_INFORM:
      {
        TimeClient_Get_Local_Time_Information();
      }
      break;
      case APPL_READ_CURRENT_TIME:
      {
        TimeClient_Get_Current_Time();
      }
      break;
      case APPL_GET_TIME_ACCU_INFO_SERVER:	
      {
        TimeClient_Get_Time_Accuracy_Info_Of_Server();
      }
      break;
      
      case APPL_DISCOVERY_PRIMARY_SERVICE:                  //PTS Automating for Time Client
      {
//         b_appl_discovery_primary_serv = TRUE;
         TimeClient_Discovery_Primary_Serv();
      }
      break;
      case APPL_DISCOVERY_CHARS_OF_CURRENT_TIME_SERVICE:    //PTS Automating for Time Client
      {
        timeClient.subState = TIME_CLIENT_STATE_DISCOVERED_ALL_PRIMARY_SERVICES;  
        TimeClient_GATT_Procedure_Complete_EvtHandler (0);
      }
      break;
      case APPL_DISCOVERY_CHARS_OF_NEXT_DS_SERVICE:        //PTS Automating for Time Client
      {
        timeClient.subState = TIME_CLIENT_STATE_DISCOVERED_ALL_DESCRIPTORS_OF_CURRENT_TIME_CHAR;  
        TimeClient_GATT_Procedure_Complete_EvtHandler (0);
      }  
      break;
      case APPL_DISCOVERY_CHARS_OF_REFERENCE_TIME_SERVICE:  //PTS Automating for Time Client
      {
        timeClient.subState = TIME_CLIENT_STATE_DISCOVERED_CHARS_OF_NEXT_DST_CHANGE_SERVICE;  
        TimeClient_GATT_Procedure_Complete_EvtHandler (0);
      }       
      break;
      case APPL_DISCOVERY_CHARS_DESCR_OF_CURRENT_TIME:      //PTS Automating for Time Client
      {
        timeClient.subState = TIME_CLIENT_STATE_DISCOVERED_CHARS_OF_CURRENT_TIME_SERVICE;  
        TimeClient_GATT_Procedure_Complete_EvtHandler (0);
      }       
      break;
      
#endif
      
#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER)
      case APPL_UPDATE_CURRENT_TIME:
      {
        APPL_MESG_DBG(profiledbgfile,"updating current time (minute: %d) as +2 minutes\n", time.minutes); 
        
        //New code for PTS tests including also PTS TEST TC_CSP_BV_06_C */
        if ((time.minutes + 2) >= 60)
        {
          time.minutes = (time.minutes + 2) - 60;
          time.hours+=1;
        }
        else
          time.minutes += 2;

        APPL_MESG_DBG(profiledbgfile,"updating current time status is %d\n", TimeServer_Update_Current_Time_Value(time));
        //deviceState = APPL_WAIT;
      }
      break;
      
      case APPL_DECREASE_CURRENT_TIME: //New code only for PTS TEST TC_CSP_BV_06_C 
      {
        /* Adjusting time less than 1 minute from previous one */
        APPL_MESG_DBG(profiledbgfile,"updating current time (minute: %d) less than 1 minute (decrease 1 second)\n",time.minutes);
        /* Just decrease current time of 1 second: required for PTS test TC_CSP_BV_06_C */
        time.seconds -= 1;

        APPL_MESG_DBG(profiledbgfile,"updating current time status is %d\n", TimeServer_Update_Current_Time_Value(time)); 
      }
      break;
      case APPL_UPDATE_CURRENT_TIME_MORE_THAN_15://New code (only for PTS TEST TC_CSP_BV_06_C)
      {
        APPL_MESG_DBG(profiledbgfile,"updating current time (minute: %d) as +15 minutes\n", time.minutes);
        if ((time.minutes + 15) >= 60)
        {
          time.minutes = (time.minutes + 15) - 60;
          time.hours+=1;
        }
        else
          time.minutes += 15;
        APPL_MESG_DBG(profiledbgfile,"updating current time status is %d\n", TimeServer_Update_Current_Time_Value(time));
      }
      break;
        
      case APPL_UPDATE_LOCAL_TIME:
      {
        tLocalTimeInformation localTimeInfo;
        localTimeInfo.timeZone = 0x03;
        localTimeInfo.dstOffset = 0x12;
        TimeServer_Update_Local_Time_Information(localTimeInfo);
      }
      break;
      case APPL_UPDATE_NEXT_DSTCHANGE_TIME:
      {
        tTimeWithDST timeDST;
        timeDST.date = 06;
        timeDST.year = 2000;
        timeDST.month = 04;
        timeDST.hours = 1;
        timeDST.minutes = 1;
        timeDST.seconds = 4;
                      
        TimeServer_Update_Next_DST_Change(timeDST);
      }
      break;
      case APPL_UPDATE_REFERENCE_TIME_INFO:
      {
        tReferenceTimeInformation refTimeInfo;
        refTimeInfo.source = 0x01;	
        refTimeInfo.accuracy = 0x02;	 
        refTimeInfo.daysSinceUpdate = 0x05;	 
        refTimeInfo.hoursSinceUpdate = 0x03;	 
        TimeServer_Update_Reference_Time_Information(refTimeInfo);
              
      }
      break;
      
      case APPL_ADJUST_REASON_MANUAL_TIME_UPDATE:
      {
        time.adjustReason = 0x01; /* ADJUST_REASON_MANUAL_TIME_UPDATE: CTS/SR/CSP/BV-02-C */
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_MANUAL_TIME_UPDATE (time.adjustReason = 0x01)\n"); 
      }
      break;
      
      case APPL_ADJUST_REASON_CHANGE_OF_TIME_ZONE:
      {
        time.adjustReason = 0x04; /* ADJUST_REASON_CHANGE_OF_TIME_ZONE: CTS/SR/CSP/BV-03-C */
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_CHANGE_OF_TIME_ZONE (time.adjustReason = 0x04)\n"); 
      }
      break;
      case APPL_ADJUST_REASON_CHANGE_OF_DST:
      {
        time.adjustReason = 0x08; /* ADJUST_REASON_CHANGE_OF_DST:     CTS/SR/CSP/BV-04-C */
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_CHANGE_OF_DST (time.adjustReason = 0x08)\n"); 
      }
      break;
      case APPL_ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE:
      {
        time.adjustReason = 0x02; /* ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE: CTS/SR/CSP/BV-05-C, CTS/SR/CSP/BV-06-C*/
        APPL_MESG_DBG(profiledbgfile,"ADJUST_REASON_EXTERNAL_REFERENCE_TIME_UPDATE (time.adjustReason = 0x02)\n"); 
      }
      break;
#endif
                
    }/* end switch(devicestate) */
  } /* end while(1) */
}/* end Host_Profile_Test_Application() */

/** \endcond 
 */
