/**
 ****************************************************************************************
 *
 * @file plf.h
 *
 * @brief Declaration of the PLF API.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef _PLF_H_
#define _PLF_H_


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>            // standard integer functions
#include <stdbool.h>           // standard boolean functions



/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// IcyTRx RF interface selection
enum RF_INTF_SEL
{
    RF_INTF_V1X_V2X,
    RF_INTF_V3X_V4X,
};

/*
 * STRUCTURES
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup PLF
 * @ingroup DRIVERS
 *
 * @brief Platform register driver
 *
 * @{
 *
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize and configure Platform registers.
 ****************************************************************************************
 */
void plf_init(void);

/**
 ****************************************************************************************
 * @brief Read RF board ID.
 *
 * @return    RF board ID number
 ****************************************************************************************
 */
uint16_t plf_read_rf_board_id(void);

/**
 ****************************************************************************************
 * @brief Check if RF clock has to be switched.
 *
 * @return   rf_switch     true: RF clock to be switched / false: no switch
 ****************************************************************************************
 */
bool plf_rf_switch(void);

/**
 ****************************************************************************************
 * @brief IcyTRx RF interface selection on Bubble boards
 *****************************************************************************************
 */
//Louis, 2016.08.15, disable API for compiler issue
//void plf_rf_interface_sel(enum RF_INTF_SEL interface);
#define plf_rf_interface_sel(a)

/// @} PLF

#endif // _PLF_H_
