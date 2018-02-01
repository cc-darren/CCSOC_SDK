#ifndef __PAH8002_API_C_H__
#define __PAH8002_API_C_H__

#include "pah8series_data_c.h"


#if defined(WIN32)
#   ifdef PXIALG_EXPORTS
#       define PXIALG_API   __declspec(dllexport)
#   else
#       define PXIALG_API   __declspec(dllimport)
#   endif
#else
#    define PXIALG_API
#endif    // WIN32


typedef pah8series_data_t       pah8002_data_t;
typedef pah8series_msg_code_t   pah8002_msg_code_t;


typedef enum {
    PAH8002_PARAM_IDX_SAMPLE_RATE                       = PAH8SERIES_PARAM_IDX_SAMPLE_RATE,
    PAH8002_PARAM_IDX_GSENSOR_MODE                      = PAH8SERIES_PARAM_IDX_GSENSOR_MODE,
    PAH8002_PARAM_IDX_PPG_CH_NUM                        = PAH8SERIES_PARAM_IDX_PPG_CH_NUM,
    PAH8002_PARAM_IDX_HAS_IR_CH                         = PAH8SERIES_PARAM_IDX_HAS_IR_CH,
    PAH8002_PARAM_IDX_SIGNAL_GRADE                      = PAH8SERIES_PARAM_IDX_SIGNAL_GRADE,
    PAH8002_PARAM_IDX_IIR_SMOOTH_PRED                   = PAH8SERIES_PARAM_IDX_IIR_SMOOTH_PRED,
    PAH8002_PARAM_IDX_IIR_LP                            = PAH8SERIES_PARAM_IDX_IIR_LP,
    PAH8002_PARAM_IDX_FIRST_HR_MULTIPLE_OFFSET_MEMS_THR = PAH8SERIES_PARAM_IDX_FIRST_HR_MULTIPLE_OFFSET_MEMS_THR,
    PAH8002_PARAM_IDX_FIRST_HR_IN_MOTION_FLAG           = PAH8SERIES_PARAM_IDX_FIRST_HR_IN_MOTION_FLAG,
    PAH8002_PARAM_IDX_FIRST_HR_MEMS_QUALITY_THR         = PAH8SERIES_PARAM_IDX_FIRST_HR_MEMS_QUALITY_THR,
    PAH8002_PARAM_IDX_FIRST_HR_PPG_QUALITY_THR          = PAH8SERIES_PARAM_IDX_FIRST_HR_PPG_QUALITY_THR,
    PAH8002_PARAM_IDX_STATIC_MODE                       = PAH8SERIES_PARAM_IDX_STATIC_MODE,
    PAH8002_PARAM_IDX_STATIC_HR_UP_IDX                  = PAH8SERIES_PARAM_IDX_STATIC_HR_UP_IDX,
    PAH8002_PARAM_IDX_STATIC_HR_DN_IDX                  = PAH8SERIES_PARAM_IDX_STATIC_HR_DN_IDX,
    PAH8002_PARAM_IDX_IS_AUTO                           = PAH8SERIES_PARAM_IDX_IS_AUTO,
    PAH8002_PARAM_IDX_IS_TAG                            = PAH8SERIES_PARAM_IDX_IS_TAG,
    PAH8002_PARAM_IDX_IS_8002                           = PAH8SERIES_PARAM_IDX_IS_8002,
    PAH8002_PARAM_IDX_FIRST_HR_6SEC                     = PAH8SERIES_PARAM_IDX_FIRST_HR_6SEC,
    PAH8002_PARAM_IDX_HR_MODEL_TIME_CONST               = PAH8SERIES_PARAM_IDX_HR_MODEL_TIME_CONST,
    PAH8002_PARAM_IDX_HR_MODEL_PROGRESS_RATIO           = PAH8SERIES_PARAM_IDX_HR_MODEL_PROGRESS_RATIO,
    PAH8002_PARAM_IDX_INIT_WALK_HR                      = PAH8SERIES_PARAM_IDX_INIT_WALK_HR,
    PAH8002_PARAM_IDX_WALK_HR_UP_IDX                    = PAH8SERIES_PARAM_IDX_WALK_HR_UP_IDX,
    PAH8002_PARAM_IDX_WALK_HR_DN_IDX                    = PAH8SERIES_PARAM_IDX_WALK_HR_DN_IDX,
    PAH8002_PARAM_IDX_HR_TOO_LOW_IDX                    = PAH8SERIES_PARAM_IDX_HR_TOO_LOW_IDX,
    PAH8002_PARAM_IDX_EN_LWPS                           = PAH8SERIES_PARAM_IDX_EN_LWPS,
    PAH8002_PARAM_IDX_EN_JUDGE_WALK                     = PAH8SERIES_PARAM_IDX_EN_JUDGE_WALK,
    PAH8002_PARAM_IDX_EN_CHK_HR_GT_MOTION               = PAH8SERIES_PARAM_IDX_EN_CHK_HR_GT_MOTION,
    PAH8002_PARAM_IDX_EN_HI_QUALITY_HR_JMP              = PAH8SERIES_PARAM_IDX_EN_HI_QUALITY_HR_JMP,
    PAH8002_PARAM_IDX_EN_SPLIT_MOTION_PEAK              = PAH8SERIES_PARAM_IDX_EN_SPLIT_MOTION_PEAK,
    PAH8002_NF_PARAM_IDX                                = PAH8SERIES_NF_PARAM_IDX,
} pah8002_param_idx_t;



PXIALG_API uint32_t pah8002_version(void);
PXIALG_API uint32_t pah8002_query_open_size(void);
PXIALG_API uint8_t pah8002_open(void *pBuffer);
PXIALG_API uint8_t pah8002_close(void);
PXIALG_API uint8_t pah8002_set_param(pah8002_param_idx_t idx, float p1);
PXIALG_API uint8_t pah8002_get_param(pah8002_param_idx_t idx, float *p1);
PXIALG_API uint8_t pah8002_get_hr(float *hr);
PXIALG_API uint8_t pah8002_get_hr_trust_level(int *hr_trust_level);
PXIALG_API uint8_t pah8002_entrance(pah8002_data_t *data8002);
PXIALG_API uint8_t pah8002_get_signal_grade(int16_t *grade);
PXIALG_API uint8_t pah8002_get_display_buffer(int32_t **buffer1, int32_t **buffer2, int32_t *size);

#endif
