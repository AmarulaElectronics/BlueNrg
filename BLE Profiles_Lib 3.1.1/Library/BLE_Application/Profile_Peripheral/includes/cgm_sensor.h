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
*   FILENAME        -  cgm_sensor.h
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
*   Comments:    cgm sensor
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the cgm sensor profile role for BlueNRG-1
* 
******************************************************************************/


/******************************************************************************
 * Include Files
******************************************************************************/

#ifndef _CGM_SENSOR_H_
#define _CGM_SENSOR_H_

#include "stdio.h"
#include "string.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "gp_timer.h"
#include "ble_profile.h"
#include "uuid.h"
#include "host_config.h"
#include "debug.h"
#include "osal.h"
#include "ble_events.h"

/******************************************************************************
* Macro Declarations
******************************************************************************/
//#define PTS_TEST_CGM 1

#define CGM_DATABASE_MAX_RECORDS  (20) 

#define DEFAULT_MEASUREMENT_INTERVAL        1  //default measurement interval value in minutes 

#ifdef PTS_TEST_CGM 
#define PTS_TEST_MEASUREMENT_INTERVAL    5000  //measurement interval value in ms used for PTS tests  
#endif

/* Number of virtual timer used */
#define CGM_ADVERTISE_TIMER                 1   //this timer id is used for advertising
#define CGM_MEASUREMENT_TIMER               2   //this timer id is used to do periodic measurements
#define CGM_PERIODIC_NOTIFICATION_TIMER     3   //this timer id is used for periodic measurements notifications

/* profile states */
#define CGM_UNINITIALIZED		         (0x00)
#define CGM_WAITING_BLE_INIT                     (0x01)
#define CGM_IDLE			         (0x02)
#define CGM_INITIALIZED				 (0x03)
#define CGM_UNDIRECTED_ADV_MODE			 (0x04)
#define CGM_ADV_IN_FAST_DISC_MODE	 	 (0x05)
#define CGM_ADV_IN_LOW_PWR_MODE			 (0x06)
#define CGM_ADV_IN_FAST_DISC_MODE_STOP           (0x07)
#define CGM_UNDIRECTED_ADV_MODE_STOP             (0X08)
#define CGM_CONNECTED			  	 (0x09)

//CGM_CONNECTED sub-state
#define CGM_CONNECTED_DO_NOTIFY			 (0x0A)
#define CGM_CONNECTED_STOP_NOTIFY 	         (0x0B)
#define CGM_CONNECTED_SEND_RACP_RESPONSE         (0x0C) //used to resend a RACP response in case of INSUFFICIENT RESOURCES
#define CGM_CONNECTED_SEND_MEASUREMENT           (0x0D) //used to resend a measurement record in case of INSUFFICIENT RESOURCES 
#define CGM_CONNECTED_SEND_SOCP_RESPONSE         (0X0E) //used to resend a SOCP response in case of INSUFFICIENT RESOURCES
#define CGM_CONNECTED_PERIODIC_NOTIFICATION      (0X0F) 
#define CGM_CONNECTED_PERIODIC_NOTIFICATION_STOP (0X10)


/* advertising intervals in terms of 625 micro seconds */
#define CGM_FAST_CONN_ADV_INTERVAL_MIN	(0x30)   /* 30 ms */ 
#define CGM_FAST_CONN_ADV_INTERVAL_MAX	(0x1E0)  /* 300 ms */
#define CGM_LOW_PWR_ADV_INTERVAL_MIN	(0x640)  /* 1 second */
#define CGM_LOW_PWR_ADV_INTERVAL_MAX	(0x4000) /* 10.24 seconds */

/* Cgm Profiles timer periods */
#define CGM_TIME_FACTOR  1 /* 1 for seconds */
#define CGM_FAST_CONNECTION_TIMER_DURATION (30 * CGM_TIME_FACTOR) /* 30 seconds */
#define CGM_UNDIRECT_CONNECTABLE_MODE_TIMER_DURATION (120 * CGM_TIME_FACTOR) /* 120 seconds: it is defined by cgm sensor */

/* Set, unset record status flag values */
#define SET_FLAG (TRUE)
#define UNSET_FLAG (FALSE)

/* Cgm stored record has not been deleted  through a RACP delete stored 
   record procedure  */
#define CGM_RECORD_NOT_DELETED_FROM_RACP (0x01)

/* Cgm stored record has been selected through a RACP procedure */
#define CGM_RECORD_FILTERED_FROM_RACP    (0x02)

#define CGM_RECORD_FILTERED_AND_NOT_DELETED (CGM_RECORD_NOT_DELETED_FROM_RACP | \
                                             CGM_RECORD_FILTERED_FROM_RACP)

/* MEASUREMENT length: The minimum size is 6 octets and is enlarged if more octets are indicated by the Flags Field 
   (Sensor Status Annunciation Field, CGM Trend Information Field and CGM Quality Field) and 
   the optional E2E-CRC Field. The Size Field itself is included in the length calculation.
*/
#define CGM_MEASUREMENT_MAX_LEN         (15)    /* 6 MIN, 15 MAX */
#define CGM_FEATURE_MAX_LEN             (6)     /* E2E-CRC field is mandatory in this characteristic. 
                                                   If the device doesn´t support E2E-safety the value of the field shall be set to 0xFFFF.*/
#define CGM_STATUS_MAX_LEN              (5 + 2) /* +2 if device supports E2E-CRC (Bit 12 is set in CGM Feature) */   

#define CGM_SESSION_START_TIME_MAX_LEN  (9 + 2) /* +2 for E2E-CRC (optional) */

#define CGM_SESSION_RUN_TIME_MAX_LEN    (2 + 2) /* +2 for E2E-CRC (optional) */

#define RACP_COMMAND_MAX_LEN            (7)  /* shall be (7) ==> Op Code 1 byte, Operator 1 byte,
                                                Operand = Filter Type 1 byte + Filter Parameters 4 byte (for couple of value) */

#define SOCP_COMMAND_MAX_LEN            (10 + 1 + 2) /* 10 byte for Calibration Data Record if supported  
                                                        + 1 byte for Op code + 2 for E2E-CRC (optional) */

/* RACP response lenght */
#define RACP_RESPONSE_LENGHT            (4)

/* SOCP response lenght */
#define SOCP_RESPONSE_LENGHT            (11+2)  /* Length of a response. DEFAULT = 3 (Response OpCode 1 + Request OpCode code 1 byte + Response Code value 1)
                                                   MIN = 2 (Communication Interval response 1 + Communication Interval minutes 1), 
                                                   MAX = 11 (Response OPCode 1 + Calibration Record 10) 
                                                   + 2 for E2E-CRC (optional)*/

/* Chacteristic Descriptors handles are properly configured */
#define RACP_DESCRIPTOR_PROPERLY_CONFIGURED                (0x01)
#define MEASUREMENT_DESCRIPTOR_PROPERLY_CONFIGURED         (0x02)
#define SOCP_DESCRIPTOR_PROPERLY_CONFIGURED                (0x04)
#define CGM_DESCRIPTORS_ALL_PROPERLY_CONFIGURED            (RACP_DESCRIPTOR_PROPERLY_CONFIGURED | \
                                                            MEASUREMENT_DESCRIPTOR_PROPERLY_CONFIGURED | \
                                                            SOCP_DESCRIPTOR_PROPERLY_CONFIGURED) 
/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )

#define STORE_LE_32(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) , \
                                   ((buf)[2] =  (uint8_t) (val>>16) ) , \
                                   ((buf)[3] =  (uint8_t) (val>>24) ) )

/******************************************************************************
* Type definitions
******************************************************************************/

/* Cgm measurement characteristic: Base Time structure */
typedef struct _tBasetime
{
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t hours;
   uint8_t minutes;
   uint8_t seconds;
} tBasetime;

/* CGM Measurement Sensor Status Annunciation */ //TBR???
typedef struct
{
    uint8_t status;                /* Status annunciation */
    uint8_t cal_temp;              /* Calibration and Temperature annunciation */
    uint8_t warning;               /* Warning annunciation */
} tSensor_Status_Annunciation;

/* Cgm measurement characteristic structure */ 
typedef struct _tCgmMeasurementVal {
  
   /**
    *  Flag to identify the cgm database record status
    * 
    */ 
   uint8_t record_status_flag;
  
    /**
    * Size of the CGM Measurement record
    */
   uint8_t size;
   /**
    * Bitmask of the presence fields  in the characteristic
    */ 
   uint8_t flags;
   /**
    * CGM glucose concentration in mg/dL as a SFLOAT data type
    */
   uint16_t cgmGlucoseConcentration;
   /**
    * Time offset in minutes as offset to the Session Start Time (Value: 0x0000 - 0xFFFF)
    */
   uint16_t timeOffset;
 
   /* ---------- Optional fields --------------------- */
   /** 
    * Optional field comprising up to three octets
    */
   tSensor_Status_Annunciation sensorStatusAnnunciation; /* (optional) */
   /**
    * Contains the CGM Trend information in (mg/dL)/min as an SFLOAT data type (optional)
    */
   uint16_t cgmTrendInformation;  /* SFLOAT units (optional) */
   /**
    * Contains the CGM Quality information in % as an SFLOAT data type (optional)
    */
   uint16_t cgmQuality;           /* SFLOAT units (optional) */
   /**
    * If the device supports E2E-safety, the measurement is secured by a CRC 
    * calculated over all fields
    */
   uint16_t E2E_CRC;  /* (optional) */
} tCgmMeasurementVal;

/* CGM Feature characteristic structure */
typedef struct _tCgmFeatureVal
{  
    /**
     * Supported features (24 bits)
     */
    uint32_t feature;
    /** 
     * The CGM Type and CGM Sample Location Fields are each a nibble (4bit)
     * (least significant: Type, most significant: Sample Location)
     */
    uint8_t typeSampleLocation; 
    /**
     * Mandatory.
     * If the device supports E2E-safety, the feature are secured by a CRC calculated. 
     * If the device doesn´t support E2E-safety, the value of the field shall be set to 0xFFFF.
    */
    uint16_t E2E_CRC; 
} tCgmFeatureVal;


/* CGM  Status structure */ 
typedef struct _tCgmStatusVal {
   /**
    * Actual relative time difference to the session start time in minutes 
    */
   uint16_t timeOffset;  
   /**
    * This structure is identical to the Status Annunciation Field, but
    * it always consists of three octets
    */
   tSensor_Status_Annunciation cgmStatus; /* Same as Status Annunciation */
   /**
    * Optional. If the device supports E2E-safety.
    */
   uint16_t E2E_CRC; 
} tCgmStatusVal;

/* CGM  Session Start Time structure */ 
typedef struct _tCgmSessionStartTimeVal {
   /**
    * the time of the initial CGM measurement (Server shall calculate and store the Session 
    * Start Time using the time of the client and its own current relative time value)
    */
   tBasetime sessionStartTime;
   /**
    * Time Zone to which the Session Start Time is related to (Offset from UTC in  
    * number of 15 minutes increments. -128 if unknown)
    */
   int8_t timeZone; 
   /**
    * Daylight Saving setting (255 if unknown)
    */
   uint8_t DSTOffset; 
   /**s
    * Optional
    */
   uint16_t E2E_CRC;  
} tCgmSessionStartTimeVal;

/* CGM  Session Run Time structure */ 
typedef struct _tCgmSessionRunTimeVal {
  /**
  * It defines the expected run time of the CGM session
  */
  uint16_t sessionRunTime; 
  /**
  * Optional
  */
  uint16_t E2E_CRC; 
} tCgmSessionRunTimeVal;

/* CGM  Calibration Data Record structure  */ 
typedef struct _tCgmCalibrDataRecordVal {
  /**
   * Glucose Concentration of Calibration (unit mg/dL)
   */
  uint16_t GlucoseConcentrat_Calibr; 
  /**
   * Calibration Time (unit minutes)
   */
  uint16_t Calibr_Time; 
  /** 
   * The Calibration Type-Sample Location are each a nibble (4bit)
   * (least significant: Type, most significant: Sample Location)
   */
  uint8_t Calibr_TypeSampleLocation;   
  /**
   * Next Calibration Time (unit minutes)
   */
  uint16_t NextCalibr_Time;  
  /**
   * Calibration Data Record Number
   */
  uint16_t Calibr_DataRecordNum; 
  /**
   * Calibration Status
   */
  uint16_t Calibr_Status;   
} tCgmCalibrDataRecordVal;

/* CGM Specific Ops Control Point structure */
typedef struct _tCgmSpecOpsCtrlPointOperandVal {    
  /**
   * CGM Calibration Data Record structure array 
   */ 
  tCgmCalibrDataRecordVal cgmCalibrDataRecord[CGM_DATABASE_MAX_RECORDS];   //CGM_DATABASE_MAX_RECORDS??? TBR???
  /**
    * CGM Calibration Data Record Number: if it is '0', no calibration value is stored
   */ 
  uint16_t cgmCalibrDataRecord_Num;
  /**
  * CGM Patient High Alert Level in mg/dL as a SFLOAT data type
  */
   uint16_t cgmPatientHighAlert;
  /**
   * CGM Patient Low Alert Level in mg/dL as a SFLOAT data type
   */
   uint16_t cgmPatientLowAlert;
 
  /**
   * CGM Hypo Alert Level in mg/dL as a SFLOAT data type
   */
   uint16_t cgmHypoAlert;
  /**
   * CGM Hyper Alert Level in mg/dL as a SFLOAT data type
   */
   uint16_t cgmHyperAlert;   
   
  /**
   * CGM Rate of Decrease Alert Level in mg/dL/min as a SFLOAT data type
   */
   uint16_t cgmRateDecreaseAlert;
  /**
   * CGM Rate of Increase Alert Level in mg/dL/min as a SFLOAT data type
   */
   uint16_t cgmRateIncreaseAlert;    
   
   uint16_t cgmE2E_CRC_SOCP; 
   
} tCgmSpecOpsCtrlPointOperandVal;


/* Cgm profile context */ 
typedef struct _tCgmContext
{
  /**
   * state of the cgm
   * sensor state machine
   */ 
  tProfileState state;
  
  /**
   * substate of the cgm
   * sensor state machine
   */ 
   tProfileState substate;
   
    /** 
   * index of next database record measurement to be sent 
   * as consequence of a RACP report stored records request 
   */
  uint16_t RACP_NextMeasurementRecord_Index;

  /** 
   * flag used to indicate if there is a further database record measurement 
   * to be sent as consequence of a RACP report stored records request 
   */
  BOOL RACP_SendNextMeasurementRecord_Flag;
   
   /**
   * flag used to trigger if RACP abort operation has been received
   */
   BOOL RACP_Abort_Flag;

  /**
   * flag used to trigger if any  RACP procedure (different from abort)
   * is already in progress currently
   */
   BOOL RACP_In_Progress_Flag;
	
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
   * cgm service handle
   */ 
  uint16_t cgmServiceHandle;
	
  /**
   * cgm measurement characteristic
   * handle
   */ 
  uint16_t MeasurementCharHandle;
	
  /**
   * cgm feature 
   * characteristic handle
   */ 
  uint16_t FeatureCharHandle;
  
  /**
   * cgm status characteristic
   * handle
   */ 
  uint16_t StatusCharHandle;
  
  /**
   * cgm session start time characteristic
   * handle
   */ 
  uint16_t SessionStartTimeCharHandle;
	
   /**
   * cgm session run time characteristic
   * handle
   */ 
  uint16_t SessionRunTimeCharHandle;
  
  /**
   * Record Access Control Point (RACP) characteristic
   * handle
   */ 
  uint16_t RACPCharHandle;
  
  /**
   * Specific Ops Control Point (SOCP) characteristic
   * handle
   */ 
  uint16_t SOCPCharHandle;

  /**
   * cgm feature characteristic value 
   */ 
  tCgmFeatureVal featureValue;  
  
  /* CGM Status value*/
  tCgmStatusVal StatusValue;
    
  /* Session Start Time value  */
  tCgmSessionStartTimeVal SessionStartTimeValue;

  /* Session Run Time value  */
  tCgmSessionRunTimeVal SessionRunTimeValue;
  
    /* received RACP command */
  uint8_t attValue[RACP_COMMAND_MAX_LEN]; 
  
  /* received SOCP command */
  uint8_t attSOCPValue[SOCP_COMMAND_MAX_LEN];
  
  /* descriptors configuration flag */
  uint8_t descriptors_configuration;  //TBR???
  
  /**
   * used to keep last time offset related to the stored measurements
   * on database (time offset from the session start time)
   */
  uint16_t timeOffset;  
    
  /**
    * Measurement time interval (in minutes)
    * The CGM Sensor measures the glucose level periodically in a device specific interval 
    */
  uint16_t measurement_interval;

  /**
   * CGM communication interval value (in minutes)
   * When the CGM Communication Interval is set for periodic communication, 
   * the CGM Sensor periodically sends notifications. */
  uint8_t communication_interval; 
  
  /**
   * CGM Specific Ops Control Point Operand values
   */ 
  tCgmSpecOpsCtrlPointOperandVal cgmSOCPvalue;

  /* Indicate if periodic notifications are enable */
  BOOL periodic_notification;
  /* Indicate if the session has been started */
  BOOL session_start_indicator;
  
  /* Count the number of measurement records inserted in db (max value is CGM_DATABASE_MAX_RECORDS) */
  uint16_t meas_records_counter;
  /* Head index of the queue: it represents the oldest db record inserted */
  uint16_t head_index;
  /* Head index of the queue: it represents the last db record inserted */
  uint16_t tail_index;  
  /* Index of the last record send by notification */
  uint16_t last_record_notif_index;

  /**
   * profile interface context. This is
   * registered with the main profile
   */ 
  tProfileInterfaceContext CGMtoBLEInf; 
  
  /**
   * max number of stored records used on cgm database
   */
  uint16_t max_number_of_used_records;
  /** 
   * cgm measurement database pointer 
   */
  tCgmMeasurementVal * cgmDatabase_MeasurementRecords_ptr;  
 
}tCgmContext;

/******************************************************************************
* Imported Variable
******************************************************************************/
extern tCgmContext cgm;

/******************************************************************************
* Function Declarations
******************************************************************************/
/**
  * @brief  Reset the initialization flags for cgm sensor 
  * @param sequenceNumber : last store sequence number on database
  * @retval None
  */
void CGM_ResetFlags(uint16_t sequenceNumber);

/**
  * @brief Init Cgm Sensor 
  * @param [in] recordNumber : last stored sequence number on database
  * @param [in] cgm_measurement_db_records : pointer to user cgm measurement database
  * @param [in] applcb :    cgm profile callback 
  * @retval tBleStatus :    init status
  */
tBleStatus CGM_Init(uint16_t recordNumber, tCgmMeasurementVal * cgm_measurement_db_records, BLE_CALLBACK_FUNCTION_TYPE applcb);

/**
  * @brief  Handle Cgm Sensor Advertising
  * @param  [in] useWhitelist : it defines which advertising mode to be used 
  * @retval tBleStatus :   function status
  */
tBleStatus CGM_Advertize(uint8_t useWhitelist);

/**
  * @brief  Send a Cgm Measurement
  * @param  [in] cgmMeasurementVal : cgm measurement to be sent
  * @retval tBleStatus :            function status
  */
tBleStatus CGM_Send_Cgm_Measurement(tCgmMeasurementVal cgmMeasurementVal);


/**
  * @brief  Set the cgm feature characteristic value
  * @param [in] value : cgm feature value
  * @retval tBleStatus : function status
  */
tBleStatus CGM_Set_Cgm_Feature_Value(tCgmFeatureVal value);

/**
  * @brief  Set the cgm status characteristic value
  * @param  value: cgm feature value
  * @retval tBleStatus: function status
  */
tBleStatus CGM_Set_Cgm_Status_Value(tCgmStatusVal status_value);

/**
  * @brief  Set the cgm Session Start Time characteristic value
  * @param [in] value : cgm session start time value
  * @retval tBleStatus : function status
  */
tBleStatus CGM_Set_Cgm_Session_Start_Time_Value(tCgmSessionStartTimeVal value);

/**
  * @brief  Set the cgm session run time characteristic value
  * @param  value: cgm feature value
  * @retval tBleStatus: function status
  */
tBleStatus CGM_Set_Cgm_Session_Run_Time_Value(tCgmSessionRunTimeVal srt_value);

/**
 * CGM_StateMachine
 * 
 * @param[in] None
 * 
 * CGM profile's state machine: to be called on application main loop. 
 */ 
tBleStatus CGM_StateMachine(void);

/**
  * @brief  Set the feature value of CGM Feauture characteristic
  * @param[in] None
  * @retval None
  */   
void Set_Feature_Value(void);

/**
  * @brief  This callback sets the CGM Measurement Char value 
  * @param [in] value :  CGM Measurement Char record value
  */
extern void CGM_Set_CGM_Measurement_Value(tCgmMeasurementVal *measurementRecord);

/**
  * @brief  Convert an uint16_t to an SFLOAT 
  * @param [in] charValue :  value to convert
  */
float get_cgm_value(uint16_t charValue);

/**
  * @brief  Calculate CRC value 
  * @param [in] data_p: array containg data to protect by a CRC
  * @param [in] length: length of the array
  * @retval CRC value
  */
uint16_t Calculate_CRC(uint8_t *data_p, uint8_t length);

#endif /* _CGM_SENSOR_H_ */

