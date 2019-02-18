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
*   FILENAME        -  cgm_database.h
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
*   Comments:    cgm database measurements records 
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the cgm database measurement records.
*  Database record data are coming from an internal database with
*  "not real but emulated" cgm sensor data.
*  User can adapt, modify it in order to add support for a real cgm sensor
*  with related implications (Base Time values coming from a internal real-time 
*  clock or equivalent, Time Offset updated through a specific user interface, ...
*  
* 
******************************************************************************/


/******************************************************************************
 * Include Files
******************************************************************************/

#ifndef _CGM_DATABASE_H_
#define _CGM_DATABASE_H_

#include <cgm_service.h>
#include <cgm_racp.h>
#include <cgm_sensor.h>

/******************************************************************************
* Macro Declarations
******************************************************************************/
/* Define the Cgm database record max size 
   Refer to the cgm_database.h for current available list of
   cgm measurement  (user can update/modify them) */

/* Enable PTS_TEST_CGM when using PTS tests */
#define CGM_PTS_NUMBER_OF_RECORDS  (10) //(3)  //(5)  //(20)

#ifdef PTS_TEST_CGM
#define CGM_NUMBER_OF_STORED_RECORDS     CGM_PTS_NUMBER_OF_RECORDS
#else
#define CGM_NUMBER_OF_STORED_RECORDS     CGM_DATABASE_MAX_RECORDS
//#define CGM_DATABASE_MAX_CONTEXT_RECORDS CGM_DATABASE_MAX_RECORDS
#endif
   
/******************************************************************************
* Type definitions
******************************************************************************/

/******************************************************************************
* Imported Variable
******************************************************************************/

/* Cgm measurement database: it's a simulated database with some default values */
tCgmMeasurementVal cgmDatabase_MeasurementRecords[CGM_DATABASE_MAX_RECORDS] =
{
  /* cgm measurement: record 1 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */      
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    1,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (mandatory if E2E_CRC supported) */
  },  
  
  /* cgm measurement: record 2 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    2,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 3 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    3,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
 
  /* cgm measurement: record 4 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    4,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
  
    /* cgm measurement: record 5 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    5,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

   /* cgm measurement: record 6 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    6,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },  
  
  /* cgm measurement: record 7 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    7,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 8 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    8,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
 
  /* cgm measurement: record 9 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    9,                                    /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
  
  /* cgm measurement: record 10 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    10,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  }, 
  

  /* cgm measurement: record 11 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    11,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
 
  /* cgm measurement: record 12 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    12,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 13 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    13,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 14 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    14,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 15 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    15,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
  
    /* cgm measurement: record 16 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    16,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
  
  /* cgm measurement: record 17 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    17,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 18 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    18,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },

  /* cgm measurement: record 19 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    19,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  },
  
    /* cgm measurement: record 20 */
  {
    CGM_RECORD_NOT_DELETED_FROM_RACP,     /* CGM_RECORD_FILTERED_AND_NOT_DELETED ? */
    15,                                   /* size */  
    CGM_MEASUREMENT_FLAGS_ALL_PRESENT,    /* flags */
    0x0050,                               /* SFLOAT exponent = 0;  80(0x50) mg/dl   (cgmGlucoseConcentration field: SFLOAT units of milligram per decilitre) */ 
    20,                                   /* timeOffset */  
    /* Status Annunciation: warning field, Status Annunciation: cal/temp field, Status Annunciation: status field */  
    {CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_3, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_2, CGM_SENSOR_STATUS_ANNUNCIATION_ALL_BYTE_1},
    0,                                    /* cgmTrendInformation; SFLOAT units (optional) */                      
    0,                                    /* cgmQuality; SFLOAT units (optional) */
    0,                                    /* E2E_CRC; (optional) */
  }
};/* end cgmDatabase_MeasurementRecords[] */

/******************************************************************************
* Function Declarations
******************************************************************************/

#endif /* _CGM_DATABASE_H_ */
