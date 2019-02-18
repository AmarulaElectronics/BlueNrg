/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : glucose_collector_config.h
* Author             : AMS - VMA, RF Application Team
* Version            : V1.0.0
* Date               : 19-January-2015
* Description        : Glucose Collector Central Configuration parameters
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLUCOSE_COLLECTOR_CONFIG_H
#define __GLUCOSE_COLLECTOR_CONFIG_H

/******************** PTS PERIPHERAL ADDRESS *******************/

#define PTS_PERIPHERAL_ADDRESS {0x97, 0x2F, 0x07, 0xdc, 0x1b, 0x00}  //{0x44, 0x06, 0x06, 0xDC, 0x1B, 0x00}  //{0x00, 0x1B, 0xDC, 0x06, 0x06, 0x44} 

/* Expected Peer Address if any */
#define PEER_ADDRESS {0x23, 0x02, 0x00, 0xE1, 0x80, 0x02} //TBR

/******************** Device Init Parameters *******************/

#define GLC_PUBLIC_ADDRESS          {0x22, 0x02, 0x00, 0xE1, 0x80, 0x02}
#define GLC_DEVICE_NAME             {'G', 'L', 'P', '_', 'C'}
#define GLC_TX_POWER_LEVEL          4

/******************** Device Security Parameters *******************/   

#define GLC_IO_CAPABILITY    IO_CAP_KEYBOARD_DISPLAY
#define GLC_MITM_MODE        MITM_PROTECTION_REQUIRED
#define GLC_OOB_ENABLE       OOB_AUTH_DATA_ABSENT
#define GLC_USE_FIXED_PIN    USE_FIXED_PIN_FOR_PAIRING
#define GLC_FIXED_PIN        111111 
#define GLC_BONDING_MODE     BONDING
#define GLC_SC_SUPPORT                          SC_IS_SUPPORTED   
#define GLC_KEYPRESS_NOTIFICATION_SUPPORT       KEYPRESS_IS_NOT_SUPPORTED
#define GLC_IDENTITY_ADDRESS_TYPE               0x00   
#define MIN_KEY_SIZE                            0x07   
#define MAX_KEY_SIZE                            0x10

/******************** Device Discovery Procedure Parameters *******************/
   
#define GLC_LIM_DISC_SCAN_INT  0x30 // 30 ms 
#define GLC_LIM_DISC_SCAN_WIND 0x30 // 30 ms 


/******************** Device Connection Parameters *******************/

#define GLC_FAST_SCAN_DURATION 30000           // 30 sec
#define GLC_FAST_SCAN_INTERVAL 48              // 30 ms
#define GLC_FAST_SCAN_WINDOW   48              // 30 ms
#define GLC_REDUCED_POWER_SCAN_INTERVAL 4096   // 2.56 sec
#define GLC_REDUCED_POWER_SCAN_WINDOW   18     // 11.25 ms
#define GLC_FAST_MIN_CONNECTION_INTERVAL 40    // 50 ms
#define GLC_FAST_MAX_CONNECTION_INTERVAL 56    // 70 ms
#define GLC_FAST_CONNECTION_LATENCY 0
#define GLC_SUPERVISION_TIMEOUT 20             // 200 ms
#define GLC_MIN_CONN_LENGTH 5                  // 3.125 ms
#define GLC_MAX_CONN_LENGTH 5                  // 3.125 ms


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

#endif /*__GLUCOSE_COLLECTOR_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/