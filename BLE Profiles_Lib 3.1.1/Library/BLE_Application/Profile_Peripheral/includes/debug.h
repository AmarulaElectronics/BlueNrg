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
*   FILENAME        -  debug.h
*
*   COMPILER        -  EWARM 
*
*******************************************************************************
*
*   CHANGE HISTORY
*   ---------------------------
*
*   $Date$:      09/03/2016
*   $Revision$:  first version
*   $Author$:    
*   Comments:    debug macro Declarations
*
*******************************************************************************
*
*  File Description 
*  ---------------------
* This file implements the debug and utility functions used for debugging
*
*******************************************************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <host_config.h>
 
/******************************************************************************
 * Feature Flags
******************************************************************************/
//#define DEBUG 1

#ifdef DEBUG
#define DEBUG_HCI 
#define DEBUG_TIMER 
#define DEBUG_PROFILE 
#define DEBUG_HRPROFILE
#define DEBUG_PROXIMITY_REPORTER
#define DEBUG_PROXIMITY_MONITOR
#define DEBUG_TIME_SERVER
#define DEBUG_TIME_CLIENT
#define DEBUG_FIND_ME
#define DEBUG_APPLICATION
#define DEBUG_PHONE_ALERT
#define DEBUG_BLOOD_PRESSURE_SENSOR
#define DEBUG_HEALTH_THERMOMETER
#define DEBUG_ALERT_NOTIFICATION_CLIENT
#define DEBUG_ALERT_NOTIFICATION_SERVER
#define DEBUG_GLUCOSE_SENSOR
#define DEBUG_CGM_SENSOR


#endif
/******************************************************************************
 * Macros
 *****************************************************************************/
/**
 * Debug Macros
 */
#if (PLATFORM_TYPE == PLATFORM_ARM)
#include <stdio.h>
#define DBG_PRINT(format,...)           printf (format ,##__VA_ARGS__);
#define DBG_ASSERT(expression) 
#define PRINT_MESG_DBG(file,format,...)   printf (format ,##__VA_ARGS__);
#define PRINT_MESG_DATA(file,dptr,dlen)  \
    do {  \
        int i=0;  \
        for (i=0;i<dlen;i++)  \
        {  \
            if (i%16 == 0)  \
            {  \
                printf ("\n[%s]:[%d]:[--DATA--]: ", __FILE__, __LINE__);  \
            }  \
            printf ("%02X ", dptr[i]);  \
        }  \
        printf ("\n ");  \
    }while(0) 

#define PRINT_MESG_INFO(file,format,...)  printf (format ,##__VA_ARGS__);
#define PRINT_MESG_ERR(file,format,...)  printf (format ,##__VA_ARGS__);


#define PRINT_NO_MESG(file,format,...)
        
#else
#define DBG_PRINT(format,...) 
#define DBG_ASSERT(expression) 
#define PRINT_MESG_DBG(file,format,...)  
#define PRINT_MESG_DATA(file,dptr,dlen)  
#define PRINT_MESG_INFO(file,format,...)  
#define PRINT_MESG_ERR(file,format,...)  
#define PRINT_NO_MESG(file,format,...)
#endif


/*
 * debug messages: XXXX_MESG_DBG()
 *                 print only if debug flag for corresponding module is set, 
 * 
 * Error messages: XXXX_MESG_ERR()
 *                 print always to log any type of error in execution, 
 *                 these errors are error in control execution, but not the 
 *                 s/w logic or implementation error. 
 * 
 * HEX Data dump: XXXX_MESG_DATA()
 *                print messages for hex data dump must be a debug messages, 
 *                as data dump is a bulkey process and not required always for
 *                info or error, its required only for debugging purpose. 
 */
      
//#define PTS_AUTOMATING   1    //PTS Automating
#ifdef PTS_AUTOMATING      
#define PTS_CODE        '@'  //to identy a message to parse for PTS automating tests
#else
#define PTS_CODE        ''     
#endif
      
#ifdef DEBUG_HCI
#define HCI_MESG_DBG  PRINT_MESG_DBG
#define HCI_MESG_DATA  PRINT_MESG_DATA
#else
#define HCI_MESG_DBG  PRINT_NO_MESG
#define HCI_MESG_DATA  PRINT_NO_MESG
#endif

#ifdef DEBUG_TIMER
#define TIMER_MESG_DBG  PRINT_MESG_DBG
#define TIMER_MESG_DATA  PRINT_MESG_DATA
#else
#define TIMER_MESG_DBG  PRINT_NO_MESG
#define TIMER_MESG_DATA  PRINT_NO_MESG
#endif

#ifdef DEBUG_PROFILE
#define PROFILE_MESG_DBG  PRINT_MESG_DBG
#else
#define PROFILE_MESG_DBG  PRINT_NO_MESG
#endif
#define PROFILE_MESG_ERR  PRINT_MESG_ERR
      
#ifdef DEBUG_HRPROFILE
#define HRS_PTS_CODE        'a'   //concatenated to PTS_CODE, this identifies an HRS message to parse for PTS automating tests 
#define HRPROFILE_MESG_DBG  PRINT_MESG_DBG
#define HRPROFILE_MESG_ERR  PRINT_MESG_ERR
#else
#define HRPROFILE_MESG_DBG  PRINT_NO_MESG
#define HRPROFILE_MESG_ERR  PRINT_NO_MESG
#endif

#ifdef DEBUG_PROXIMITY_REPORTER
#define PROXIMITY_REPORTER_MESG_DBG  PRINT_MESG_DBG
#else
#define PROXIMITY_REPORTER_MESG_DBG  PRINT_NO_MESG
#endif
#define PROXIMITY_REPORTER_MESG_ERR  PRINT_MESG_ERR

#ifdef DEBUG_PROXIMITY_MONITOR
#define PXP_PTS_CODE        'l'   //concatenated to PTS_CODE, this identifies an PXP message to parse for PTS automating tests       
#define PROXIMITY_MONITOR_MESG_DBG  PRINT_MESG_DBG
#define PROXIMITY_MONITOR_MESG_DATA  PRINT_MESG_DATA
#else
#define PROXIMITY_MONITOR_MESG_DBG  PRINT_NO_MESG
#define PROXIMITY_MONITOR_MESG_DATA  PRINT_NO_MESG
#endif
#define PROXIMITY_MONITOR_MESG_ERR  PRINT_MESG_ERR

#ifdef DEBUG_TIME_SERVER
#define TIS_PTS_CODE        'm'   //concatenated to PTS_CODE, this identifies an TIS message to parse for PTS automating tests       
#define TIME_SERVER_MESG_DBG  PRINT_MESG_DBG
#define TIME_SERVER_MESG_DATA  PRINT_MESG_DATA
#else
#define TIME_SERVER_MESG_DBG  PRINT_NO_MESG
#define TIME_SERVER_MESG_DATA  PRINT_NO_MESG
#endif
#define TIME_SERVER_MESG_ERR  PRINT_MESG_ERR
      
#ifdef DEBUG_TIME_CLIENT
#define TIP_PTS_CODE        'b'   //concatenated to PTS_CODE, this identifies an TIP message to parse for PTS automating tests       
#define TIME_CLIENT_MESG_DBG  PRINT_MESG_DBG
#define TIME_CLIENT_MESG_DATA  PRINT_MESG_DATA
#else
#define TIME_CLIENT_MESG_DBG  PRINT_NO_MESG
#define TIME_CLIENT_MESG_DATA  PRINT_NO_MESG
#endif
#define TIME_CLIENT_MESG_ERR  PRINT_MESG_ERR

#ifdef DEBUG_FIND_ME
#define FMT_PTS_CODE        'g'   //concatenated to PTS_CODE, this identifies an FMT message to parse for PTS automating tests       
#define FML_PTS_CODE        'h'   //concatenated to PTS_CODE, this identifies an FML message to parse for PTS automating tests       
#define FMT_DBG_MSG   PRINT_MESG_DBG
#else
#define FMT_DBG_MSG   PRINT_NO_MESG
#endif

#ifdef DEBUG_PHONE_ALERT
#define PAC_PTS_CODE        'i'   //concatenated to PTS_CODE, this identifies an PAC message to parse for PTS automating tests       
#define PAC_DBG_MSG   PRINT_MESG_DBG
#else
#define PAC_DBG_MSG   PRINT_NO_MESG
#endif

#ifdef DEBUG_BLOOD_PRESSURE_SENSOR
#define BPS_PTS_CODE        'd'   //concatenated to PTS_CODE, this identifies an BPS message to parse for PTS automating tests       
#define BPS_DBG_MSG   PRINT_MESG_DBG
#else
#define BPS_DBG_MSG   PRINT_NO_MESG
#endif

#ifdef DEBUG_HEALTH_THERMOMETER
#define HTS_PTS_CODE        'e'   //concatenated to PTS_CODE, this identifies an HTS message to parse for PTS automating tests       
#define HT_DBG_MSG PRINT_MESG_DBG
#else
#define HT_DBG_MSG PRINT_NO_MESG
#endif

#ifdef DEBUG_ALERT_NOTIFICATION_CLIENT
#define ANC_PTS_CODE        'f'   //concatenated to PTS_CODE, this identifies an HTS message to parse for PTS automating tests       
#define ANC_DBG_MSG PRINT_MESG_DBG
#define ANC_DBG_DATA PRINT_MESG_DATA
#else
#define ANC_DBG_MSG PRINT_NO_MESG
#define ANC_DBG_DATA PRINT_NO_MESG
#endif

#ifdef DEBUG_ALERT_NOTIFICATION_SERVER
#define ANS_DBG_MSG PRINT_MESG_DBG
#define ANS_DBG_DATA PRINT_MESG_DATA
#else
#define ANS_DBG_MSG PRINT_NO_MESG
#define ANS_DBG_DATA PRINT_NO_MESG
#endif

#ifdef DEBUG_GLUCOSE_SENSOR
#define GLS_PTS_CODE        'c'   //concatenated to PTS_CODE, this identifies an GLS message to parse for PTS automating tests       
#define GL_DBG_MSG PRINT_MESG_DBG
#else
#define GL_DBG_MSG PRINT_NO_MESG
#endif

#ifdef DEBUG_CGM_SENSOR
//#define CGMS_PTS_CODE        'c'   //concatenated to PTS_CODE, this identifies an GLS message to parse for PTS automating tests       
#define CGM_DBG_MSG PRINT_MESG_DBG
#else
#define CGM_DBG_MSG PRINT_NO_MESG
#endif

#ifdef DEBUG_APPLICATION
#define APPL_MESG_DBG   PRINT_MESG_DBG
#define APPL_MESG_DATA  PRINT_MESG_DATA
#else
#define APPL_MESG_DBG  PRINT_NO_MESG
#define APPL_MESG_DATA  PRINT_NO_MESG
#endif


#endif /* _DEBUG_H_ */
