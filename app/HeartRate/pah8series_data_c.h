#ifndef __PAH8SERIES_DATA_C_H__
#define __PAH8SERIES_DATA_C_H__

#include <stdint.h>

typedef struct {
    uint8_t                     frame_count;
    uint32_t                    time;
    uint8_t                     touch_flag;
    uint32_t                    nf_ppg_channel;
    uint32_t                    nf_ppg_per_channel;
    int32_t                     *ppg_data;
    uint32_t                    nf_mems;
    int16_t                     *mems_data;
} pah8series_data_t;

typedef enum {
    SENSOR_STATUS_UNRELIABLE = 0,
    SENSOR_STATUS_ACCURACY_LOW,
    SENSOR_STATUS_ACCURACY_MEDIUM,
    SENSOR_STATUS_ACCURACY_HIGH,
    SENSOR_STATUS_NO_CONTACT = (-1),
}sensor_status;

typedef enum {
    PAH8SERIES_PARAM_IDX_SAMPLE_RATE = 0,
    PAH8SERIES_PARAM_IDX_GSENSOR_MODE,
    PAH8SERIES_PARAM_IDX_PPG_CH_NUM,
    PAH8SERIES_PARAM_IDX_HAS_IR_CH,
    PAH8SERIES_PARAM_IDX_SIGNAL_GRADE,
    PAH8SERIES_PARAM_IDX_IIR_SMOOTH_PRED,
    PAH8SERIES_PARAM_IDX_IIR_LP,
    PAH8SERIES_PARAM_IDX_FIRST_HR_MULTIPLE_OFFSET_MEMS_THR,
    PAH8SERIES_PARAM_IDX_FIRST_HR_IN_MOTION_FLAG,
    PAH8SERIES_PARAM_IDX_FIRST_HR_MEMS_QUALITY_THR,
    PAH8SERIES_PARAM_IDX_FIRST_HR_PPG_QUALITY_THR,
    PAH8SERIES_PARAM_IDX_STATIC_MODE,
    PAH8SERIES_PARAM_IDX_STATIC_HR_UP_IDX,
    PAH8SERIES_PARAM_IDX_STATIC_HR_DN_IDX,
    PAH8SERIES_PARAM_IDX_IS_AUTO,
    PAH8SERIES_PARAM_IDX_IS_TAG,
    PAH8SERIES_PARAM_IDX_IS_8002,
    PAH8SERIES_PARAM_IDX_FIRST_HR_6SEC,
    PAH8SERIES_PARAM_IDX_HR_MODEL_TIME_CONST,
    PAH8SERIES_PARAM_IDX_HR_MODEL_PROGRESS_RATIO,
    PAH8SERIES_PARAM_IDX_INIT_WALK_HR,
    PAH8SERIES_PARAM_IDX_WALK_HR_UP_IDX,
    PAH8SERIES_PARAM_IDX_WALK_HR_DN_IDX,
    PAH8SERIES_PARAM_IDX_HR_TOO_LOW_IDX,
    PAH8SERIES_PARAM_IDX_EN_LWPS,
    PAH8SERIES_PARAM_IDX_EN_JUDGE_WALK,
    PAH8SERIES_PARAM_IDX_EN_CHK_HR_GT_MOTION,
    PAH8SERIES_PARAM_IDX_EN_HI_QUALITY_HR_JMP,
    PAH8SERIES_PARAM_IDX_EN_SPLIT_MOTION_PEAK,
    PAH8SERIES_NF_PARAM_IDX,
} pah8series_param_idx_t;

typedef enum {
    MSG_SUCCESS = 0,
    MSG_ALG_NOT_OPEN,
    MSG_ALG_REOPEN,
    MSG_MEMS_LEN_TOO_SHORT,
    MSG_NO_TOUCH,
    MSG_PPG_LEN_TOO_SHORT,
    MSG_FRAME_LOSS,
    MSG_INVALID_ARGUMENT,

    MSG_NO_MEM = 14,
    MSG_ECG_LEN_TOO_SHORT = 15,

    MSG_HR_READY = 0x30,
    MSG_SIGNAL_POOR = 0x40
} pah8series_msg_code_t;


#endif // PAH8002_DATA_H__
