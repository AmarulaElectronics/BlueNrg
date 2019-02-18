/*******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2016 STMicroelectronics International
*
*   FILENAME        -  profile_Glucose_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      06/09/2016
*   $Revision$:  first version
*   $Author$:    AMS RF Application team
*   Comments:    Profiles PTS test application for Glucose  Collector. 
*                It uses the BlueNRG Host Profiles Library.
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
#include "string.h"
#include "osal.h"
#include "stdio.h"
#include "ble_const.h" 
#include "bluenrg1_api.h"
#include "BlueNRG1_stack.h" 
#include "host_config.h"
#include "Profile_config.h"
#include "debug.h"
#include "sleep.h"
#include "SDK_EVAL_Config.h"
#include <host_config.h>
#include "master_basic_profile.h"

#if (BLE_CURRENT_PROFILE_ROLES & GLUCOSE_COLLECTOR)
#include <glucose_service.h>
#include <glucose_collector_config.h>
#include <glucose_collector.h>
#include <glucose_collector_racp_CB.h>


/*******************************************************************************
 * Macros
*******************************************************************************/
/* PERIPHERAL peer address */
#define PERIPHERAL_PEER_ADDRESS PTS_PERIPHERAL_ADDRESS //PTS_PERIPHERAL_ADDRESS or PEER_ADDRESS

/* application states */
#define APPL_UNINITIALIZED		(0x00) //TBR
#define APPL_INIT_DONE			(0x01) //TBR

/* Glucose Collector test configuration for some PTS tests */

/* 4.2.3 Collector: Configure Sensor for use with RACP
1. Establish an ATT Bearer connection between the Tester and IUT (Glucose Collector).
2. The handles of the Glucose Measurement characteristic, Glucose Measurement Context characteristic and Record Access Control Point
   characteristic have been previously discovered by the Tester during the test procedures in section 4.3. 
3. The handles of the Client Characteristic Configuration descriptor of the Glucose Measurement characteristic, Glucose Measurement 
   Context characteristic and Record Access Control Point characteristic have been previously discovered by the Tester during
   the test procedure in section 4.3 or are known to the Tester by other means.
4. If the Tester and IUT were not previously bonded, perform a bonding procedure. 
   If previously bonded, enable encryption if not already enabled.
5. The Glucose Measurement characteristic and Glucose Measurement Context characteristic are configured for notifications.
6. The Record Access Control Point characteristic is configured for indications.
*/

/* 4.2.4 Collector: Scan to detect Glucose Sensor advertisements
1. The upper tester commands the Collector IUT to initiate a connection and the IUT starts scanning.
2. The Glucose Sensor (lower tester) advertises to the Collector IUT either using:
    - ALT 1: GAP Directed Connectable Mode (send ADV_DIRECT_IND packets)
   or
   - ALT 2: GAP Undirected Connectable Mode (send ADV_IND packets).
3. The lower tester waits for responses from the Collector IUT.
4. The Collector IUT sends a CONNECT_REQ and an optionally empty PDU to the lower tester.
*/

/* --------------------- User Inputs for Glucose Collector PTS tests -----------------*/

///@cond BLE_CURRENT_PROFILE_ROLES & GLUCOSE_COLLECTOR
/**
 *@defgroup Glucose_Collector
 *@brief PTS test application user input commands Glucose Collector Central role
 *@{
 */

//General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 *  Glucose Collector user input command: Start Discovery procedure for detecting an Glucose Sensor(<b>GL_Collector_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVER_GLUCOSE_SENSOR                            'a' 

/** 
 *  Glucose Collector user input command: Start Connection procedure for connecting to the discovered Glucose Sensor(<b>GL_Collector_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_GLUCOSE_SENSOR                             'b' // Start Connection procedure for connecting to the discovered glucose sensor

/** 
 *  Glucose Collector user input command: Terminate the current connection with the Glucose Sensor(<b>GL_Collector_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_GLUCOSE_SENSOR                          'c' // Terminate the current connection with the glucose sensor

/** 
 *   Glucose Collector user input command: Start the pairing procedure (or enable the encryption) with the connected Glucose Sensor (<b>GL_Collector_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_GLUCOSE_SENSOR                           'd' // Start the pairing  procedure (or enable the encryption) with the connected glucose sensor

/** 
 *  Glucosey Collector user input command: Clear the security database (it removes any bonded device) (<b>GL_Collector_Clear_Security_Database() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE                            'e' // Clear the security database (it removes any bonded device) 

/* ---------- 4.3: Discover Services and Characteristics tests */

/** 
 *  Glucose Collector user input command: Discover All Primary Services (<b>GL_Collector_ServicesDiscovery() </b> API)
 */
#define APPL_DISCOVER_GLUCOSE_SERVICES                          'f' // TP/GLD/CO/BV-01-I [Discover Glucose Service]
                                                                    // TP/GLD/CO/BV-02-I [Discover Device Information Service]

/** 
 *  Glucose Collector user input command: Discover All Characteristics of Glucose Service -->  Glucose Measurement, Measurement Context,RACP, Feature Characteristics (<b>GL_Collector_DiscCharacServ(GLUCOSE_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVER_GLUCOSE_SERVICE_CHARACTERISTICS           'h' // TP/GLD/CO/BV-04-I [Discover Glucose Measurement Characteristic], 
                                                                    // TP/GLD/CO/BV-06-I [Discover Glucose Measurement Context Characteristic],
                                                                    // TP/GLD/CO/BV-08-I [Discover Record Access Control Point Characteristic],
                                                                    // TP/GLD/CO/BV-10-I [Discover Glucose Feature Characteristic]
/** 
 *  Glucose Collector user input command: Discover Glucose Measurement Client Characteristic Configuration Descriptor (<b>GL_Collector_Start_Glucose_Measurement_Characteristic_Descriptor_Discovery() </b> API)
 */
#define APPL_DISCOVER_GL_MEASUREMENT_CHAR_DESCRIPTOR            'k' // TP/GLD/CO/BV-05-I [Discover Glucose Measurement Client Characteristic Configuration Descriptor]

/** 
 *  Glucose Collector user input command: Discover Glucose Measurement Context Client Characteristic Configuration Descriptor (<b>GL_Collector_Start_Glucose_Measurement_Context_Characteristic_Descriptor_Discovery() </b> API)
 */
#define APPL_DISCOVER_GL_MEASUREMENT_CONTEXT_CHAR_DESCRIPTOR    'i' // TP/GLD/CO/BV-07-I [Discover Glucose Measurement Context Client Characteristic Configuration Descriptor] 

/** 
 *  Glucose Collector user input command: Discover Record Access Control Point Client Characteristic Configuration Descriptor (<b>GL_Collector_Start_RACP_Characteristic_Descriptor_Discovery() </b> API)
 */
#define APPL_DISCOVER_GL_RACP_CHAR_DESCRIPTOR                   'j' // TP/GLD/CO/BV-09-I [Discover Record Access Control Point Client Characteristic Configuration Descriptor] 

/** 
 *  Glucose Collector user input command: Discover Characteristics of Discover Device Information Service (<b>GL_Collector_DiscCharacServ(DEVICE_INFORMATION_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVER_DIS_CHARACTERISTICS                       'l' // TP/GLD/CO/BV-11-I [Discover Device Information Service Characteristics]

/** 
 *  Glucose Collector user input command: Read DIS manufacturer name (<b>GL_Collector_ReadDISManufacturerNameChar() </b> API)
 */
#define APPL_READ_DIS_MANUFACTURER_NAME_CHAR                    'm' // TP/GLD/CO/BV-12-I [Read Device Information Service Characteristics] 

/** 
 *  Glucose Collector user input command: Read DIS model number (<b>GL_Collector_ReadDISModelNumberChar() </b> API)
 */
#define APPL_READ_DIS_MODEL_NUMBER_CHAR                         'n' // TP/GLD/CO/BV-12-I [Read Device Information Service Characteristics] 

/** 
 *  Glucose Collector user input command: Read DIS system ID (<b>GL_Collector_ReadDISSystemIDChar() </b> API)
 */
#define APPL_READ_DIS_SYSTEM_ID_CHAR                            'o' // TP/GLD/CO/BV-12-I [Read Device Information Service Characteristics] 

/* ---------- 4.4: Glucose Features tests */

/* ---------- 4.4: Receive Glucose Measurement Notifications (just perform preamble 4.2.4) */
// TP/GLF/CO/BV-10-I [Receive Glucose Measurement Notifications] 
// TP/GLF/CO/BI-01-I [Receive Glucose Measurement Notifications with reserved Flags bits] 
// TP/GLF/CO/BI-02-I [Receive Glucose Measurement Notifications with reserved Type-Sample Location] 
// TP/GLF/CO/BI-03-I [Receive Glucose Measurement Notifications with reserved sensor status annunciation bits]
// TP/GLF/CO/BI-04-I [Receive Glucose Measurement Notifications with additional octets not comprehended]

/** 
 *  Glucose Collector user input command: Configure Glucose Measurement Characteristic for Notification (<b>GL_Collector_Enable_Glucose_Measurement_Char_Notification() </b> API)
 */
#define APPL_ENABLE_GL_MEASUREMENT_CHAR_NOTIFICATION            'p' // TP/GLF/CO/BV-09-I [Configure Glucose Measurement Characteristic for Notification] 

/** 
 *  Glucose Collector user input command: Configure Glucose Measurement Context Characteristic for Notification (<b>GL_Collector_Enable_Glucose_Measurement_Context_Char_Notification() </b> API)
 */
#define APPL_ENABLE_GL_MEASUREMENT_CONTEXT_CHAR_NOTIFICATION    'q'// TP/GLF/CO/BV-11-I [Configure Glucose Measurement Context Characteristic for Notification]

/** 
 *  Glucose Collector user input command: Configure Glucose RACP  Characteristic for Notification (<b>GL_Collector_Enable_Glucose_RACP_Char_Indication() </b> API)
 */
#define APPL_ENABLE_GL_RACP_CHAR_INDICATION                     'r'//     [Configure Glucose RACP Characteristic for Indication]

/* ---------- 4.4:  Receive Glucose Measurement Context  Notifications (just perform preamble 4.2.4) */
// TP/GLF/CO/BV-12-I [Receive Glucose Measurement Context Notifications]
// TP/GLF/CO/BI-05-I [Receive Glucose Measurement Context Notifications with reserved flags]
// TP/GLF/CO/BI-06-I [Receive Glucose Measurement Context Notifications with reserved Extended Flags bits] 
// TP/GLF/CO/BI-07-I [Receive Glucose Measurement Context Notifications with reserved Carbohydrate ID value]
// TP/GLF/CO/BI-08-I [Receive Glucose Measurement Context Notifications with reserved Meal value] 
// TP/GLF/CO/BI-09-I [Receive Glucose Measurement Context Notifications with reserved Medication ID value]
// TP/GLF/CO/BI-10-I [Receive Glucose Measurement Context Notifications with additional octets not comprehended]

/** 
 *  Glucose Collector user input command: Read Glucose Feature characteristic (<b>GL_Collector_ReadFeatureChar() </b> API)
 */
#define APPL_READ_GL_FEATURE_CHAR                               's' // TP/GLF/CO/BV-13-I [Read Glucose Feature characteristic]
                                                                    // TP/GLF/CO/BI-11-I [Read Glucose Feature characteristic with reserved value]: refer to APPL_READ_GL_FEATURE_CHAR  
// TP/GLF/CO/BV-14-I [Verify Bond Status on Reconnection]: TBR

/* ---------- 4.5: Service Procedures: Report Stored Records tests */

/** 
 *  Glucose Collector user input command: Report Stored Records - All records (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE,RACP_OPERATOR_ALL,RACP_FILTER_TYPE_NONE,NULL,NULL) </b> API)
 */
#define APPL_RACP_REPORT_ALL                                    't' // GLP/COL/SPR/BV-01-I[Report Stored Records - All records]

/** 
 *  Glucose Collector user input command: Report Stored Records - Less than or equal to Sequence Number (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_LESS_EQUAL, RACP_FILTER_TYPE_SEQUENCE_NUMBER, &racp_filter_parameter_1, NULL) </b> API)
 */
#define APPL_RACP_REPORT_LESS_THAN_SEQ_NUM                      'v' // TP/SPR/CO/BV-02-I [Report Stored Records - Less than or equal to Sequence Number]

/** 
 *  Glucose Collector user input command: Report Stored Records - Greater than or equal to Sequence Number 1 (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_GREATER_EQUAL, RACP_FILTER_TYPE_SEQUENCE_NUMBER, &racp_filter_parameter_1, NULL) </b> API)
 */
#define APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM                   'w' // TP/SPR/CO/BV-03-I [Report Stored Records - Greater than or equal to Sequence Number] 

/** 
 *  Glucose Collector user input command: Report Stored Records - Greater than or equal to Sequence Number 2 (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_GREATER_EQUAL, RACP_FILTER_TYPE_SEQUENCE_NUMBER, &racp_filter_parameter_1, NULL) </b> API)
 */
#define APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2                  'W' // TP/SPR/CO/BV-03-I [Report Stored Records - Greater than or equal to Sequence Number] 

/** 
 *  Glucose Collector user input command: Report Report Stored Records - Greater than or equal to User Facing Time (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_GREATER_EQUAL, RACP_FILTER_TYPE_USER_FACING_TIME, &racp_filter_parameter_1, NULL)</b> API)
 */
#define APPL_RACP_REPORT_GREATER_THAN_UFT                       'z' // TP/SPR/CO/BV-04-I [Report Stored Records - Greater than or equal to User Facing Time] 

/** 
 *  Glucose Collector user input command: Report Stored Records - Within range of Sequence Number value pair (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_WITHIN_RANGE, RACP_FILTER_TYPE_SEQUENCE_NUMBER, &racp_filter_parameter_1, &racp_filter_parameter_2) </b> API)
 */
#define APPL_RACP_REPORT_WITHIN_SEQ_NUMS                        'A' // TP/SPR/CO/BV-05-I [Report Stored Records - Within range of Sequence Number value pair]

/** 
 *  Glucose Collector user input command: Report Stored Records - Last record (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_FIRST_RECORD, RACP_FILTER_TYPE_NONE, NULL, NULL) </b> API)
 */
#define APPL_RACP_REPORT_FIRST                                  'B' // TP/SPR/CO/BV-06-I [Report Stored Records - First record]

/** 
 *  Glucose Collector user input command: Report Stored Records - Last record (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, RACP_OPERATOR_LAST_RECORD, RACP_FILTER_TYPE_NONE, NULL, NULL) </b> API)
 */
#define APPL_RACP_REPORT_LAST                                   'C' // TP/SPR/CO/BV-07-I [Report Stored Records - Last record]

// TP/SPR/CO/BV-08-I [Report Stored Records - All records – Record Added]: refer to APPL_RACP_REPORT_ALL  
// TP/SPR/CO/BV-09-I [Report Stored Records - All records – Record Deleted]: refer to APPL_RACP_REPORT_ALL  

/* ---------- 4.6: Service Procedures: Delete Stored Records tests */

/** 
 *  Glucose Collector user input command: Delete Stored Records - All records (<b>GL_Collector_Send_RACP(RACP_DELETE_STORED_RECORDS_OP_CODE, RACP_OPERATOR_ALL, RACP_FILTER_TYPE_NONE, NULL, NULL) </b> API)
 */
#define APPL_RACP_DELETE_ALL                                    'D' // TP/SPD/CO/BV-01-I [Delete Stored Records - All records]

/** 
 *  Glucose Collector user input command: Delete Stored Records - Within range of Sequence Number value pair (<b>GL_Collector_Send_RACP(RACP_DELETE_STORED_RECORDS_OP_CODE, RACP_OPERATOR_WITHIN_RANGE, RACP_FILTER_TYPE_SEQUENCE_NUMBER, &racp_filter_parameter_1 &racp_filter_parameter_2) </b> API)
 */
#define APPL_RACP_DELETE_WITHIN_SEQ_NUMS                        'E' // TP/SPD/CO/BV-02-I [Delete Stored Records - Within range of Sequence Number value pair] 

/* ---------- 4.7: Service Procedures tests: Abort Operation */

/** 
 *  Glucose Collector user input command: Abort Operation - Report Stored Records (<b>GL_Collector_Send_RACP(RACP_ABORT_OPERATION_OP_CODE, RACP_OPERATOR_NULL, RACP_FILTER_TYPE_NONE, NULL, NULL) </b> API)
 */
#define APPL_RACP_ABORT_RACP_REPORT                             'F'// TP/SPA/CO/BV-01-I [Abort Operation - Report Stored Records]

/* ---------- 4.8: Service Procedures: Report Number of Stored Records tests */

/** 
 *  Glucose Collector user input command: Report Number of Stored Records - All records (<b>GL_Collector_Send_RACP(RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE,RACP_OPERATOR_ALL,RACP_FILTER_TYPE_NONE,NULL,NULL) </b> API)
 */
#define APPL_RACP_REPORT_NUMBER_ALL                             'G' // TP/SPN/CO/BV-01-I [Report Number of Stored Records - All records]

/** 
 *  Glucose Collector user input command: Report Number of Stored Records - Greater than or equal to Sequence Number (<b>GL_Collector_Send_RACP(RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE, RACP_OPERATOR_GREATER_EQUAL, RACP_FILTER_TYPE_SEQUENCE_NUMBER, &racp_filter_parameter_1, NULL) </b> API)
 */
#define APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM            'H' // TP/SPN/CO/BV-02-I [Report Number of Stored Records - Greater than or equal to Sequence Number]

/* ---------- 4.9: Service Procedures: General Error Handling tests */

/** 
 *  Glucose Collector user input command: Unsupported Op Code (<b>GL_Collector_Send_RACP(UNSUPPORTED_RACP_CODE,RACP_OPERATOR_NULL,RACP_FILTER_TYPE_NONE,NULL,NULL) </b> API)
 */
#define APPL_RACP_WRONG_OP_CODE                                 'K' // TP/SPE/CO/BI-01-I [Unsupported Op Code]

/** 
 *  Glucose Collector user input command: Unsupported Operator (<b>GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, UNSUPPORTED_RACP_CODE,RACP_FILTER_TYPE_NONE,NULL,NULL) </b> API)
 */
#define APPL_RACP_WRONG_OPERATOR                                'I' // TP/SPE/CO/BI-02-I [Unsupported Operator]

/** 
 *  Glucose Collector user input command: Unsupported Operand (<b>GL_Collector_Send_RACP(RACP_DELETE_STORED_RECORDS_OP_CODE,RACP_OPERATOR_GREATER_EQUAL, UNSUPPORTED_RACP_CODE,NULL,NULL) </b> API)
 */
#define APPL_RACP_WRONG_OPERAND                                 'J' // TP/SPE/CO/BI-03-I [Unsupported Operand] 
//#define APPL_RACP_PROCEDURE_TIMEOUT                               // TP/SPE/CO/BI-04-I [RACP Procedure Timeout]

/** 
 *  Glucose Collector user input command: utility for checking if there are some notifications related to a single RACP procedure to be anaylzed  (utility). (<b>GL_Collector_Util_Perform_RACP_Post_processing() </b> API)
 */
#define APPL_RACP_POST_PROCESSING                                'P' // Analyse notifications data coming from a single RACP procedure

/** 
 *  Glucose Collector user input command: Starts all the connection, service discovery, characteristic discovery, ..... procedures.
 */
#define APPL_START_FULL_CONFIGURATION                            'Z' 

/** 
 *  Glucose Collector user input command: Display PTS user input commands list.
 */
#define DISPLAY_PTS_MENU                                         '?' 

/**
 * @}
 */
///@endcond

/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/* --------------- Glucose Collector define values required for PTS test cases ------------------ */
/* Sequence numbers used for filtering */ 
#define PTS_SEQUENCE_NUMBER_FILTER_VALUE_1 0x0004 /* 4.5.2, 4,5,3,  */ 
#define PTS_SEQUENCE_NUMBER_FILTER_VALUE_2 0x0006 /* 4.5.3 */ 

/* User Facing time used for filtering */ /* NOTE Which ones?.  to be customized for test TP/SPR/CO/BV-04-I */
#define PTS_UFT_FILTER_VALUE_YEAR     2018  
#define PTS_UFT_FILTER_VALUE_MONTH    12  
#define PTS_UFT_FILTER_VALUE_DAY      1  
#define PTS_UFT_FILTER_VALUE_HOURS    17 
#define PTS_UFT_FILTER_VALUE_MINUTES  55
#define PTS_UFT_FILTER_VALUE_SECONDS  2

/* Unsupported code for RACP */
#define UNSUPPORTED_RACP_CODE 0xFF

/******************************************************************************
 * Local Variable Declarations
******************************************************************************/
uint8_t deviceState = APPL_UNINITIALIZED;

SleepModes sleep_mode = SLEEPMODE_WAKETIMER;

/******************************************************************************
 * Global Variable Declarations
******************************************************************************/

void *profiledbgfile;

uint8_t status;

/* Glucose Collector input filter values for PTS (sequence numbers or user facing times) */
static tfilterTypeParameter racp_filter_parameter_1;
static tfilterTypeParameter racp_filter_parameter_2;

/******************************************************************************
 * Function Declarations
******************************************************************************/
uint8_t __io_getcharNonBlocking(uint8_t *data);
/******************************************************************************
 * Function Definitions 
******************************************************************************/
/******************************************************************************
 * Function Definitions 
******************************************************************************/

/* Enable the Sleep mode: SLEEPMODE_NOTIMER or SLEEPMODE_WAKETIMER with wake up on push1 button */
void Enter_Sleep_Mode(void)
{
  uint8_t ret, wakeup_source, wakeup_level;   
  
  /* BlueNRG-1 STEVAL-IDB007V1 PUSH1 Button: WAKEUP_IO13 */
  wakeup_source = WAKEUP_IO13; 
  wakeup_level = (WAKEUP_IOx_LOW << WAKEUP_IO13_SHIFT_MASK); 
  
  //printf("\r\nEnter to Sleep(mode: %d)\r\n", sleepMode);
  ret = BlueNRG_Sleep(sleep_mode, wakeup_source, wakeup_level);
  if (ret != BLE_STATUS_SUCCESS) 
  {
    printf("BlueNRG_Sleep() error 0x%02x\r\n", ret);
    while(1);
  }
}/* end Enter_Sleep_Mode*/

void printDebugServices(uint8_t numReporterServices, uint8_t *reporterServices)
{
  uint8_t i , j;
  uint16_t handle, uuid;

  APPL_MESG_INFO(profiledbgfile,"****    Number of Primary Service: %d\r\n", numReporterServices);
  j = 0;
  for (i=0; i<numReporterServices; i++) {
    if (reporterServices[j] == UUID_TYPE_16) {
      Osal_MemCpy(&uuid, &reporterServices[j+5], 2);
      switch (uuid) {
      case GLUCOSE_SERVICE_UUID:
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c1)", PTS_CODE, GLP_PTS_CODE);
#endif         
        APPL_MESG_INFO(profiledbgfile,"****    Glucose Service UUID found\r\n");
	break;
      case DEVICE_INFORMATION_SERVICE_UUID:
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c2)", PTS_CODE, GLP_PTS_CODE);
#endif 
        APPL_MESG_INFO(profiledbgfile,"****    Device Information Service UUID found\r\n");
	break;
      default:
	APPL_MESG_INFO(profiledbgfile,"****    UUID 0x%04x\r\n", uuid);
        break;
      }
    }
    j++;
    Osal_MemCpy(&handle, &reporterServices[j], 2);
    APPL_MESG_INFO(profiledbgfile,"****       Start Handle = 0x%04x\r\n", handle);
    j += 2;
    Osal_MemCpy(&handle, &reporterServices[j], 2);
    APPL_MESG_INFO(profiledbgfile,"****       End Handle = 0x%04x\r\n", handle);
    j += 2;
    if (reporterServices[j-5] == UUID_TYPE_16) {
      j += 2;
    } else {
      j += 16;
    }
  }
}

void printDebugCharac(uint8_t numCharac, uint8_t *charac)
{
  uint8_t i, index, sizeElement, properties;
  uint16_t value_handle, charac_handle, uuid_charac;

  APPL_MESG_INFO(profiledbgfile,"****    Number of Charac of a Service: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 8;
    Osal_MemCpy(&uuid_charac, &charac[index+6], 2);
    } else {
      sizeElement = 22;
    }

    switch(uuid_charac) {
    case GLUCOSE_MEASUREMENT_CHAR_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c3)", PTS_CODE, GLP_PTS_CODE);
#endif      
      APPL_MESG_INFO(profiledbgfile,"****      Glucose Measurement Characteristic found\r\n");
      break;
    case GLUCOSE_MEASUREMENT_CONTEXT_CHAR_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c4)", PTS_CODE, GLP_PTS_CODE);
#endif 
      APPL_MESG_INFO(profiledbgfile,"****      Glucose Measurement Context Characteristic found\r\n");
      break;
    case GLUCOSE_FEATURE_CHAR_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c5)", PTS_CODE, GLP_PTS_CODE);
#endif 
      APPL_MESG_INFO(profiledbgfile,"****      Glucose Feature Characteristic found\r\n");
      break;
      
    case GLUCOSE_RACP_CHAR_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c6)", PTS_CODE, GLP_PTS_CODE);
#endif 
      APPL_MESG_INFO(profiledbgfile,"****      Glucose RACP Characteristic found\r\n");
      break;  
    
    case MANUFACTURER_NAME_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c10)", PTS_CODE, GLP_PTS_CODE);
#endif       
      APPL_MESG_INFO(profiledbgfile,"****      Device Information Manufacturer Name Characteristic found\r\n");
      break;
   
    case MODEL_NUMBER_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c9)", PTS_CODE, GLP_PTS_CODE);
#endif 
      APPL_MESG_INFO(profiledbgfile,"****      Device Information Model Number Characteristic found\r\n");
      break;
        
    case SYSTEM_ID_UUID:
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c8)", PTS_CODE, GLP_PTS_CODE);
#endif     
      APPL_MESG_INFO(profiledbgfile,"****      Device Information System ID Characteristic found\r\n");
      break;
        
    default:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic found\r\n");
      break;
    }

    Osal_MemCpy(&charac_handle, &charac[index+1], 2);
    properties = charac[index+3];
    Osal_MemCpy(&value_handle, &charac[index+4], 2);
    APPL_MESG_INFO(profiledbgfile,"****      Characteristic Handle = 0x%04x\r\n", charac_handle);
    APPL_MESG_INFO(profiledbgfile,"****      Properties = 0x%02x\r\n", properties);
    APPL_MESG_INFO(profiledbgfile,"****      ValueHandle = 0x%04x\r\n", value_handle);
    APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_charac);

    index += sizeElement;
  }
}

void printDebugCharacDesc(uint8_t numCharac, uint8_t *charac)
{
  uint8_t i, index, sizeElement;
  uint16_t handle_characDesc, uuid_characDesc;

  APPL_MESG_INFO(profiledbgfile,"****    Number of Charac Descriptor: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 5;
      Osal_MemCpy(&uuid_characDesc, &charac[index+3], 2);
    } else {
      sizeElement = 19;
    }

    Osal_MemCpy(&handle_characDesc, &charac[index+1], 2);

    if (CHAR_PRESENTATION_FORMAT_DESCRIPTOR_UUID) {
    }
    switch(uuid_characDesc) {
    case CHAR_PRESENTATION_FORMAT_DESCRIPTOR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Presentation Format Descriptor found\r\n");
      break;
    case CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID:
      APPL_MESG_INFO(profiledbgfile,"****      Client Characteristic Configuration Descriptor found\r\n");
      break;
    default:
      APPL_MESG_INFO(profiledbgfile,"****      Characteristic Descriptor found\r\n");
      break;
    }
    
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c7)", PTS_CODE, GLP_PTS_CODE);
#endif 
    APPL_MESG_INFO(profiledbgfile,"****      Characteristic Descriptor Handle = 0x%04x\r\n", handle_characDesc);
    APPL_MESG_INFO(profiledbgfile,"****      UUID = 0x%04x\r\n", uuid_characDesc);

    index += sizeElement;
  }
}

void Display_Appl_Menu()
{
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GLUCOSE_SENSOR: %c\n",APPL_DISCOVER_GLUCOSE_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_GLUCOSE_SENSOR: %c\n",APPL_CONNECT_GLUCOSE_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_GLUCOSE_SENSOR: %c\n",APPL_DISCONNECT_GLUCOSE_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_GLUCOSE_SENSOR: %c\n",APPL_PAIR_WITH_GLUCOSE_SENSOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: %c\n",APPL_CLEAR_SECURITY_DATABASE);
 
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GLUCOSE_SERVICE_CHARACTERISTICS: %c\n",APPL_DISCOVER_GLUCOSE_SERVICE_CHARACTERISTICS);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GL_MEASUREMENT_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_GL_MEASUREMENT_CHAR_DESCRIPTOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GL_MEASUREMENT_CONTEXT_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_GL_MEASUREMENT_CONTEXT_CHAR_DESCRIPTOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GL_RACP_CHAR_DESCRIPTOR: %c\n",APPL_DISCOVER_GL_RACP_CHAR_DESCRIPTOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_DIS_CHARACTERISTICS: %c\n",APPL_DISCOVER_DIS_CHARACTERISTICS);
  APPL_MESG_DBG(profiledbgfile,"APPL_READ_DIS_MANUFACTURER_NAME_CHAR: %c\n",APPL_READ_DIS_MANUFACTURER_NAME_CHAR);
  APPL_MESG_DBG(profiledbgfile,"APPL_READ_DIS_MODEL_NUMBER_CHAR: %c\n",APPL_READ_DIS_MODEL_NUMBER_CHAR);
  APPL_MESG_DBG(profiledbgfile,"APPL_READ_DIS_SYSTEM_ID_CHAR: %c\n",APPL_READ_DIS_SYSTEM_ID_CHAR);
  APPL_MESG_DBG(profiledbgfile,"APPL_ENABLE_GL_MEASUREMENT_CHAR_NOTIFICATION: %c\n",APPL_ENABLE_GL_MEASUREMENT_CHAR_NOTIFICATION);
  APPL_MESG_DBG(profiledbgfile,"APPL_ENABLE_GL_MEASUREMENT_CONTEXT_CHAR_NOTIFICATION: %c\n",APPL_ENABLE_GL_MEASUREMENT_CONTEXT_CHAR_NOTIFICATION);
  APPL_MESG_DBG(profiledbgfile,"APPL_ENABLE_GL_RACP_CHAR_INDICATION: %c\n",APPL_ENABLE_GL_RACP_CHAR_INDICATION);
  APPL_MESG_DBG(profiledbgfile,"APPL_READ_GL_FEATURE_CHAR: %c\n",APPL_READ_GL_FEATURE_CHAR);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_ALL: %c\n",APPL_RACP_REPORT_ALL);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_LESS_THAN_SEQ_NUM: %c\n",APPL_RACP_REPORT_LESS_THAN_SEQ_NUM);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM: %c\n",APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2: %c\n",APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_GREATER_THAN_UFT: %c\n",APPL_RACP_REPORT_GREATER_THAN_UFT);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_WITHIN_SEQ_NUMS: %c\n",APPL_RACP_REPORT_WITHIN_SEQ_NUMS);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_FIRST: %c\n",APPL_RACP_REPORT_FIRST);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_LAST: %c\n",APPL_RACP_REPORT_LAST);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_DELETE_ALL: %c\n",APPL_RACP_DELETE_ALL);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_DELETE_WITHIN_SEQ_NUMS: %c\n",APPL_RACP_DELETE_WITHIN_SEQ_NUMS);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_ABORT_RACP_REPORT: %c\n",APPL_RACP_ABORT_RACP_REPORT);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_NUMBER_ALL: %c\n",APPL_RACP_REPORT_NUMBER_ALL);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM: %c\n",APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_WRONG_OP_CODE: %c\n",APPL_RACP_WRONG_OP_CODE);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_WRONG_OPERATOR: %c\n",APPL_RACP_WRONG_OPERATOR);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_WRONG_OPERAND: %c\n",APPL_RACP_WRONG_OPERAND);
  APPL_MESG_DBG(profiledbgfile,"APPL_RACP_POST_PROCESSING: %c\n",APPL_RACP_POST_PROCESSING);    
  
  APPL_MESG_DBG(profiledbgfile,"APPL_START_FULL_CONFIGURATION: %c\n",APPL_START_FULL_CONFIGURATION);
  APPL_MESG_DBG(profiledbgfile,"DISPLAY_PTS_MENU: %c\n",DISPLAY_PTS_MENU);          
  
    
}/* end Display_Appl_Menu() */
 
void Device_Init(void)
{
  glcInitDevType InitDev; 
  uint8_t public_addr[6]=GLC_PUBLIC_ADDRESS;
  uint8_t device_name[]=GLC_DEVICE_NAME;
  tBleStatus ble_status;

  InitDev.public_addr =  public_addr;
  InitDev.txPower =      GLC_TX_POWER_LEVEL;
  InitDev.device_name_len = sizeof(device_name);
  InitDev.device_name = device_name;
  /* Init Glucose Collector device with RACP post processing for PTS tests
     analysis */
  InitDev.racp_post_processing = TRUE; //FALSE
  InitDev.max_num_of_single_racp_notifications = RACP_MAX_EXPECTED_NOTIFICATIONS_NUMBER;
  
  ble_status = GL_Collector_Init(InitDev);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_Init() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_Init() Error: %02X\n", ble_status);
  }
}

void Device_SetSecurity(void)
{
  glcSecurityType param;
  tBleStatus ble_status;
  
  param.ioCapability =  GLC_IO_CAPABILITY;
  param.mitm_mode =     GLC_MITM_MODE;
  param.bonding_mode =  GLC_BONDING_MODE;
  param.use_fixed_pin = GLC_USE_FIXED_PIN;
  param.fixed_pin =     GLC_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
    
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = GLC_SC_SUPPORT;
  param.keypress_support = GLC_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = GLC_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    GLC_OOB_ENABLE;
#endif
    
  ble_status = GL_Collector_SecuritySet(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_SecuritySet() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_SecuritySet() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_Procedure(void)
{
  glcDevDiscType param;
  tBleStatus ble_status;
  
  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  GLC_LIM_DISC_SCAN_INT;
  param.scanWindow =    GLC_LIM_DISC_SCAN_WIND;
  ble_status = GL_Collector_DeviceDiscovery(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_DeviceDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_DeviceDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Connection_Procedure(void)
{
  glcConnDevType param;
  tBleStatus ble_status;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS;

  param.fastScanDuration =         GLC_FAST_SCAN_DURATION;
  param.fastScanInterval =         GLC_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           GLC_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = GLC_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   GLC_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        GLC_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        GLC_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             GLC_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      GLC_SUPERVISION_TIMEOUT;
  param.min_conn_length =          GLC_MIN_CONN_LENGTH;
  param.max_conn_length =          GLC_MAX_CONN_LENGTH;
  ble_status = GL_Collector_DeviceConnection(param);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_DeviceConnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_DeviceConnection() Error: %02X\n", ble_status);
  }
}
 
void Device_StartPairing(void)
{
  tBleStatus ble_status;
  ble_status = GL_Collector_StartPairing();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_StartPairing() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_StartPairing() Error: %02X\n", ble_status);
  }
}

void Device_ServicesDiscovery(void)
{
  tBleStatus ble_status;
  ble_status = GL_Collector_ServicesDiscovery();

  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_ServicesDiscovery() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_ServicesDiscovery() Error: %02X\n", ble_status);
  }
}

void Device_Disconnection(void)
{
  tBleStatus ble_status;
  ble_status = GL_Collector_DeviceDisconnection();
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_DeviceDisconnection() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"GL_Collector_DeviceDisconnection() Error: %02X\n", ble_status);
  }
}

void Device_Discovery_CharacServ(uint16_t uuid_service)
{
  tBleStatus ble_status;
  ble_status = GL_Collector_DiscCharacServ(uuid_service);
  if (ble_status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_DBG(profiledbgfile,"Device_Discovery_CharacServ() Call: OK\n" );
  }
  else
  {
    APPL_MESG_DBG(profiledbgfile,"Device_Discovery_CharacServ() Error: %02X, uuid_service: 0x%02X\n", ble_status,uuid_service);
  }
}

void Device_StartFullConfig(void)
{
  glcConnDevType connParam;
  //glcConfDevType confParam;
  uint8_t peer_addr[6] = PERIPHERAL_PEER_ADDRESS; 

  /* Connection Parameter */
  connParam.fastScanDuration =         GLC_FAST_SCAN_DURATION;
  connParam.fastScanInterval =         GLC_FAST_SCAN_INTERVAL;
  connParam.fastScanWindow =           GLC_FAST_SCAN_WINDOW;
  connParam.reducedPowerScanInterval = GLC_REDUCED_POWER_SCAN_INTERVAL;
  connParam.reducedPowerScanWindow =   GLC_REDUCED_POWER_SCAN_WINDOW;
  connParam.peer_addr_type =           PUBLIC_ADDR;
  connParam.peer_addr =                peer_addr;
  connParam.own_addr_type =            PUBLIC_ADDR;
  connParam.conn_min_interval =        GLC_FAST_MIN_CONNECTION_INTERVAL;
  connParam.conn_max_interval =        GLC_FAST_MAX_CONNECTION_INTERVAL;
  connParam.conn_latency =             GLC_FAST_CONNECTION_LATENCY;
  connParam.supervision_timeout =      GLC_SUPERVISION_TIMEOUT;
  connParam.min_conn_length =          GLC_MIN_CONN_LENGTH;
  connParam.max_conn_length =          GLC_MAX_CONN_LENGTH;

  /* Configuration Parameter */

  if (GL_Collector_ConnConf(connParam) != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"Error in GL_Collector_ConnConf()\r\n");
  } 
  //else {
    //glucoseCollectorContext.useConnConf = TRUE;
  //}
}

//TBR
void Host_Profile_Test_Application (void)
{
  uint8_t ret;
  
  deviceState = APPL_UNINITIALIZED;
  APPL_MESG_INFO(profiledbgfile,"\n GLC Test Application: Starting testing the BLE Profiles (z for menu) \n" );
  
  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init Glucose Collector Central role device with RACP post processing for PTS tests
     analysis */
  Device_Init();

  /* Init Profile Security */
  Device_SetSecurity();
  
  deviceState = APPL_INIT_DONE;
  
  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();
    
    /* Call the Profile Master role state machine */
    Master_Process();
    
    if (glucoseCollectorContext.fullConf) 
      GL_Collector_StateMachine(); 
    
//    Enter_Sleep_Mode();  //TBR???
    
      GL_Collector_PostProcess_RACP_Notification_SM();
    
#ifndef PTS_AUTOMATING
      Enter_Sleep_Mode();
#endif
    
    /* if the profiles have nothing more to process, then
     * wait for application input
     */ 
    if(deviceState >= APPL_INIT_DONE) //TBR
    {      
      uint8_t input = 200;
      deviceState = input;
      if (__io_getcharNonBlocking(&input))
      {
        APPL_MESG_INFO(profiledbgfile,"io--- input: %c\n",input); 

        switch(input)
        {           
          case DISPLAY_PTS_MENU: 
           
          case APPL_DISCOVER_GLUCOSE_SENSOR:
          case APPL_CONNECT_GLUCOSE_SENSOR:
          case APPL_DISCONNECT_GLUCOSE_SENSOR:
          case APPL_PAIR_WITH_GLUCOSE_SENSOR:
          case APPL_CLEAR_SECURITY_DATABASE:
            
          case APPL_DISCOVER_GLUCOSE_SERVICES:
          case APPL_DISCOVER_GLUCOSE_SERVICE_CHARACTERISTICS:
          case APPL_DISCOVER_GL_MEASUREMENT_CHAR_DESCRIPTOR:
          case APPL_DISCOVER_GL_MEASUREMENT_CONTEXT_CHAR_DESCRIPTOR:
          case APPL_DISCOVER_GL_RACP_CHAR_DESCRIPTOR:
            
          case APPL_DISCOVER_DIS_CHARACTERISTICS:
          case APPL_READ_DIS_MANUFACTURER_NAME_CHAR:
          case APPL_READ_DIS_MODEL_NUMBER_CHAR:
          case APPL_READ_DIS_SYSTEM_ID_CHAR:
            
          case APPL_ENABLE_GL_MEASUREMENT_CHAR_NOTIFICATION:
          case APPL_ENABLE_GL_MEASUREMENT_CONTEXT_CHAR_NOTIFICATION:
          case APPL_ENABLE_GL_RACP_CHAR_INDICATION:
            
          case APPL_READ_GL_FEATURE_CHAR:
            
          case APPL_RACP_REPORT_ALL:
          case APPL_RACP_REPORT_LESS_THAN_SEQ_NUM:
          case APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM:
          case APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2:
          case APPL_RACP_REPORT_GREATER_THAN_UFT:
          case APPL_RACP_REPORT_WITHIN_SEQ_NUMS:
          case APPL_RACP_REPORT_FIRST:
          case APPL_RACP_REPORT_LAST:
         
          case APPL_RACP_DELETE_ALL:
          case APPL_RACP_DELETE_WITHIN_SEQ_NUMS:
          case APPL_RACP_ABORT_RACP_REPORT:
          case APPL_RACP_REPORT_NUMBER_ALL:
          case APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM:
            
          case APPL_RACP_WRONG_OP_CODE:
          case APPL_RACP_WRONG_OPERATOR:
          case APPL_RACP_WRONG_OPERAND:
          case APPL_RACP_POST_PROCESSING:
          case APPL_START_FULL_CONFIGURATION:
          {
            deviceState = input;
          }
          break;
          default:
          break; //continue
        }/* end switch(input) */
      } /* end if _IO.... */
    }/* end if(deviceState >= APPL_CONNECTED) */
            
    /* application specific processing */	
    switch(deviceState)
    {
      case DISPLAY_PTS_MENU:
      {
        Display_Appl_Menu();
      }
      break;
      case APPL_DISCOVER_GLUCOSE_SENSOR:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_GLUCOSE_SENSOR: call Device_Discovery_Procedure() \n"); 
        Device_Discovery_Procedure();
      }
      break;
      case APPL_CONNECT_GLUCOSE_SENSOR:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CONNECT_GLUCOSE_SENSOR: call Device_Connection_Procedure() \n"); 
        Device_Connection_Procedure();
      }
      break;
      case APPL_DISCONNECT_GLUCOSE_SENSOR:
      {
         APPL_MESG_DBG(profiledbgfile,"APPL_DISCONNECT_GLUCOSE_SENSOR: call Device_Disconnection() \n"); 
         Device_Disconnection();
      }
      break;
      case APPL_PAIR_WITH_GLUCOSE_SENSOR:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_PAIR_WITH_GLUCOSE_SENSOR: call Device_StartPairing() \n"); 
        Device_StartPairing();
      }
      break;
      case APPL_CLEAR_SECURITY_DATABASE:
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_CLEAR_SECURITY_DATABASE: call Clear_Security_Database() \n"); 
        status = GL_Collector_Clear_Security_Database();
        if (status == BLE_STATUS_SUCCESS) 
        {
          APPL_MESG_DBG(profiledbgfile,"GL_Collector_Clear_Security_Database() Call: OK\n" );
        }
        else
        {
          APPL_MESG_DBG(profiledbgfile,"GL_Collector_Clear_Security_Database() Error: %02X\n", status);
        }
      }
      break; 
      case APPL_DISCOVER_GLUCOSE_SERVICES: /* 4.3.1 TP/GLD/CO/BV-01-I 
                                              4.3.2 TP/GLD/CO/BV-02-I */
      {
        /* It discover all the primary services of the connected glucose sensor device */
        APPL_MESG_DBG(profiledbgfile,"Call Device_ServicesDiscovery() x connected glucose sensor device\n"); 
        Device_ServicesDiscovery();
      }
      break;
      
      case APPL_DISCOVER_GLUCOSE_SERVICE_CHARACTERISTICS: /* 4.3.4  TP/GLD/CO/BV-04-I 
                                                             4.3.6  TP/GLD/CO/BV-06-I
                                                             4.3.8  TP/GLD/CO/BV-08-I
                                                             4.3.10 TP/GLD/CO/BV-10-I */ 
      {
        /* It discovers all the characteristics of the connected glucose service */
        APPL_MESG_DBG(profiledbgfile,"Call Device_Discovery_CharacServ() x Glucose Service\n"); 
        Device_Discovery_CharacServ(GLUCOSE_SERVICE_UUID);
      }
      break;
      
      case APPL_DISCOVER_GL_MEASUREMENT_CHAR_DESCRIPTOR: /* 4.3.5  TP/GLD/CO/BV-05-I */            
      {
        /* It discovers the characteristic descriptors of the connected Glucose Sensor Measurement Characteristic */
        APPL_MESG_DBG(profiledbgfile,"Call GL_Collector_Start_Glucose_Measurement_Characteristic_Descriptor_Discovery\n"); 
        status = GL_Collector_Start_Glucose_Measurement_Characteristic_Descriptor_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Start_Glucose_Measurement_Characteristic_Descriptor_Discovery() call: %02X\n", status);
        }
      }
      break;

      case APPL_DISCOVER_GL_MEASUREMENT_CONTEXT_CHAR_DESCRIPTOR: /* 4.3.7  TP/GLD/CO/BV-07-I */
      {
        /* It discovers the characteristic descriptors of the connected Glucose Sensor Measurement Context Characteristic */
        APPL_MESG_DBG(profiledbgfile,"Call Master_GetCharacDescriptors() x Glucose Measurement Context Characteristic\n"); 
        status = GL_Collector_Start_Glucose_Measurement_Context_Characteristic_Descriptor_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Start_Glucose_Measurement_Context_Characteristic_Descriptor_Discovery() call: %02X\n", status);
        }
      }
      break;
      
      case APPL_DISCOVER_GL_RACP_CHAR_DESCRIPTOR: /* 4.3.9  TP/GLD/CO/BV-09-I */
       { 
        /* It discovers the characteristic descriptors of the connected Glucose Sensor RACP Characteristic */
        APPL_MESG_DBG(profiledbgfile,"Call GL_Collector_Start_RACP_Characteristic_Descriptor_Discovery() x Glucose RACP Characteristic\n"); 
        status = GL_Collector_Start_RACP_Characteristic_Descriptor_Discovery();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Start_RACP_Characteristic_Descriptor_Discovery() call: %02X\n", status);
        }
      }
      break;

      case APPL_DISCOVER_DIS_CHARACTERISTICS: /* 4.3.11 TP/GLD/CO/BV-11-I */
      {
        /* It discovers all the characteristics of the connected device information service */
        APPL_MESG_DBG(profiledbgfile,"APPL_DISCOVER_DIS_CHARACTERISTICS: call Device_Discovery_CharacServ() x Device Information Service\n"); 
        Device_Discovery_CharacServ(DEVICE_INFORMATION_SERVICE_UUID);
        
      }
      break;
      
    case APPL_READ_DIS_MANUFACTURER_NAME_CHAR: /* 4.3.12 TP/GLD/CO/BV-12-I */
      {
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c11)", PTS_CODE, GLP_PTS_CODE);
#endif           
        APPL_MESG_DBG(profiledbgfile,"Read Device Information Service: Manufacturer Name Characteristic \n");
        
        /* Read Device Info Manufacturer Name Characteristic */
        status = GL_Collector_ReadDISManufacturerNameChar();

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_ReadDISManufacturerNameChar() call: %02X\n", status);
        }
      }
      break;
      case APPL_READ_DIS_MODEL_NUMBER_CHAR: /* 4.3.12 TP/GLD/CO/BV-12-I*/
      {
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c12)", PTS_CODE, GLP_PTS_CODE);
#endif        
        APPL_MESG_DBG(profiledbgfile,"Read Device Information Service: Model Number Characteristic \n");
        
        /* Read Device Info Model Number Characteristic */
        status = GL_Collector_ReadDISModelNumberChar();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_ReadDISModelNumberChar() call: %02X\n", status);
        }
      }
      break;
      
      case APPL_READ_DIS_SYSTEM_ID_CHAR: /* 4.3.12 TP/GLD/CO/BV-12-I */
      {
#ifdef PTS_AUTOMATING
        APPL_MESG_INFO(profiledbgfile,"(%c%c13)", PTS_CODE, GLP_PTS_CODE);
#endif      
        APPL_MESG_DBG(profiledbgfile,"Read Device Information Service: System ID Characteristic \n");
      
        /* Read Device Info System ID Characteristic */
        status = GL_Collector_ReadDISSystemIDChar();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_ReadDISSystemIDChar() call (Device Info (System ID)): %02X\n", status);
        }
      }
      break;
      
      case APPL_ENABLE_GL_MEASUREMENT_CHAR_NOTIFICATION: /* 4.4.9 TP/GLF/CO/BV-09-I */
      {
        /* Enable Glucose Measurement Char Descriptor for notifications */
        APPL_MESG_DBG(profiledbgfile,"Call GL_Collector_Enable_Glucose_Measurement_Char_Notification() \n");
        status = GL_Collector_Enable_Glucose_Measurement_Char_Notification();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Enable_Glucose_Measurement_Char_Notification() call: %02X\n", status);
        }
      }
      break;
      case APPL_ENABLE_GL_MEASUREMENT_CONTEXT_CHAR_NOTIFICATION: /* 4.4.15 TP/GLF/CO/BV-11-I  */
      {
        APPL_MESG_DBG(profiledbgfile,"Call GL_Collector_Enable_Glucose_Measurement_Context_Char_Notification() \n");
        status = GL_Collector_Enable_Glucose_Measurement_Context_Char_Notification();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Enable_Glucose_Measurement_Context_Char_Notification() call: %02X\n", status);
        }
      }
      break;
      
      case APPL_ENABLE_GL_RACP_CHAR_INDICATION: /* New command */
      {
        APPL_MESG_DBG(profiledbgfile,"Call GL_Collector_Enable_Glucose_RACP_Char_Indication() \n");
        status = GL_Collector_Enable_Glucose_RACP_Char_Indication();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Enable_Glucose_RACP_Char_Indication() call: %02X\n", status);
        }
      }
      break;
      
      case APPL_READ_GL_FEATURE_CHAR: /* 4.4.23	TP/GLF/CO/BV-13-I */
      {
       
        APPL_MESG_DBG(profiledbgfile,"Read Glucose Feature Characteristic \n");
        /* Read Glucose Feature Characteristic */
        status = GL_Collector_ReadFeatureChar();
        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the Master_Read_Value() call (Glucose Feature Characteristic): %02X\n", status);
        }
      }
      break;
      
      
      case APPL_RACP_REPORT_ALL: /* 4.5.1 GLP/COL/SPR/BV-01-I*/
      {
        /* NOTE: it is also used for:
        
           test 4.5.8 TP/SPR/CO/BV-08-I
           test: 4.5.9 TP/SPR/CO/BV-09-I
           case APPL_RACP_DELETE_ALL: (4.6.1 TP/SPD/CO/BV-01-I)
           case APPL_RACP_ABORT_RACP_REPORT: (4.7.1 TP/SPA/CO/BV-01-I (~200 records))
           test: 4.9.4 TP/SPE/CO/BI-04-I
        */
       
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_ALL \n");
        /* Send RACP command: Report Stored Records - Report All */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_ALL,                  // racp operator
                                        RACP_FILTER_TYPE_NONE,              // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_ALL): %02X\n", status);
        }
      
      }
      break;
      case APPL_RACP_REPORT_LESS_THAN_SEQ_NUM: /* 4.5.2	TP/SPR/CO/BV-02-I: sequence_number = 0x0004 */
      { 
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_LESS_THAN_SEQ_NUM \n");
        racp_filter_parameter_1.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1; //TBR
        
        /* Send RACP command: Report Stored Records - Less than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_LESS_EQUAL,           // racp operator
                                        RACP_FILTER_TYPE_SEQUENCE_NUMBER,   // racp filter type
                                        &racp_filter_parameter_1,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_LESS_THAN_SEQ_NUM): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM: /* 4.5.3 TP/SPR/CO/BV-03-I */
      { 
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM \n");
        racp_filter_parameter_1.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1; //TBR

        /* Send RACP command: Report Stored Records - Greater than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_GREATER_EQUAL,        // racp operator
                                        RACP_FILTER_TYPE_SEQUENCE_NUMBER,   // racp filter type
                                        &racp_filter_parameter_1,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM): %02X\n", status);
        }
      }
      break;
      
      case APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2: /* 4.5.3 TP/SPR/CO/BV-03-I */
      { 
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2 \n");
        racp_filter_parameter_1.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_2; //TBR

        /* Send RACP command: Report Stored Records - Greater than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_GREATER_EQUAL,        // racp operator
                                        RACP_FILTER_TYPE_SEQUENCE_NUMBER,   // racp filter type
                                        &racp_filter_parameter_1,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_GREATER_THAN_SEQ_NUM2): %02X\n", status);
        }
      }
      break;
      
      case APPL_RACP_REPORT_GREATER_THAN_UFT: /* 4.5.4 GLP/COL/SPR/BV-04-I */ //TBR
      {  
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_GREATER_THAN_UFT \n");
        
        racp_filter_parameter_1.user_facing_time.year = PTS_UFT_FILTER_VALUE_YEAR;       //TBR
        racp_filter_parameter_1.user_facing_time.month = PTS_UFT_FILTER_VALUE_MONTH;     //TBR
        racp_filter_parameter_1.user_facing_time.day = PTS_UFT_FILTER_VALUE_DAY;         //TBR
        racp_filter_parameter_1.user_facing_time.hours = PTS_UFT_FILTER_VALUE_HOURS;     //TBR
        racp_filter_parameter_1.user_facing_time.minutes = PTS_UFT_FILTER_VALUE_MINUTES; //TBR
        racp_filter_parameter_1.user_facing_time.seconds = PTS_UFT_FILTER_VALUE_SECONDS; //TBR

        /* Send RACP command: Report Stored Records - Greater than or equal to User Facing Time */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE,  //racp opcode
                                        RACP_OPERATOR_GREATER_EQUAL,         // racp operator
                                        RACP_FILTER_TYPE_USER_FACING_TIME,   // racp filter type
                                        &racp_filter_parameter_1,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_GREATER_THAN_UFT): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_REPORT_WITHIN_SEQ_NUMS: /* 4.5.5 TP/SPR/CO/BV-05-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_WITHIN_SEQ_NUMS \n");
        racp_filter_parameter_1.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1; //TBR
        racp_filter_parameter_2.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1 + 1; //TBR
        /* Send RACP command: Report Stored Records - Less than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_WITHIN_RANGE,         // racp operator
                                        RACP_FILTER_TYPE_SEQUENCE_NUMBER,   // racp filter type
                                        &racp_filter_parameter_1,
                                        &racp_filter_parameter_2);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_WITHIN_SEQ_NUMS): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_REPORT_FIRST: /* 4.5.6 TP/SPR/CO/BV-06-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_FIRST \n");
        /* Send RACP command: Report Stored Records - Less than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_FIRST_RECORD,         // racp operator
                                        RACP_FILTER_TYPE_NONE,              // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_FIRST): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_REPORT_LAST: /* 4.5.7 TP/SPR/CO/BV-07-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_LAST \n");
        /* Send RACP command: Report Stored Records - Less than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_LAST_RECORD,          // racp operator
                                        RACP_FILTER_TYPE_NONE,              // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_LAST): %02X\n", status);
        }
      }
      break;
      
      /* 4.5.8 TP/SPR/CO/BV-08-I */
     
      /* Step 1: Send RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE.
         Refer to APPL_RACP_REPORT_NUMBER_ALL: (4.8.1 TP/SPN/CO/BV-01-I) 
      
         Step 2: A new record is added at the Tester via the user interface or other means.
      
         Step 3: it  should be followed by this other operation:
         IUT writes the Report stored records Op Code (0x01) to the RACP using
         an Operator of ‘all records’ (0x01) and no Operand. 
         Refer to: 
         case APPL_RACP_REPORT_ALL: (4.5.1 TP/SPR/CO/BV-01-I)
      */
      
      /* 4.5.9 TP/SPR/CO/BV-09-I steps:  */ //TBR
  
      /* Step 1: Send RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE.
         Refer to:
         Refer to APPL_RACP_REPORT_NUMBER_ALL: (4.8.1 TP/SPN/CO/BV-01-I) 
      
         Step 2: The second record is deleted at the Tester via the user interface or other means.
         Step 3: It should be followed by this other operation:
         IUT writes the Report stored records Op Code (0x01) to the RACP using
         an Operator of ‘all records’ (0x01) and no Operand. 
         Refer to: 
         case APPL_RACP_REPORT_ALL: (4.5.1 TP/SPR/CO/BV-01-I)
      */

      case APPL_RACP_DELETE_ALL: /* 4.6.1 TP/SPD/CO/BV-01-I */
      {
        /* Prerequisite: run case APPL_RACP_REPORT_ALL: (4.5.1 TP/SPR/CO/BV-01-I) */
        
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_DELETE_ALL \n");
        
        /* Send RACP command: Delete Stored Records - All records */
        status = GL_Collector_Send_RACP(RACP_DELETE_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_ALL,                  // racp operator
                                        RACP_FILTER_TYPE_NONE,              // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_DELETE_ALL): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_DELETE_WITHIN_SEQ_NUMS: /* 4.6.2 TP/SPD/CO/BV-02-I (4 records)*/
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_DELETE_ALL \n");
        
        racp_filter_parameter_1.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1; //TBR
        racp_filter_parameter_2.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1+1; //TBR
        /* Send RACP command: Delete Stored Records - All records */
        status = GL_Collector_Send_RACP(RACP_DELETE_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_WITHIN_RANGE,         // racp operator
                                        RACP_FILTER_TYPE_SEQUENCE_NUMBER,   // racp filter type
                                        &racp_filter_parameter_1,
                                        &racp_filter_parameter_2);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_DELETE_ALL): %02X\n", status);
        }
      }
      break;
      
      case APPL_RACP_ABORT_RACP_REPORT: /* 4.7.1 TP/SPA/CO/BV-01-I (~200 records) */
      {
        /* Prerequisite: run case APPL_RACP_REPORT_ALL: (4.5.1 TP/SPR/CO/BV-01-I) */
        
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_DELETE_ALL \n");
        
        /* Send RACP command: Delete Stored Records - All records */
        status = GL_Collector_Send_RACP(RACP_ABORT_OPERATION_OP_CODE, //racp opcode
                                        RACP_OPERATOR_NULL,           // racp operator
                                        RACP_FILTER_TYPE_NONE,        // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_DELETE_ALL): %02X\n", status);
        }
       
      }
      break;
     
      case APPL_RACP_REPORT_NUMBER_ALL: /* 4.8.1 TP/SPN/CO/BV-01-I */
      {
        /* NOTE: It is also used on 
           4.5.8 TP/SPR/CO/BV-08-I 
           4.5.9 TP/SPR/CO/BV-09-I */
        
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_NUMBER_ALL \n");
        /* Send RACP command: Report Number of Stored Records - All records */
        status = GL_Collector_Send_RACP(RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_ALL,                         // racp operator
                                        RACP_FILTER_TYPE_NONE,                     // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_NUMBER_ALL): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM: /* 4.8.2 TP/SPN/CO/BV-02-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM \n");
        racp_filter_parameter_1.sequence_number = PTS_SEQUENCE_NUMBER_FILTER_VALUE_1; //TBR
        /* Send RACP command: Report Number of Stored Records - Greater than or equal to Sequence Number */
        status = GL_Collector_Send_RACP(RACP_REPORT_NUMBER_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_GREATER_EQUAL,               // racp operator
                                        RACP_FILTER_TYPE_SEQUENCE_NUMBER,          // racp filter type
                                        &racp_filter_parameter_1,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_REPORT_NUMBER_GREATER_THAN_SEQ_NUM): %02X\n", status);
        }
      }
      break;
        
      case APPL_RACP_WRONG_OP_CODE: /* 4.9.1 TP/SPE/CO/BI-01-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_WRONG_OP_CODE \n");
       
        /* Send RACP command: Unsupported Op Code */
        status = GL_Collector_Send_RACP(UNSUPPORTED_RACP_CODE,          //racp opcode
                                        RACP_OPERATOR_NULL,             // racp operator
                                        RACP_FILTER_TYPE_NONE,          // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_WRONG_OP_CODE): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_WRONG_OPERATOR: /* 4.9.2 TP/SPE/CO/BI-02-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_WRONG_OPERATOR \n");
       
        /* Send RACP command: Unsupported Operator */
        status = GL_Collector_Send_RACP(RACP_REPORT_STORED_RECORDS_OP_CODE, //racp opcode
                                        UNSUPPORTED_RACP_CODE,              // racp operator
                                        RACP_FILTER_TYPE_NONE,              // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_WRONG_OPERATOR): %02X\n", status);
        }
      }
      break;
      case APPL_RACP_WRONG_OPERAND: /* 4.9.3 TP/SPE/CO/BI-03-I */
      {
        APPL_MESG_DBG(profiledbgfile,"APPL_RACP_WRONG_OPERAND \n");
       
        /* Send RACP command: Unsupported Operand */
        status = GL_Collector_Send_RACP(RACP_DELETE_STORED_RECORDS_OP_CODE, //racp opcode
                                        RACP_OPERATOR_GREATER_EQUAL,        // racp operator
                                        UNSUPPORTED_RACP_CODE,              // racp filter type
                                        NULL,
                                        NULL);

        if (status!= BLE_STATUS_SUCCESS)
        {
          APPL_MESG_DBG(profiledbgfile,"Error in the GL_Collector_Send_RACP() call (APPL_RACP_WRONG_OPERAND): %02X\n", status);
        }
      }
      break;
      /*  4.9.4 TP/SPE/CO/BI-04-I */           
      /* Step1: same as 4.5.1 + check on racp timeout event after 30 seconds */
       
      case APPL_RACP_POST_PROCESSING:
      {
         /* check if there are some notifications related to a single RACP procedure to be anaylzed */
         if (GL_Collector_Util_Perform_RACP_Post_processing())
         {
           APPL_MESG_DBG(profiledbgfile,"\n");
           APPL_MESG_DBG(profiledbgfile,"***** Post Processing of received %d notifications for last RACP procedure\n",glucoseCollectorContext.number_of_notifications_for_racp);
         }
      }
      break;
      
     case APPL_START_FULL_CONFIGURATION:
        APPL_MESG_DBG(profiledbgfile,"Call Device_StartFullConfig()\n");
        Device_StartFullConfig();
      break;
    }/* end switch(devicestate) */

  } /* end while(1) */
}/* end Host_Profile_Test_Application() */


/***************  Public Function callback ******************/

void GL_Collector_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
                                     uint8_t data_length, uint8_t *data, 
                                     uint8_t RSSI)
{
  uint8_t addr_to_find[6]=PERIPHERAL_PEER_ADDRESS;
  
  if (memcmp(addr, addr_to_find, 6) == 0) {
    APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
    APPL_MESG_INFO(profiledbgfile,"*\r\n");
    APPL_MESG_INFO(profiledbgfile,"**** Peer Device Found\r\n");
    APPL_MESG_INFO(profiledbgfile,"**** Addr = 0x");
    for (uint8_t i=0; i<6; i++)
      APPL_MESG_INFO(profiledbgfile,"%02x", addr[i]);
    APPL_MESG_INFO(profiledbgfile,"\r\n");
    APPL_MESG_INFO(profiledbgfile,"*\r\n");
    APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  }
}

void GL_Collector_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  switch(connection_evt) {
  case CONNECTION_ESTABLISHED_EVT:
    if (status == BLE_STATUS_SUCCESS) {
      APPL_MESG_INFO(profiledbgfile,"****  Connection Established with Success\r\n");
    }
    break;
  case CONNECTION_FAILED_EVT:
    APPL_MESG_INFO(profiledbgfile,"****  Connection Failed with Status = 0x%02x\r\n", status);
    break;
  case DISCONNECTION_EVT:
    APPL_MESG_INFO(profiledbgfile,"****  Disconnection with peer device\r\n");
    break;
  }
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
}

void GL_Collector_ServicesDiscovery_CB(uint8_t status, uint8_t numServices, uint8_t *services)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Primary Service Discovery Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugServices(numServices, services);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void GL_Collector_CharacOfService_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristics of a Service Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharac(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");  
}

void GL_Collector_CharacDesc_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Discovery Characteristic Descriptor Procedure\r\n");
  APPL_MESG_INFO(profiledbgfile,"****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharacDesc(numCharac, charac);
  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");    
}

void GL_Collector_DataValueRead_CB(uint8_t status, uint16_t data_len, uint8_t *data)
{
  uint8_t i;

  APPL_MESG_INFO(profiledbgfile,"****************************************\r\n");
  APPL_MESG_INFO(profiledbgfile,"**** Data Read Value Procedure: Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS) 
  {
    APPL_MESG_INFO(profiledbgfile,"****    Len data Read = %d\r\n", data_len);
    if (data_len == GLUCOSE_READ_SIZE)
    {
#ifdef PTS_AUTOMATING
      APPL_MESG_INFO(profiledbgfile,"(%c%c29)", PTS_CODE, GLP_PTS_CODE);
#endif
      APPL_MESG_INFO(profiledbgfile,"**** Glucose Feature Read Value: 0x%04x\n",glucoseCollectorContext.glucose_feature);
    }
    else
    {
      APPL_MESG_INFO(profiledbgfile,"****    Data: ");
#ifdef PTS_AUTOMATING
      for (i=0; i<data_len; i++) {
        APPL_MESG_INFO(profiledbgfile,"%c", data[i]);
      }  
      APPL_MESG_INFO(profiledbgfile,"\n"); 
      APPL_MESG_INFO(profiledbgfile,"****    Data: 0x");
      if (data_len == 4)
        for (i=data_len; i>0; i--) {
          APPL_MESG_INFO(profiledbgfile,"%02x", data[i-1]);
        } 
      else
        for (i=0; i<data_len; i++) {
          APPL_MESG_INFO(profiledbgfile,"%02x", data[i]);
        } 
      APPL_MESG_INFO(profiledbgfile,"\n");      
#else
      for (i=0; i<data_len; i++) {
        APPL_MESG_INFO(profiledbgfile,"0x%0x (%d)", data[i], data[i]);
      }
      APPL_MESG_INFO(profiledbgfile,"\r\n");
#endif
      
    }
  }
}

void GL_Collector_FullConfError_CB(uint8_t error_type, uint8_t code)
{
  if (glucoseCollectorContext.fullConf) {
    APPL_MESG_INFO(profiledbgfile,"***** GL_Collector_FullConf_CB() Error Type = 0x%0x with Code = 0x%0x\r\n", error_type, code);
  }
}

void GL_Collector_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  APPL_MESG_INFO(profiledbgfile,"**** Required Security Pin Code\r\n");

  /* Insert the pin code according the glucose collector indication */
  //pinCode = ....

  ret = GL_Collector_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    APPL_MESG_INFO(profiledbgfile,"**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

#if 0 //TBR
void GL_Collector_RACP_Response_CB(uint8_t err_code)
{
}

void GL_Collector_RACP_Received_Indication_CB(uint8_t racp_response, uint8_t value, uint8_t  num_records)   
{
}
#endif 
                                        
void GL_Collector_NotificationReceived_CB(uint8_t handle, uint8_t length, uint8_t * data_value)
{
  APPL_MESG_INFO(profiledbgfile,"NOTIFICATION_DATA_RECEIVED: handle=0x%04x, data length=%d, data:\r\n",
	   handle, length);
    
  for (uint8_t i=0; i<length; i++) {
    APPL_MESG_INFO(profiledbgfile,"%02x ", data_value[i]);
  }
  APPL_MESG_INFO(profiledbgfile,"\r\n");  
}
  
void GL_Collector_Pairing_CB(uint16_t conn_handle, uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"Pairing CB conn_handle = 0x%04x with status = 0x%02x***************\n", conn_handle, status);
  
}

void GL_Collector_EnableNotificationIndication_CB(uint8_t status)
{
  APPL_MESG_INFO(profiledbgfile,"**** Enable Notification/Indication procedure ended with Status = 0x%0x\r\n", status);
}

#endif    


/** \endcond 
 */
