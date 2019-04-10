/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

/******************************************************************************
 * Filename:
 * ---------
 * sw_spi.c
 *
 * Project:
 * --------
 * cc6801
 *
 * Description:
 * ------------
 * This is software SPI driver
 *
 * Author:
 * -------
 * CloudChip
 *
 * ============================================================================
 *  2019/02/19 C.H.Wu initial version
 *****************************************************************************/

#include <stdbool.h>
#include <limits.h>

#include "sw_spi.h"

void
cc6801_sw_spi_init (
    void
    )
{
    SET_SW_SPI_CS_INACTIVE;
    SET_SW_SPI_CLK_INACTIVE;
    SET_SW_SPI_MOSI_HIGH;
}

void
cc6801_sw_spi_write (
    uint8_t const   command,
    uint8_t const   * const p_write,
    int32_t const   write_length
    )
{
    int32_t i;
    int32_t j;

    SET_SW_SPI_CS_ACTIVE;
    {
        // command or instruction
        for (i = 0; i < sizeof (command); i++)
        {
            for (j = 0; j <= CHAR_BIT; j++)
            {
                if (j == CHAR_BIT)
                {
                    if (SW_SPI_CPHA)
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    else
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    SET_SW_SPI_MOSI_HIGH;
                    break;
                }

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_ACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_INACTIVE;
                }
                if (command & (((uint8_t) 0x80) >> j))
                {
                    SET_SW_SPI_MOSI_HIGH;
                }
                else
                {
                    SET_SW_SPI_MOSI_LOW;
                }
                SW_SPI_DELAY;

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_INACTIVE;
                    SET_SW_SPI_CLK_INACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_ACTIVE;
                    SET_SW_SPI_CLK_ACTIVE;
                }
                SW_SPI_DELAY;
            }
        }

        // write
        for (i = 0; (i < write_length) && (p_write != NULL); i++)
        {
            for (j = 0; j <= CHAR_BIT; j++)
            {
                if (j == CHAR_BIT)
                {
                    if (SW_SPI_CPHA)
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    else
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    SET_SW_SPI_MOSI_HIGH;
                    break;
                }

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_ACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_INACTIVE;
                }
                if (p_write[i] & (((uint8_t) 0x80) >> j))
                {
                    SET_SW_SPI_MOSI_HIGH;
                }
                else
                {
                    SET_SW_SPI_MOSI_LOW;
                }
                SW_SPI_DELAY;

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_INACTIVE;
                    SET_SW_SPI_CLK_INACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_ACTIVE;
                    SET_SW_SPI_CLK_ACTIVE;
                }
                SW_SPI_DELAY;
            }
        }
    }
    SET_SW_SPI_CS_INACTIVE;
}

void
cc6801_sw_spi_write_then_read (
    uint8_t const   command,
    uint8_t const   * const p_write,
    int32_t const   write_length,
    int32_t const   dummy_length,
    uint8_t         * const p_read,
    int32_t const   read_length
    )
{
    int32_t i;
    int32_t j;

    SET_SW_SPI_CS_ACTIVE;
    {
        // command or instruction
        for (i = 0; i < sizeof (command); i++)
        {
            for (j = 0; j <= CHAR_BIT; j++)
            {
                if (j == CHAR_BIT)
                {
                    if (SW_SPI_CPHA)
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    else
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    SET_SW_SPI_MOSI_HIGH;
                    break;
                }

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_ACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_INACTIVE;
                }
                if (command & (((uint8_t) 0x80) >> j))
                {
                    SET_SW_SPI_MOSI_HIGH;
                }
                else
                {
                    SET_SW_SPI_MOSI_LOW;
                }
                SW_SPI_DELAY;

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_INACTIVE;
                    SET_SW_SPI_CLK_INACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_ACTIVE;
                    SET_SW_SPI_CLK_ACTIVE;
                }
                SW_SPI_DELAY;
            }
        }

        // write
        for (i = 0; (i < write_length) && (p_write != NULL); i++)
        {
            for (j = 0; j <= CHAR_BIT; j++)
            {
                if (j == CHAR_BIT)
                {
                    if (SW_SPI_CPHA)
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    else
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    SET_SW_SPI_MOSI_HIGH;
                    break;
                }

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_ACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_INACTIVE;
                }
                if (p_write[i] & (((uint8_t) 0x80) >> j))
                {
                    SET_SW_SPI_MOSI_HIGH;
                }
                else
                {
                    SET_SW_SPI_MOSI_LOW;
                }
                SW_SPI_DELAY;

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_INACTIVE;
                    SET_SW_SPI_CLK_INACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_ACTIVE;
                    SET_SW_SPI_CLK_ACTIVE;
                }
                SW_SPI_DELAY;
            }
        }

        // dummy
        for (i = 0; i < dummy_length; i++)
        {
            for (j = 0; j <= CHAR_BIT; j++)
            {
                if (j == CHAR_BIT)
                {
                    if (SW_SPI_CPHA)
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    else
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    SET_SW_SPI_MOSI_HIGH;
                    break;
                }

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_ACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_INACTIVE;
                }
                if (true)
                {
                    SET_SW_SPI_MOSI_HIGH;
                }
                else
                {
                    SET_SW_SPI_MOSI_LOW;
                }
                SW_SPI_DELAY;

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_INACTIVE;
                    SET_SW_SPI_CLK_INACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_ACTIVE;
                    SET_SW_SPI_CLK_ACTIVE;
                }
                SW_SPI_DELAY;
            }
        }

        // read
        for (i = 0; (i < read_length) && (p_read != NULL); i++)
        {
            p_read[i] = 0;

            for (j = 0; j <= CHAR_BIT; j++)
            {
                if (j == CHAR_BIT)
                {
                    if (SW_SPI_CPHA)
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    else
                    {
                        SET_SW_SPI_CLK_INACTIVE;
                    }
                    break;
                }

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_ACTIVE;
                    SET_SW_SPI_CLK_ACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_INACTIVE;
                    SET_SW_SPI_CLK_INACTIVE;
                }
                SW_SPI_DELAY;

                if (SW_SPI_CPHA)
                {
                    SET_SW_SPI_CLK_INACTIVE;
                }
                else
                {
                    SET_SW_SPI_CLK_ACTIVE;
                }
                p_read[i] |= (((uint8_t) GET_SW_SPI_MISO) << (7 - j));
                SW_SPI_DELAY;
            }
        }
    }
    SET_SW_SPI_CS_INACTIVE;
}
