#ifndef __liftarm_H__
#define __liftarm_H__

#define LIB_OPEN_PASS           1
#define LIB_OPEN_FAIL         0

#ifdef __cplusplus
extern "C" {
#endif

#ifndef struct_emxArray_int32_T_60
#define struct_emxArray_int32_T_60
struct emxArray_int32_T_60
{
    int data[60];
    int size[1];
};
#endif /*struct_emxArray_int32_T_60*/

#ifndef typedef_emxArray_int32_T_60
#define typedef_emxArray_int32_T_60
typedef struct emxArray_int32_T_60 emxArray_int32_T_60;
#endif /*typedef_emxArray_int32_T_60*/

#ifndef struct_emxArray_int32_T_61
#define struct_emxArray_int32_T_61
struct emxArray_int32_T_61
{
    int data[61];
    int size[1];
};
#endif /*struct_emxArray_int32_T_61*/
#ifndef typedef_emxArray_int32_T_61

#define typedef_emxArray_int32_T_61
typedef struct emxArray_int32_T_61 emxArray_int32_T_61;
#endif /*typedef_emxArray_int32_T_61*/

void liftarm_open(void);
void liftarm_close(void);
void liftarm_process(const int AccIn[3],int8_t* GestureOut, int8_t* WinStatus);
void liftarm_set_hangconfig(char hangconfig); // 0: left hang (default), 1: right hang
unsigned char liftarm_is_open(void);
void CC_Liftarm_onStateChange(uint8_t State);

/**
 * @brief Get the version.
 * 
 * @return int32_t Return a 32 bits value it includes 
 *  AlGOTITHM_MAJOR(4bits).MINOR(4bits).BUILD(4bits).Reserve(4bits).INTERFACE_MAJOR(4bits).MINOR(4bits).BUILD(4bits).Reserve(4bits)
 *         
 */
unsigned long liftarm_get_version(void);    

#ifdef __cplusplus
}
#endif

#endif /**< CFG_MODULE_LIFTARM */


