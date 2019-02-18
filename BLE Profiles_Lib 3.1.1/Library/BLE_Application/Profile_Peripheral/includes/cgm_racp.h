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
*   FILENAME        -  cgm_racp.h
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
*   Comments:    cgm record access point handler for BlueNRG-1
*
*******************************************************************************
*
*  File Description 
*  ---------------------
*  header file for the cgm record access point handler
* 
******************************************************************************/


/******************************************************************************
 * Include Files
******************************************************************************/

#ifndef _CGM_RACP_H_
#define _CGM_RACP_H_

#include <host_config.h>
#include <hal_types.h>

#include <cgm_sensor.h>

/******************************************************************************
* Macro Declarations
******************************************************************************/

/******************************************************************************
* Type definitions
******************************************************************************/


/******************************************************************************
* Imported Variable
******************************************************************************/

/******************************************************************************
* Function Declarations
******************************************************************************/
/**
  * @brief  Reset all the not deleted records to  non filtered status
  *         before doing a new operation.
  * @param  [in] action : TRUE (reset flag); FALSE (set flag)
  * @retval number of not deleted records (TBR)
  */
uint16_t cgmDatabase_ResetAllNotDeletedRecords(BOOL action);

/**
  * @brief  Find next filtered record on cgm database
  * @param  [in] database_index : index from which starting the search
  * @retval database index
  */
uint16_t cgmDataBase_FindNextSelectedRecord(uint16_t database_index);

/**
  * @brief RACP request handler 
  * @param [in] attValueLen : received RACP request lenght
  * @retval none
  */
void cgmRACP_RequestHandler(uint8_t attValueLen);

/**
  * @brief Manage the notification of next cgm measurment from database records 
  *        It should be called periodically on notification timer timeout
  * @retval TRUE : if other selected measurement has to be sent; false otherwise
  */
BOOL cgmRACP_SendNextMeasurementRecord(void);

/* UTILITIES */

/**
  * @brief Send a RACP response as consequence of a RACP request
  * @param op_code:   RACP request op code  
  * @param response: RACP response op code or number of stored records
  * @retval none
  */
void cgmRACP_SendResponse(uint8_t op_code, uint16_t response_code);


#endif /* _CGM_RACP_H_ */
