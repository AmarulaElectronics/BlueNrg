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
*   FILENAME        -  cgm_socp.h
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
*   Comments:    cgm socp header file for BlueNRG-1,2
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

#ifndef _CGM_SOCP_H_
#define _CGM_SOCP_H_

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
  * @brief SOCP request handler 
  * @param [in] attValueLen : received RACP request lenght
  * @retval none
  */
void cgmSOCP_RequestHandler(uint8_t attValueLen);

/**
  * @brief Send a socp response as consequence of a SOCP request
  * @param op_code:   SOPC request op code  
  * @param response: SOCP response op code or number of stored records
  * @retval none
  */
void cgmSOCP_SendResponse(uint8_t response_opCode, uint8_t response_codeValue[]);


#endif /* _CGM_SOCP_H_ */
