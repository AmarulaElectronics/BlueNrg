/******************************************************************************
*
* THE PRESENT CODE AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* (C) 2015 STMicroelectronics International NV
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
*   $Date$:      01/07/2015
*   $Revision$:  first version
*   $Author$:    
*   Comments:    debug macro Declarations x Profiles Central
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
#ifdef DEBUG 
//#define DEBUG_TIMER 
#define DEBUG_PROFILE 
#define DEBUG_HRPROFILE

#define PRINTF(...) printf(__VA_ARGS__)

#define DEBUG_APPLICATION
#define DEBUG_CALLBACKS //Central general callbacks 

/* Central devices */
#define DEBUG_GLUCOSE_COLLECTOR
#define DEBUG_PHONE_ALERT_STATUS_SERVER
#define DEBUG_HEART_RATE_COLLECTOR
#define DEBUG_HEALTH_THERMOMETER_COLLECTOR
#define DEBUG_FIND_ME_LOCATOR
#define DEBUG_FIND_ME_TARGET
#define DEBUG_ALERT_NOTIFICATION_SERVER 
#define DEBUG_BLOOD_PRESSURE_COLLECTOR
#define DEBUG_TIME_SERVER
#define DEBUG_ALERT_NOTIFICATION_CLIENT
#define DEBUG_TIME_CLIENT
#define DEBUG_PROXIMITY_REPORTER
#endif
/******************************************************************************
 * Macros
 *****************************************************************************/
/**
 * Debug Macros
 */
#if (PLATFORM_TYPE == PLATFORM_ARM)
#include <stdio.h>

#define PRINTF(...) printf(__VA_ARGS__)
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

#define  PROFILES_SIZE 0  /* Set this define to 1 only for evaluating profiles size on release configuration */ 
#if PROFILES_SIZE 
#define PRINT_MESG_INFO(file,format,...) 
#define PRINT_MESG_ERR(file,format,...)    
#else   
#define PRINT_MESG_INFO(file,format,...)  printf (format ,##__VA_ARGS__);
#define PRINT_MESG_ERR(file,format,...)  printf (format ,##__VA_ARGS__);
#endif 

#define PRINT_NO_MESG(file,format,...)
        
#else
#define PRINTF(...)
#define DBG_PRINT(format,...) 
#define DBG_ASSERT(expression) 
#define PRINT_MESG_DBG(file,format,...)  
#define PRINT_MESG_DATA(file,dptr,dlen)  
#define PRINT_MESG_INFO(file,format,...)  
#define PRINT_MESG_ERR(file,format,...)  
#define PRINT_NO_MESG(file,format,...)
#endif

//#define PTS_AUTOMATING  1    //PTS Automating 
      
#ifdef PTS_AUTOMATING      
#define PTS_CODE        '@'  //to identy a message to parse for PTS automating tests
#define HRP_PTS_CODE        'p'   //concatenated to PTS_CODE, this identifies an HRP message to parse for PTS automating tests       
#define GLP_PTS_CODE        'q'   //concatenated to PTS_CODE, this identifies an GLP message to parse for PTS automating tests       
#define BLP_PTS_CODE        'r'   //concatenated to PTS_CODE, this identifies an BLP message to parse for PTS automating tests       
#define HTP_PTS_CODE        's'   //concatenated to PTS_CODE, this identifies an HTP message to parse for PTS automating tests                   
#define ANS_PTS_CODE        't'   //concatenated to PTS_CODE, this identifies an ANS message to parse for PTS automating tests             
#define FML_PTS_CODE        'u'   //concatenated to PTS_CODE, this identifies an FML message to parse for PTS automating tests       
#define FMT_PTS_CODE        'v'   //concatenated to PTS_CODE, this identifies an FMT message to parse for PTS automating tests             
#define PASS_PTS_CODE       'z'   //concatenated to PTS_CODE, this identifies an PASS message to parse for PTS automating tests                   
#define PXP_PTS_CODE        'P'   //concatenated to PTS_CODE, this identifies an PXP message to parse for PTS automating tests                        
#define PXR_PTS_CODE        'Q'   //concatenated to PTS_CODE, this identifies an PXR message to parse for PTS automating tests  
#define TIS_PTS_CODE        'R'   //concatenated to PTS_CODE, this identifies an TIS message to parse for PTS automating tests     
#define HID_PTS_CODE        'S'   //concatenated to PTS_CODE, this identifies an TIS message to parse for PTS automating tests 
#else
#define PTS_CODE        ''     
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
      
#ifdef DEBUG_CALLBACKS
#define CALLBACKS_MESG_DBG  PRINT_MESG_DBG
#else
#define CALLBACKS_MESG_DBG  PRINT_NO_MESG 
#endif
      
#ifdef DEBUG_APPLICATION
#define APPL_MESG_DBG   PRINT_MESG_DBG
#define APPL_MESG_INFO  PRINT_MESG_INFO
#define APPL_MESG_DATA  PRINT_MESG_DATA
#else
#define APPL_MESG_DBG  PRINT_NO_MESG
#define APPL_MESG_INFO PRINT_MESG_INFO //TBR
#define APPL_MESG_DATA PRINT_NO_MESG
#endif    
      
#ifdef DEBUG_GLUCOSE_COLLECTOR
#define GL_DBG_MSG  PRINT_MESG_DBG
#define GL_INFO_MSG PRINT_MESG_INFO
#define GL_ERR_MSG  PRINT_MESG_ERR
#else
#define GL_DBG_MSG PRINT_NO_MESG
#define GL_INFO_MSG PRINT_MESG_INFO //TBR
#define GL_ERR_MSG  PRINT_NO_MESG
#endif

#ifdef DEBUG_PHONE_ALERT_STATUS_SERVER
#define PAS_DBG_MSG  PRINT_MESG_DBG
#define PAS_INFO_MSG PRINT_MESG_INFO
#define PAS_ERR_MSG  PRINT_MESG_ERR
#else
#define PAS_DBG_MSG  PRINT_NO_MESG
#define PAS_INFO_MSG PRINT_NO_MESG
#define PAS_ERR_MSG  PRINT_NO_MESG
#endif
      
#ifdef DEBUG_HEART_RATE_COLLECTOR 
#define HR_DBG_MSG PRINT_MESG_DBG 
#define HR_INFO_MSG PRINT_MESG_INFO 
#define HR_ERR_MSG PRINT_MESG_ERR 
#else 
#define HR_DBG_MSG PRINT_NO_MESG 
#define HR_INFO_MSG PRINT_MESG_INFO //TBR
#define HR_ERR_MSG PRINT_NO_MESG 
#endif
      
#ifdef DEBUG_FIND_ME_LOCATOR
#define FML_DBG_MSG  PRINT_MESG_DBG
#define FML_INFO_MSG PRINT_MESG_INFO
#define FML_ERR_MSG  PRINT_MESG_ERR
#else
#define FML_DBG_MSG  PRINT_NO_MESG
#define FML_INFO_MSG PRINT_NO_MESG
#define FML_ERR_MSG  PRINT_NO_MESG
#endif
      
#ifdef DEBUG_FIND_ME_TARGET
#define FMT_DBG_MSG  PRINT_MESG_DBG
#define FMT_INFO_MSG PRINT_MESG_INFO
#define FMT_ERR_MSG  PRINT_MESG_ERR
#else
#define FMT_DBG_MSG  PRINT_NO_MESG
#define FMT_INFO_MSG PRINT_NO_MESG
#define FMT_ERR_MSG  PRINT_NO_MESG
#endif
      
#ifdef DEBUG_HEALTH_THERMOMETER_COLLECTOR 
#define HT_DBG_MSG PRINT_MESG_DBG 
#define HT_INFO_MSG PRINT_MESG_INFO 
#define HT_ERR_MSG PRINT_MESG_ERR 
#else 
#define HT_DBG_MSG PRINT_NO_MESG 
#define HT_INFO_MSG PRINT_MESG_INFO //TBR
#define HT_ERR_MSG PRINT_NO_MESG 
#endif
      
#ifdef DEBUG_ALERT_NOTIFICATION_SERVER 
#define ANS_DBG_MSG PRINT_MESG_DBG 
#define ANS_INFO_MSG PRINT_MESG_INFO 
#define ANS_ERR_MSG PRINT_MESG_ERR 
#define ANS_DBG_DATA PRINT_MESG_DATA
#else 
#define ANS_DBG_MSG PRINT_NO_MESG 
#define ANS_INFO_MSG PRINT_MESG_INFO //TBR
#define ANS_ERR_MSG PRINT_NO_MESG 
#define ANS_DBG_DATA PRINT_NO_MESG
#endif

#ifdef DEBUG_BLOOD_PRESSURE_COLLECTOR 
#define BP_DBG_MSG PRINT_MESG_DBG 
#define BP_INFO_MSG PRINT_MESG_INFO 
#define BP_ERR_MSG PRINT_MESG_ERR 
#else 
#define BP_DBG_MSG PRINT_NO_MESG 
#define BP_INFO_MSG PRINT_MESG_INFO //TBR
#define BP_ERR_MSG PRINT_NO_MESG 
#endif
      
#ifdef DEBUG_TIME_SERVER
#define TIME_SERVER_MESG_DBG  PRINT_MESG_DBG
#define TIME_SERVER_MESG_INFO PRINT_MESG_INFO
#define TIME_SERVER_MESG_DATA PRINT_MESG_DATA
#define TIME_SERVER_MESG_ERR  PRINT_MESG_ERR
#else
#define TIME_SERVER_MESG_DBG  PRINT_NO_MESG
#define TIME_SERVER_MESG_INFO PRINT_MESG_INFO //TBR
#define TIME_SERVER_MESG_DATA PRINT_NO_MESG
#define TIME_SERVER_MESG_ERR  PRINT_NO_MESG
#endif
      
#ifdef DEBUG_ALERT_NOTIFICATION_CLIENT 
#define ANC_DBG_MSG PRINT_MESG_DBG 
#define ANC_INFO_MSG PRINT_MESG_INFO 
#define ANC_ERR_MSG PRINT_MESG_ERR 
#define ANC_DBG_DATA PRINT_MESG_DATA
#else 
#define ANC_DBG_MSG PRINT_NO_MESG 
#define ANC_INFO_MSG PRINT_MESG_INFO //TBR
#define ANC_ERR_MSG PRINT_NO_MESG 
#define ANC_DBG_DATA PRINT_NO_MESG
#endif   
   
#ifdef DEBUG_TIME_CLIENT
#define TIME_CLIENT_MESG_DBG  PRINT_MESG_DBG
#define TIME_CLIENT_MESG_INFO PRINT_MESG_INFO
#define TIME_CLIENT_MESG_DATA PRINT_MESG_DATA
#define TIME_CLIENT_MESG_ERR  PRINT_MESG_ERR
#else
#define TIME_CLIENT_MESG_DBG  PRINT_NO_MESG
#define TIME_CLIENT_MESG_INFO PRINT_MESG_INFO //TBR
#define TIME_CLIENT_MESG_DATA PRINT_NO_MESG
#define TIME_CLIENT_MESG_ERR  PRINT_NO_MESG
#endif

#ifdef DEBUG_PROXIMITY_REPORTER
#define PROXIMITY_REPORTER_MESG_DBG  PRINT_MESG_DBG
#define PROXIMITY_REPORTER_MESG_INFO  PRINT_MESG_INFO
#define PROXIMITY_REPORTER_MESG_ERR  PRINT_MESG_ERR
#else
#define PROXIMITY_REPORTER_MESG_DBG  PRINT_NO_MESG
#define PROXIMITY_REPORTER_MESG_INFO  PRINT_MESG_INFO //TBR
#define PROXIMITY_REPORTER_MESG_ERR  PRINT_MESG_ERR
#endif

#endif /* _DEBUG_H_ */
