

/**
  ******************************************************************************
  * @file    DTM_cmd_db.h
  * @author  RF Application Team - AMG
  * @version V1.0.0
  * @date    21-Sept-2015
  * @brief   Autogenerated files, do not edit!!
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef _DTM_CMD_DB_H_
#define _DTM_CMD_DB_H_

#include <stdint.h>
typedef uint16_t (*hci_command_process_and_response_type)(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length) ;
typedef struct hci_command_table_type_s {
  uint16_t opcode;
  hci_command_process_and_response_type execute;
} hci_command_table_type;

extern const hci_command_table_type hci_command_table[132];
extern void send_event(uint8_t *buffer_out, uint16_t buffer_out_length, int8_t overflow_index);
extern void send_event_isr(uint8_t *buffer_out, uint16_t buffer_out_length, int8_t overflow_index);
#endif /* _DTM_CMD_DB_H_ */
