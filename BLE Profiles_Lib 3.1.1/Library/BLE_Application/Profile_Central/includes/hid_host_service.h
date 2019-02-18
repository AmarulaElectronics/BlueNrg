/******************** (C) COPYRIGHT 2016 STMicroelectronics ********************
* File Name          : hid_host_service.h
* Author             : RF Application Team
* Version            : V1.0.0
* Date               : 27/09/2016
* Description        : Header for master.c module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HID_HOST_SERVICE_H
#define __HID_HOST_SERVICE_H

/* Defines -------------------------------------------------------------------*/


/***************** SERVICE AND ATTRIBUTE CONSTANT ************************/
/**
 * @brief Max number of primary service supported. 
 * - 1 HID Service
 * - 1 Battery Service
 * - 1 Device Information Service
 * - 1 Scan Parameter Service
 * - 1 Other service 
 */
#define NUM_MAX_HID_SERVICES 5 

/**   
 * @brief Max number of HID  characteristics supported for each service
 */
#define NUM_MAX_HID_CHARAC_REPORT 7

/**
 * @brief Max number of device information characteristics
 * supported.
 * @note If this value is set to 9 all the devices information
 * are supported
 */
#define NUM_MAX_DEV_INF_CHARAC  9 


/* HID Report Type */

/**
 * @brief Input report type 
 */
#define INPUT_REPORT 0x01

/**
 * @brief Output report type 
 */
#define OUTPUT_REPORT 0x02

/**
 * @brief Boot Keyboard input report type
 */
#define BOOT_KEYBOARD_INPUT_REPORT  0x03

/**
 * @brief Boot Keyboard output report type
 */
#define BOOT_KEYBOARD_OUTPUT_REPORT 0x04

/**
 * @brief Boot Mouse input report type
 */
#define BOOT_MOUSE_INPUT_REPORT     0x05

/**
 * @brief Feature report type 
 */
#define FEATURE_REPORT 0x03

/* HID Protocolo Mode */

/**
 * @brief Boot Protocol Mode
 */
#define BOOT_PROTOCOL_MODE 0x00

/**
 * @brief Report Protocol Mode
 */
#define REPORT_PROTOCOL_MODE 0x01

/******************************************************************************
* Type definitions
******************************************************************************/

/******************************************************************************
* Function Declarations
******************************************************************************/

#endif /* _HID_HOST_SERVICE_H_ */

