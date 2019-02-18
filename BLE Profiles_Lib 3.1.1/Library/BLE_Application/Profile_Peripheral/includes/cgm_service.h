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
*   FILENAME        -  cgm_service.h
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
*   Comments:    cgm service for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the cgm service, characteristics
* 
******************************************************************************/

#ifndef _CGM_SERVICE_H_
#define _CGM_SERVICE_H_

/******************************************************************************
* Macro Declarations
******************************************************************************/

/******** Cgm measurement characteristic *********************************/

/* CGM Measurement: values for Flags (8 bit) field (to be used for 
   selecting presence of characteristic optional fields) */
#define CGM_MEASUREMENT_FLAGS_TREND_IS_PRESENT                                (0x01) /* bit 0 correspond to bit 15 of CGM Featue Field */
#define CGM_MEASUREMENT_FLAGS_QUALITY_IS_PRESENT                              (0x02) /* bit 1 correspond to bit 16 of CGM Featue Field */
#define CGM_MEASUREMENT_FLAGS_STATUS_ANNUNCIATION_WARNING_IS_PRESENT          (0x20) /* bit 5 */
#define CGM_MEASUREMENT_FLAGS_STATUS_ANNUNCIATION_CAL_TEMP_IS_PRESENT         (0x40) /* bit 6 */
#define CGM_MEASUREMENT_FLAGS_STATUS_ANNUNCIATION_STATUS_IS_PRESENT           (0x80) /* bit 7 */

/* Flags all present: (to be used for selecting all the characteristic optional fields) */
#define CGM_MEASUREMENT_FLAGS_ALL_PRESENT (CGM_MEASUREMENT_FLAGS_TREND_IS_PRESENT |         \
                                           CGM_MEASUREMENT_FLAGS_QUALITY_IS_PRESENT |       \
                                           CGM_MEASUREMENT_FLAGS_STATUS_ANNUNCIATION_WARNING_IS_PRESENT | \
                                           CGM_MEASUREMENT_FLAGS_STATUS_ANNUNCIATION_CAL_TEMP_IS_PRESENT | \
                                           CGM_MEASUREMENT_FLAGS_STATUS_ANNUNCIATION_STATUS_IS_PRESENT  )

/* CGM Measurement: list of available Key Values for Sensor Status Annunciation field  */
/* CGM Measurement Sensor Status Annunciation */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SESSION_STOPPED                  (0x01) /* Status: Session Stopped */
#define CGM_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW               (0x02) /* Status: Device Battery Low */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_TYPE_INCORRECT_FOR_DEVICE (0x04) /* Status: Sensor type incorrect for device */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION               (0x08) /* Status: Sensor malfunction */
#define CGM_SENSOR_STATUS_ANNUNCIATION_DEVICE_SPECIFIC_ALERT            (0x10) /* Status: Device Specific Alert */
#define CGM_SENSOR_STATUS_ANNUNCIATION_GENERAL_DEVICE_FAULT             (0x20) /* Status: General device fault has occurred in the sensor */

#define CGM_SENSOR_STATUS_ANNUNCIATION_TIME_SYNCHRONIZATION             (0x01) /* Cal/Temp: Time synchronization between sensor and collector required */
#define CGM_SENSOR_STATUS_ANNUNCIATION_CALIBRATION_NOT_ALLOWED          (0x02) /* Cal/Temp: Calibration not allowed  */
#define CGM_SENSOR_STATUS_ANNUNCIATION_CALIBRATION_RECOMMENDED          (0x04) /* Cal/Temp: Calibration recommended */
#define CGM_SENSOR_STATUS_ANNUNCIATION_CALIBRATION_REQUIRED             (0x08) /* Cal/Temp: Calibration required  */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_TEMPERATURE_TOO_HIGH      (0x10) /* Cal/Temp: Sensor Temperature too high for valid test/result at time of measurement */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_TEMPERATURE_TOO_LOW       (0x20) /* Cal/Temp: Sensor temperature too low for valid test/result at time of measurement  */

#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_LOWER_PATIENT_LOW_LEVEL   (0x01) /* Warning: Sensor result lower than the Patient Low level */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_HIGHER_PATIENT_HIGH_LEVEL (0x02) /* Warning: Sensor result higher than the Patient High level */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_LOWER_HYPO_LEVEL          (0x04) /* Warning: Sensor result lower than the Hypo level */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_HIGHER_HYPER_LEVEL        (0x08) /* Warning: Sensor result higher than the Hyper level */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_RATE_DECREASE_EXCEEDED    (0x10) /* Warning: Sensor Rate of Decrease exceeded */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_RATE_INCREASE_EXCEEDED    (0x20) /* Warning: Sensor Rate of Increase exceeded */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_LOWER_DEVICE              (0x40) /* Warning: Sensor result lower than the device can process */
#define CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_HIGHER_DEVICE             (0x80) /* Warning: Sensor result higher than the device can process */


/****************************** TBR??? *****************************************/
//// This depends by CGM Feature Field
//TBR???
#define CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1 (CGM_SENSOR_STATUS_ANNUNCIATION_SESSION_STOPPED                       | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW                    | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_TYPE_INCORRECT_FOR_DEVICE      | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION                    | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_DEVICE_SPECIFIC_ALERT                 | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_GENERAL_DEVICE_FAULT)

#define CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2 (CGM_SENSOR_STATUS_ANNUNCIATION_TIME_SYNCHRONIZATION          | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_CALIBRATION_NOT_ALLOWED       | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_CALIBRATION_RECOMMENDED       | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_CALIBRATION_REQUIRED          | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_TEMPERATURE_TOO_HIGH   | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_TEMPERATURE_TOO_LOW)

#define CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3 (CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_LOWER_PATIENT_LOW_LEVEL        | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_HIGHER_PATIENT_HIGH_LEVEL      | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_LOWER_HYPO_LEVEL               | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_HIGHER_HYPER_LEVEL             | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_RATE_DECREASE_EXCEEDED         | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_RATE_INCREASE_EXCEEDED         | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_LOWER_DEVICE                   | \
                                                   CGM_SENSOR_STATUS_ANNUNCIATION_SENSOR_HIGHER_DEVICE)

//TBR???
#define CGM_SENSOR_STATUS_ANNUNCIATION_ALL (((uint32_t)(CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3 << 16)) | \
                                            ((uint32_t)(CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2 << 8))  | \
                                            ((uint32_t)(CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1)))
/*********************************** TBR??? **************************************/


/******** Cgm feature characteristic **************************/
/* CGM feature values (24  bits): to be used for identify supported features  */
#define CGM_FEATURE_CALIBRATION_IS_SUPPORTED                           (0x000001)  /* Calibration */
#define CGM_FEATURE_PATIENT_HIGH_LOW_ALERTS_IS_SUPPORTED               (0x000002)  /* Patient High/Low Alerts */
#define CGM_FEATURE_HYPO_ALERTS_IS_SUPPORTED                           (0x000004)  /* Hypo Alerts */
#define CGM_FEATURE_HYPER_ALERTS_IS_SUPPORTED                          (0x000008)  /* Hyper Alerts */

#define CGM_FEATURE_RATE_OF_INCREASE_DECREASE_ALERTS_IS_SUPPORTED      (0x000010)  /* Rate of Increase/Decrease Alerts */
#define CGM_FEATURE_DEVICE_SPECIFIC_ALERT_IS_SUPPORTED                 (0x000020)  /* Device Specific Alert */
#define CGM_FEATURE_SENSOR_MALFUNCTION_DETECTION_IS_SUPPORTED          (0x000040)  /* Sensor Malfunction Detection */
#define CGM_FEATURE_SENSOR_TEMPERATURE_HIGH_LOW_DETECTION_IS_SUPPORTED (0x000080)  /* Sensor Temperature High-Low Detection */

#define CGM_FEATURE_SENSOR_RESULT_HIGH_LOW_DETECTION_IS_SUPPORTED      (0x000100)  /* Sensor Result High-Low Detection */
#define CGM_FEATURE_LOW_BATTERY_DETECTION_IS_SUPPORTED                 (0x000200)  /* Low Battery Detection */
#define CGM_FEATURE_SENSOR_TYPE_ERROR_DETECTION_IS_SUPPORTED           (0x000400)  /* Sensor Type Error Detection */
#define CGM_FEATURE_GENERAL_DEVICE_FAULT_IS_SUPPORTED                  (0x000800)  /* General Device Fault */
#define CGM_FEATURE_E2E_CRC_IS_SUPPORTED                               (0x001000)  /* E2E-CRC */
#define CGM_FEATURE_MULTIPLE_BOND_IS_SUPPORTED                         (0x002000)  /* Multiple Bond */
#define CGM_FEATURE_MULTIPLE_SESSIONS_IS_SUPPORTED                     (0x004000)  /* Multiple Sessions */
#define CGM_FEATURE_CGM_TREND_INFORMATION_IS_SUPPORTED                 (0x008000)  /* CGM Trend Information: bit 15 */

#define CGM_FEATURE_CGM_QUALITY_IS_SUPPORTED                           (0x010000)  /* CGM Quality: bit 16 */   

#define CGM_FEATURE_BASIC_SUPPORTED (CGM_FEATURE_CGM_QUALITY_IS_SUPPORTED) | (CGM_FEATURE_CGM_TREND_INFORMATION_IS_SUPPORTED)

#define CGM_FEATURE_ALL     (CGM_FEATURE_CALIBRATION_IS_SUPPORTED                               | \
                             CGM_FEATURE_PATIENT_HIGH_LOW_ALERTS_IS_SUPPORTED                   | \
                             CGM_FEATURE_HYPO_ALERTS_IS_SUPPORTED                               | \
                             CGM_FEATURE_HYPER_ALERTS_IS_SUPPORTED                              | \
                             CGM_FEATURE_RATE_OF_INCREASE_DECREASE_ALERTS_IS_SUPPORTED          | \
                             CGM_FEATURE_DEVICE_SPECIFIC_ALERT_IS_SUPPORTED                     | \
                             CGM_FEATURE_SENSOR_MALFUNCTION_DETECTION_IS_SUPPORTED              | \
                             CGM_FEATURE_SENSOR_TEMPERATURE_HIGH_LOW_DETECTION_IS_SUPPORTED     | \
                             CGM_FEATURE_SENSOR_RESULT_HIGH_LOW_DETECTION_IS_SUPPORTED          | \
                             CGM_FEATURE_LOW_BATTERY_DETECTION_IS_SUPPORTED                     | \
                             CGM_FEATURE_SENSOR_TYPE_ERROR_DETECTION_IS_SUPPORTED               | \
                             CGM_FEATURE_GENERAL_DEVICE_FAULT_IS_SUPPORTED                      | \
                             CGM_FEATURE_E2E_CRC_IS_SUPPORTED                                   | \
                             CGM_FEATURE_MULTIPLE_BOND_IS_SUPPORTED                             | \
                             CGM_FEATURE_MULTIPLE_SESSIONS_IS_SUPPORTED                         | \
                             CGM_FEATURE_CGM_TREND_INFORMATION_IS_SUPPORTED                     | \
                             CGM_FEATURE_CGM_QUALITY_IS_SUPPORTED)

/* Multi bond is not supported TBR??? */
#define CGM_FEATURE_ALL_SUPPORTED_NO_MULTI_BOND_LOW     (CGM_FEATURE_CALIBRATION_IS_SUPPORTED                               | \
                                                         CGM_FEATURE_PATIENT_HIGH_LOW_ALERTS_IS_SUPPORTED                   | \
                                                         CGM_FEATURE_HYPO_ALERTS_IS_SUPPORTED                               | \
                                                         CGM_FEATURE_HYPER_ALERTS_IS_SUPPORTED                              | \
                                                         CGM_FEATURE_RATE_OF_INCREASE_DECREASE_ALERTS_IS_SUPPORTED          | \
                                                         CGM_FEATURE_DEVICE_SPECIFIC_ALERT_IS_SUPPORTED                     | \
                                                         CGM_FEATURE_SENSOR_MALFUNCTION_DETECTION_IS_SUPPORTED              | \
                                                         CGM_FEATURE_SENSOR_TEMPERATURE_HIGH_LOW_DETECTION_IS_SUPPORTED     | \
                                                         CGM_FEATURE_SENSOR_RESULT_HIGH_LOW_DETECTION_IS_SUPPORTED          | \
                                                         CGM_FEATURE_LOW_BATTERY_DETECTION_IS_SUPPORTED                     | \
                                                         CGM_FEATURE_SENSOR_TYPE_ERROR_DETECTION_IS_SUPPORTED               | \
                                                         CGM_FEATURE_GENERAL_DEVICE_FAULT_IS_SUPPORTED                      | \
                                                         CGM_FEATURE_E2E_CRC_IS_SUPPORTED                                   | \
                                                         CGM_FEATURE_MULTIPLE_SESSIONS_IS_SUPPORTED                         | \
                                                         CGM_FEATURE_CGM_TREND_INFORMATION_IS_SUPPORTED                     | \
                                                         CGM_FEATURE_CGM_QUALITY_IS_SUPPORTED)

/* end CGM Feature field */

  
/* CGM Feature: list of available Key values for Type field (nibble) */
#define CGM_TYPE_CAPILLARY_WHOLE_BLOOD     (0x01)
#define CGM_TYPE_CAPILLARY_PLASMA          (0x02)
#define CGM_TYPE_VENOUS_WHOLE_BLOOD        (0x03)
#define CGM_TYPE_VENOUS_PLASMA             (0x04)
#define CGM_TYPE_ARTERIAL_WHOLE_BLOOD      (0x05)
#define CGM_TYPE_ARTERIAL_PLASMA           (0x06)
#define CGM_TYPE_UNDERTERMINED_WHOLE_BLOOD (0x07)
#define CGM_TYPE_UNDERTERMINED_PLASMA      (0x08)
#define CGM_TYPE_INTERSISTIAL_FLUID        (0x09) /* ISF*/
#define CGM_TYPE_CONTROL                   (0x0A)
/* NOTE: 0x0, 0xB - 0xF keys values are reserved for future use */

/* CGM Feature: list of available Key values for Sample Location field (nibble) */
#define CGM_SAMPLE_LOCATION_FINGER              (0x10)
#define CGM_SAMPLE_LOCATION_AST                 (0x20) /* Alternate Site Test */
#define CGM_SAMPLE_LOCATION_EARLOBE             (0x30)
#define CGM_SAMPLE_LOCATION_CONTROL_SOLUTION    (0x40)
#define CGM_SAMPLE_LOCATION_SUBCUTANEOUS_TISSUE (0x50) /* New for CGM profile */
#define CGM_SAMPLE_LOCATION_VALUE_NOT_AVAILABLE (0xF0)
/* NOTE: 0x0, 0x6 - 0xE keys values are reserved for future use */


/******** CGM record access control point (RACP) characteristic *****/

/* Record Access Control Point: Op Code Values */
/* RACP requests */
#define RACP_REPORT_STORED_RECORDS_OP_CODE          (0x01)
#define RACP_DELETE_STORED_RECORDS_OP_CODE          (0x02) 
#define RACP_ABORT_OPERATION_OP_CODE                (0x03)
#define RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE   (0x04)
/* RACP responses */
#define RACP_NUMBER_STORED_RECORDS_RESPONSE_OP_CODE (0x05)
#define RACP_RESPONSE_OP_CODE                       (0x06)
 
/* Record Access Control Point: procedure Operator values */
#define RACP_OPERATOR_NULL          (0x00)
#define RACP_OPERATOR_ALL           (0x01)  
#define RACP_OPERATOR_LESS_EQUAL    (0x02)
#define RACP_OPERATOR_GREATER_EQUAL (0x03)
#define RACP_OPERATOR_WITHIN_RANGE  (0x04)
#define RACP_OPERATOR_FIRST_RECORD  (0x05)
#define RACP_OPERATOR_LAST_RECORD   (0x06)  

/* Record Access Control Point: filter types values */
#define RACP_FILTER_TYPE_NONE        (0x00)
#define RACP_FILTER_TYPE_TIME_OFFSET (0x01)

/* Record Access Control Point: filter types valid lenght */
#define RACP_FILTER_TYPE_NO_OPERAND_LENGHT               (2)
#define RACP_FILTER_TYPE_LESS_GREATER_TIME_OFFSET_LENGHT (5)
#define RACP_FILTER_TYPE_WITHIN_RANGE_TIME_OFFSET_LENGHT (7)  

/* Record Access Control Point: fields position */
#define RACP_OP_CODE_POSITION     (0)
#define RACP_OPERATOR_POSITION    (1)
#define RACP_FILTER_TYPE_POSITION (2)

/* Record Access Control Point: responses values */
#define RACP_RESPONSE_SUCCESS                           (0x01)
#define RACP_RESPONSE_OPCODE_NOT_SUPPORTED              (0x02)
#define RACP_RESPONSE_INVALID_OPERATOR                  (0x03)
#define RACP_RESPONSE_OPERATOR_NOT_SUPPORTED            (0x04)
#define RACP_RESPONSE_INVALID_OPERAND                   (0x05)
#define RACP_RESPONSE_NO_RECORDS                        (0x06) 
#define RACP_RESPONSE_ABORT_UNSUCCESSFUL                (0x07) 
#define RACP_RESPONSE_PROCEDURE_NOT_COMPLETED           (0x08)
#define RACP_RESPONSE_OPERAND_NOT_SUPPORTED             (0x09) /* TBR: RACP_RESPONSE_FILTER_TYPE_NOT_SUPPORTED */

/* RACP Error code: Procedure Already In Progress */
#define RACP_RESPONSE_PROCEDURE_ALREADY_IN_PROGRESS (0xFE)

/* RACP Error code: Client Characteristic Configuration Descriptor Improperly Configured */
#define RACP_RESPONSE_CLIENT_CHAR_CONF_DESC_IMPROPERLY_CONFIGURED (0xFD)


/******** CGM Specific Ops Control Point (SOCP) characteristic *****/

/* SOCP Set and Get procedures: Op Code Values */

#define SOCP_SET_CGM_COMMUNICATION_INTERVAL          (0x01)     //mandatory
#define SOCP_GET_CGM_COMMUNICATION_INTERVAL          (0x02)     //mandatory
#define SOCP_GET_CGM_COMMUNICATION_INTERVAL_RESPONSE (0x03)     //mandatory
/* Optional */
#define SOCP_SET_GLUCOSE_CALIBRATION_VALUE           (0x04)
#define SOCP_GET_GLUCOSE_CALIBRATION_VALUE           (0x05)
#define SOCP_GLUCOSE_CALIBRATION_VALUE_RESPONSE      (0x06)

#define SOCP_SET_PATIENT_HIGH_ALERT_LEVEL            (0x07)
#define SOCP_GET_PATIENT_HIGH_ALERT_LEVEL            (0x08)
#define SOCP_PATIENT_HIGH_ALERT_LEVEL_RESPONSE       (0x09)

#define SOCP_SET_PATIENT_LOW_ALERT_LEVEL             (0x0A)
#define SOCP_GET_PATIENT_LOW_ALERT_LEVEL             (0x0B)
#define SOCP_PATIENT_LOW_ALERT_LEVEL_RESPONSE        (0x0C)

#define SOCP_SET_HYPO_ALERT_LEVEL                    (0x0D) 
#define SOCP_GET_HYPO_ALERT_LEVEL                    (0x0E)
#define SOCP_HYPO_ALERT_LEVEL_RESPONSE               (0x0F)  

#define SOCP_SET_HYPER_ALERT_LEVEL                   (0x10)                                 
#define SOCP_GET_HYPER_ALERT_LEVEL                   (0x11) 
#define SOCP_HYPER_ALERT_LEVEL_RESPONSE              (0x12) 

#define SOCP_SET_RATE_OF_DECREASE_ALERT_LEVEL        (0x13)
#define SOCP_GET_RATE_OF_DECREASE_ALERT_LEVEL        (0x14)
#define SOCP_RATE_OF_DECREASE_ALERT_LEVEL_RESPONSE   (0x15) 

#define SOCP_SET_RATE_OF_INCREASE_ALERT_LEVEL        (0x16)
#define SOCP_GET_RATE_OF_INCREASE_ALERT_LEVEL        (0x17)
#define SOCP_RATE_OF_INCREASE_ALERT_LEVEL_RESPONSE   (0x18)

#define SOCP_RESET_DEVICE_SPECIFIC_ALERT             (0x19)

#define SOCP_START_THE_SESSION                       (0x1A)
#define SOCP_STOP_THE_SESSION                        (0x1B)

/* SOCP Procedures Response: Op Code Values */
#define SOCP_RESPONSE_CODE                           (0x1C)

/* Specific Ops Control Point: Operand valid lenght */
// If E2E_CRC support, to these lenghts must be add 2 bytes
#define SOCP_OPERAND_NOT_APPLICABLE_LENGHT                      (1) //1 byte for Op Code 
#define SOCP_OPERAND_COMMUNICATION_INTERVAL_LENGHT              (2) //+2 //1 + 1 bytes for Operand 
                                                                    //(communication interval in minutes)
#define SOCP_OPERAND_ALERT_LEVEL_LENGHT                         (3) //1 + 2 bytes for Operand 
                                                                    //(calibration data record number or alerts level)
#define SOCP_OPERAND_CALIBRATION_DATA_RECORD_LENGHT             (11)//1 + 10 bytes for Operand 
                                                                    //(calibration data record)
#define SOCP_OPERAND_RESPONSE_LENGHT                            (2) //1 + 1 bytes for  Response

/* Specific Ops Control Point: responses values */
#define SOCP_RESPONSE_SUCCESS                        (0x01)
#define SOCP_RESPONSE_OPCODE_NOT_SUPPORTED           (0x02)
#define SOCP_RESPONSE_INVALID_OPERAND                (0x03)
#define SOCP_RESPONSE_PROCEDURE_NOT_COMPLETED        (0x04)
#define SOCP_RESPONSE_OUT_OF_RANGE                   (0x05)

/* Specific Ops Control Point: Calibration Status (Mandatory if Calibration is supported */
#define SOCP_CALIBRATION_STATUS_CALIBRATION_DATA_REJECTED       (0x01) /* Calibration Status: Calibration Data rejected (Calibration failed) */
#define SOCP_CALIBRATION_STATUS_CALIBRATION_DATA_OUT_RANGE      (0x02) /* Calibration Status: Calibration Data out of range */
#define SOCP_CALIBRATION_STATUS_CALIBRATION_PROCESS_PENDING     (0x04) /* Calibration Status: Calibration Process Pending */

/* SOCP Error code: Client Characteristic Configuration Descriptor Improperly Configured */
#define SOCP_RESPONSE_CLIENT_CHAR_CONF_DESC_IMPROPERLY_CONFIGURED (0xFD) 

/* Specific Ops Control Point: fields position  TBR??? */
#define SOCP_OP_CODE_POSITION     (0)
#define SOCP_OPERAND_POSITION     (1)

/* Specific Ops Control Point: Min Communication interval */
#define MIN_COMMUNICATION_INTERVAL  (1)

/* Specific Ops Control Point: Alerts Levels (Device specific values) */
#define PATIENT_HIGH_ALERT_LEVEL   (0x0258) // 600 //(0x00C8)   //SFLOAT exponent = 0; 200(0xC8) mg/dl  //TBR???     //(0xB0FA)  //(0x07f0) 
#define PATIENT_LOW_ALERT_LEVEL    (0x0032) //SFLOAT exponent = 0;  50(0x32) mg/dl  //TBR???     //(0xB00A)  //(0x070a)

#define HYPER_ALERT_LEVEL          (0x0258) //SFLOAT exponent = 0; 600(0x258) mg/dl  //TBR???   
#define HYPO_ALERT_LEVEL           (0x0032) //SFLOAT exponent = 0;  50(0x32) mg/dl  //TBR??? 

#define RATE_INCREASE_ALERT_LEVEL  (0x0003)  //SFLOAT exponent = 0;  3(0xFFD) mg/dl/min  //TBR???
#define RATE_DECREASE_ALERT_LEVEL  (0x0FFD)  //SFLOAT exponent = 0; -3(0xFFD) mg/dl/min  //TBR???

/* Specific Ops Control Point: Calibration Data Record fields ranges */
#define GLUCOSE_CONCENTRATION_MAX_RANGE   (0x0258) //SFLOAT exponent = 0; 600(0x258) mg/dl //TBR???
#define GLUCOSE_CONCENTRATION_MIN_RANGE   (0x0032) //SFLOAT exponent = 0;  50(0x32) mg/dl  //TBR???     //(0xB00A)  //(0x070a)
#define CALIBRAT_TIME_MAX_RANGE           (0x05A0) //1440 minutes (24 hours)      
#define CALIBRAT_TIME_MIN_RANGE           (0x0001) //1 minute     

/* Session Start Time characteristic */
// The time zone value for start time/ run time etc. 
#define TIME_ZONE_UTC_M_1200                                (0xD0)      /* -48 := UTC-12:00 */
#define TIME_ZONE_UTC_M_1100                                (0xD4)      /* -44 := UTC-11:00 */
#define TIME_ZONE_UTC_M_1000                                (0xD8)      /* -40 := UTC-10:00 */
#define TIME_ZONE_UTC_M_0930                                (0xDA)      /* -38 := UTC-9:30 */
#define TIME_ZONE_UTC_M_0900                                (0xDC)      /* -36 := UTC-9:00 */
#define TIME_ZONE_UTC_M_0800                                (0xE0)      /* -32 := UTC-8:00 */
#define TIME_ZONE_UTC_M_0700                                (0xE4)      /* -28 := UTC-7:00 */
#define TIME_ZONE_UTC_M_0600                                (0xE8)      /* -24 := UTC-6:00 */
#define TIME_ZONE_UTC_M_0500                                (0xEC)      /* -20 := UTC-5:00 */
#define TIME_ZONE_UTC_M_0430                                (0xEE)      /* -18 := UTC-4:30 */
#define TIME_ZONE_UTC_M_0400                                (0xF0)      /* -16 := UTC-4:00 */
#define TIME_ZONE_UTC_M_0330                                (0xF2)      /* -14 := UTC-3:30 */
#define TIME_ZONE_UTC_M_0300                                (0xF4)      /* -12 := UTC-3:00 */
#define TIME_ZONE_UTC_M_0200                                (0xF8)      /* -8  := UTC-2:00 */
#define TIME_ZONE_UTC_M_0100                                (0xFC)      /* -4  := UTC-1:00 */
#define TIME_ZONE_UTC_0_0000                                (0x00)      /* 0   := UTC+0:00 */
#define TIME_ZONE_UTC_P_0100                                (0x04)      /* 4   := UTC+1:00 */
#define TIME_ZONE_UTC_P_0200                                (0x08)      /* 8   := UTC+2:00 */
#define TIME_ZONE_UTC_P_0300                                (0x0C)      /* 12  := UTC+3:00 */
#define TIME_ZONE_UTC_P_0330                                (0x0E)      /* 14  := UTC+3:30 */
#define TIME_ZONE_UTC_P_0400                                (0x10)      /* 16  := UTC+4:00 */
#define TIME_ZONE_UTC_P_0430                                (0x12)      /* 18  := UTC+4:30 */
#define TIME_ZONE_UTC_P_0500                                (0x14)      /* 20  := UTC+5:00 */
#define TIME_ZONE_UTC_P_0530                                (0x16)      /* 22  := UTC+5:30 */
#define TIME_ZONE_UTC_P_0545                                (0x17)      /* 23  := UTC+5:45 */
#define TIME_ZONE_UTC_P_0600                                (0x18)      /* 24  := UTC+6:00 */
#define TIME_ZONE_UTC_P_0630                                (0x1A)      /* 26  := UTC+6:30 */
#define TIME_ZONE_UTC_P_0700                                (0x1C)      /* 28  := UTC+7:00 */
#define TIME_ZONE_UTC_P_0800                                (0x20)      /* 32  := UTC+8:00 */
#define TIME_ZONE_UTC_P_0845                                (0x23)      /* 35  := UTC+8:45 */
#define TIME_ZONE_UTC_P_0900                                (0x24)      /* 36  := UTC+9:00 */
#define TIME_ZONE_UTC_P_0930                                (0x26)      /* 38  := UTC+9:30 */
#define TIME_ZONE_UTC_P_1000                                (0x28)      /* 40  := UTC+10:00 */
#define TIME_ZONE_UTC_P_1030                                (0x2A)      /* 42  := UTC+10:30 */
#define TIME_ZONE_UTC_P_1100                                (0x2C)      /* 44  := UTC+11:00 */
#define TIME_ZONE_UTC_P_1130                                (0x2E)      /* 46  := UTC+11:30 */
#define TIME_ZONE_UTC_P_1200                                (0x30)      /* 48  := UTC+12:00 */
#define TIME_ZONE_UTC_P_1245                                (0x33)      /* 51  := UTC+12:45 */
#define TIME_ZONE_UTC_P_1300                                (0x34)      /* 52  := UTC+13:00 */
#define TIME_ZONE_UTC_P_1400                                (0x38)      /* 56  := UTC+14:00 */
#define TIME_ZONE_UNKNOWN                                   (0x80)      /*-128	< Unknown> */

// DST Offset information uint8
// https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.dst_offset.xml
#define DST_STANDARD_TIME			0x00	///< Standard Time
#define	DST_HALF_HOUR_DAYLIGHT			0x02	///< Half An Hour Daylight Time (+0.5h)
#define	DST_ONE_HOUR_DAYLIGHT			0x04	///< Daylight Time (+1h)
#define	DST_DOUBLE_HOUR_DAYLIGHT		0x08	///< Double Daylight Time (+2h)
#define	DST_UNKNOWN				0xFF	///< Daylight Time unknown


#define MINUTES_IN_HOUR   (60)

#define INITIAL_YEAR      (2000)

/* Days values */
#define JANUARY              (0x01)
#define FEBRUARY             (0x02)
#define MARCH                (0x03)
#define APRIL                (0x04)
#define MAY                  (0x05)
#define JUNE                 (0x06)
#define JULY                 (0x07)
#define AUGUST               (0x08)
#define SEPTEMBER            (0x09)
#define OCTOBER              (0x0A)
#define NOVEMBER             (0x0B)
#define DECEMBER             (0x0C)
/* Number of days values */
#define FEBRUARY_NORMAL_DAYS (28)
#define FEBRUARY_LEAP_DAYS   (29)
#define EVEN_MONTH_DAYS      (30)
#define ODD_MONTH_DAYS       (31) 
#define NORMAL_YEAR_DAYS     (365)
#define LEAP_YEAR_DAYS       (366)
/* is a leap year (366 days) or not (365 days) */
#define	IS_LEAP_YEAR(year)    (!((year) % 400) ||               \
			       (((year) % 100) && !((year) % 4)))

/********** Error Code ********************/
#define MISSING_CRC     (0x80)   //If E2E-CRC is supported and a Write procedure is processed without CRC attached
#define INVALID_CRC     (0x81)   //If E2E-CRC is supported and a Write procedure is processed with incorrect or invalid CRC value attached
#define OUT_OF_RANGE    (0xFF)  

/******** Special Short Float Values *****************************************/
#define NAN_VALUE               (0x07FF)        /* Not a number value */
#define NRES_VALUE              (0x0800)        /* Not at this resolution value */
#define POSITIVE_INFINITY_VALUE (0x07FE)
#define NEGATIVE_INFINITY_VALUE (0x0802)
#define RESERVED_FOR_FUTURE_USE (0x0801)        /* Reserved for future use value */

/* Procedure timeout (ms) */
#define CGM_PROCEDURE_TIMEOUT             30000
/******************************************************************************
* Type definitions
******************************************************************************/

/******************************************************************************
* Function Declarations
******************************************************************************/

#endif /* _CGM_SERVICE_H_ */
