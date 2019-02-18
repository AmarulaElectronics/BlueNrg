/*******************************************************************************
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
*   FILENAME        -  blood_pressure.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      06/04/2016
*   $Revision$:  second version
*   $Author$:    
*   Comments:    Redesigned the blood pressure sensor profile according to 
*                unified BlueNRG-1 DK framework 
*
********************************************************************************
*
*  File Description 
*  ---------------------
*  header for blood pressure profile
*
*******************************************************************************/

#ifndef _BLOOD_PRESSURE_H_
#define _BLOOD_PRESSURE_H_

#include "hal_types.h"
/*******************************************************************************
* Macros
*******************************************************************************/

/* Number of virtual timer used */
#define BPS_ADVERTISE_TIMER                 1
#define BPS_IDLE_CONNECTION_TIMER           2
//#define BPS_INTERVAL_TIMER                  3

/* profile states */
#define BPS_UNINITIALIZED		        (0x00)
#define BPS_WAITING_BLE_INIT                    (0x01)
#define BPS_IDLE				(0x02)
#define BPS_INITIALIZED			        (0x03)
#define BPS_UNDIRECTED_ADV_MODE			(0x04)
#define BPS_UNDIRECTED_ADV_MODE_STOP		(0x05)
#define BPS_ADV_IN_FAST_DISC_MODE		(0x06)
#define BPS_ADV_IN_FAST_DISC_MODE_STOP          (0x07)
#define BPS_ADV_IN_LOW_PWR_MODE			(0x08)
#define BPS_CONNECTED				(0x09)
/* connected substates */
#define BPS_CONNECTED_DO_BPM		        (0x0A)
#define BPS_CONNECTED_STOP_BPM			(0x0B)
#define BPS_CONNECTED_DO_ICP			(0x0C)
#define BPS_CONNECTED_STOP_ICP		        (0x0D)

#define BPS_ICP_CHAR_SUPPORTED			(0x40)
#define MULTIPLE_BONDS_SUPPORTED 		(0x20)

/* advertising intervals in terms of 625 micro seconds */
#define BPS_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define BPS_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define BPS_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define BPS_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

#define MIN_ENCRYPTION_KEY_SIZE          (10)


#define MAX_BPM_CHAR_VAL_LEN			(19)
#define MAX_BPF_CHAR_VAL_LEN			(2)
#define MAX_ICP_CHAR_VAL_LEN			(19)

/* flags */
#define FLAG_BLOOD_PRESSURE_UNITS_KPA		(0x01)
#define FLAG_TIME_STAMP_PRESENT			(0x02)
#define FLAG_PULSE_RATE_PRESENT			(0x04)
#define FLAG_USER_ID_PRESENT			(0x08)
#define FLAG_MEASUREMENT_STATUS_PRESENT	        (0x10)

/* measurement status field bitmasks */
#define BODY_MOVEMENT_DETECTION_FLAG		(0x01)
#define CUFF_FIT_DETECTION_FLAG			(0x02)
#define IRREGULAR_PULSE_DETECTION_FLAG		(0x04)
#define PULSE_RATE_RANGE_EXCEEDS_UPPER_LIMIT	(0x08)
#define PULSE_RATE_RANGE_BELOW_LOWER_LIMIT	(0x10)
#define MEASUREMENT_POSITION_DETECTION_FLAG	(0x20)

/* blood pressure feature flags */
#define BODY_MOVEMENT_DETECTION_SUPPORT_BIT	 	(0x01)
#define CUFF_FIT_DETECTION_SUPPORT_BIT	 		(0x02)
#define IRREGULAR_PULSE_DETECTION_SUPPORT_BIT		(0x04)
#define PULSE_RATE_RANGE_DETECTION_SUPPORT_BIT	 	(0x08)
#define MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT	(0x10)
/*******************************************************************************
* Function Declarations
*******************************************************************************/
typedef struct _tDateTime
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
}tDateTime;

typedef struct _tMeasurementStatus
{
   /**
    * BODY_MOVEMENT_DETECTION_FLAG
    *     0 -> no body movement 
    * 	1 -> body movement during measurement
    */ 
  uint8_t bodyMovementDetectionFlag;

  /**
   * CUFF_FIT_DETECTION_FLAG
   *     0 -> cuff fits properly
   *     1 -> cuff too lose
   */ 
  uint8_t cuffFitDetectionFlag;

   /**
    * IRREGULAR_PULSE_DETECTION_FLAG
    *     0 -> no irregular pulse detected
    *     1 -> irregular pulse detected
    */ 
    
  uint8_t irregularPulseDetectionFlag;

   /**
    * PULSE_RATE_RANGE_DETECTION_FLAG
    *     00 -> pulse rate is within the range
  *     01 -> pulse rate exceeds upper limit
    *     02 -> pulse rate is lesser than lower limit
    */
  uint8_t pulseRateRangeDetectionFlag;

  /**
    * MEASUREMENT_POSITION_DETECTION_FLAG
    *     0 -> proper measurement position
    *     1 -> improper measurement position
    */
  uint8_t measurementPositionDetectionFlag;
}tMeasurementStatus;

typedef struct _tBPMValue
{
  /**
   * the flags is a bitmask which tells the peer
   * of the data to follow:\n
   * bit0 : a value of 1 indicates that the unit is kPa
   *        a value of 0 indicates that the unit is mmHg
   * bit1 : a value of 1 implies there is a timestamp field
   *        present in the data
   * bit2 : a value of 1 implies there is a pulse rate field
   *        present in the data
   * bit3 : a value of 1 implies there is a user ID field
   *        present in the data
   * bit4 : a value of 1 implies there is a measurement status
   * 		  field present in the data
   */ 
  uint8_t flags;

  /**
   * systolic pressure
   * the units of this field will be considered
   * in accordance with the bpUnitsFlag field
   */ 
  uint16_t systolic;

  /**
   * diastolic pressure
   * the units of this field will be considered
   * in accordance with the bpUnitsFlag field
   */ 
  uint16_t diastolic;

  /**
   * Mean Arterial Pressur
   * the units of this field will be considered
   * in accordance with the bpUnitsFlag field
   */ 
  uint16_t map;

  /**
   * if the timestamp flag is set to true,
   * this field chould contain the date and
   * time
   */ 
  tDateTime timeStamp;

  /**
   * if the pulse rate flag is set to true,
   * this field should contain the pulse rate
   */
   uint16_t pulseRate;
   
   /**
    * if the user id flag is set to true,
    * this field should contain the unique USER ID
    */ 
   uint8_t userID;
   
   /**
    * measurement status 
    */ 
    tMeasurementStatus measurementStatus;
}tBPMValue;

typedef struct _tICPValue
{
  /**
   * the flags is a bitmask which tells the peer
   * of the data to follow:\n
   * bit0 : a value of 1 indicates that the unit is kPa
   *        a value of 0 indicates that the unit is mmHg
   * bit3 : a value of 1 implies there is a user ID field
   *        present in the data
   * bit4 : a value of 1 implies there is a measurement status
   * 		  field present in the data
   */ 
  uint8_t flags;
  
  /**
   * intermediate cuff pressure
   * the units of this field will be considered
   * in accordance with the bit 0 of flags field
   */ 
  uint16_t icp;
  
   /**
    * if the user id flag is set to true,
    * this field should contain the unique USER ID
    */ 
   uint8_t userID;
   
   /**
    * measurement status 
    */ 
    tMeasurementStatus measurementStatus;
}tICPValue;

typedef struct _tBloodPressureContext
{
  /**
   * state of the blood pressure
   * sensor state machine
   */ 
  tProfileState bpsState;
  
  /**
   * state of the blood pressure
   * sensor state machine
   */ 
  tProfileState bpsSubState;
      
  /**
   * callback to notify the applciation with
   * the profile specific events
   */ 
  BLE_CALLBACK_FUNCTION_TYPE applcb;
  
  /**
  * connection handle
  */ 
  uint16_t connHandle;
      
   /**
   * flag which indicates the features supported
   * by the blood pressure sensor
   */ 
  uint8_t bpsFeaturesSupported;
      
  /**
   * blood pressure service handle
   */ 
  uint16_t bpServiceHandle;
  
  /**
   * blood pressure measurement characteristic
   * handle
   */ 
  uint16_t bpmCharHandle;
  
  /**
   * blood pressure feature characteristic
   * handle
   */ 
  uint16_t bpfCharHandle;
      
  /**
   * intermediate cuff pressure 
   * characteristic handle
   */ 
  uint16_t icpCharHandle;
      
  /**
   * profile interface context. This is
   * registered with the main profile
   */ 
  tProfileInterfaceContext BPStoBLEInf;
}tBloodPressureContext;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
/**
 *
 * @param[in] intermediateCuffPressureChar :
 * indicates whether the blood pressure service should support the
 * intermediate cuff pressure characteristic.
 * @param[in] feature : a bitmask representing the features
 *            supported by the device:\n
 * bit 0 : BODY_MOVEMENT_DETECTION_SUPPORT_BIT
 *     0 -> body movement detection feature not supported
 * 	   1 -> body movement detection feature supported
 * bit 1: CUFF_FIT_DETECTION_SUPPORT_BIT
 *     0 -> cuff fit detection bit not supported
 *     1 -> cuff fit detection bit supported
 * bit 2 : IRREGULAR_PULSE_DETECTION_SUPPORT_BIT
 *     0 -> irregular pulse detection feature not supported
 *     1 -> irregular pulse detection feature supported
 * bit 3 : PULSE_RATE_RANGE_DETECTION_SUPPORT_BIT
 *     0 -> pulse rate range detection not supported
 *     1 -> pulse rate range detection supported
 * bit 4 : MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT
 *     0 -> measurement position detection feature is not supported
 *     1 -> measurement position detection feature is supported
 * @param[in] BPScb : function to be called by the profile
 * to notify the application of the profile specific events
 *
 * Registers the blood pressure sensor profile with the
 * main profile.
 *
 * @return BLE_STATUS_SUCCESS if the profile is initialized
 * successfully
 */
tBleStatus BPS_Init(BOOL intermediateCuffPressureChar,
                    uint8_t feature,
                    BLE_CALLBACK_FUNCTION_TYPE BPScb);

/**
 * 
 * @param[in] useWhiteList : This should be set to 
 *            true if the user is trying to establish
 *            a connection with a previously bonded
 *            device. 
 * 
 * If the useWhiteList is set to TRUE, then the device
 * is configured to use the whitelist which is configured
 * with bonded devices at the time of initialization else
 * limited discoverable mode is entered to connect to any
 * of the avialable devices
 */ 
tBleStatus BPS_Advertize(uint8_t useWhitelist);

/**
 * 
 * @param[in] icpval : the intermediate cuff pressure value
 * 			  to be sent
 * 
 * This function has to be used to send the intermediate cuff
 * pressure values during the measurement process until a
 * stable value is obtained. Once a stable value is obtained,
 * BPS_Send_Blood_Pressure_Measurement function should be
 * used to send the blood pressure measurement values.
 * This function can be used only if the intermediateCuffPressureChar
 * is set to true during initialization
 */ 
tBleStatus BPS_Send_Intermediate_Cuff_Pressure(tICPValue icpval);

/**
 * 
 * @param[in] bpmval : the blood pressure measurement value
 * 			  to be sent
 * 
 * updates the blood pressure measurement characteristic with
 * the value if the device is connected
 */ 
tBleStatus BPS_Send_Blood_Pressure_Measurement(tBPMValue bpmval);

/**
 * 
 * 
 * HR profile's state machine: to be called on application main loop. 
 */ 
tBleStatus BPS_StateMachine(void);

#endif  /* _BLOOD_PRESSURE_H_ */
