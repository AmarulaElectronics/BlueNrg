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
*   FILENAME        -  heart_rate.h
*
*   COMPILER        -  EWARM
*
********************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      09/03/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    Heart Rate Profile Header File
*
********************************************************************************
*
*  File Description 
*  ---------------------
*  heart rate profile SAP declarations and related macros
*
*******************************************************************************/
#ifndef _HEART_RATE_H_
#define _HEART_RATE_H_

/*******************************************************************************
* Macros
*******************************************************************************/
/* Number of virtual Advertising timer used */
#define ADVERTISE_TIMER         1

/* Advertising intervals in terms of 625 micro seconds */
#define HRP_FAST_CONN_ADV_INTERVAL_MIN	               (0x20)  /* 20ms */
#define HRP_FAST_CONN_ADV_INTERVAL_MAX	               (0x30)  /* 30ms */
#define HRP_LOW_PWR_ADV_INTERVAL_MIN	               (0x640) /* 1 second */
#define HRP_LOW_PWR_ADV_INTERVAL_MAX	               (0xFA0) /* 2.5 seconds */

/* heart rate profile main states */
#define HRPROFILE_STATE_UNINITIALIZED                  (0x00)
#define HRPROFILE_STATE_WAITING_BLE_INIT               (0x10)
#define HRPROFILE_STATE_INITIALIZED                    (0x20)
#define HRPROFILE_STATE_ADVERTIZE                      (0x30)
#define HRPROFILE_STATE_CONNECTED                      (0x40)
#define HRPROFILE_STATE_INVALID                        (0xFF)

/* substates for HRPROFILE_STATE_ADVERTIZE main state */
#define HRPROFILE_STATE_ADVERTISABLE_IDLE                            (0x31) 
#define HRPROFILE_STATE_ADVERTISING_FOR_WHITE_LIST                   (0x32) 
/* This substate is set on timer timeout */
#define HRPROFILE_STATE_ADVERTISING_FOR_WHITE_LIST_TIMEOUT_DO_STOP   (0x33)
#define HRPROFILE_STATE_FAST_DISCOVERABLE_DO_START                   (0x34)
#define HRPROFILE_STATE_FAST_DISCOVERABLE                            (0x35) 
/* This substate is set on timer timeout */
#define HRPROFILE_STATE_FAST_DISCOVERABLE_TIMEOUT_DO_STOP            (0x36)
#define HRPROFILE_STATE_LOW_POWER_DISCOVERABLE_DO_START              (0x37)
#define HRPROFILE_STATE_LOW_POWER_DISCOVERABLE                       (0x38) 
/* This substate is set on timer timeout */
#define HRPROFILE_STATE_LOW_POWER_DISCOVERABLE_TIMEOUT_DO_STOP       (0x39)
#define HRPROFILE_STATE_BLE_ADVERTISING                              (0x3A) 
#define HRPROFILE_STATE_PROCESSING_PAIRING                           (0x3B)

/* substates for HRPROFILE_STATE_CONNECTED main state */
#define HRPROFILE_STATE_CONNECTED_IDLE                 (0x41)
#define HRPROFILE_STATE_DO_DISCONNECT                  (0x42)
#define HRPROFILE_STATE_DISCONNECTING                  (0x43)
#define HRPROFILE_STATE_DISCONNECTED                   (0x44)


#define BODY_SENSOR_LOCATION_SUPPORT_MASK              (0x01)
#define ENERGY_EXTENDED_INFO_SUPPORT_MASK              (0x02)

#define HR_CNTL_POINT_RESET_ENERGY_EXPENDED            (0x01)

#define HRM_CNTL_POINT_VALUE_IS_SUPPORTED              (0x00)
#define HRM_CNTL_POINT_VALUE_NOT_SUPPORTED             (0x80)

#define CHAR_MODIFIED_NOTIFICATION		       (0x01)


/*******************************************************************************
* type definitions
*******************************************************************************/

#define HRM_VALUE_FORMAT_UINT8                         (0x00)
#define HRM_VALUE_FORMAT_UINT16                        (0x01)

#define HRM_SENSOR_CONTACT_NOT_PRESENT                 (0x00)
#define HRM_SENSOR_CONTACT_PRESENT                     (0x02)

#define HRM_SENSOR_CONTACT_NOT_SUPPORTED               (0x00)
#define HRM_SENSOR_CONTACT_SUPPORTED                   (0x04)

#define HRM_ENERGY_EXPENDED_NOT_PRESENT                (0x00)
#define HRM_ENERGY_EXPENDED_PRESENT                    (0x08)

#define HRM_RR_INTERVAL_NOT_PRESENT                    (0x00)
#define HRM_RR_INTERVAL_PRESENT                        (0x10)


typedef uint8_t valformat;

typedef struct _tHeartRateMeasure
{
	/**
	 * indicates the format of the 
	 * heart measurement value:
	 * 0 if UINT8
	 * 1 if UINT16
	 */ 
	valformat valueformat;

	/**
	 * this field indicates whether the
	 * sensor is in contact with the body
	 * 0 no or poor contact
	 * 1 otherwise
	 * NOTE: this field is valid only if the
	 * sensorContactSupport is set to TRUE
	 * during initialization. Else this value 
	 * will be ignored
	 */ 
	uint8_t sensorContact;

	/**
	 * indicates whether the EE field is present
	 * in the current characteristic value
	 * 0 not present
	 * 1 present
	 * if set to 0,the enrgyExpended will be ignored
	 */ 
	uint8_t energyExpendedStatus;

	/**
	 * indicates whether RR interval values
	 * are present in the current characteristic value
	 * 0 not present
	 * 1 present
	 * if set to 0, the rrInterval field will be ignored
	 */ 
	uint8_t rrIntervalStatus;

	/**
	 * can be 16 bit or 8bit 
	 * this will be specified by the flag
	 */
	uint16_t heartRateValue;

	 /**
	  * if present,
	  * this is always a 16 bit value 
	  */
	uint16_t enrgyExpended;

	/**
	 * 9 is the maximum length of RR interval values allowed
	 * (if EE is not present and mesurement value is 8bit)
	 * if the length is more than 9, then only the last 9
	 * will be considered assuming they correspond to the 
	 * latest collected data. 
     */
	uint8_t numOfRRIntervalvalues;
	
	/** 
	 * it is the responsibility of application to provide maximum
     * of 9 latest RR Interval values filled in the following 
     * buffer in chronicle order with increasing buffer index. 
	 */
	uint16_t rrIntervalValues[9]; 
}tHeartRateMeasure;


typedef uint8_t tBodySensorLocation;
#define BODY_SENSOR_LOCATION_OTHER           (0x00)
#define BODY_SENSOR_LOCATION_CHEST           (0x01)
#define BODY_SENSOR_LOCATION_WRIST           (0x02)
#define BODY_SENSOR_LOCATION_FINGER          (0x03)
#define BODY_SENSOR_LOCATION_HAND            (0x04)
#define BODY_SENSOR_LOCATION_EAR_LOBE        (0x05)
#define BODY_SENSOR_LOCATION_FOOT            (0x06)


typedef struct _tHeartRateContext
{
  /**
  * main state of the heart rate
  * sensor state machine
  */ 
  tProfileState mainState;
  
  /**
  * sub state of the heart rate
  * sensor state machine
  */ 
  tProfileState subState;
  
  /**
  * FOR INTERNAL USE ONLY
  * the sensor location information is not sent by
  * default in the heart rate measurement. If it needs to
  * be sent, the application needs to enable it before 
  * sending out any heart rate measurement updates.
  * this flag keeps track of the communication status
  * of the heart rate measurement
  */
  uint8_t hrmCommunicationStarted;
  
  /**
  * callback to notify the applciation with
  * the profile specific events
  */ 
  BLE_CALLBACK_FUNCTION_TYPE applicationNotifyFunc;
  
  /**
  * connection handle
  */ 
  uint16_t connHandle;
  /**
  * the characteristics supported in
  * the heart rate service
  */ 
  uint16_t serviceSupport;
  
  /**
  * heart rate service handle
  */ 
  uint16_t hrServiceHandle;
  
  /**
  * heart rate measurement characteristic
  * handle
  */ 
  uint16_t hrmCharHandle;
  
  /**
  * heart rate measurement characteristic
  * descriptor handle
  */ 
  uint16_t hrmCharDescHandle;
  
  /**
  * body sensor location 
  * characteristic handle
  */ 
  uint16_t bodySensorLocCharHandle;
  
  /**
  * control point characteristic
  * handle
  */ 
  uint16_t ctrlPointCharHandle;
  
  /**
  * device information characteristic
  * handle
  */ 
  uint16_t devInfoServiceHandle;
  
  /**
  * manufacturer string characteristic
  * handle of the device information service
  */ 
  uint16_t manufacturerStringCharHandle;
  
  /**
  * sensor contact support flag
  * when set, the sensor location
  * information is included in the
  * heart rate measurement characteristic
  */ 
  uint8_t sensorContactSupport;
  
  /**
  * profile interface context. This is
  * registered with the main profile
  */ 
  tProfileInterfaceContext HRStoBLEInf;
  
  /**
  * body sensor location
  */ 
  tBodySensorLocation hrsBodySensorLocation;
}tHeartRateContext;


/*******************************************************************************
 * function prototypes
*******************************************************************************/
/**
 * 
 * @param[in] featureSupportFlag : characteristics to be exposed in the
 *            heart rate service
 * @param[in] hrProfileCallbackFunc: callback to be used by the profile
 *            to notify the application of profile specific events
 * @param[in] sensorLocation : value for sensor location
 * 
 * initializes the heart rate sensor
 * 
 * @return BLE_STATUS_SUCCESS if the profile is successfully initialized
 * else BLE_STATUS_FAILED. If the callback is NULL, BLE_STATUS_NULL_PARAM
 * is returned. If another instance of the profile is already running,
 * BLE_STATUS_ACTION_DISALLOWED is returned
 */
tBleStatus HRProfile_Init(uint8_t featureSupportFlag, 
                          BLE_CALLBACK_FUNCTION_TYPE hrProfileCallbackFunc, 
                          tBodySensorLocation sensorLocation);


/**
 * 
 * @param[in] useBoundedDeviceList: advertize with whitelist which contains previously
 *            bonded devices
 * 
 * puts the device into discoverable mode
 *
 * @return BLE_STATUS_SUCCESS if the command to put the device into
 * discoverable mode was issued successfully
 */
tBleStatus HR_Sensor_Make_Discoverable(uint8_t useBoundedDeviceList);

/**
 * this function will be called by the application when the
 * sensor has collected some data that has to be sent
 */ 
tBleStatus HRProfile_Send_HRM_Value(tHeartRateMeasure heartRateVal);


/**
 * 
 * this function has to be called by the application once
 * a connection is established before sending any data.
 * The default value is not supported. If this function is 
 * called after sending data in a connection , it has no effect
 * and will return BLE_STATUS_FAILED
 */ 
tBleStatus HRProfile_Set_Sensor_Contact_Support_Bit(void);

/**
 * 
 * @param[in] bdsValue : body sensor location value
 * 
 * updates the body sensor location characteristic with
 * the value provided. the body sensor location is static
 * when in connection. So it is allowed to be set only when
 * the device is not in connection
 */ 
tBleStatus HRProfile_Set_Body_Sensor_Location(tBodySensorLocation bdsValue);

/**
 * 
 * HR profile's state machine: to be called on application main loop. 
 */ 
tBleStatus HRProfile_StateMachine(void);

#endif  /* _HEART_RATE_H_ */

