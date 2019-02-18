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
*   FILENAME        -  profiles_HidHost_central_test_application.c
*
*******************************************************************************
*
*   CHANGE HISTORY
*  ---------------------------
*
*   $Date$:      26/09/2016
*   $Revision$:  first version
*   $Author$:    RF Application team
*   Comments:    Profiles PTS test application for HOGP profile. 
*                It uses the BlueNRG-1 Host Profiles Library
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  Test application file for HID host profile central role 
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
#include "uuid.h"
#include "SDK_EVAL_Config.h"
#include "master_basic_profile.h"

#if (BLE_CURRENT_PROFILE_ROLES & HID_HOST)

#include "hid_host.h"
#include "hid_host_config.h"

/*******************************************************************************
 * Macros
*******************************************************************************/

/* --------------------- User Inputs for HID Device PTS tests -----------------*/
///@cond BLE_CURRENT_PROFILE_ROLES & HID_HOST
/**
 *@defgroup Hid_Host 
 *@brief PTS test application user input commands HID Host Central role
 *@{
 */

//General operations: discovery, connection, disconnection, bonding, clear security database 

/** 
 * HID Host user input command: Start Discovery procedure for detecting an HID peripheral device(<b>HID_DeviceDiscovery() </b> API)
 */
#define APPL_DISCOVERY_HID_PERIPHERAL                       'a' 

/** 
 * HID Host user input command: Start Connection procedure for connecting to the discovered HID peripheral device(<b>HID_DeviceConnection() </b> API)
 */
#define APPL_CONNECT_HID_PERIPHERAL                         'b'

/** 
 * HID Host user input command: Terminate the current connection with the HID peripheral device(<b>HID_DeviceDisconnection() </b> API)
 */
#define APPL_DISCONNECT_HID_PERIPHERAL                      'c' // Terminate the current connection with the HID peripheral device

/** 
 * HID Host user input command: Start the pairing  procedure (or enable the encryption) with the connected HID peripheral device(<b>HID_StartPairing() </b> API)
 */
#define APPL_PAIR_WITH_HID_PERIPHERAL                       'd' 

/** 
 * HID Host user input command: Clear the security database (it removes any bonded device)(<b>HID_ClearBondedDevices() </b> API)
 */
#define APPL_CLEAR_SECURITY_DATABASE                        'e' 

/** 
 *  HID Host user input command: Starts all the connection, service discovery, characteristic discovery, ..... procedures.
 */
#define APPL_START_FULL_CONFIGURATION                       'z' 

/** 
 *  HID Host user input command: Display PTS user input commands list.
 */
#define APPL_PTS_TEST_MENU                                  '?'

/* ---------- HID Host tests  ------------*/

/** 
 *  HID Host user input command: Discover All Primary Services (<b>HID_ServicesDiscovery() </b> API)
 */
#define APPL_DISCOVERY_PEER_DEVICE_SERVICES                 'f' // TP/HGDS/HH/BV-01-I [Discover HID Services]
                                                                // TP/HGDS/HH/BV-02-I [Discover Battery Service]
                                                                // TP/HGDS/HH/BV-03-I [Discover Device Information Service]
                                                                // TP/SPDS/SC/BV-01-I [Discover Scan Parameters Service]

/** 
 *  HID Host user input command: Find Includes Services (<b>HID_GetIncludedBatterySerivces() </b> API)
 */
#define APPL_FIND_INCLUDED_SERVICES                         'g' // TP/HGDR/RH/BV-01-I [Find Included Services]

/** 
 *  HID Host user input command: Discover All Characteristics of HID Service --> Report Map, HID Information, HID Control Point, Protocol Mode, Boot Keyboard Input Report,Boot Keyboard Output, Boot Mouse Input Report, .. Characteristics (<b>HID_DiscCharacServ(HUMAN_INTERFACE_DEVICE_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVERY_ALL_HID_SERVICE_CHARAC               'i' // TP/HGDC/RH/BV-01-I [Discover Report Map Characteristics]
                                                                // TP/HGDC/RH/BV-06-I [Discover HID Information Characteristics]
                                                                // TP/HGDC/RH/BV-07-I [Discover HID Control Point Characteristics]
                                                                // TP/HGDC/HH/BV-08-I [Discover Protocol Mode Characteristics]
                                                                // TP/HGDC/BH/BV-09-I [Discover Boot Keyboard Input Report Characteristic]
                                                                // TP/HGDC/BH/BV-11-I [Discover Boot Keyboard Output Report Characteristic]
                                                                // TP/HGDC/BH/BV-12-I [Discover Boot Mouse Input Report Characteristic]

/** 
 *  HID Host user input command: Discover External Report Reference Characteristic Descriptors for Report Map Characteristic (<b>HID_DiscCharacDesc(REPORT_MAP_CHAR_UUID) </b> API)
 */
#define APPL_DISCOVERY_EXTERNAL_REPORT_REFERENCE            'l' // TP/HGDC/RH/BV-02-I [Discover External Report Reference Characteristic Descriptors for Report Map Characteristics]

/** 
 *  HID Host user input command:  Discover Report Characteristic Client Characteristic Configuration & Report Reference Descriptors (<b>HID_DiscCharacDesc(REPORT_CHAR_UUID) </b> API)
 */
#define APPL_DISCOVERY_REPORT_CHARAC_DESCRIPTOR             'm' // TP/HGDC/RH/BV-04-I [Discover Report Characteristic Client Characteristic Configuration Descriptors]
                                                                // TP/HGDC/RH/BV-05-I [Discover Report Characteristic Report Reference Characteristic Descriptors]

/** 
 *  HID Host user input command: Discover Boot Keyboard Input Report Client Characteristic Configuration Descriptor (<b>HID_DiscCharacDesc(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_DISCOVERY_BOOT_KEYBOARD_INPUT_CHARAC_DESC      'n' // TP/HGDC/BH/BV-10-I [Discover Boot Keyboard Input Report Client Characteristic Configuration Descriptor]


/** 
 *  HID Host user input command: Discover Boot Mouse Input Report Client Characteristic Configuration Descriptor (<b>HID_DiscCharacDesc(BOOT_MOUSE_INPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_DISCOVERY_BOOT_MOUSE_INPUT_CHARAC_DESC         'o' // TP/HGDC/BH/BV-13-I [Discover Boot Mouse Input Report Client Characteristic Configuration Descriptor]

/** 
 *  HID Host user input command: Discover Battery Level Characteristic (<b>HID_DiscCharacDesc(BATTERY_LEVEL_CHAR_UUID) </b> API)
 */
#define APPL_DISOCVERY_BATTERY_SERVICE_CHARAC               'p' // TP/HGDC/HH/BV-14-I [Discover Battery Level Characteristics]

/** 
 *  HID Host user input command: Discover Battery Level Client Characteristic Configuration Descriptor (<b>HID_DiscCharacDesc(BATTERY_LEVEL_CHAR_UUID) </b> API)
 */
#define APPL_DISCOVERY_BATTERY_LEVEL_CHARAC_DESC            'q' // TP/HGDC/HH/BV-15-I [Discover Battery Level Client Characteristic Configuration Descriptors] 

/** 
 *  HID Host user input command: Discover PnP ID Characteristic (<b>HID_DiscCharacServ(DEVICE_INFORMATION_SERVICE_UUID) </b> API)
 */
#define APPL_DISCOVERY_DEVICE_INF_SERVICE_CHARAC            'r' // TP/HGDC/HH/BV-16-I [Discover PnP ID Characteristic]  

/** 
 *  HID Host user input command: Read Report Map Characteristics (<b>HID_ReadReportDescriptor() </b> API)
 */
#define APPL_READ_HID_REPORT_MAP                            's' // TP/HGRF/RH/BV-01-I [Read Report Map Characteristics] 

/** 
 *  HID Host user input command: Read External Report Reference Characteristic Descriptors for Report Map Characteristics (<b>HID_ReadReportValue(EXTERNAL_REPORT_REFERENCE_DESCRIPTOR_UUID) </b> API)
 */
#define APPL_READ_EXTERNAL_REPORT_CHARAC_DESC               't' // TP/HGRF/RH/BV-02-I [Read External Report Reference Characteristic Descriptors for Report Map Characteristics]

/** 
 *  HID Host user input command: Read Report Reference Characteristic Descriptors for Report Characteristics - Input Report, Output Report (<b>HID_ReadReportValue(REPORT_REFERENCE_DESCRIPTOR_UUID) </b> API)
 */
#define APPL_READ_REPORT_REFERENCE_DESC                     'u' // TP/HGRF/RH/BV-04-I [Read Report Reference Characteristic Descriptors for Report Characteristics - Input Report]
                                                                // TP/HGRF/RH/BV-06-I [Read Report Reference Characteristic Descriptors for Report Characteristics - Output Report]

/** 
 *  HID Host user input command: Read Report Characteristics - Input Report (<b>HID_ReadReportValue(INPUT_REPORT) </b> API)
 */
#define APPL_READ_INPUT_REPORT_VALUE                        'v' // TP/HGRF/RH/BV-03-I [Read Report Characteristics - Input Report]

/** 
 *  HID Host user input command: Read Client Characteristic Configuration Descriptors for Report Characteristics - Input Report (<b>HID_ReadReportValue(CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID) </b> API)
 */
#define APPL_READ_INPUT_REPORT_CLIENT_CONFIG_DESC_VALUE     'w' // TP/HGRF/RH/BV-05-I [Read Client Characteristic Configuration Descriptors for Report Characteristics - Input Report]

/** 
 *  HID Host user input command: Read Report Characteristics - Output Report (<b>HID_ReadReportValue(OUTPUT_REPORT) </b> API)
 */
#define APPL_READ_OUTPUT_REPORT_VALUE                       'x' // TP/HGRF/RH/BV-19-I [Read Report Characteristics - Output Report]

/** 
 *  HID Host user input command: Read Report Characteristics - Feature Report (<b>HID_ReadReportValue(FEATURE_REPORT) </b> API)
 */
#define APPL_READ_FEATURE_REPORT_VALUE                      'y' // TP/HGRF/RH/BV-07-I [Read Report Characteristics - Feature Report]

/** 
 *  HID Host user input command: Read HID Information Characteristic (<b>HID_ReadHidInformation() </b> API)
 */
#define APPL_READ_HID_INFORMATION_VALUE                     'A' // TP/HGRF/RH/BV-09-I [Read HID Information Characteristics]

/** 
 *  HID Host user input command: Read Battery Level Characteristic (<b>HID_ReadBatteryLevel() </b> API)
 */
#define APPL_READ_BATTERY_LEVEL_VALUE                       'B' // TP/HGRF/HH/BV-10-I [Read Battery Level Characteristic]

/** 
 *  HID Host user input command: Read Client Characteristic Configuration Descriptor for Battery Level Characteristic (<b>HID_ReadBatteryClientCharacDesc() </b> API)
 */
#define APPL_READ_BATTERY_LEVEL_CHARAC_DESC                 'C' // TP/HGRF/HH/BV-11-I [Read Client Characteristic Configuration Descriptor for Battery Level Characteristic]

/** 
 *  HID Host user input command: Read Client PnP ID Characteristic (<b>HID_ReadPnPID() </b> API)
 */
#define APPL_READ_PNP_ID_VALUE                              'D' // TP/HGRF/HH/BV-12-I [Read PnP ID Characteristic]

/** 
 *  HID Host user input command: Read Boot Keyboard Input Report Characteristic (<b>HID_ReadBootReport(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_READ_BOOT_KEYBOARD_INPUT_REPORT                'E' // TP/HGRF/HH/BV-12-I TP/HGRF/BH/BV-13-I [Read Boot Keyboard Input Report Characteristic]

/** 
 *  HID Host user input command: Read Client Characteristic Configuration Descriptors for Boot Keyboard Input Report Characteristic (<b>HID_ReadBootReportClientCharacDesc(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_READ_BOOT_KEYBOARD_INPUT_CHARAC_DESC           'F' // TP/HGRF/BH/BV-14-I [Read Client Characteristic Configuration Descriptors for Boot Keyboard Input Report Characteristics]

/** 
 *  HID Host user input command: Read Boot Keyboard Output Report Characteristic (<b>HID_ReadBootReport(BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_READ_BOOT_KEYBOARD_OUTPUT_REPORT               'G' // TP/HGRF/BH/BV-15-I [Read Boot Keyboard Output Report Characteristic]

/** 
 *  HID Host user input command: Read Boot Mouse Input Report Characteristic (<b>HID_ReadBootReport(BOOT_MOUSE_INPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_READ_BOOT_MOUSE_INPUT_REPORT                   'H' // TP/HGRF/BH/BV-16-I [Read Boot Mouse Input Report Characteristic]

/** 
 *  HID Host user input command: Read Client Characteristic Configuration Descriptor for Boot Mouse Input Report Characteristic (<b>HID_ReadBootReportClientCharacDesc(BOOT_MOUSE_INPUT_REPORT_CHAR_UUID) </b> API)
 */
#define APPL_READ_BOOT_MOUSE_INPUT_CHARAC_DESC              'J' // TP/HGRF/BH/BV-17-I [Read Client Characteristic Configuration Descriptor for Boot Mouse Input Report Characteristic]

/** 
 *  HID Host user input command: Read Protocol Mode Characteristics (Get Protocol Mode Command) (<b>HID_GetProtocol() </b> API)
 */
#define APPL_READ_PROTOCOL_MODE                             'K' // TP/HGRF/HH/BV-18-I [Read Protocol Mode Characteristics (Get Protocol Mode Command)]

/** 
 *  HID Host user input command: Write Report Characteristics - Input Report (<b>HID_SetReport(FALSE, INPUT_REPORT,...) </b> API)
 */
#define APPL_WRITE_INPUT_REPORT                             'L' // TP/HGWF/RH/BV-01-I [Write Report Characteristics - Input Report]

/** 
 *  HID Host user input command: Write Report Characteristics - Output Report (<b>HID_SetReport(FALSE, OUTPUT_REPORT,...) </b> API)
 */
#define APPL_WRITE_OUTPUT_REPORT                            'M' // TP/HGWF/RH/BV-02-I [Write Report Characteristics - Output Report]

/** 
 *  HID Host user input command: Write Command Report Characteristics – Output Report (<b>HID_SetReport(TRUE, OUTPUT_REPORT,...) </b> API)
 */
#define APPL_WRITE_COMMAND_OUTPUT_REPORT                    'N' // TP/HGWF/RH/BV-03-I [Write Command - Report Characteristics – Output Report]

/** 
 *  HID Host user input command: Write Command Report Characteristics – Feature Report (<b>HID_SetReport(FALSE, FEATURE_REPORT,...) </b> API)
 */
#define APPL_WRITE_FEATURE_REPORT                           'O' // TP/HGWF/RH/BV-04-I [Write Report Characteristics – Feature Report]

/** 
 *  HID Host user input command: Write HID Control Point Characteristics – Suspend (<b>HID_SetControlPoint(TRUE) </b> API)
 */
#define APPL_SET_SUSPEND_MODE                               'P' // TP/HGWF/RH/BV-05-I [Write HID Control Point Characteristics – Suspend]

/** 
 *  HID Host user input command: Write HID Control Point Characteristics – Exit Suspend (<b>HID_SetControlPoint(FALSE) </b> API)
 */
#define APPL_EXIT_SUSPEND_MODE                              'Q' // TP/HGWF/RH/BV-06-I [Write HID Control Point Characteristics – Exit Suspend]

/** 
 *  HID Host user input command: Write Protocol Mode Characteristics – Set Protocol Command (Protocol Mode = Report Protocol Mode) (<b>HID_SetProtocol(REPORT_PROTOCOL_MODE) </b> API)
 */
#define APPL_SET_REPORT_PROTOCOL_MODE                       'R' // TP/HGWF/RH/BV-07-I [Write Protocol Mode Characteristics – Set Protocol Command (Protocol Mode = Report Protocol Mode)]

/** 
 *  HID Host user input command: Write Protocol Mode Characteristics – Set Protocol Command (Protocol Mode = Boot Protocol Mode) (<b>HID_SetProtocol(BOOT_PROTOCOL_MODE) </b> API)
 */
#define APPL_SET_BOOT_PROTOCOL_MODE                         'S' // TP/HGWF/BH/BV-08-I [Write Protocol Mode Characteristics – Set Protocol Command (Protocol Mode = Boot Protocol Mode)]

/** 
 *  HID Host user input command: Write Boot Keyboard Input Report Characteristic (<b>HID_SetBootReport(BOOT_KEYBOARD_INPUT_REPORT) </b> API)
 */
#define APPL_SET_BOOT_KEYBOARD_INPUT_REPORT                 'T' // TP/HGWF/BH/BV-09-I [Write Boot Keyboard Input Report Characteristic]

/** 
 *  HID Host user input command: Write Boot Keyboard Output Report Characteristic (<b>HID_SetBootReport(BOOT_KEYBOARD_OUTPUT_REPORT) </b> API)
 */
#define APPL_SET_BOOT_KEYBOARD_OUTPUT_REPORT                'U' // TP/HGWF/BH/BV-10-I [Write Boot Keyboard Output Report Characteristic]

/** 
 *  HID Host user input command: Write Boot Mouse Input Report Characteristic](<b>HID_SetBootReport(BOOT_MOUSE_INPUT_REPORT) </b> API)
 */
#define APPL_SET_BOOT_MOUSE_INPUT_REPORT                    'V' // TP/HGWF/BH/BV-11-I [Write Boot Mouse Input Report Characteristic]

/** 
 *  HID Host user input command: Report Characteristic – Input Reports – enable notifications (write with 0x0001)(<b>HID_SetNotificationStatus(INPUT_REPORT, TRUE) </b> API)
 */
#define APPL_ENABLE_INPUT_REPORT_NOTIFICATION               'W' // TP/HGCF/RH/BV-01-I [Report Characteristic – Input Reports – enable notifications (write with 0x0001)]
                                                                // TP/HGNF/RH/BV-01-I [Report Characteristic Configuration, receive notifications]

/** 
 *  HID Host user input command: Report Characteristic – Input Reports – disable notifications (write with 0x0000)(<b>HID_SetNotificationStatus(INPUT_REPORT, FALSE) </b> API)
 */
#define APPL_DISABLE_INPUT_REPORT_NOTIFICATION              'X' // TP/HGCF/RH/BV-02-I [Report Characteristic – Input Reports – disable notifications (write with 0x0000)]

/** 
 *  HID Host user input command: Boot Keyboard Input Report Characteristic – enable notifications (write with 0x0001) (<b>HID_SetNotificationStatus(BOOT_KEYBOARD_INPUT_REPORT, TRUE) </b> API)
 */
#define APPL_ENABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION        'Y' // TP/HGCF/BH/BV-03-I [Boot Keyboard Input Report Characteristic – enable notifications (write with 0x0001)]
                                                                // TP/HGNF/BH/BV-02-I [Boot Keyboard Input Report Characteristic Configuration, receive notifications]

/** 
 *  HID Host user input command: Boot Keyboard Input Report Characteristic – disable notifications (write with 0x0000) (<b>HID_SetNotificationStatus(BOOT_KEYBOARD_INPUT_REPORT, FALSE) </b> API)
 */
#define APPL_DISABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION       'Z' // TP/HGCF/BH/BV-04-I [Boot Keyboard Input Report Characteristic – disable notifications (write with 0x0000)]

/** 
 *  HID Host user input command: Boot Mouse Input Report Characteristic – enable notifications (write with 0x0001) (<b>HID_SetNotificationStatus(BOOT_MOUSE_INPUT_REPORT, TRUE) </b> API)
 */
#define APPL_ENABLE_BOOT_MOUSE_INPUT_NOTIFICATION           '1' // TP/HGCF/BH/BV-05-I [Boot Mouse Input Report Characteristic – enable notifications (write with 0x0001)]
                                                                // TP/HGNF/BH/BV-03-I [Boot Mouse Input Report Characteristic Configuration, receive notifications]

/** 
 *  HID Host user input command: Boot Mouse Input Report Characteristic – disable notifications (write with 0x0000) (<b>HID_SetNotificationStatus(BOOT_MOUSE_INPUT_REPORT, FALSE) </b> API)
 */
#define APPL_DISABLE_BOOT_MOUSE_INPUT_NOTIFICATION          '2' // TP/HGCF/BH/BV-06-I [Boot Mouse Input Report Characteristic – disable notifications (write with 0x0000)]

/** 
 *  HID Host user input command: Report Characteristic Configuration, ignore notifications, Boot Host (<b>HID_SetHostMode(BOOT_PROTOCOL_MODE) </b> API)
 */
#define APPL_SET_HID_BOOT_HOST                              '3' // TP/HGNF/BH/BI-01-I [Report Characteristic Configuration, ignore notifications, Boot Host]

/** 
 *  HID Host user input command: Boot Keyboard Input Report Characteristic Configuration, ignore notifications, Report Host (<b>HID_SetHostMode(REPORT_PROTOCOL_MODE) </b> API)
 */
#define APPL_SET_HID_REPORT_HOST                            '4' // TP/HGNF/RH/BI-01-I [Boot Keyboard Input Report Characteristic Configuration, ignore notifications, Report Host]
                                                                // TP/HGNF/RH/BI-02-I [Boot Mouse Input Report Characteristic Configuration, ignore notifications, Report Host]

/** 
 *  HID Host user input command: Discover Scan Interval Window Characteristic (<b>HID_DiscCharacServ(SCAN_PARAMETER_SERVICE_UUID)</b> API)
 */
#define APPL_DISCOVERY_SCAN_PARAMETER_CHARAC                '5' // TP/SPDC/SC/BV-01-I [Discover Scan Interval Window Characteristic]
                                                                // TP/SPDC/SC/BV-02-I [Discover Scan Refresh Characteristic]
/** 
 *  HID Host user input command: Discover Scan Refresh Client Characteristic Configuration Descriptor (<b>HID_DiscCharacDesc(SCAN_REFRESH_CHAR_UUID)</b> API)
 */
#define APPL_DISCOVERY_SCAN_REFRESH_CHARAC_DESC             '6' // TP/SPDC/SC/BV-03-I [Discover Scan Refresh Client Characteristic Configuration Descriptor]

/** 
 *  HID Host user input command: Write Scan Interval Window Characteristic Value  (<b>HID_WriteScanIntervalWindowParam(0xCCFC, 0xECAB)</b> API)
 */
#define APPL_WRITE_SCAN_INTERVAL_WINDOW                     '7' // TP/SPWF/SC/BV-01-I [Write Scan Interval Window Characteristic Value]

/** 
 *  HID Host user input command: Scan Refresh Client Characteristic Configuration - Enable Notifications (Write with 0x0001) (<b>HID_ScanRefreshNotificationStatus(TRUE)</b> API)
 */
#define APPL_ENABLE_SCAN_REFRESH_CHARAC_DESC                '8' // TP/SPCF/SC/BV-01-I [Scan Refresh Client Characteristic Configuration - Enable Notifications (Write with 0x0001)]

/** 
 *  HID Host user input command: Read the Input Report with Report ID 4 (<b>HID_GetReport(INPUT_REPORT, 4)</b> API)
 */
#define APPL_GET_REPORT_COMMAND                             '9' // Read the Input Report with Report ID 4


#define APPL_PROFILE_RESET                                  '!' //reset the device
/**
 * @}
 */
///@endcond

/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

#define MAX_NUMBER_REPORT_DESC 2
#define MAX_PREPORT_DESC_ARRAY_SIZE 255

/******************************************************************************
 * Local Variable Declarations
******************************************************************************/
SleepModes sleep_mode = SLEEPMODE_WAKETIMER; 

/******************************************************************************
 * Global Variables
******************************************************************************/
uint8_t useConnConf = FALSE;
uint8_t reportData[MAX_PREPORT_DESC_ARRAY_SIZE];
uint8_t numReportDescPresent;
uint16_t reportDescLen[MAX_NUMBER_REPORT_DESC];

/******************************************************************************
 * Function Declarations
******************************************************************************/
uint8_t __io_getcharNonBlocking(uint8_t *data);

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

void printDebugServices(uint8_t numDeviceServices, uint8_t *deviceServices)
{
  uint8_t i , j;
  uint16_t handle, uuid; 
  
  printf("****    Number of Primary Services: %d\r\n", numDeviceServices);
  j = 0;
  for (i=0; i<numDeviceServices; i++) {
    if (deviceServices[j] == UUID_TYPE_16) {
      memcpy(&uuid, &deviceServices[j+5], 2);
      switch (uuid) {
      case HUMAN_INTERFACE_DEVICE_SERVICE_UUID:
#ifdef PTS_AUTOMATING
        printf("(%c%c1)", PTS_CODE, HID_PTS_CODE);
#endif        
	printf("****    Human Interface Device Service UUID found\r\n");
	break;
      case DEVICE_INFORMATION_SERVICE_UUID:
#ifdef PTS_AUTOMATING
        printf("(%c%c3)", PTS_CODE, HID_PTS_CODE);
#endif         
	printf("****    Device Information Service UUID found\r\n");
	break;
      case BATTERY_SERVICE_SERVICE_UUID:
#ifdef PTS_AUTOMATING
        printf("(%c%c2)", PTS_CODE, HID_PTS_CODE);
#endif         
	printf("****    Battery Service UUID found \r\n");
	break;
      case SCAN_PARAMETER_SERVICE_UUID: 
#ifdef PTS_AUTOMATING
        printf("(%c%c33)", PTS_CODE, HID_PTS_CODE);
#endif        
	printf("****    Scan Parameter Service UUID found \r\n");
	break;
      default:
	printf("****    UUID 0x%04x\r\n", uuid);
      }
    }
    j++;
    memcpy(&handle, &deviceServices[j], 2);
    printf("****       Start Handle = 0x%04x\r\n", handle);
    j += 2;
    memcpy(&handle, &deviceServices[j], 2);
    printf("****       End Handle = 0x%04x\r\n", handle);
    j += 2;
    if (deviceServices[j-5] == UUID_TYPE_16) {
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

  printf("****    Number of Charac of a Service: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 8;
    memcpy(&uuid_charac, &charac[index+6], 2);
    } else {
      sizeElement = 22;
    }

    switch(uuid_charac) {
    case PROTOCOL_MODE_CHAR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c12)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Protocol Mode Characteristic found\r\n");
      break;
    case REPORT_CHAR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c7)", PTS_CODE, HID_PTS_CODE);
#endif     
      printf("****      Report Characteristic found\r\n");
      break;
    case REPORT_MAP_CHAR_UUID:
#ifdef PTS_AUTOMATING
     printf("(%c%c5)", PTS_CODE, HID_PTS_CODE);
#endif      
      printf("****      Report Map Characteristic found\r\n");
      break;
    case BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID:
#ifdef PTS_AUTOMATING
     printf("(%c%c23)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Boot Keyboard Input Report Characteristic found\r\n");
      break;
    case BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID:
#ifdef PTS_AUTOMATING
     printf("(%c%c24)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Boot Keyboard Output Report Characteristic found\r\n");
      break;
    case BOOT_MOUSE_INPUT_REPORT_CHAR_UUID:
#ifdef PTS_AUTOMATING
     printf("(%c%c25)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Boot Mouse Input Report Characteristic found\r\n");
      break;
    case HID_INFORMATION_CHAR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c10)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      HID Information Characteristic found\r\n");
      break;
    case HID_CONTROL_POINT_CHAR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c11)", PTS_CODE, HID_PTS_CODE);
#endif      
      printf("****      HID Control Point Characteristic found\r\n");
      break;
    case BATTERY_LEVEL_CHAR_UUID:
#ifdef PTS_AUTOMATING 
      printf("(%c%c13)", PTS_CODE, HID_PTS_CODE);  
#endif      
      printf("****      Battery Level Characteristic found\r\n");
      break;
    case PNP_ID_UUID:
#ifdef PTS_AUTOMATING 
      printf("(%c%c14)", PTS_CODE, HID_PTS_CODE);  
#endif       
      printf("****      PnP ID Characteristic found\r\n");
      break;
    case SCAN_INTERVAL_WINDOW_CHAR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c34)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Scan Interval Window Characteristic found\r\n");
      break;
    case SCAN_REFRESH_CHAR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c35)", PTS_CODE, HID_PTS_CODE);
#endif      
      printf("****      Scan Refresh Characteristic found\r\n");
      break;
    default:
      printf("****      Characteristic found\r\n");
    }

    memcpy(&charac_handle, &charac[index+1], 2);
    properties = charac[index+3];
    memcpy(&value_handle, &charac[index+4], 2);
    printf("****         Characteristic Handle = 0x%04x\r\n", charac_handle);
    printf("****         Properties = 0x%02x\r\n", properties);
    printf("****         ValueHandle = 0x%04x\r\n", value_handle);
    printf("****         UUID = 0x%04x\r\n", uuid_charac);

    index += sizeElement;
  }
}

void printDebugCharacDesc(uint8_t numCharac, uint8_t *charac)
{
  uint8_t i, index, sizeElement;
  uint16_t handle_characDesc, uuid_characDesc;

  printf("****    Number of Charac Descriptor: %d\r\n", numCharac);
  index = 0;
  for (i=0; i<numCharac; i++) { 
    if (charac[index] == 1) {
      sizeElement = 5;
      memcpy(&uuid_characDesc, &charac[index+3], 2);
    } else {
      sizeElement = 19;
    }

    memcpy(&handle_characDesc, &charac[index+1], 2);

    switch(uuid_characDesc) {
    case EXTERNAL_REPORT_REFERENCE_DESCRIPTOR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c6)", PTS_CODE, HID_PTS_CODE);
#endif      
      printf("****      External Report Reference descriptor found\r\n");
      break;
    case REPORT_REFERENCE_DESCRIPTOR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c9)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Rerport Reference Descriptor found\r\n");
      break;
    case CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID:
#ifdef PTS_AUTOMATING
      printf("(%c%c8)", PTS_CODE, HID_PTS_CODE);
#endif       
      printf("****      Client Characteristic Descriptor found\r\n");
      break;
    case CHAR_PRESENTATION_FORMAT_DESCRIPTOR_UUID:
      printf("****      Characteristic Presentation Format Descriptor found\r\n");
      break;
    default:
      printf("****      Characteristic Descriptor found\r\n");
    }

    printf("****         Characteristic Descriptor Handle = 0x%04x\r\n", handle_characDesc);
    printf("****         UUID = 0x%04x\r\n", uuid_characDesc);

    index += sizeElement;
  }
}

void Display_Appl_Menu()
{
  printf("APPL_DISCOVERY_HID_PERIPHERAL:                    %c\n", APPL_DISCOVERY_HID_PERIPHERAL);
  printf("APPL_CONNECT_HID_PERIPHERAL:                      %c\n", APPL_CONNECT_HID_PERIPHERAL);
  printf("APPL_DISCONNECT_HID_PERIPHERAL:                   %c\n", APPL_DISCONNECT_HID_PERIPHERAL); 
  printf("APPL_PAIR_WITH_HID_PERIPHERAL:                    %c\n", APPL_PAIR_WITH_HID_PERIPHERAL);
  printf("APPL_CLEAR_SECURITY_DATABASE:                     %c\n", APPL_CLEAR_SECURITY_DATABASE);
  printf("APPL_DISCOVERY_PEER_DEVICE_SERVICES:              %c\n", APPL_DISCOVERY_PEER_DEVICE_SERVICES);
  printf("APPL_FIND_INCLUDED_SERVICES:                      %c\n", APPL_FIND_INCLUDED_SERVICES);
  printf("APPL_DISCOVERY_ALL_HID_SERVICE_CHARAC:            %c\n", APPL_DISCOVERY_ALL_HID_SERVICE_CHARAC);
  printf("APPL_DISCOVERY_EXTERNAL_REPORT_REFERENCE:         %c\n", APPL_DISCOVERY_EXTERNAL_REPORT_REFERENCE);
  printf("APPL_DISCOVERY_REPORT_CHARAC_DESCRIPTOR:          %c\n", APPL_DISCOVERY_REPORT_CHARAC_DESCRIPTOR);
  printf("APPL_DISCOVERY_BOOT_KEYBOARD_INPUT_CHARAC_DESC:   %c\n", APPL_DISCOVERY_BOOT_KEYBOARD_INPUT_CHARAC_DESC);
  printf("APPL_DISCOVERY_BOOT_MOUSE_INPUT_CHARAC_DESC:      %c\n", APPL_DISCOVERY_BOOT_MOUSE_INPUT_CHARAC_DESC);
  printf("APPL_DISOCVERY_BATTERY_SERVICE_CHARAC:            %c\n", APPL_DISOCVERY_BATTERY_SERVICE_CHARAC);
  printf("APPL_DISCOVERY_BATTERY_LEVEL_CHARAC_DESC:         %c\n", APPL_DISCOVERY_BATTERY_LEVEL_CHARAC_DESC);
  printf("APPL_DISCOVERY_DEVICE_INF_SERVICE_CHARAC:         %c\n", APPL_DISCOVERY_DEVICE_INF_SERVICE_CHARAC);
  printf("APPL_READ_HID_REPORT_MAP:                         %c\n", APPL_READ_HID_REPORT_MAP);
  printf("APPL_READ_EXTERNAL_REPORT_CHARAC_DESC:            %c\n", APPL_READ_EXTERNAL_REPORT_CHARAC_DESC);
  printf("APPL_READ_REPORT_REFERENCE_DESC:                  %c\n", APPL_READ_REPORT_REFERENCE_DESC);
  printf("APPL_READ_INPUT_REPORT_VALUE:                     %c\n", APPL_READ_INPUT_REPORT_VALUE);
  printf("APPL_READ_INPUT_REPORT_CLIENT_CONFIG_DESC_VALUE:  %c\n", APPL_READ_INPUT_REPORT_CLIENT_CONFIG_DESC_VALUE);
  printf("APPL_READ_OUTPUT_REPORT_VALUE:                    %c\n", APPL_READ_OUTPUT_REPORT_VALUE);
  printf("APPL_READ_FEATURE_REPORT_VALUE:                   %c\n", APPL_READ_FEATURE_REPORT_VALUE);
  printf("APPL_READ_HID_INFORMATION_VALUE:                  %c\n", APPL_READ_HID_INFORMATION_VALUE);
  printf("APPL_READ_BATTERY_LEVEL_VALUE:                    %c\n", APPL_READ_BATTERY_LEVEL_VALUE);
  printf("APPL_READ_BATTERY_LEVEL_CHARAC_DESC:              %c\n", APPL_READ_BATTERY_LEVEL_CHARAC_DESC);
  printf("APPL_READ_PNP_ID_VALUE:                           %c\n", APPL_READ_PNP_ID_VALUE);
  printf("APPL_READ_BOOT_KEYBOARD_INPUT_REPORT:             %c\n", APPL_READ_BOOT_KEYBOARD_INPUT_REPORT);
  printf("APPL_READ_BOOT_KEYBOARD_INPUT_CHARAC_DESC:        %c\n", APPL_READ_BOOT_KEYBOARD_INPUT_CHARAC_DESC);
  printf("APPL_READ_BOOT_KEYBOARD_OUTPUT_REPORT:            %c\n", APPL_READ_BOOT_KEYBOARD_OUTPUT_REPORT);
  printf("APPL_READ_BOOT_MOUSE_INPUT_REPORT:                %c\n", APPL_READ_BOOT_MOUSE_INPUT_REPORT);
  printf("APPL_READ_BOOT_MOUSE_INPUT_CHARAC_DESC:           %c\n", APPL_READ_BOOT_MOUSE_INPUT_CHARAC_DESC);
  printf("APPL_READ_PROTOCOL_MODE:                          %c\n", APPL_READ_PROTOCOL_MODE);
  printf("APPL_WRITE_INPUT_REPORT:                          %c\n", APPL_WRITE_INPUT_REPORT);
  printf("APPL_WRITE_OUTPUT_REPORT:                         %c\n", APPL_WRITE_OUTPUT_REPORT);
  printf("APPL_WRITE_COMMAND_OUTPUT_REPORT:                 %c\n", APPL_WRITE_COMMAND_OUTPUT_REPORT);
  printf("APPL_WRITE_FEATURE_REPORT:                        %c\n", APPL_WRITE_FEATURE_REPORT);
  printf("APPL_SET_SUSPEND_MODE:                            %c\n", APPL_SET_SUSPEND_MODE);
  printf("APPL_EXIT_SUSPEND_MODE:                           %c\n", APPL_EXIT_SUSPEND_MODE);
  printf("APPL_SET_REPORT_PROTOCOL_MODE:                    %c\n", APPL_SET_REPORT_PROTOCOL_MODE);
  printf("APPL_SET_BOOT_PROTOCOL_MODE:                      %c\n", APPL_SET_BOOT_PROTOCOL_MODE);
  printf("APPL_SET_BOOT_KEYBOARD_INPUT_REPORT:              %c\n", APPL_SET_BOOT_KEYBOARD_INPUT_REPORT);
  printf("APPL_SET_BOOT_KEYBOARD_OUTPUT_REPORT:             %c\n", APPL_SET_BOOT_KEYBOARD_OUTPUT_REPORT);
  printf("APPL_SET_BOOT_MOUSE_INPUT_REPORT:                 %c\n", APPL_SET_BOOT_MOUSE_INPUT_REPORT);
  printf("APPL_ENABLE_INPUT_REPORT_NOTIFICATION:            %c\n", APPL_ENABLE_INPUT_REPORT_NOTIFICATION);
  printf("APPL_DISABLE_INPUT_REPORT_NOTIFICATION:           %c\n", APPL_DISABLE_INPUT_REPORT_NOTIFICATION);
  printf("APPL_ENABLE_BOOT_KEYBOARD_INPUT_NOTIFIFICATION:   %c\n", APPL_ENABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION);
  printf("APPL_DISABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION:    %c\n", APPL_DISABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION);
  printf("APPL_ENABLE_BOOT_MOUSE_INPUT_NOTIFICATION:        %c\n", APPL_ENABLE_BOOT_MOUSE_INPUT_NOTIFICATION);
  printf("APPL_DISABLE_BOOT_MOUSE_INPUT_NOTIFICATION:       %c\n", APPL_DISABLE_BOOT_MOUSE_INPUT_NOTIFICATION);
  printf("APPL_SET_HID_BOOT_HOST:                           %c\n", APPL_SET_HID_BOOT_HOST);
  printf("APPL_SET_HID_REPORT_HOST:                         %c\n", APPL_SET_HID_REPORT_HOST);
  printf("APPL_DISCOVERY_SCAN_PARAMETER_CHARAC:             %c\n", APPL_DISCOVERY_SCAN_PARAMETER_CHARAC);
  printf("APPL_DISCOVERY_SCAN_REFRESH_CHARAC_DESC:          %c\n", APPL_DISCOVERY_SCAN_REFRESH_CHARAC_DESC);
  printf("APPL_WRITE_SCAN_INTERVAL_WINDOW:                  %c\n", APPL_WRITE_SCAN_INTERVAL_WINDOW);
  printf("APPL_ENABLE_SCAN_REFRESH_CHARAC_DESC:             %c\n", APPL_ENABLE_SCAN_REFRESH_CHARAC_DESC);
  printf("APPL_GET_REPORT_COMMAND:                          %c\n", APPL_GET_REPORT_COMMAND);
  printf("APPL_START_FULL_CONFIGURATION:                    %c\n", APPL_START_FULL_CONFIGURATION);
  printf("APPL_PTS_TEST_MENU:                               %c\n", APPL_PTS_TEST_MENU);
  printf("APPL_PROFILE_RESET:                               %c\n", APPL_PROFILE_RESET);  
                                 
}/* end Display_Appl_Menu() */

void Device_Init(void)
{
  hidInitDevType hidInitDev; 
  uint8_t public_addr[6]=HID_PUBLIC_ADDRESS;
  uint8_t device_name[]=HID_DEVICE_NAME;

  useConnConf = FALSE;

  hidInitDev.public_addr  = public_addr;
  hidInitDev.txPower      = HID_TX_POWER_LEVEL;
  hidInitDev.device_name_len = sizeof(device_name);
  hidInitDev.device_name = device_name;
  hidInitDev.hostMode     = HID_HOST_MODE; 
  if (HID_Init(hidInitDev) != BLE_STATUS_SUCCESS)
    printf("Error in HID_Init()\r\n");
}

void Device_SetSecurity(void)
{
  hidSecurityType param;

  param.ioCapability =  HID_IO_CAPABILITY;
  param.mitm_mode =     HID_MITM_MODE;
  param.bonding_mode =  HID_BONDING_MODE;
  param.use_fixed_pin = HID_USE_FIXED_PIN;
  param.fixed_pin =     HID_FIXED_PIN;
  param.Min_Encryption_Key_Size = MIN_KEY_SIZE;
  param.Max_Encryption_Key_Size = MAX_KEY_SIZE;
      
#if BLE_2_X /* BLE stack FW version >= 2.x: BLE Security v4.2 is supported */   
  param.sc_support = HID_SC_SUPPORT;
  param.keypress_support = HID_KEYPRESS_NOTIFICATION_SUPPORT;  
  param.identity_address_type = HID_IDENTITY_ADDRESS_TYPE ;
#else  
  param.oob_enable =    HID_OOB_ENABLE;
#endif
  
  if (HID_SecuritySet(param) != BLE_STATUS_SUCCESS) {
    printf("Error in HID_SecuritySet() function call\r\n");
  }
}

void Device_Discovery_Procedure(void)
{
  hidDevDiscType param;

  param.own_addr_type = PUBLIC_ADDR;
  param.scanInterval =  HID_LIM_DISC_SCAN_INT;
  param.scanWindow =    HID_LIM_DISC_SCAN_WIND;
  if (HID_DeviceDiscovery(param) != BLE_STATUS_SUCCESS) {
    printf("Error in HID_DeviceDiscovery() function call\r\n");
  }
}

void Device_Connection_Procedure(void)
{
  hidConnDevType param;
  uint8_t pts_peer_addr[6] = PTS_PERIPHERAL_ADDRESS; 

  param.fastScanDuration =         HID_FAST_SCAN_DURATION;
  param.fastScanInterval =         HID_FAST_SCAN_INTERVAL;
  param.fastScanWindow =           HID_FAST_SCAN_WINDOW;
  param.reducedPowerScanInterval = HID_REDUCED_POWER_SCAN_INTERVAL;
  param.reducedPowerScanWindow =   HID_REDUCED_POWER_SCAN_WINDOW;
  param.peer_addr_type =           PUBLIC_ADDR;
  param.peer_addr =                pts_peer_addr;
  param.own_addr_type =            PUBLIC_ADDR;
  param.conn_min_interval =        HID_FAST_MIN_CONNECTION_INTERVAL;
  param.conn_max_interval =        HID_FAST_MAX_CONNECTION_INTERVAL;
  param.conn_latency =             HID_FAST_CONNECTION_LATENCY;
  param.supervision_timeout =      HID_SUPERVISION_TIMEOUT;
  param.min_conn_length =          HID_MIN_CONN_LENGTH;
  param.max_conn_length =          HID_MAX_CONN_LENGTH;
  if (HID_DeviceConnection(param) != BLE_STATUS_SUCCESS) {
    printf("Error in HID_DeviceConnection()\r\n");
  }
}

void Device_Disconnection(void)
{
  if (HID_DeviceDisconnection()) {
    printf("Error in HID_DisconnectionDevice()\r\n");
  }
}

void Device_ClearSecurityDatabase(void)
{
  if (HID_ClearBondedDevices()) {
    printf("Error in HID_ClearBondedDevices()\r\n");
  }
}

void Device_StartPairing(void)
{
  if (HID_StartPairing()) {
    printf("Error in HID_StartPairing()\r\n");
  }
}

void Device_ServicesDiscovery(void)
{
  if (HID_ServicesDiscovery()) {
    printf("Error in HID_ServicesDiscovery()\r\n");
  }
}

void Device_IncludedServiceDiscovery(void)
{
  if (HID_GetIncludedBatterySerivces()) {
    printf("Error in HID_GetIncludedBatterySerivces()\r\n");
  }
}

void Device_StartFullConfig(void)
{
  hidConnDevType connParam;
  hidConfDevType confParam;
  uint8_t pts_peer_addr[6] = PTS_PERIPHERAL_ADDRESS; 

  /* Connection Parameters */
  connParam.fastScanDuration =         HID_FAST_SCAN_DURATION;
  connParam.fastScanInterval =         HID_FAST_SCAN_INTERVAL;
  connParam.fastScanWindow =           HID_FAST_SCAN_WINDOW;
  connParam.reducedPowerScanInterval = HID_REDUCED_POWER_SCAN_INTERVAL;
  connParam.reducedPowerScanWindow =   HID_REDUCED_POWER_SCAN_WINDOW;
  connParam.peer_addr_type =           PUBLIC_ADDR;
  connParam.peer_addr =                pts_peer_addr;
  connParam.own_addr_type =            PUBLIC_ADDR;
  connParam.conn_min_interval =        HID_FAST_MIN_CONNECTION_INTERVAL;
  connParam.conn_max_interval =        HID_FAST_MAX_CONNECTION_INTERVAL;
  connParam.conn_latency =             HID_FAST_CONNECTION_LATENCY;
  connParam.supervision_timeout =      HID_SUPERVISION_TIMEOUT;
  connParam.min_conn_length =          HID_MIN_CONN_LENGTH;
  connParam.max_conn_length =          HID_MAX_CONN_LENGTH;

  /* Configuration Parameters */
  confParam.maxNmbReportDesc = MAX_NUMBER_REPORT_DESC;
  confParam.maxReportDataSize = MAX_PREPORT_DESC_ARRAY_SIZE;
  confParam.numReportDescPresent = &numReportDescPresent;
  confParam.reportDescLen = reportDescLen;
  confParam.reportData = reportData;

  if (HID_ConnConf(connParam, confParam) != BLE_STATUS_SUCCESS) {
    printf("Error in HID_ConnConf()\r\n");
  } else {
    useConnConf = TRUE;
  }
}

void Device_Discovery_CharacServ(uint16_t uuid_service)
{
  if (HID_DiscCharacServ(uuid_service)) {
    printf("Error in HID_DiscCharacServ()\r\n");
  }
}

void Device_Discovery_ExternalReportRef(void)
{
  if (HID_DiscCharacDesc(REPORT_MAP_CHAR_UUID)) {
    printf("Error in HID_DiscCharacDesc()\r\n");
  }
}

void Device_Discovery_ReportCharacDescriptor(void)
{
  if (HID_DiscCharacDesc(REPORT_CHAR_UUID)) {
    printf("Error in HID_DiscCharacDesc()\r\n");
  }
}

void Device_Discovery_BootKeyboardInput_CharacDesc(void)
{
  if (HID_DiscCharacDesc(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID)) {
    printf("Error in HID_DiscCharacDesc()\r\n");
  }
}

void Device_Discovery_BootMouseInput_CharacDesc(void)
{
  if (HID_DiscCharacDesc(BOOT_MOUSE_INPUT_REPORT_CHAR_UUID)) {
    printf("Error in HID_DiscCharacDesc()\r\n");
  }
}

void Device_Discovery_BatteryLevel_CharacDesc(void)
{
  if (HID_DiscCharacDesc(BATTERY_LEVEL_CHAR_UUID)) {
    printf("Error in HID_DiscCharacDesc()\r\n");
  }
}

void Device_ReadReportMap(void)
{
  static uint8_t reportToRead=1;
  static uint16_t offset;

  if (reportToRead == 1) {
    offset = 0;
  } else {
    offset += reportDescLen[reportToRead-2];
  }

//  printf("!!!!!!!!!!reportToRead: %d\n", reportToRead);
    
  if (HID_ReadReportDescriptor(reportToRead, &reportDescLen[reportToRead-1], &reportData[offset], MAX_PREPORT_DESC_ARRAY_SIZE-offset)) {
    printf("Error in HID_ReadReportDescriptor()\n\n");
  } else {
    numReportDescPresent = reportToRead;
    reportToRead++;
  }
}

void Device_ReadExtReportRefCharacDesc(void)
{
  if(HID_ReadReportValue(EXTERNAL_REPORT_REFERENCE_DESCRIPTOR_UUID)) {
     printf("Error in HID_ReadReportValue(EXTERNAL_REPORT_REFERENCE_DESCRIPTOR_UUID)\n");
  }
}

void Device_ReadReportRefDesc(void)
{
  if (HID_ReadReportValue(REPORT_REFERENCE_DESCRIPTOR_UUID)) {
    printf("Error in HID_ReadReportValue(REPORT_REFERENCE_DESCRIPTOR_UUID)\n");
  }
}

void Device_ReadReportValue(uint8_t reportType)
{
  if (HID_ReadReportValue(reportType)) {
    printf("Error in HID_ReadReportValue()\n");
  }
}

void Device_ReadReportClientCharacDescValue(void)
{
  if (HID_ReadReportValue(CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID)) {
    printf("Error in HID_ReadReportValue(INPUT_REPORT)\n");
  }
}

void Device_ReadHidInformationValue(void)
{
  if (HID_ReadHidInformation()) {
    printf("Error in HID_ReadHidInformation()\n");
  }
}

void Device_ReadBatteryLevelValue(void)
{
  if (HID_ReadBatteryLevel()) {
    printf("Error in HID_ReadBatteryLevel()\n");
  }
}

void Device_ReadBatteryCharacDesc(void)
{
  if (HID_ReadBatteryClientCharacDesc()) {
    printf("Error in HID_ReadBatteryClientCharacDesc()\n");
  }
}

void Device_ReadPnPID(void)
{
  if (HID_ReadPnPID()) {
    printf("Error in HID_ReadPnPID()\n");
  }
}

void Device_ReadBootReport(uint16_t bootReportUUID)
{
  if (HID_ReadBootReport(bootReportUUID)) {
    printf("Error in HID_ReadBootReport()\n");
  }
}

void Device_ReadBootClientCharacDesc(uint16_t bootReportUUID)
{
  if (HID_ReadBootReportClientCharacDesc(bootReportUUID)) {
    printf("Error in HID_ReadBootReportClientCharacDesc()\n");
  }
}

void Device_ReadProtocolMode(void)
{
  if (HID_GetProtocol()) {
    printf("Error in HID_GetProtocol()\n");
  }
}

void Device_WriteInputReport(void)
{
  uint8_t numInputReport, i;
  uint8_t reportId[20];
  static uint8_t reportToWrite=0;
  uint8_t data[]={0xAA, 0xB3, 0xF8, 0xA6, 0xCD};

  HID_GetReportId(INPUT_REPORT, &numInputReport, reportId);
  printf("****************************************\r\n");
  printf("**** Input Report found %d\r\n", numInputReport);
  for (i=0; i<numInputReport; i++) {
    printf("**** ReportID = 0x%02x\r\n", reportId[i]);
  }
  printf("**** Set the Report with ID %d\r\n", reportId[reportToWrite]);
  if (HID_SetReport(FALSE, INPUT_REPORT, reportId[reportToWrite], sizeof(data), data)) {
    printf("****   Error during the Set Report procedure\r\n");
  } else {
    reportToWrite++;
  }
  printf("****************************************\r\n");

}

void Device_WriteOutputReport(void)
{
  uint8_t numReport, i;
  uint8_t reportId[20];
  static uint8_t reportToWrite=0;
  uint8_t data[]={0xEF, 0x90, 0x78, 0x56, 0x34, 0x12, 0x00};

  HID_GetReportId(OUTPUT_REPORT, &numReport, reportId);
  printf("****************************************\r\n");
  printf("**** Output Report found %d\r\n", numReport);
  for (i=0; i<numReport; i++) {
    printf("**** ReportID = 0x%02x\r\n", reportId[i]);
  }
  printf("**** Set the Report with ID %d\r\n", reportId[reportToWrite]);
  if (HID_SetReport(FALSE, OUTPUT_REPORT, reportId[reportToWrite], sizeof(data), data)) {
    printf("****   Error during the Set Report procedure\r\n");
  } else {
    reportToWrite++;
  }
  printf("****************************************\r\n");
}

void Device_WriteCommandReport(void)
{
  uint8_t numReport, i;
  uint8_t reportId[20];
  static uint8_t reportToWrite=0;
  uint8_t data[]={0xEF, 0x90, 0x78, 0x56, 0x34, 0x12, 0x00};

  HID_GetReportId(OUTPUT_REPORT, &numReport, reportId);
  printf("****************************************\r\n");
  printf("**** Output Report found %d\r\n", numReport);
  for (i=0; i<numReport; i++) {
    printf("**** ReportID = 0x%02x\r\n", reportId[i]);
  }
  printf("**** Set the Report with ID %d using write without response\r\n", reportId[reportToWrite]);
  if (HID_SetReport(TRUE, OUTPUT_REPORT, reportId[reportToWrite], sizeof(data), data)) {
    printf("****   Error during the Set Report procedure\r\n");
  } else {
    reportToWrite++;
  }
  printf("****************************************\r\n");  
}

void Device_WriteFeatureReport(void)
{
  uint8_t numReport, i;
  uint8_t reportId[20];
  static uint8_t reportToWrite=0;
  uint8_t data[]={0xEA, 0x45, 0x3F, 0x2D, 0x87};

  HID_GetReportId(FEATURE_REPORT, &numReport, reportId);
  printf("****************************************\r\n");
  printf("**** Feature Report found %d\r\n", numReport);
  for (i=0; i<numReport; i++) {
    printf("**** ReportID = 0x%02x\r\n", reportId[i]);
  }
  printf("**** Set the Report with ID %d\r\n", reportId[reportToWrite]);
  if (HID_SetReport(FALSE, FEATURE_REPORT, reportId[reportToWrite], sizeof(data), data)) {
    printf("****   Error during the Set Report procedure\r\n");
  } else {
    reportToWrite++;
  }
  printf("****************************************\r\n");  
}

void Device_SetControlPoint(uint8_t suspend)
{
  if (HID_SetControlPoint(suspend)) {
    printf("****************************************\r\n");
    printf("**** Error during the setup control point\r\n");
    printf("****************************************\r\n");
  } else {
    printf("****************************************\r\n");
    printf("**** Control point setup SUCCESS\r\n");
    printf("****************************************\r\n");    
  }
}

void Device_SetProtocolMode(uint8_t mode)
{
    printf("****************************************\r\n");
  if (HID_SetProtocol(mode)) {
    printf("**** Error during the Set Protocol procedure\r\n");
  } else {
    printf("**** Set Protocol Mode setup SUCCESS\r\n");
  }
  printf("****************************************\r\n");    
}

void Device_SetBootReport(uint8_t bootReportType)
{
  uint8_t data[]={0xF8, 0xA6, 0xAA, 0xEC};
  uint8_t i;

  printf("****************************************\r\n");    
  if (HID_SetBootReport(bootReportType, sizeof(data), data)) {
    printf("**** Error during the Set Boot Report procedure\r\n");
  } else {
    
#ifdef PTS_AUTOMATING
  uint16_t LE_data;  
  uint16_t LE_data2;
  
  printf("(%c%c27)", PTS_CODE, HID_PTS_CODE);
  printf("**** Set Boot Report procedure with data = 0x");  
  LE_data = LE_TO_HOST_16(&data[0]); 
  LE_data2 = LE_TO_HOST_16(&data[2]); 
  printf("%04X %04X\n", LE_data, LE_data2);
  
  printf("(%c%c28)", PTS_CODE, HID_PTS_CODE);
  printf("**** Set Boot Report procedure with data = 0x");  
  for (i=0; i<3; i++)
      printf("%02X ", data[i]);
  printf("%02X\n", data[i]);
  
#else    
    printf("**** Set Boot Report procedure with data = 0x");
    for (i=0; i<4; i++)
      printf("%02x", data[i]);
    printf("\r\n");
#endif    
  }
  printf("****************************************\r\n");    
}

void Device_EnableInputReportNotification(uint8_t type, uint8_t enabled)
{
  printf("****************************************\r\n");    
  if(HID_SetNotificationStatus(type, enabled)) {
    printf("**** Error during the change notification status\r\n");
  } else {
    printf("**** Change notification status success\r\n");
  }
  printf("****************************************\r\n");    
}

void Device_SetHostMode(uint8_t mode)
{
  HID_SetHostMode(mode);
  printf("****************************************\r\n");    
  printf("**** Set Host Mode success\r\n");
  if (mode == BOOT_PROTOCOL_MODE)
    printf("****     Mode set HID Boot Host\r\n");
  else
    printf("****     Mode set HID Report Host\r\n");
  printf("****************************************\r\n");      
}

void Device_Discovery_ScanRefreshCharacDesc(void)
{
  if (HID_DiscCharacDesc(SCAN_REFRESH_CHAR_UUID)) {
    printf("Error in HID_DiscCharacDesc()\r\n");
  }
}

void Device_SetScanIntervalWindow(void)
{
  printf("****************************************\r\n");
  printf("**** Set Scan Interval e Scan Window\r\n");
  if (HID_WriteScanIntervalWindowParam(0xCCFC, 0xECAB)) {
    printf("****  Error in HID_WriteScanIntervalWindowParam()\r\n");
  } else {
    printf("****    Set Success\r\n");
  }
  printf("****************************************\r\n");
}

void Device_EnableScanRefreshNotification(void)
{
  printf("****************************************\r\n");
  if (HID_ScanRefreshNotificationStatus(TRUE)) {
    printf("**** Enable Scan Refresh notification Error\n");
  } else {
    printf("**** Enable Scan Refresh notification Success\n");
  }
  printf("****************************************\r\n");
}

void Device_GetReport(void)
{
  printf("****************************************\r\n");
  if (HID_GetReport(INPUT_REPORT, 4) == BLE_STATUS_SUCCESS) {
    printf("**** GetReport() success\r\n");
  } else {
    printf("**** GetReport() failed\r\n");
  }
  printf("****************************************\r\n");
}

void runProcedure(uint8_t input)
{
  switch(input) {
  case APPL_CLEAR_SECURITY_DATABASE:
    Device_ClearSecurityDatabase();
    break;
  case APPL_DISCOVERY_HID_PERIPHERAL:
    Device_Discovery_Procedure();
    break;
  case APPL_CONNECT_HID_PERIPHERAL:
    Device_Connection_Procedure();
    break;
  case APPL_DISCONNECT_HID_PERIPHERAL:
    Device_Disconnection();
    break;
  case APPL_PAIR_WITH_HID_PERIPHERAL:
    Device_StartPairing();
    break;
  case APPL_DISCOVERY_PEER_DEVICE_SERVICES:
    Device_ServicesDiscovery();
    break;
  case APPL_FIND_INCLUDED_SERVICES:
    Device_IncludedServiceDiscovery();
    break; 
  case APPL_DISCOVERY_ALL_HID_SERVICE_CHARAC:
    Device_Discovery_CharacServ(HUMAN_INTERFACE_DEVICE_SERVICE_UUID);
    break;
  case APPL_DISCOVERY_EXTERNAL_REPORT_REFERENCE:
    Device_Discovery_ExternalReportRef();
    break;
  case APPL_DISCOVERY_REPORT_CHARAC_DESCRIPTOR:
    Device_Discovery_ReportCharacDescriptor();
    break;
  case APPL_DISCOVERY_BOOT_KEYBOARD_INPUT_CHARAC_DESC:
    Device_Discovery_BootKeyboardInput_CharacDesc();
    break;
  case APPL_DISCOVERY_BOOT_MOUSE_INPUT_CHARAC_DESC:
    Device_Discovery_BootMouseInput_CharacDesc();
    break;
  case APPL_DISOCVERY_BATTERY_SERVICE_CHARAC:
    Device_Discovery_CharacServ(BATTERY_SERVICE_SERVICE_UUID);
    break;
  case APPL_DISCOVERY_BATTERY_LEVEL_CHARAC_DESC:
    Device_Discovery_BatteryLevel_CharacDesc();
    break;
  case APPL_DISCOVERY_DEVICE_INF_SERVICE_CHARAC:
    Device_Discovery_CharacServ(DEVICE_INFORMATION_SERVICE_UUID);
    break;
  case APPL_READ_HID_REPORT_MAP:
    Device_ReadReportMap();
    break;
  case APPL_READ_EXTERNAL_REPORT_CHARAC_DESC:
    Device_ReadExtReportRefCharacDesc();
    break;
  case APPL_READ_REPORT_REFERENCE_DESC:
    Device_ReadReportRefDesc();
    break;
  case APPL_READ_INPUT_REPORT_VALUE:
    Device_ReadReportValue(INPUT_REPORT);
    break;
  case APPL_READ_INPUT_REPORT_CLIENT_CONFIG_DESC_VALUE:
    Device_ReadReportClientCharacDescValue();
    break;
  case APPL_READ_OUTPUT_REPORT_VALUE:
    Device_ReadReportValue(OUTPUT_REPORT);
    break;
  case APPL_READ_FEATURE_REPORT_VALUE:
    Device_ReadReportValue(FEATURE_REPORT);
    break;
  case APPL_READ_HID_INFORMATION_VALUE:
    Device_ReadHidInformationValue();
    break;
  case APPL_READ_BATTERY_LEVEL_VALUE:
    Device_ReadBatteryLevelValue();
    break;
  case APPL_READ_BATTERY_LEVEL_CHARAC_DESC:
    Device_ReadBatteryCharacDesc();
    break;
  case APPL_READ_PNP_ID_VALUE:
    Device_ReadPnPID();
    break;
  case APPL_READ_BOOT_KEYBOARD_INPUT_REPORT:
    Device_ReadBootReport(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID);
    break;
  case APPL_READ_BOOT_KEYBOARD_INPUT_CHARAC_DESC:
    Device_ReadBootClientCharacDesc(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID);
    break;
  case APPL_READ_BOOT_KEYBOARD_OUTPUT_REPORT:
    Device_ReadBootReport(BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID);
    break;
  case APPL_READ_BOOT_MOUSE_INPUT_REPORT:
    Device_ReadBootReport(BOOT_MOUSE_INPUT_REPORT_CHAR_UUID);
    break;
  case APPL_READ_BOOT_MOUSE_INPUT_CHARAC_DESC:
    Device_ReadBootClientCharacDesc(BOOT_MOUSE_INPUT_REPORT_CHAR_UUID);
    break;
  case APPL_READ_PROTOCOL_MODE:
    Device_ReadProtocolMode();
    break;
  case APPL_WRITE_INPUT_REPORT:
    Device_WriteInputReport();
    break;
  case APPL_WRITE_OUTPUT_REPORT:
    Device_WriteOutputReport();
    break;
  case APPL_WRITE_COMMAND_OUTPUT_REPORT:
    Device_WriteCommandReport();
    break;
  case APPL_WRITE_FEATURE_REPORT:
    Device_WriteFeatureReport();
    break;
  case APPL_SET_SUSPEND_MODE:
    Device_SetControlPoint(TRUE);
    break;
  case APPL_EXIT_SUSPEND_MODE:
    Device_SetControlPoint(FALSE);
    break;
  case APPL_SET_REPORT_PROTOCOL_MODE:
    Device_SetProtocolMode(REPORT_PROTOCOL_MODE);
    break;
  case APPL_SET_BOOT_PROTOCOL_MODE:
    Device_SetProtocolMode(BOOT_PROTOCOL_MODE);
    break;
  case APPL_SET_BOOT_KEYBOARD_INPUT_REPORT:
    Device_SetBootReport(BOOT_KEYBOARD_INPUT_REPORT);
    break;
  case APPL_SET_BOOT_KEYBOARD_OUTPUT_REPORT:
    Device_SetBootReport(BOOT_KEYBOARD_OUTPUT_REPORT);
    break;
  case APPL_SET_BOOT_MOUSE_INPUT_REPORT:
    Device_SetBootReport(BOOT_MOUSE_INPUT_REPORT);
    break;
  case APPL_ENABLE_INPUT_REPORT_NOTIFICATION:
    Device_EnableInputReportNotification(INPUT_REPORT, TRUE);
    break;
  case APPL_DISABLE_INPUT_REPORT_NOTIFICATION:
    Device_EnableInputReportNotification(INPUT_REPORT, FALSE);
    break;
  case APPL_ENABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION:
    Device_EnableInputReportNotification(BOOT_KEYBOARD_INPUT_REPORT, TRUE);
    break;
  case APPL_DISABLE_BOOT_KEYBOARD_INPUT_NOTIFICATION:
    Device_EnableInputReportNotification(BOOT_KEYBOARD_INPUT_REPORT, FALSE);
    break;
  case APPL_ENABLE_BOOT_MOUSE_INPUT_NOTIFICATION:
    Device_EnableInputReportNotification(BOOT_MOUSE_INPUT_REPORT, TRUE);
    break;
  case APPL_DISABLE_BOOT_MOUSE_INPUT_NOTIFICATION:
    Device_EnableInputReportNotification(BOOT_MOUSE_INPUT_REPORT, FALSE);
    break;
  case APPL_SET_HID_BOOT_HOST:
    Device_SetHostMode(BOOT_PROTOCOL_MODE);
    break;
  case APPL_SET_HID_REPORT_HOST:
    Device_SetHostMode(REPORT_PROTOCOL_MODE);
    break;
  case APPL_DISCOVERY_SCAN_PARAMETER_CHARAC:
    Device_Discovery_CharacServ(SCAN_PARAMETER_SERVICE_UUID);
    break;
  case APPL_DISCOVERY_SCAN_REFRESH_CHARAC_DESC:
    Device_Discovery_ScanRefreshCharacDesc();
    break;
  case APPL_WRITE_SCAN_INTERVAL_WINDOW:
    Device_SetScanIntervalWindow();
    break;
  case APPL_ENABLE_SCAN_REFRESH_CHARAC_DESC:
    Device_EnableScanRefreshNotification();
    break;
  case APPL_GET_REPORT_COMMAND:
    Device_GetReport();
    break;
  case APPL_START_FULL_CONFIGURATION:
    Device_StartFullConfig();
    break;
  case APPL_PTS_TEST_MENU:
    Display_Appl_Menu();
    break;
  case APPL_PROFILE_RESET:
    NVIC_SystemReset();  
    break;
  }
}

void Host_Profile_Test_Application (void)
{
  uint8_t input;
  uint8_t ret;
  
  printf("HID Test Application: Starting testing the BLE Profiles (? for menu) \n" );
//  printf("Digit h to print instruction supported\n");

    /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Init Proximity Monitor Central Role */  
  Device_Init();

  /* Init Profile Security */
  Device_SetSecurity();

  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();
    
    /* Call the Profile Master role state machine */
    Master_Process();
    
    if (useConnConf) {
      HID_StateMachine();
    }
  
#ifndef PTS_AUTOMATING    
    Enter_Sleep_Mode();
#endif
    
    /* if the profiles have nothing more to process, then
     * wait for application input
     */ 
    {      
      if (__io_getcharNonBlocking(&input))
      {
        printf("io--- input: %c\n",input); 
	runProcedure(input);
      }
    }
  }
}


/***************  Public Function callback ******************/

void HID_DeviceDiscovery_CB(uint8_t status, uint8_t addr_type, uint8_t *addr, 
			    uint8_t data_length, uint8_t *data, 
			    uint8_t RSSI)
{
  uint8_t addr_to_find[6]=PTS_PERIPHERAL_ADDRESS;
  
  if (memcmp(addr, addr_to_find, 6) == 0) {
    printf("****************************************\r\n");
    printf("*\r\n");
    printf("**** HID Peripheral Device Found\r\n");
    printf("*\r\n");
    printf("****************************************\r\n");
  }
}

void HID_ConnectionStatus_CB(uint8_t connection_evt, uint8_t status)
{
  printf("****************************************\r\n");
  switch(connection_evt) {
  case CONNECTION_ESTABLISHED_EVT:
    if (status == BLE_STATUS_SUCCESS) {
      printf("****  Connection Established with Success\r\n");
    }
    break;
  case CONNECTION_FAILED_EVT:
    printf("****  Connection Failed with Status = 0x%02x\r\n", status);
    break;
  case DISCONNECTION_EVT:
    printf("****  Disconnection with peer device\r\n");
    break;
  }
  printf("****************************************\r\n");
}

void HID_ConnectionParameterUpdateReq_CB(hidConnUpdateParamType *param)
{
  printf("**** HID_ConnectionParameterUpdateReq() received\r\n");
}

void HID_PinCodeRequired_CB(void)
{
  uint8_t ret;
  uint32_t pinCode=0;

  printf("**** Required Security Pin Code\r\n");

  /* Insert the pin code */
  pinCode = HID_FIXED_PIN;

  ret = HID_SendPinCode(pinCode);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("**** Error during the Pairing procedure -> Pin Code set (Status = 0x%x)\r\n", ret);
  }
}

void HID_PairingFailed_CB(void)
{
  printf("**** Error in connection during pairing procedure\r\n");
  printf("**** Repeat all the pairing procedure on the HID Device\r\n");
}

void HID_FullConfError_CB(uint8_t error_type, uint8_t code)
{
  if (useConnConf) {
    printf("***** HID_FullConf_CB() Error Type = 0x%0x with Code = 0x%0x\r\n", error_type, code);
  }
}

void HID_ServicesDiscovery_CB(uint8_t status, uint8_t numServices, uint8_t *services)
{
  printf("****************************************\r\n");
  printf("**** Primary Service Discovery Procedure\r\n");
  printf("****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugServices(numServices, services);
  printf("****************************************\r\n");  
}

void HID_IncludedServices_CB(uint8_t status, uint8_t numIncludedServices, uint8_t *includedServices)
{
  uint8_t i, index;
  uint16_t uuid, handle;

  printf("****************************************\r\n");
  printf("**** Included Service Discovery Procedure\r\n");
  printf("****    Number of Included services %d\r\n", numIncludedServices);
  printf("****    Status = 0x%02x\r\n", status);  
  if (status == BLE_STATUS_SUCCESS) {
    index = 0;
    for (i=0; i<numIncludedServices; i++) {
      if (includedServices[index] == 8) {
	memcpy(&uuid, &includedServices[index+7], 2);
#ifdef PTS_AUTOMATING
        printf("(%c%c4)", PTS_CODE, HID_PTS_CODE);
#endif                 
	if (uuid == BATTERY_SERVICE_SERVICE_UUID) {
	  printf("****    Included Battery Service discovered\n");
	  memcpy(&handle, &includedServices[index+1], 2);
	  printf("****      Characeristic handle  0x%04x\r\n", handle);
	  memcpy(&handle, &includedServices[index+3], 2);
	  printf("****      Start Included handle  0x%02x\r\n", handle);
	  memcpy(&handle, &includedServices[index+5], 2);
	  printf("****      End Included handle  0x%02x\r\n", handle);
	  printf("****      UUID 0x%02x\r\n", uuid);
	} 
	index += 9;
      } else {
	index += 7;
      }
    }
  }
  printf("****************************************\r\n");  
}

void HID_CharacOfService_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  printf("****************************************\r\n");
  printf("**** Discovery Characterisitc of a Service Procedure\r\n");
  printf("****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharac(numCharac, charac);
  printf("****************************************\r\n");  
}

void HID_CharacDesc_CB(uint8_t status, uint8_t numCharac, uint8_t *charac)
{
  printf("****************************************\r\n");
  printf("**** Discovery Characterisitc Descriptor Procedure\r\n");
  printf("****    Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS)
    printDebugCharacDesc(numCharac, charac);
  printf("****************************************\r\n");    
}

void HID_ReadReportDescriptor_CB(uint8_t status)
{
  uint16_t i;
  static uint16_t offset=0;
  static uint8_t alreadyRead=0;

  printf("****************************************\r\n"); 
  printf("****  Read Report Descriptor End with Status 0x%02x\r\n", status);
  printf("****    Number of Report Descriptor Present %d\r\n", numReportDescPresent);
#ifdef PTS_AUTOMATING 
      printf("(%c%c15)", PTS_CODE, HID_PTS_CODE);  
#endif   
  printf("****      Report Descriptor %d with Length %d\r\n", alreadyRead+1, reportDescLen[alreadyRead]);
  printf("****      Data = 0x");
  for (i=offset; i<(offset+reportDescLen[alreadyRead]); i++) {
    printf("%02X", reportData[i]);
  }
  printf("\r\n");
  offset += reportDescLen[alreadyRead];
  alreadyRead++;
  if (alreadyRead == numReportDescPresent) {
    offset = 0;
    alreadyRead = 0;
  }
  printf("****************************************\r\n"); 
}

void HID_DataValueRead_CB(uint8_t status, uint16_t data_len, uint8_t *data)
{
  uint8_t i;

  printf("****************************************\r\n");
  printf("**** Data Read Value Procedure\r\n");
  printf("****   Status = 0x%02x\r\n", status);
  if (status == BLE_STATUS_SUCCESS) {
    printf("****    Len data Read = %d\r\n", data_len);
    
#ifdef PTS_AUTOMATING
      printf("(%c%c16)", PTS_CODE, HID_PTS_CODE);
      printf("****    Data = 0x");
      for (i=data_len; i>0; i--) {
        printf("%02X", data[i-1]);
      }
      printf("\n(%c%c17)", PTS_CODE, HID_PTS_CODE);
      printf("****    Data = 0x");
      for (i=0; i<data_len; i++) {
        printf("%02X", data[i]);
      }      
      if (data_len == 2) {  
        printf("\n(%c%c18)", PTS_CODE, HID_PTS_CODE);
        printf("****    Data_1 = 0x%02X, Data_2 = %02X", data[0], data[1]);
      }  
#else   
    printf("****    Data = ");  
    for (i=0; i<data_len; i++) {
      printf("0x%02X ", data[i]);
    }
#endif   
    printf("\n");
//    printf("\r\n");
  }
  printf("****************************************\r\n");
}

void HID_InformationData_CB(uint8_t status, uint16_t version, 
                            uint8_t countryCode, uint8_t remoteWake, 
                            uint8_t normallyConnectable)
{
  printf("****************************************\r\n");
  printf("**** HID Information data read:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****   Version = 0x%02x\r\n", version);
  printf("****   Country Code = 0x%02x\r\n", countryCode);
  printf("****   Remote Wake = 0x%02x\r\n", remoteWake);
  printf("****   Normally Connectable = 0x%02x\r\n", normallyConnectable);
  printf("****************************************\r\n");
}

void HID_BetteryLevelData_CB(uint8_t status, uint8_t namespace, uint16_t description, uint8_t level)
{
  printf("****************************************\r\n");
  printf("**** Battery Level data read:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****   Namespace = 0x%02x\r\n", namespace);
  printf("****   description = 0x%04x\r\n", description);
#ifdef PTS_AUTOMATING
  printf("(%c%c20)", PTS_CODE, HID_PTS_CODE);
#endif      
  printf("****   Level = %d (0x%02x)\r\n", level, level);
  printf("****************************************\r\n");
}

void HID_BatteryClientCahracDesc_CB(uint8_t status, uint8_t notification, uint8_t indication)
{
  printf("****************************************\r\n");

  printf("**** Battery Level Client Characteristic Descriptor data read:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****   Notification = 0x%02x\r\n", notification);
  printf("****   Indication = 0x%02x\r\n", indication);
  printf("****************************************\r\n");
}

void HID_PnPID_CB(uint8_t status, uint8_t vendorIdSource, uint16_t vendorId, uint16_t productId, uint16_t productVersion)
{
  printf("****************************************\r\n");
#ifdef PTS_AUTOMATING
  printf("(%c%c21)", PTS_CODE, HID_PTS_CODE);
#endif 
  printf("**** PnP ID Characteristic data read:\n");
  printf("****   Status = 0x%02X\r\n", status);
  printf("****   Vendor ID Source = 0x%02X\r\n", vendorIdSource);
  printf("****   Vendor ID = 0x%04X\r\n", vendorId);
  printf("****   Product ID = 0x%04X\r\n", productId);
  printf("****   Product Version = 0x%04X\r\n", productVersion);
  printf("****************************************\r\n");
}

void HID_BootReportValue_CB(uint8_t status, uint8_t dataLen, uint8_t *data)
{
//  uint8_t i;
 
  printf("****************************************\r\n");
  printf("**** Boot Report data read:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****   Data Length = 0x%02x\r\n", dataLen);
  
#ifdef PTS_AUTOMATING
  printf("(%c%c26)", PTS_CODE, HID_PTS_CODE);
  printf("****   Data = 0x");
  uint16_t LE_data;
  
  if (dataLen == 2)
  {
    LE_data = LE_TO_HOST_16(data); 
    printf("%04X", LE_data);
  }  
  else if (dataLen == 4)
  {
    uint16_t LE_data2;
    LE_data = LE_TO_HOST_16(&data[0]); 
    LE_data2 = LE_TO_HOST_16(&data[2]); 
    printf("%04X %04X", LE_data, LE_data2);
  }  
#else   
  printf("****   Data = 0x");
  for (uint8_t i=0; i<dataLen; i++)
    printf("%02x", data[i]);  
 #endif
  
  printf("\n");
//  printf("\r\n");
  printf("****************************************\r\n");
}

void HID_ReadBootReportClientCharacDesc_CB(uint8_t status, uint8_t notification, uint8_t indication)
{
  printf("****************************************\r\n");
  printf("**** Boot Report Client Characteristic Descriptor data read:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****   Notification = 0x%02x\r\n", notification);
  printf("****   Indication = 0x%02x\r\n", indication);
  printf("****************************************\r\n");
}

void HID_ProtocolMode_CB(uint8_t status, uint8_t mode)
{
  printf("****************************************\r\n");
  printf("**** Protocol Mode data read:\n");
  printf("****   Status = 0x%02x\r\n", status);
#ifdef PTS_AUTOMATING
  printf("(%c%c22)", PTS_CODE, HID_PTS_CODE);
#endif   
  printf("****   Mode = 0x%02x\r\n", mode);
  printf("****************************************\r\n");
}

void HID_SetProcedure_CB(uint8_t status)
{
  printf("****************************************\r\n");
  printf("**** Set Procedure ended:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****************************************\r\n");  
}

void HID_NotificationChageStatus_CB(uint8_t status)
{
  printf("****************************************\r\n");
  printf("**** Notification Change status procedure ended:\n");
  printf("****   Status = 0x%02x\r\n", status);
  printf("****************************************\r\n");  
}

void HID_ReportDataReceived_CB(uint8_t type, uint8_t id, uint8_t data_length, uint8_t *data_value)
{
//  uint8_t i;

  printf("****************************************\r\n");
  printf("**** Notification data received:\n");
  printf("****   Report Type = 0x%02x\r\n", type);
  printf("****   Report ID = 0x%02x\r\n", id);
  printf("****   Report Length = 0x%02x\r\n", data_length);
  
#ifdef PTS_AUTOMATING
  uint16_t LE_data;  
  uint16_t LE_data2;
  
  if (data_length == 2) {  
    printf("(%c%c29)", PTS_CODE, HID_PTS_CODE);
    printf("****   Report Data = 0x");  
    LE_data = LE_TO_HOST_16(&data_value[0]); 
    printf("%04X\n", LE_data);
    printf("(%c%c30)", PTS_CODE, HID_PTS_CODE);
    printf("****   Report Data = 0x");  
    printf("%02X%02X\n", data_value[0], data_value[1]);
  }
  else if (data_length == 4) {  
    printf("(%c%c31)", PTS_CODE, HID_PTS_CODE);
    printf("****   Report Data = 0x");  
    LE_data = LE_TO_HOST_16(&data_value[0]); 
    LE_data2 = LE_TO_HOST_16(&data_value[2]); 
    printf("%04X %04X\n", LE_data, LE_data2);
  }
#else 
  printf("****   Report Data = 0x");
  for (uint8_t i=0; i<data_length; i++) {
    printf("%02x", data_value[i]);
  }
  printf("\r\n");  
  
#endif
  printf("****************************************\r\n");  
}

#endif //BLE_CURRENT_PROFILE_ROLES & HID_HOST

/** \endcond 
 */
