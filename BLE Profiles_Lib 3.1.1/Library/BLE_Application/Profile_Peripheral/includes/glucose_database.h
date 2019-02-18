/******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2014 STMicroelectronics International NV
*
*   FILENAME        -  glucose_database.h
*
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      01/07/2014
*   $Revision$:  first version
*   $Author$:    
*   Comments:    glucose database measurements records 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the glucose database measurement records.
*  Database record data are coming from an internal database with
*  "not real but emulated" glucose sensor data.
*  User can adapt, modify it in order to add support for a real glucose sensor
*  with related implications (Base Time values coming from a internal real-time 
*  clock or equivalent, Time Offset updated through a specific user interface, ...
*  
* 
******************************************************************************/


/******************************************************************************
 * Include Files
******************************************************************************/

#ifndef _GLUCOSE_DATABASE_H_
#define _GLUCOSE_DATABASE_H_

#include <glucose_service.h>
#include <glucose_racp.h>
#include <glucose_sensor.h>

/******************************************************************************
* Macro Declarations
******************************************************************************/
/* Define the Glucose database record max size 
   Refer to the glucose_database.h for current available list of
   glucose measurement  (user can update/modify them) */
#define GLUCOSE_DATABASE_MAX_RECORDS  (200) /* up to 255 could be supported */

/* Number of test records requested by SIG PTS glucose test suite
   (to be UPDATED based on specific test case: it has to be <= 
   GLUCOSE_DATABASE_MAX_RECORDS.
   
   NOTEs: 
   a) Set to 3 for all PTS Glucose Service Tests from 4.3.1 to 4.8.3, 4.8.5, 
   
   b) Set to 1 for test 4.8.4 (Glucose concentration is set to mol/L)
   c) Set to 5 for tests from 4.8.6 to 4.8.10, 4.8.12, 4.8.13(record with measurement
      context  + [liter is requested for record 5 measurement context] [it's default configuration].
   d) Set to 5 for test 4.8.11 + [Kg is requested for record 5 measurement context: MODIFY (REBUILD CODE): 
     NOTE: restore liter on record 5 measurement context after test 4.8.11. 

   e) Set to 3 for test 4.9.1 
   f) Set to 3 for tests 4.9.2,4.9.3, 4.9.4, 4.9.6, 4.9.7 
   g) Set to 4 for tests 4.9.5 (no context from record 4)
        
   h) Set to 3 for test 4.10.1 (delete all records).
      NOTE: It requires user action for sending 1 more record after all records are deleted:
            On profile_test_application uncomment: glucoseDatabase_Change_Database_Manually(1, SET_FLAG); [it's default configuration].
            When running PTS test, press Push Button when PTS ask for performing an action which will induce it to generate 1 more record.
   i) Set to 4 for test 4.10.2 
   l) NOTE: test 4.11 (abort operation)  requires ~ 200 records (NOT DONE)
   m) Set to 3 for test 4.12.1, 4.12.2, 
   n) Set to 3 for test 4.12.3 (n. of records not indicated).
      NOTE: It requires user action for deleting all records before running the PTS test. 
            On profile_test_application uncomment: glucoseDatabase_Change_Database_Manually(3, UNSET_FLAG); MODIFY (REBUILD CODE).
            When running the PTS test, press Push Button first time PTS asks  for  Secure ID.
            Restore profile_test_application.c after running this test.
   o) Set to 9 for test 4.13.1.
   p) Set to 3 for test 4.14.1 to 4.14.6, 4.14.8, 4.14.9
   q) NOTE: test 4.14.7 requires ~ 100 records (NOT DONE)
   
        
   For PTS tests don't change the glucose data content of the  available database 
   records 
*/
/* Enable PTS_TEST_GLUCOSE when using PTS tests */
#define PTS_TEST_GLUCOSE 1 
#define GLUCOSE_PTS_NUMBER_OF_RECORDS (3)  // (3), (9), (4), (5), (1), (200)

#ifdef PTS_TEST_GLUCOSE
#define GLUCOSE_NUMBER_OF_STORED_RECORDS     GLUCOSE_PTS_NUMBER_OF_RECORDS
#define GLUCOSE_DATABASE_MAX_CONTEXT_RECORDS  (9)
#else
#define GLUCOSE_NUMBER_OF_STORED_RECORDS     GLUCOSE_DATABASE_MAX_RECORDS
#define GLUCOSE_DATABASE_MAX_CONTEXT_RECORDS GLUCOSE_DATABASE_MAX_RECORDS
#endif
   
/******************************************************************************
* Type definitions
******************************************************************************/

/******************************************************************************
* Imported Variable
******************************************************************************/

/* Glucose measurement database: it's a simulated database with some default values */
tGlucoseMeasurementVal glucoseDatabase_MeasurementRecords[GLUCOSE_DATABASE_MAX_RECORDS] =
{
  /* glucose measurement: record 1 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    1,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL                             /* Status Annunciation field */
  },
  /* glucose measurement: record 2 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, 
    GLUCOSE_MEASUREMENT_FLAGS_ALL_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,  /* mmol/L */   
    //GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* (It is used for test 4.9.2 - 4.9.4, 4.9.6,4.9.7)*/
    2,                                    /* Sequence Number field */  
    {2013,3,22,4,2,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */               
    0xD032,                               /* SFLOAT exponent = -3(0xD); value: 0.050 mol/L Concentration field */ 
    GLUCOSE_TYPE_CAPILLARY_PLASMA | GLUCOSE_SAMPLE_LOCATION_AST ,/* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },

  /* glucose measurement: record 3 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT & ~GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,   /* kg/L */
    3,                                    /* Sequence Number field */  
    {2013,8,1,12,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */    
    0xB050,                               /* SFLOAT exponent = -5(0xB);80(0x50) mg/dl  Concentration field */
    GLUCOSE_TYPE_ARTERIAL_PLASMA | GLUCOSE_SAMPLE_LOCATION_EARLOBE,/* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_TEMPERATURE_TOO_HIGH | GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_TEMPERATURE_TOO_LOW /* Status Annunciation field */
  },
  /* glucose measurement: record 4 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP,/* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    //GLUCOSE_MEASUREMENT_FLAGS_ALL_PRESENT & ~GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,   /* kg/L */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* it is used for test 4.9.5 */
    4,                                   /* Sequence Number field */  
    {2013,10,19,9,30,50},                /* Base Time  field */
    60,                                   /* Time offset field = +1hr */ 
    /* 0xB062,                             SFLOAT exponent = -5(0xB);98(0x62) mg/dl  Concentration field */
    0xD037,                              /* SFLOAT exponent = -3(0xD); value: 0.055 mol/L Concentration field */ 
    GLUCOSE_TYPE_INTERSISTIAL_FLUID | GLUCOSE_SAMPLE_LOCATION_AST,/* Type & Location fields */
    0                                    /* Status Annunciation field */
  },
  /* glucose measurement: record 5 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,/* mol/L */ /* (Used for test 4.8.6 - 4.8.13)*/
    5,                                    /* Sequence Number field */  
    {2013,12,18,13,10,10},                /* Base Time  field */
    60,                                   /* Time offset field = +1hr */ 
    0xD040,                               /* SFLOAT exponent = -3(0xD); value: 0.064 mol/L Concentration field */ 
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOCATION_FINGER,
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW | GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION /* Status Annunciation field */
  },
      
  /* glucose measurement: record 6 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,/* mol/L */ /* (Used for test 4.8.6 - 4.8.13)*/
    6,                                    /* Sequence Number field */  
    {2013,12,18,13,10,16},                /* Base Time  field */
    60,                                   /* Time offset field = +1hr */ 
    0xD040,                               /* SFLOAT exponent = -3(0xD); value: 0.064 mol/L Concentration field */ 
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOCATION_FINGER,
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW | GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION /* Status Annunciation field */
  },
  
  /* glucose measurement: record 7 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,/* mol/L */ /* (Used for test 4.8.6 - 4.8.13)*/
    7,                                    /* Sequence Number field */  
    {2013,12,18,13,10,18},                /* Base Time  field */
    -60,                                   /* Time offset field = -1 hrs */ 
    0xD040,                               /* SFLOAT exponent = -3(0xD); value: 0.064 mol/L Concentration field */ 
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOCATION_FINGER,
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW | GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION /* Status Annunciation field */
  },
  
  /* glucose measurement: record 8 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,/* mol/L */ /* (Used for test 4.8.6 - 4.8.13)*/
    8,                                    /* Sequence Number field */  
    {2013,12,18,13,10,19},                /* Base Time  field */
     -60,                                   /* Time offset field = -1 hrs */ 
    0xD040,                               /* SFLOAT exponent = -3(0xD); value: 0.064 mol/L Concentration field */ 
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOCATION_FINGER,
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW | GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION /* Status Annunciation field */
  },
  
  /* glucose measurement: record 9 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS,/* mol/L */ /* (Used for test 4.8.6 - 4.8.13)*/
    9,                                    /* Sequence Number field */  
    {2013,12,18,13,10,20},                /* Base Time  field */
     -60,                                   /* Time offset field = -1 hrs */ 
    0xD040,                               /* SFLOAT exponent = -3(0xD); value: 0.064 mol/L Concentration field */ 
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOCATION_FINGER,
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_DEVICE_BATTERY_LOW | GLUCOSE_SENSOR_STATUS_ANNUNCIATION_SENSOR_MALFUNCTION /* Status Annunciation field */
  },
  /* glucose measurement: record 10 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    10,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 11 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    11,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 12 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    12,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 13 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    13,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 14 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    14,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 15 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    15,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 16 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    16,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 17 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    17,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 18 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    18,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 19 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    19,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 20 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    20,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 21 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    21,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 22 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    22,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 23 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    23,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 24 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    24,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 25 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    25,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 26 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    26,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 27 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    27,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 28 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    28,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 29 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    29,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 30 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    30,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 31 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    31,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 32 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    32,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 33 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    33,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 34 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    34,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 35 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    35,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 36 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    36,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 37 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    37,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 38 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    38,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 39 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    39,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 40 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    40,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 41 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    41,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 42 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    42,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 43 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    43,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 44 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    44,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 45 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    45,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 46 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    46,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 47 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    47,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 48 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    48,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 49 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    49,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 50 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    50,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 51 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    51,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 52 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    52,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 53 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    53,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 54 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    54,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 55 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    55,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 56 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    56,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 57 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    57,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 58 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    58,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 59 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    59,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 60 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    60,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 61 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    61,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 62 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    62,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 63 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    63,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 64 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    64,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 65 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    65,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 66 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    66,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 67 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    67,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 68 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    68,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 69 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    69,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 70 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    70,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 71 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    71,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 72 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    72,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 73 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    73,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 74 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    74,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 75 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    75,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 76 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    76,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 77 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    77,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 78 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    78,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 79 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    79,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 80 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    80,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 81 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    81,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 82 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    82,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 83 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    83,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 84 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    84,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 85 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    85,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 86 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    86,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 87 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    87,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 88 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    88,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 89 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    89,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 90 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    90,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 91 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    91,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 92 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    92,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 93 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    93,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 94 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    94,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 95 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    95,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 96 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    96,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 97 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    97,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 98 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    98,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 99 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    99,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 100 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    100,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 101 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    101,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 102 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    102,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 103 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    103,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 104 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    104,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 105 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    105,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 106 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    106,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 107 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    107,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 108 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    108,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 109 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    109,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 110 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    110,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 111 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    111,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 112 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    112,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 113 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    113,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 114 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    114,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 115 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    115,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 116 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    116,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 117 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    117,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 118 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    118,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 119 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    119,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 120 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    120,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 121 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    121,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 122 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    122,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 123 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    123,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 124 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    124,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 125 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    125,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 126 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    126,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 127 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    127,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 128 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    128,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 129 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    129,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 130 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    130,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 131 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    131,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 132 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    132,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 133 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    133,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 134 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    134,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 135 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    135,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 136 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    136,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 137 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    137,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 138 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    138,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 139 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    139,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 140 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    140,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 141 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    141,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 142 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    142,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 143 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    143,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 144 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    144,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 145 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    145,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 146 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    146,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 147 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    147,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 148 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    148,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 149 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    149,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 150 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    150,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 151 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    151,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 152 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    152,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 153 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    153,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 154 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    154,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 155 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    155,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 156 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    156,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 157 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    157,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 158 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    158,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 159 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    159,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 160 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    160,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 161 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    161,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 162 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    162,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 163 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    163,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 164 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    14,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 165 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    165,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 166 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    166,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 167 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    167,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 168 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    168,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 169 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    169,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 170 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    170,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 171 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    171,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 172 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    172,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 173 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    173,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 174 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    174,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 175 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    175,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 176 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    176,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 177 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    177,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 178 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    178,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 179 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    179,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 180 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    180,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 181 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    181,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 182 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    182,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 183 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    183,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 184 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    184,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 185 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    185,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 186 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    186,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 187 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    187,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 188 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    188,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 189 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    19,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 190 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    190,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 191 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    191,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 192 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    192,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 193 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    193,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 194 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    194,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 195 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    195,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 196 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    196,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 197 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    197,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 198 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    18,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 199 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    199,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
  /* glucose measurement: record 200 */
  {
    GLUCOSE_RECORD_NOT_DELETED_FROM_RACP, /* GLUCOSE_RECORD_FILTERED_AND_NOT_DELETED ? */
    GLUCOSE_MEASUREMENT_FLAGS_ALL_NO_CONTEXT_PRESENT | GLUCOSE_MEAUREMENTS_FLAG_MMOL_L_UNITS, /* mmol/L */   
    200,                                    /* Sequence Number field */  
    {2013,2,22,4,0,0},                    /* Base Time  field */  
    0,                                    /* Time offset field */                        
    0xD046,                               /* SFLOAT exponent = -3(0xD); value: 0.070 mol/L Concentration field */ 
    GLUCOSE_TYPE_VENOUS_PLASMA | GLUCOSE_SAMPLE_LOCATION_CONTROL_SOLUTION, /* Type & Location fields */
    GLUCOSE_SENSOR_STATUS_ANNUNCIATION_ALL,                               /* Status Annunciation field */
  },
};/* end glucoseDatabase_MeasurementRecords[] */


/* Glucose measurement context database: it's a simulated database with some default values */
tGlucoseMeasurementContextVal glucoseDataBase_MeasurementContextRecords[GLUCOSE_DATABASE_MAX_CONTEXT_RECORDS] =
{
  /* glucose measurement context: record 1 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */
    1,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_LUNCH,             /* carbohydrate Id */
    0xD00F,                                                     /* SFLOAT exponent = -3(0xD), 15 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_POSTPRANDIAL,              /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_DURING_MENSES,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_SHORT_ACTING_INSULIN,/* medication Id */
    0xA05A,                                                     /* SFLOAT exponent = -6(0xA); value: 90(0x05A) mg medication units */
    40                                                          /* HbA1c */
  },
  /* glucose measurement context: record 2 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */
//    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */ /* x test 4.8.11 */
    2,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_SNACK,             /* carbohydrate Id */
    0xD002,                                                     /* SFLOAT exponent = -3(0xD), 2 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_PREPRANDIAL,               /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_NO_ISSUE,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_INTERMEDIATE_ACTING_INSULIN,/* medication Id */
//    0xA032,                                                     /* SFLOAT exponent = -6(0xA); value: 50(0x032) mg medication units */ /* x test 4.8.11 */
    0xD050,                                                     /* SFLOAT exponent = -3(0xD); value: 80(0x050) ml medication units */   
    40                                                          /* HbA1c */
  },
  
  /* glucose measurement context: record 3 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */
    3,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_DRINK,             /* carbohydrate Id */
    0xD003,                                                     /* SFLOAT exponent = -3(0xD), 3 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_CASUAL,                    /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_LAB_TEST | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_UNDER_STRESS,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_LONG_ACTING_INSULIN, /* medication Id */
    0xD050,                                                     /* SFLOAT exponent = -3(0xD); value: 80(0x050) ml medication units */
    40                                                          /* HbA1c */
  },
  /* glucose measurement context: record 4 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */
    4,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_SUPPER,            /* carbohydrate Id */
    0xD004,                                                     /* SFLOAT exponent = -3(0xD), 4 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_BEDTIME,                   /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_HEALTH_CARE_PROFESSIONAL | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_UNDER_STRESS,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_PRE_MIXED_INSULIN,   /* medication Id */
    0xD046,                                                     /* SFLOAT exponent = -3(0xD); value: 70(0x046) ml medication units */
    40                                                          /* HbA1c */
  },

  /* glucose measurement context: record 5 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */ /* tests from 4.8.6 to 4.8.13 except 4.8.11*/
    //GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */ /* only x test 4.8.11 */
    5,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_DINNER,            /* carbohydrate Id */
    0xD005,                                                     /* SFLOAT exponent = -3(0xD), 5 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_PREPRANDIAL,               /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_MINOR_ISSUES,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_ID_RAPID_ACTING_INSULIN,/* medication Id */
    0xD03C,                                                    /* SFLOAT exponent = -3(0xD); value: 60(0x03C) ml medication units */ /* tests from 4.8.6 to 4.8.13 except 4.8.11*/
    //0xA031,                                                     /* SFLOAT exponent = -6(0xA); value: 49(0x031) mg medication units */ /* only x test 4.8.11 */
    40                                                          /* HbA1c */
  },
  
  /* glucose measurement context: record 6 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */ 
    //GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */ 
    6,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_DINNER,            /* carbohydrate Id */
    0xD005,                                                     /* SFLOAT exponent = -3(0xD), 5 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_PREPRANDIAL,               /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_MINOR_ISSUES,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_ID_RAPID_ACTING_INSULIN,/* medication Id */
    0xD03C,                                                    /* SFLOAT exponent = -3(0xD); value: 60(0x03C) ml medication units */ /* tests from 4.8.6 to 4.8.13 except 4.8.11*/
    //0xA031,                                                     /* SFLOAT exponent = -6(0xA); value: 49(0x031) mg medication units */ /* only x test 4.8.11 */
    41                                                          /* HbA1c */
  },
  
  /* glucose measurement context: record 7 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */ 
    //GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */ 
    7,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_DINNER,            /* carbohydrate Id */
    0xD005,                                                     /* SFLOAT exponent = -3(0xD), 5 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_PREPRANDIAL,               /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_MINOR_ISSUES,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_ID_RAPID_ACTING_INSULIN,/* medication Id */
    0xD03C,                                                    /* SFLOAT exponent = -3(0xD); value: 60(0x03C) ml medication units */ /* tests from 4.8.6 to 4.8.13 except 4.8.11*/
    //0xA031,                                                     /* SFLOAT exponent = -6(0xA); value: 49(0x031) mg medication units */ /* only x test 4.8.11 */
    42                                                          /* HbA1c */
  },
  
  /* glucose measurement context: record 8 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */ 
    //GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */ 
    8,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_DINNER,            /* carbohydrate Id */
    0xD005,                                                     /* SFLOAT exponent = -3(0xD), 5 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_PREPRANDIAL,               /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_MINOR_ISSUES,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_ID_RAPID_ACTING_INSULIN,/* medication Id */
    0xD03C,                                                    /* SFLOAT exponent = -3(0xD); value: 60(0x03C) ml medication units */ /* tests from 4.8.6 to 4.8.13 except 4.8.11*/
    //0xA031,                                                     /* SFLOAT exponent = -6(0xA); value: 49(0x031) mg medication units */ /* only x test 4.8.11 */
    42                                                          /* HbA1c */
  },
  
  /* glucose measurement context: record 9 */
  {
    GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT | GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS,/* flags & liter unit */ 
    //GLUCOSE_MEASUREMENT_CONTEXT_FLAG_ALL_PRESENT & ~GLUCOSE_MEASUREMENT_CONTEXT_FLAG_MEDICATION_LITER_UNITS, /* flags & kg(mg)unit */ 
    9,                                                          /* sequence number */
    0,                                                          /* extended flags */
    GLUCOSE_MEASUREMENT_CONTEXT_CARBOHYDRATE_DINNER,            /* carbohydrate Id */
    0xD005,                                                     /* SFLOAT exponent = -3(0xD), 5 mg carbohydrate units */
    GLUCOSE_MEASUREMENT_CONTEXT_MEAL_PREPRANDIAL,               /* meal */
    GLUCOSE_MEASUREMENT_CONTEXT_TESTER_SELF | GLUCOSE_MEASUREMENT_CONTEXT_HEALTH_MINOR_ISSUES,/* tester health */
    1500,                                                       /* exercise duration */
    50,                                                         /* exercise intensity */
    GLUCOSE_MEASUREMENT_CONTEXT_MEDICATION_ID_RAPID_ACTING_INSULIN,/* medication Id */
    0xD03C,                                                    /* SFLOAT exponent = -3(0xD); value: 60(0x03C) ml medication units */ /* tests from 4.8.6 to 4.8.13 except 4.8.11*/
    //0xA031,                                                     /* SFLOAT exponent = -6(0xA); value: 49(0x031) mg medication units */ /* only x test 4.8.11 */
    43                                                          /* HbA1c */
  }
};/* end glucoseDataBase_MeasurementContextRecords[] */

/******************************************************************************
* Function Declarations
******************************************************************************/

#endif /* _GLUCOSE_DATABASE_H_ */
