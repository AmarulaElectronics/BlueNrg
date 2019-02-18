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
*   FILENAME        -  glucose_sensor.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      20/04/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    glucose sensor
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the glucose sensor profile role for BlueNRG-1
* 
******************************************************************************/


/******************************************************************************
 * Include Files
******************************************************************************/

#ifndef _GLUCOSE_SENSOR_H_
#define _GLUCOSE_SENSOR_H_

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
/* Number of virtual timer used */
#define GL_ADVERTISE_TIMER                 1
#define GL_IDLE_CONNECTION_TIMER           2

/* profile states */
#define GL_UNINITIALIZED		        (0x00)
#define GL_WAITING_BLE_INIT                     (0x01)
#define GL_IDLE					(0x02)
#define GL_INITIALIZED				(0x03)
#define GL_UNDIRECTED_ADV_MODE			(0x04)
#define GL_ADV_IN_FAST_DISC_MODE	 	(0x05)
#define GL_ADV_IN_LOW_PWR_MODE			(0x06)
#define GL_CONNECTED				(0x07)
//GL_CONNECTED sub-state
#define GL_CONNECTED_DO_NOTIFY			(0x08)
#define GL_CONNECTED_STOP_NOTIFY 	        (0x09)
#define GL_CONNECTED_DO_INDICATION              (0x0A) 
#define GL_CONNECTED_DO_WRITE_RESP              (0x0B) 

#define GL_ADV_IN_FAST_DISC_MODE_STOP           (0x0C)
#define GL_UNDIRECTED_ADV_MODE_STOP             (0X0D)


/* advertising intervals in terms of 625 micro seconds */
#define GL_FAST_CONN_ADV_INTERVAL_MIN	(0x20)  /* 20ms */
#define GL_FAST_CONN_ADV_INTERVAL_MAX	(0x30)  /* 30ms */
#define GL_LOW_PWR_ADV_INTERVAL_MIN	(0x640) /* 1 second */
#define GL_LOW_PWR_ADV_INTERVAL_MAX	(0xFA0) /* 2.5 seconds */

/* Glucose Profiles timer periods */

#define GL_TIME_FACTOR  1 /* 1 for seconds */
#define GL_FAST_CONNECTION_TIMER_DURATION (30 * GL_TIME_FACTOR) /* 30 seconds */
#define GL_UNDIRECT_CONNECTABLE_MODE_TIMER_DURATION (120 * GL_TIME_FACTOR) /* 120 seconds: it is defined by glucose sensor */
#define GL_IDLE_CONNECTION_TIMER_DURATION (5 * GL_TIME_FACTOR)  /* 5 seconds */


/* Set, unset record status flag values */
#define SET_FLAG (TRUE)
#define UNSET_FLAG (FALSE)

/* Glucose stored record has not been deleted  through a RACP delete stored 
   record procedure  */
#define GLUCOSE_RECORD_NOT_DELETED_FROM_RACP (0x01)

/* Glucose stored record has been selected through a RACP procedure */
#define GLUCOSE_RECORD_FILTERED_FROM_RACP   (0x02)

#define GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED (GLUCOSE_RECORD_NOT_DELETED_FROM_RACP | \
                                                 GLUCOSE_RECORD_FILTERED_FROM_RACP)

#define GLUCOSE_MEASUREMENT_MAX_LEN         (17)
#define GLUCOSE_MEASUREMENT_CONTEXT_MAX_LEN (17)
#define GLUCOSE_FEATURE_MAX_LEN             (2)
#define RACP_COMMAND_MAX_LEN                (17) /* TBC (Within Range with Time)*/
/* RACP response lenght */
#define RACP_RESPONSE_LENGHT                (4)

/* Chacteristic Descriptors handles are properly configured */
#define RACP_DESCRIPTOR_PROPERLY_CONFIGURED                (0x01)
#define MEASUREMENT_DESCRIPTOR_PROPERLY_CONFIGURED         (0x02)
#define MEASUREMENT_CONTEXT_DESCRIPTOR_PROPERLY_CONFIGURED (0x04)
#define GLUCOSE_DESCRIPTORS_ALL_PROPERLY_CONFIGURED        (RACP_DESCRIPTOR_PROPERLY_CONFIGURED | \
                                                            MEASUREMENT_DESCRIPTOR_PROPERLY_CONFIGURED | \
                                                            MEASUREMENT_CONTEXT_DESCRIPTOR_PROPERLY_CONFIGURED)
/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )

/******************************************************************************
* Type definitions
******************************************************************************/

/* Glucose measurement characteristic: Base Time structure */
typedef struct _tBasetime
{
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t hours;
   uint8_t minutes;
   uint8_t seconds;
} tBasetime;

/* Glucose measurement characteristic structure */ 
typedef struct _tGlucoseMeasurementVal {
   /**
    *  Flag to identify the glucose database record status
    * 
    */ 
   uint8_t record_status_flag;
   /**
    * bitmask of the presence fields  in the characteristic
    */ 
   uint8_t flags;
   uint16_t sequenceNumber;
   tBasetime baseTime;
   int16_t timeOffset;
   uint16_t glucoseConcentration; /* SFLOAT units of Kgs or Liters */
   uint8_t typeSampleLocation;
   uint16_t sensorStatusAnnunciation;
} tGlucoseMeasurementVal;

/* Glucose measurement context characteristic structure */ 
typedef struct _tGlucoseMeasurementContextVal {
   /**
    * bitmask of the presence fields  in the characteristic
    */ 
   uint8_t flags;
   uint16_t sequenceNumber;
   uint8_t extendedFlags;
   uint8_t carbohydrateId;
   uint16_t carbohydrateUnits; /* SFLOAT units of Kgs */
   uint8_t meal;
   uint8_t testerHealth;
   uint16_t exerciseDuration;
   uint8_t exerciseIntensity;
   uint8_t medicationId;
   uint16_t medicationUnits; /* SFLOAT units of Kgs or Liters */
   uint16_t HbA1c; /* SFLOAT units of Kgs or Liters */
} tGlucoseMeasurementContextVal;

/* Glucose profile context */ 
typedef struct _tGlucoseContext
{
  /**
   * state of the glucose
   * sensor state machine
   */ 
  tProfileState state;
  
  /**
   * substate of the glucose
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
   * flag used to indicate if there is a database record measurement context
   * to be sent  
   */
  BOOL RACP_SendNextMeasurementContextRecord_Flag;

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
   * flag used to keep last sequence number related to the 
   * stored measurments on database. 
   */
  uint16_t sequenceNumber;
	
  /**
   * callback to notify the applciation with
   * the profile specific events
   */ 
  BLE_CALLBACK_FUNCTION_TYPE applcb;
	
  /**
   * glucose feature characteristic value 
   */ 
  uint16_t featureValue;
	
  /**
   * glucose service handle
   */ 
  uint16_t glServiceHandle;
	
  /**
   * glucose measurement characteristic
   * handle
   */ 
  uint16_t MeasurementCharHandle;
	
  /**
   * glucose measurement context characteristic
   * handle
   */ 
  uint16_t MeasurementContextCharHandle;
	
  /**
   * glucose feature 
   * characteristic handle
   */ 
  uint16_t FeatureCharHandle;
	
  /**
   * Record Access Control Point (RACP) characteristic
   * handle
   */ 
  uint16_t RACPCharHandle;
	
  /* received RACP command */
  uint8_t attValue[RACP_COMMAND_MAX_LEN]; 

  /* descriptors configuration flag */
  uint8_t descriptors_configuration;
  /**
   * connection handle
   */
  uint16_t connHandle;
 
  /**
   * profile interface context. This is
   * registered with the main profile
   */ 
  tProfileInterfaceContext GLtoBLEInf;
  
  /**
   * max number of stored records used on glucose database
   */
  uint16_t max_number_of_used_records;
  /** 
   * glucose measurement database pointer 
   */
  tGlucoseMeasurementVal * glucoseDatabase_MeasurementRecords_ptr;  
  /**
   * Glucose measurement context database pointer 
   */
  tGlucoseMeasurementContextVal * glucoseDataBase_MeasurementContextRecords_ptr;
}tGlucoseContext;

/******************************************************************************
* Imported Variable
******************************************************************************/
extern tGlucoseContext glucose;

/******************************************************************************
* Function Declarations
******************************************************************************/
/**
  * @brief  Reset the initialization flags for glucose sensor 
  * @param sequenceNumber : last store sequence number on database
  * @retval None
  */
void GL_ResetFlags(uint16_t sequenceNumber);

/**
  * @brief Init Glucose Sensor 
  * @param [in] sequenceNumber : last stored sequence number on database
  * @param [in] gl_measurement_db_records : pointer to user glucose measurement database
  * @param [in] gl_measurement_context_db_records : pointer to user glucose measurement context database
  * @param [in] applcb :    glucose profile callback 
  * @retval tBleStatus :    init status
  */
tBleStatus GL_Init(uint16_t sequenceNumber, tGlucoseMeasurementVal * gl_measurement_db_records, tGlucoseMeasurementContextVal * gl_measurement_context_db_records, BLE_CALLBACK_FUNCTION_TYPE applcb);

/**
  * @brief  Handle Glucose Sensor Advertising
  * @param  [in] useWhitelist : it defines which advertising mode to be used 
  * @retval tBleStatus :   function status
  */
tBleStatus GL_Advertize(uint8_t useWhitelist);

/**
  * @brief  Send a Glucose Measurement
  * @param  [in] glucoseMeasurementVal : glucose measurement to be sent
  * @retval tBleStatus :            function status
  */
tBleStatus GL_Send_Glucose_Measurement(tGlucoseMeasurementVal glucoseMeasurementVal);

/**
  * @brief  Send a Glucose Measurement Context (it follows a Glucose Measurement)
  * @param [in] glucoseMeasurementVal : glucose measurement context to be sent
  * @retval tBleStatus :            function status
  */
tBleStatus GL_Send_Glucose_Measurement_Context(tGlucoseMeasurementContextVal glucoseMeasurementContextVal);

/**
  * @brief  Set the glucose feature characteristic value
  * @param [in] value : glucose feature value
  * @retval tBleStatus : function status
  */
tBleStatus GL_Set_Glucose_Feature_Value(uint16_t value);

/**
 * GL_StateMachine
 * 
 * @param[in] None
 * 
 * GL profile's state machine: to be called on application main loop. 
 */ 
tBleStatus GL_StateMachine(void);


#endif /* _GLUCOSE_SENSOR_H_ */
