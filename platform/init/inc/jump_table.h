#ifndef JUMP_TABLE_H__
#define JUMP_TABLE_H__

#include "global.h"

#define JUMP_TABLE_SIZE    (88)

extern const uint32_t * const jump_table_base[JUMP_TABLE_SIZE] __attribute__((section("jump_table_mem_area")));
#define jump_table_struct (uint32_t)jump_table_base

/// @} BT Stack Configuration
/// @} ROOT

#define JT_POS_TASK_ID_APP                     0
#define JT_POS_FUNC_APP_MAIN                   1
#define JT_POS_FUNC_APPM_INIT                  2
#define JT_POS_FUNC_RF_INIT                    3
#define JT_POS_FUNC_RF_RESERVE_1               4
#define JT_POS_FUNC_RF_RESERVE_2               5
#define JT_POS_FUNC_EIF_GET                    6
#define JT_POS_FUNC_PRF_INIT                   7
#define JT_POS_FUNC_PRF_CREATE                 8
#define JT_POS_FUNC_PRF_ADD_PROFILE            9
#define JT_POS_FUNC_PRF_CLEANUP                10
#define JT_POS_FUNC_PRF_GET_ID_FROM_TASK       11
#define JT_POS_FUNC_PRF_GET_TASK_FROM_ID       12
#define JT_POS_PARA_NVDS_ENABLE                13
#define JT_POS_NVDS_INIT                       14
#define JT_POS_NVDS_GET                        15
#define JT_POS_NVDS_PUT                        16
#define JT_POS_NVDS_DEL                        17
#define JT_POS_TASK_DESC_GAPC			   	   18
#define JT_POS_GAPC_SIG_HANDLERS			   19
#define JT_POS_TASK_DESC_GAPM				   20
#define JT_POS_GAPM_HCI_CMD_CMP_EVT_HANDLER_TAB 21
#define JT_POS_GAPM_HCI_CMD_STAT_EVT_HANDLER_TAB 22
#define JT_POS_HCI_LE_EVT_HANDLER_TAB			23
#define JT_POS_HCI_CMD_CMP_EVT_HANDLER_TAB		24
#define JT_POS_HCI_CMD_STAT_EVT_HANDLER_TAB		25
#define JT_POS_GAPM_HCI_LE_EVT_HANDLER_TAB		26
#define JT_POS_HCI_EVT_HANDLER_TAB				27
#define JT_POS_SMPC_CONSTRUCT_PDU				28
#define JT_POS_SMPC_RECV_PDU					29
#define JT_POS_TASK_DESC_GATTC					30
#define JT_POS_TASK_DESC_GATTM					31
#define JT_POS_ATTC_HANDLER						32
#define JT_POS_ATTS_HANDLER						33
#define JT_POS_L2CC_SIG_HANDLER					34
#define JT_POS_TASK_DESC_L2CC					35
#define JT_POS_LLM_HCI_CMD_HANDLER_TAB			36
#define JT_POS_LLC_HCI_CMD_HANDLER_TAB			37
#define JT_POS_TASK_DESC_LLC					38
#define JT_POS_TASK_DESC_LLM					39
#define JT_POS_DBG_HCI_CMD_HANDLER_TAB			40
#define JT_POS_TASK_DESC_DBG					41
#define JT_POS_LLD_SLEEP_COMPENSATE				42
#define JT_POS_LLD_SLEEP_INIT					43
#define JT_POS_LLD_SLEEP_US_2_LPCYCLES			44
#define JT_POS_LLD_SLEEP_LPCYCLES_2_US			45


#endif

