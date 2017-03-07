/**
 ****************************************************************************************
 *
 * @file ll.h
 *
 * @brief Declaration of low level functions.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef LL_H_
#define LL_H_

#ifndef __arm__
#error "File only included with RVDS!"
#endif // __arm__

#include <stdint.h>
//#include "arch.h"
//#include "reg_intc.h"

//PATTAYA: copied from cmsis_armcc.h
/**
  \brief   Get Priority Mask
  \details Returns the current state of the priority mask bit from the Priority Mask Register.
  \return               Priority Mask value
 */
static __inline uint32_t __get_PRIMASK(void)
{
  register uint32_t __regPriMask         __asm("primask");
  return(__regPriMask);
}

//PATTAYA: copied from cmsis_armcc.h
/**
  \brief   Set Priority Mask
  \details Assigns the given value to the Priority Mask Register.
  \param [in]    priMask  Priority Mask
 */
static __inline void __set_PRIMASK(uint32_t priMask)
{
  register uint32_t __regPriMask         __asm("primask");
  __regPriMask = (priMask);
}

/** @brief Enable interrupts globally in the system.
 * This macro must be used when the initialization phase is over and the interrupts
 * can start being handled by the system.
 */
#define GLOBAL_INT_START()     __enable_irq();

/** @brief Disable interrupts globally in the system.
 * This macro must be used when the system wants to disable all the interrupt
 * it could handle.
 */
#define GLOBAL_INT_STOP()      __disable_irq();


/** @brief Disable interrupts globally in the system.
 * This macro must be used in conjunction with the @ref GLOBAL_INT_RESTORE macro since this
 * last one will close the brace that the current macro opens.  This means that both
 * macros must be located at the same scope level.
 */
#define GLOBAL_INT_DISABLE()                                                \
do {                                                                        \
    uint32_t __l_irq_rest = __get_PRIMASK();                                \
    __set_PRIMASK(1);


/** @brief Restore interrupts from the previous global disable.
 * @sa GLOBAL_INT_DISABLE
 */
#define GLOBAL_INT_RESTORE()                                                \
    if(__l_irq_rest == 0)                                                   \
    {                                                                       \
        __set_PRIMASK(0);                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        __set_PRIMASK(1);                                                   \
    }                                                                       \
} while(0)


/** @brief Invoke the wait for interrupt procedure of the processor.
 *
 * @warning It is suggested that this macro is called while the interrupts are disabled
 * to have performed the checks necessary to decide to move to sleep mode.
 *
 */
#define WFI()       __wfi()
#define WFE()       __wfe()

#endif // LL_H_
