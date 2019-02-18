/******************** (C) COPYRIGHT 2016 STMicroelectronics ********************
* File Name          : hid_host_config.h
* Author             : RF Application Team
* Version            : V1.0.0
* Date               : 26/09/2016
* Description        : HID Host Central Role Configuration parameters
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HID_HOST_CONFIG_H
#define __HID_HOST_CONFIG_H

/******************** PTS PERIPHERAL ADDRESS *******************/
#define PTS_PERIPHERAL_ADDRESS  {0x97, 0x2F, 0x07, 0xdc, 0x1b, 0x00}   //{0x44, 0x06, 0x06, 0xdc, 0x1b, 0x00} 

/******************** Device Init Parameters *******************/

#define HID_PUBLIC_ADDRESS          {0x22, 0x02, 0x00, 0xE1, 0x80, 0x02}  //{0x88, 0x55, 0x44, 0x33, 0x22, 0x11}
#define HID_TX_POWER_LEVEL          4
#define HID_HOST_MODE               REPORT_PROTOCOL_MODE // BOOT_PROTOCOL_MODE
#define HID_DEVICE_NAME             {'H', 'I', 'D', ' ', 'H', 'o', 's','t'}

/******************** Device Security Parameters *******************/   

#define HID_IO_CAPABILITY    IO_CAP_KEYBOARD_DISPLAY
#define HID_MITM_MODE        MITM_PROTECTION_REQUIRED
#define HID_OOB_ENABLE       OOB_AUTH_DATA_ABSENT
#define HID_USE_FIXED_PIN    USE_FIXED_PIN_FOR_PAIRING
#define HID_FIXED_PIN        111111
#define HID_BONDING_MODE     BONDING
#define HID_SC_SUPPORT                          SC_IS_SUPPORTED   
#define HID_KEYPRESS_NOTIFICATION_SUPPORT       KEYPRESS_IS_NOT_SUPPORTED
#define HID_IDENTITY_ADDRESS_TYPE               0x00  
#define MIN_KEY_SIZE                            0x07   
#define MAX_KEY_SIZE                            0x10

/******************** Device Discovery Procedure Parameters *******************/
   
#define HID_LIM_DISC_SCAN_INT  0x30 // 30 ms 
#define HID_LIM_DISC_SCAN_WIND 0x30 // 30 ms 


/******************** Device Connection Parameters *******************/

#define HID_FAST_SCAN_DURATION 180000          // 180 sec
#define HID_FAST_SCAN_INTERVAL 36              // 22.5 ms
#define HID_FAST_SCAN_WINDOW   18              // 11.25 ms
#define HID_REDUCED_POWER_SCAN_INTERVAL 1600   // 1 sec
#define HID_REDUCED_POWER_SCAN_WINDOW   18     // 11.25 ms
#define HID_FAST_MIN_CONNECTION_INTERVAL 6     // 7.5 ms
#define HID_FAST_MAX_CONNECTION_INTERVAL 40    // 50 ms
#define HID_FAST_CONNECTION_LATENCY 0
#define HID_SUPERVISION_TIMEOUT 100            // 1 sec
#define HID_MIN_CONN_LENGTH 5                  // 3.125 ms
#define HID_MAX_CONN_LENGTH 5                  // 3.125 ms

#endif /*__HID_HOST_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
