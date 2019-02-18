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
*   FILENAME        -  health_thermometer.h
*
*   COMPILER        - 
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      07/07/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    health thermometer sensor profile 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the health thermometer profile
* 
******************************************************************************/

#ifndef _HEALTH_THERMOMETER_H_
#define _HEALTH_THERMOMETER_H_

/******************************************************************************
* Macro Declarations
******************************************************************************/

/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER                 1
#define IDLE_CONNECTION_TIMER           2
#define HT_INTERVAL_TIMER               3

/* profile states */
#define HT_UNINITIALIZED				(0x00)
#define HT_WAITING_BLE_INIT                             (0x01)

#define HT_IDLE						(0x02)
#define HT_INITIALIZED					(0x03)
#define HT_UNDIRECTED_ADV_MODE				(0x04)
#define HT_UNDIRECTED_ADV_MODE_STOP		        (0x05)
#define HT_ADV_IN_FAST_DISC_MODE			(0x06)
#define HT_ADV_IN_FAST_DISC_MODE_STOP                   (0x07)
#define HT_ADV_IN_LOW_PWR_MODE				(0x08)
#define HT_CONNECTED					(0x09)

/* connected substates */
#define HT_CONNECTED_DO_TEMPERATURE_MEASUREMENT		(0x0A)
#define HT_CONNECTED_STOP_TEMPERATURE_MEASUREMENT	(0x0B)
#define HT_CONNECTED_DO_INTERMEDIATE_TEMPERATURE	(0x0C)
#define HT_CONNECTED_STOP_INTERMEDIATE_TEMPERATURE      (0x0D)
#define HT_CONNECTED_DO_MEASUREMENT_INTERVAL		(0x0E)
#define HT_CONNECTED_STOP_MEASUREMENT_INTERVAL		(0x0F)
 
/* advertising intervals in terms of 625 micro seconds */
#define HT_FAST_CONN_ADV_INTERVAL_MIN	(0x20) /* 20ms */
#define HT_FAST_CONN_ADV_INTERVAL_MAX	(0x30) /* 30ms */
#define HT_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define HT_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */


#define TEMPERATURE_MEASUREMENT_MAX_LEN  (13)

/* NOTE: out of range error code was defined as 0x80 in HTS 1.0. 
   But in Core spec supplement (CSS v4), common profile and service error code is 
   defined and Out of Range is 0xFF now. (Page 30 of CSS v4):

   0xFD Client Characteristic Configuration Descriptor Improperly Configured
   0xFE Procedure Already in Progress
   0xFF Out of Range
*/

/* PTS 6.0.0 is using again 0x80 */
#define HTS_SPECIFICATION_1_0   1 
#if HTS_SPECIFICATION_1_0
/* health thermometer profile specific error code */
#define INTERVAL_OUT_OF_RANGE 			(0x80)
#else
/* Core spec supplement (CSS v4) error code or out of range: PTS 5.2.0, TC_SP_BI_01_C 
   is expecting this value */
#define INTERVAL_OUT_OF_RANGE 			(0xFF)
#endif

#define MIN_ENCRYPTION_KEY_SIZE			(10)



/* thermometerFeatures bitmasks */
typedef uint8_t tThermometerFeatures;
#define INTERMEDIATE_TEMPERATURE_CHAR 		 (0x01)
#define MEASUREMENT_INTERVAL_CHAR	         (0x02)
#define TEMPERATURE_TYPE 			 (0x04)

/* bitmask for the flags field */
#define FLAG_TEMPERATURE_UNITS_FARENHEIT 	 (0x01)
#define FLAG_TIMESTAMP_PRESENT 		         (0x02)
#define FLAG_TEMPERATURE_TYPE 		         (0x04)

/* temperature type flags */
typedef uint8_t tTemperatureType;
#define TEMP_MEASURED_AT_ARMPIT 		 (0x01)
#define TEMP_MEASURED_FOR_BODY 			 (0x02)/* this is the generic default value */
#define TEMP_MEASURED_AT_EAR 			 (0x03)
#define TEMP_MEASURED_AT_FINGER 		 (0x04)
#define TEMP_MEASURED_AT_GASTRO_INTESTINAL_TRACT (0x05)
#define TEMP_MEASURED_AT_MOUTH 			 (0x06)
#define TEMP_MEASURED_AT_RECTUM 	         (0x07)
#define TEMP_MEASURED_AT_TOE 			 (0x08)
#define TEMP_MEASURED_AT_TYMPANUM 	         (0x09) /* measured at ear drum */

/******************************************************************************
* Type definitions
******************************************************************************/
typedef struct _tTimestamp
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
}tTimestamp;

typedef struct _tTempMeasurementVal
{
  /**
   * bitmask of the fields supported in the
   * characteristic
   */ 
  uint8_t flags;
    
  /**
   * temperature measurement value
   * if the measurement value is unavailable
   * due to hardware failure or some other reason,
   * then this field has to be set to 0x007FFFFF
   */
   uint32_t temperature;

  /**
   * timestamp of the measurement
   * a value of 0 is not accepted for
   * month and year
   */ 
  tTimestamp timeStamp;
    
  /**
   * temperature type
   */
  tTemperatureType temperatureType; 
}tTempMeasurementVal;


typedef struct _tThermometerContext
{
  /**
   * state of the blood pressure
   * sensor state machine
   */ 
  tProfileState state;
  
  /**
   * substate of the blood pressure
   * sensor state machine
   */ 
  tProfileState substate;    
  
  /**
   * callback to notify the applciation with
   * the profile specific events
   */ 
  BLE_CALLBACK_FUNCTION_TYPE applcb;
      
   /**
   * flag which indicates the features supported
   * by the blood pressure sensor
   */ 
  uint8_t thermometerFeatures;
      
  /**
   * minimum valid interval for the measurement
   * interval characteristic
   */ 
  uint16_t minValidInterval;
      
  /**
   * maximum valid interval for the measurement
   * interval characteristic
   */ 
  uint16_t maxValidInterval;
      
  /**
   * current measurement interval. This field is
   * updated everytime, the client writes to this
   * characteristic or the user updates this field.
   * The default value of this field is 0
   */ 
  uint16_t measurementInterval;
      
  /**
   * health thermometer service handle
   */ 
  uint16_t htServiceHandle;
      
  /**
   * temperature measurement characteristic
   * handle
   */ 
  uint16_t tempMeasurementCharHandle;
      
  /**
   * temperature type characteristic
   * handle
   */ 
  uint16_t tempTypeCharHandle;
      
  /**
   * intermediate temperature
   * characteristic handle
   */ 
  uint16_t intermediateTempCharHandle;
      
  /**
   * measurement interval characteristic
   * handle
   */ 
  uint16_t miCharHandle;
  
  /**
   * connection handle
   */
   uint16_t connHandle;

  /**
   * profile interface context. This is
   * registered with the main profile
   */ 
  tProfileInterfaceContext HTtoBLEInf;
}tThermometerContext;


/******************************************************************************
* Function Declarations
******************************************************************************/
/**
 * 
 * @param[in] thermometerFeatures: 
 * bitmask for the characteristics to be added to the
 * health thermometer service\n
 * INTERMEDIATE_TEMPERATURE_CHAR - bit 0
 * MEASUREMENT_INTERVAL_CHAR     - bit 1
 * TEMPERATURE_TYPE				 - bit 3
 * if bit3 is set, the temperature type characteristic will
 * be added and its value cannot be updated while in a connection.
 * If the temperature type has to be notified in every measurement
 * value, then this bit should not be set
 * @param[in] minValidInterval: the minimum valid interval
 * value for the measurement interval characteritic. This is
 * valid only if the MEASUREMENT_INTERVAL_CHAR flag is set
 * in the thermometerFeatures
 * @param[in] maxValidInterval: the maximum valid interval
 * value for the measurement interval characteritic. This is
 * valid only if the MEASUREMENT_INTERVAL_CHAR flag is set
 * in the thermometerFeatures
 * @param[in] applcb: callback function to be called by
 * the profile to notify the application of the events
 * 
 * Initializes the health thermometer profile and registers
 * it with the BLE profile
 * 
 * @return BLE_STATUS_SUCCESS if the profile was successfully
 * registered
 */ 
tBleStatus HT_Init(tThermometerFeatures thermometerFeatures,
                   uint16_t minValidInterval,
                   uint16_t maxValidInterval,
                   BLE_CALLBACK_FUNCTION_TYPE applcb); 

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
tBleStatus HT_Advertize(uint8_t useWhitelist);

/**
 * 
 * @param[in] tempMeasurementVal :  temperature measurement value
 * the flags have to be set according to the flag bitmask defined 
 * in the health_thermometer header
 * 
 * updates the temperature measurement characteristic
 * 
 * @return BLE_STATUS_SUCCESS if the update was started succesfully.
 * Once the update is completed, an event 
 */ 
tBleStatus HT_Send_Temperature_Measurement(tTempMeasurementVal tempMeasurementVal);

/**
 * 
 * @param[in] tempMeasurementVal :  temperature measurement value
 * the flags have to be set according to the flag bitmask defined 
 * in the health_thermometer header. It is recommended not to use 
 * the timestamp and temperature type fields
 * 
 * updates the temperature measurement characteristic
 * 
 * @return BLE_STATUS_SUCCESS if the update was started succesfully.
 * Once the update is completed, an event 
 */ 
tBleStatus HT_Send_Intermediate_Temperature(tTempMeasurementVal tempMeasurementVal);

/**
 * 
 * @param[in] interval : the interval value to be written to
 * the measurement interval characteristic. The temperature
 * measurements will be sent only after a gap of the interval
 * specified. If the application gives a value to be sent before
 * the interval is passed, an error is returned. A value of
 * 0 indicates that there need not be a periodic update
 */ 
tBleStatus HT_Update_Measurement_Interval(uint16_t interval);

/**
 * 
 * @param[in] type : one of the values defined in
 *            the header
 * 
 * Updates the temperature type. The temperature
 * type indicates the part of the body where the
 * temperature is being measured. This has to be 
 * static during an active connection
 */ 
tBleStatus HT_Update_Temperature_Type(uint8_t type);

/**
 * 
 * HR profile's state machine: to be called on application main loop. 
 */ 
tBleStatus HT_StateMachine(void);


#endif /* _HEALTH_THERMOMETER_H_ */
