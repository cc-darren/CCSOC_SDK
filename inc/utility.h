/******************************************************************************
*  Copyright 2017, Cloud Chip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloud Chip, Inc. (C) 2017
******************************************************************************/
#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <stdbool.h>
#include "ARMCM.h"

#define EXTERNAL_INT_VECTOR_OFFSET 16
#define APP_PRIORITY_THREAD    4

typedef enum
{
    APP_IRQ_PRIORITY_HIGHEST = 0,
    APP_IRQ_PRIORITY_HIGH    = 1,
    APP_IRQ_PRIORITY_MID     = 2,
    APP_IRQ_PRIORITY_LOW     = 3
} app_irq_priority_t;

#define UNUSED_VARIABLE(X)  ((void)(X))
#define UNUSED_PARAMETER(X) UNUSED_VARIABLE(X)

/**@brief Implementation specific macro for delayed macro expansion used in string concatenation
*
* @param[in]   lhs   Left hand side in concatenation
* @param[in]   rhs   Right hand side in concatenation
*/
#define STRING_CONCATENATE_IMPL(lhs, rhs) lhs ## rhs

/**@brief Macro used to concatenate string using delayed macro expansion
*
* @note This macro will delay concatenation until the expressions have been resolved
*
* @param[in]   lhs   Left hand side in concatenation
* @param[in]   rhs   Right hand side in concatenation
*/
#define STRING_CONCATENATE(lhs, rhs) STRING_CONCATENATE_IMPL(lhs, rhs)


/**@brief Macro for doing static (i.e. compile time) assertion.
*
* @note If the EXPR isn't resolvable, then the error message won't be shown.
*
* @note The output of STATIC_ASSERT_MSG will be different across different compilers.
*
* @param[in] EXPR Constant expression to be verified.
*/
#if defined ( __COUNTER__ )

#define STATIC_ASSERT(EXPR) \
    ;enum { STRING_CONCATENATE(static_assert_, __COUNTER__) = 1/(!!(EXPR)) }

#else

#define STATIC_ASSERT(EXPR) \
    ;enum { STRING_CONCATENATE(assert_line_, __LINE__) = 1/(!!(EXPR)) }

#endif


#if !defined(MAX)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#if !defined(MIN)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif


/**@brief Perform rounded integer division (as opposed to truncating the result).
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Rounded (integer) result of dividing A by B.
 */
#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))


/**@brief Perform integer division, making sure the result is rounded up.
 *
 * @details One typical use for this is to compute the number of objects with size B is needed to
 *          hold A number of bytes.
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Integer result of dividing A by B, rounded up.
 */
#define CEIL_DIV(A, B)          (((A) + (B) - 1) / (B))

/**@brief Function for finding the current interrupt level.
 *
 * @return   Current interrupt level.
 * @retval   APP_IRQ_PRIORITY_HIGH    We are running in Application High interrupt level.
 * @retval   APP_IRQ_PRIORITY_LOW     We are running in Application Low interrupt level.
 * @retval   APP_IRQ_PRIORITY_THREAD  We are running in Thread Mode.
 */
static __INLINE uint8_t current_int_priority_get(void)
{
    uint32_t isr_vector_num = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    if (isr_vector_num > 0)
    {
        int32_t irq_type = ((int32_t)isr_vector_num - EXTERNAL_INT_VECTOR_OFFSET);
        return (NVIC_GetPriority((IRQn_Type)irq_type) & 0xFF);
    }
    else
    {
        return APP_PRIORITY_THREAD;
    }
}


static __ASM void __INLINE cc_delay_us(uint32_t volatile number_of_us)
{
loop
        SUBS    R0, R0, #1
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#ifdef NRF52
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#endif
        BNE    loop
        BX     LR
}

/**@brief Function for checking if a pointer value is aligned to a 4 byte boundary.
 *
 * @param[in]   p   Pointer value to be checked.
 *
 * @return      TRUE if pointer is aligned to a 4 byte boundary, FALSE otherwise.
 */
static __INLINE bool is_word_aligned(void const* p)
{
    return (((uintptr_t)p & 0x03) == 0);
}


















#endif //_UTILITY_H_
