#ifndef _PROFILE_CONFIG_H_
#define _PROFILE_CONFIG_H_

#include "bluenrg1_stack.h"
#include "stack_user_cfg.h"


/* This file contains all the information needed to init the BlueNRG-1 stack. 
 * These constants and variables are used from the BlueNRG-1 stack to reserve RAM and FLASH 
 * according the application requests
 */

/* Default number of link */
#define MIN_NUM_LINK                1
/* Default number of GAP and GATT services */
#define DEFAULT_NUM_GATT_SERVICES   2
/* Default number of GAP and GATT attributes */
#define DEFAULT_NUM_GATT_ATTRIBUTES 9

/* Number of services and attibute for Heart Rate sensor */
#if (BLE_CURRENT_PROFILE_ROLES & HEART_RATE_SENSOR)
  /* Number of services requests from the Heart Rate sensor */
  #define NUM_APP_GATT_SERVICES   (2) /* 1 Heart Rate service + 1 DIS service */

  /* Number of attributes requests from the Heart Rate sensor */
  #define NUM_APP_GATT_ATTRIBUTES (16) /* 3 characteristics for Heart Rate service + 
                                          1 for DIS Service */
#endif

/* Number of services and attibute for Health Thermometer */
#if (BLE_CURRENT_PROFILE_ROLES & HEALTH_THERMOMETER)
  /* Number of services requests from the Health Thermometer */
  #define NUM_APP_GATT_SERVICES   (2) /* 1 Health Thermometer service + 1 DIS service */

  /* Number of attributes requests from the Health Thermometer sensor */
  #define NUM_APP_GATT_ATTRIBUTES (20) /* 4 characteristics for Health Thermometer service characteristics + 
                                          1 for DIS Service characteristics */
#endif

/* Number of services and attibute for Blood Pressure sensor */
#if (BLE_CURRENT_PROFILE_ROLES & BLOOD_PRESSURE_SENSOR)
  /* Number of services requests from the Health Thermometer sensor */
  #define NUM_APP_GATT_SERVICES   (2) /* 1 Health Thermometer service + 1 DIS service */

  /* Number of attributes requests from the Blood Pressure sensor */
  #define NUM_APP_GATT_ATTRIBUTES (24) /* 3 characteristics for Blood Pressure service characteristics + 
                                          3 for DIS Service characteristics */
#endif

/* Number of services and attibute for Glucose sensor */
#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_SENSOR)
  /* Number of services requests from the Glucose sensor */
  #define NUM_APP_GATT_SERVICES   (2) /* 1 Glucose service + 1 DIS service */

  /* Number of attributes requests from the Glucose sensor */
  #define NUM_APP_GATT_ATTRIBUTES (28) /* 4 characteristics for Glucose service characteristics + 
                                          3 for DIS Service characteristics */
#endif
                                            
 /* Number of services and attibute for CGM sensor */ //????
#if (BLE_CURRENT_PROFILE_ROLES & CGM_SENSOR)
  /* Number of services requests from the CGM sensor */
  #define NUM_APP_GATT_SERVICES   (2) /* 1 CGM service + 1 DIS service */

  /* Number of attributes requests from the CGM sensor */
  #define NUM_APP_GATT_ATTRIBUTES (28) /* 4 characteristics for CGM service characteristics + 
                                          3 for DIS Service characteristics */
#endif                                           

#if (BLE_CURRENT_PROFILE_ROLES & FIND_ME_TARGET) || (BLE_CURRENT_PROFILE_ROLES & FIND_ME_LOCATOR)
  /* Number of services requests from the Find Me Target device */
  #define NUM_APP_GATT_SERVICES   (1) /* 1 Immediate Alert Service */

  /* Number of attributes requests from the Find Me Target device */
  #define NUM_APP_GATT_ATTRIBUTES (4) /* 1 characteristic for Immediate Alert Service  */
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_REPORTER) || (BLE_CURRENT_PROFILE_ROLES & PROXIMITY_MONITOR)
  /* Number of services requests from the Proximity Profile */
  #define NUM_APP_GATT_SERVICES   (4) /* 1 Link Loss Service + 
                                         1 Immediate Alert Service +
                                         1 Tx Power Service +
                                         1 DIS service*/

  /* Number of attributes requests from the Proximity Profile */
  #define NUM_APP_GATT_ATTRIBUTES (20) /* 1 characteristic for Immediate Alert Service  
                                          1 characteristic for Link Loss Service
                                          1 characteristic for Tx Power Service */
#endif

#if (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_SERVER) || (BLE_CURRENT_PROFILE_ROLES & ALERT_NOTIFICATION_CLIENT)
  /* Number of services requests from the Alert Notification Profile */
  #define NUM_APP_GATT_SERVICES   (1) /* 1 Alert Notification Service */

  /* Number of attributes requests from the  Alert Notification Profile */
  #define NUM_APP_GATT_ATTRIBUTES (20) /* 5 characteristics for Alert Notification Service */
#endif

#if (BLE_CURRENT_PROFILE_ROLES & TIME_SERVER) || (BLE_CURRENT_PROFILE_ROLES & TIME_CLIENT)
  /* Number of services requests from the Time Profile */
  #define NUM_APP_GATT_SERVICES   (3) /* 1 Current Time Service + 
                                         1 Next DST Change Service +
                                         1 Reference Time Update Service */

  /* Number of attributes requests from the  Time Profile */
  #define NUM_APP_GATT_ATTRIBUTES (24) /* 3 characteristics for Current Time Service  
                                          1 characteristic for Next DST Change Service 
                                          2 characteristics for Reference Time Update Service */
#endif

#if (BLE_CURRENT_PROFILE_ROLES & PHONE_ALERT_CLIENT)
  /* Number of services requests from the Time Profile */
  #define NUM_APP_GATT_SERVICES   (1) /* 1 Phone Alert Status Service */

  /* Number of attributes requests from the  Time Profile */
  #define NUM_APP_GATT_ATTRIBUTES (12) /* 3 characteristics for Phone Alert Status Service */
#endif

/* Number of links needed for the demo: 1
 * Only 1 the default
 */
#define NUM_LINKS               (MIN_NUM_LINK)

/* Number of GATT attributes needed for the profile peripheral demo. */
#define NUM_GATT_ATTRIBUTES     (DEFAULT_NUM_GATT_ATTRIBUTES + NUM_APP_GATT_ATTRIBUTES)

/* Number of GATT services needed for the profile peripheral demo. */
#define NUM_GATT_SERVICES       (DEFAULT_NUM_GATT_SERVICES + NUM_APP_GATT_SERVICES)

/* Array size for the attribute value */
#define ATT_VALUE_ARRAY_SIZE    (300) //(1344)

/* Flash security database size */
#define FLASH_SEC_DB_SIZE       (0x400)

/* Flash server database size */
#define FLASH_SERVER_DB_SIZE    (0x400)

/* Set supported max value for ATT_MTU enabled by the application. Allowed values in range: [23:158] [New parameter added on BLE stack v2.x] */
#define MAX_ATT_MTU             (DEFAULT_ATT_MTU) 

/* Set supported max value for attribute size: it is the biggest attribute size enabled by the application */
#define MAX_ATT_SIZE            (30) //(1)  

/* Set the minumum number of prepare write requests needed for a long write procedure for a characteristic with len > 20bytes: 
 * 
 * It returns 0 for characteristics with len <= 20bytes
 * 
 * NOTE: If prepare write requests are used for a characteristic (reliable write on multiple characteristics), then 
 * this value should be set to the number of prepare write needed by the application.
 * 
 *  [New parameter added on BLE stack v2.x] 
*/
#define PREPARE_WRITE_LIST_SIZE PREP_WRITE_X_ATT(MAX_ATT_SIZE) 

/* Additional number of memory blocks  to be added to the minimum */
#define OPT_MBLOCKS		(6) /* 6:  for reaching the max throughput: ~220kbps (same as BLE stack 1.x) */

/* Set the number of memory block for packet allocation */
#define MBLOCKS_COUNT           (MBLOCKS_CALC(PREPARE_WRITE_LIST_SIZE, MAX_ATT_MTU, NUM_LINKS) + OPT_MBLOCKS)

/* RAM reserved to manage all the data stack according the number of links,
 * number of services, number of attributes and attribute value length
 */
NO_INIT(uint32_t dyn_alloc_a[TOTAL_BUFFER_SIZE(NUM_LINKS,NUM_GATT_ATTRIBUTES,NUM_GATT_SERVICES,ATT_VALUE_ARRAY_SIZE,MBLOCKS_COUNT,CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED)>>2]);

/* FLASH reserved to store all the security database information and
 * and the server database information
 */
NO_INIT_SECTION(uint32_t stacklib_flash_data[TOTAL_FLASH_BUFFER_SIZE(FLASH_SEC_DB_SIZE, FLASH_SERVER_DB_SIZE)>>2], ".noinit.stacklib_flash_data");

/* FLASH reserved to store: security root keys, static random address, public address */
NO_INIT_SECTION(uint8_t stacklib_stored_device_id_data[56], ".noinit.stacklib_stored_device_id_data");

/* Maximum duration of the connection event */
#define MAX_CONN_EVENT_LENGTH 0xFFFFFFFF

/* Sleep clock accuracy */
#if (LS_SOURCE == LS_SOURCE_INTERNAL_RO)

/* Sleep clock accuracy in Slave mode */
#define SLAVE_SLEEP_CLOCK_ACCURACY 500

/* Sleep clock accuracy in Master mode */
#define MASTER_SLEEP_CLOCK_ACCURACY MASTER_SCA_500ppm

#else

/* Sleep clock accuracy in Slave mode */
#define SLAVE_SLEEP_CLOCK_ACCURACY 100

/* Sleep clock accuracy in Master mode */
#define MASTER_SLEEP_CLOCK_ACCURACY MASTER_SCA_100ppm

#endif

/* Low Speed Oscillator source */
#if (LS_SOURCE == LS_SOURCE_INTERNAL_RO)
#define LOW_SPEED_SOURCE  1 // Internal RO
#else
#define LOW_SPEED_SOURCE  0 // External 32 KHz
#endif

/* High Speed start up time */
#define HS_STARTUP_TIME 328 // 800 us

/* Radio Config Hot Table */
extern uint8_t hot_table_radio_config[];

/* Low level hardware configuration data for the device */
#define CONFIG_TABLE            \
{                               \
  (uint32_t*)hot_table_radio_config,          \
  MAX_CONN_EVENT_LENGTH,        \
  SLAVE_SLEEP_CLOCK_ACCURACY,   \
  MASTER_SLEEP_CLOCK_ACCURACY,  \
  LOW_SPEED_SOURCE,             \
  HS_STARTUP_TIME               \
}

/* This structure contains memory and low level hardware configuration data for the device */
const BlueNRG_Stack_Initialization_t BlueNRG_Stack_Init_params = {
    (uint8_t*)stacklib_flash_data,
    FLASH_SEC_DB_SIZE,
    FLASH_SERVER_DB_SIZE,
    (uint8_t*)stacklib_stored_device_id_data,
    (uint8_t*)dyn_alloc_a,
    TOTAL_BUFFER_SIZE(NUM_LINKS,NUM_GATT_ATTRIBUTES,NUM_GATT_SERVICES,ATT_VALUE_ARRAY_SIZE,MBLOCKS_COUNT,CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED),
    NUM_GATT_ATTRIBUTES,
    NUM_GATT_SERVICES,
    ATT_VALUE_ARRAY_SIZE,
    NUM_LINKS,
    0, /* reserved for future use */
    PREPARE_WRITE_LIST_SIZE,
    MBLOCKS_COUNT,
    MAX_ATT_MTU,
    CONFIG_TABLE,
};

#endif // _PROFILE_CONFIG_H_
