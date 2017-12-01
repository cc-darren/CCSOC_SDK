/*******************************************************************************
* File Name          : ppg_pah8002.c
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "project.h"
#include "ppg_pah8002.h"
#include "tracer.h"

#if (MODULE_PPG == PPG_PXT_PAH8002)

/* Imported constants --------------------------------------------------------*/

/* Imported function --------------------------------------------------------*/

/* Imported macro ------------------------------------------------------------*/
#if (PPG_IF_TYPE == IF_SPI)

#elif (PPG_IF_TYPE == IF_I2C)
    #include "drvi_i2c.h"
    #define PAH8002_IF_WriteThenRead(tbuf,tlen,rbuf,rlen)   drvi_I2cWriteThenRead(PPG_IF_ID,tbuf,tlen,rbuf,rlen)
    #define PAH8002_IF_Write(buf,len)                       drvi_I2cWrite(PPG_IF_ID,buf,len)
#else
    #define PAH8002_IF_WriteThenRead(tbuf,tlen,rbuf,rlen)      
    #define PAH8002_IF_Write(buf,len)       
#endif

/* Private macro -------------------------------------------------------------*/
#define PAH8002_IF_ADDRESS  0x15  //I2C 7-bit ID
/* Private variables ---------------------------------------------------------*/
static uint8_t m_rx_buf[32];
static uint8_t m_tx_buf[32];
/* Private function prototypes -----------------------------------------------*/


/**
 * @brief  initialization.
 */
void pah8002_twi_init (void)
{
         
    #if (PPG_IF_TYPE == IF_I2C)
    T_I2cDevice i2c_sensor_config = {
       .bBusNum          = PPG_IF_ID,
       .bAddr            = PAH8002_IF_ADDRESS,
    };

    drvi_I2cDeviceRegister(&i2c_sensor_config);
    #endif
}

void pah8002_twi_deinit(void)
{
    // Nothing to do
}

uint8_t pah8002_write_reg(uint8_t addr, uint8_t data)
{
    m_tx_buf[0] = addr;
    m_tx_buf[1] = data;

    PAH8002_IF_Write(m_tx_buf,2);
    return MEMS_SUCCESS;
}

uint8_t pah8002_read_reg(uint8_t addr, uint8_t *data)
{
    m_tx_buf[0] = addr;

    PAH8002_IF_WriteThenRead(m_tx_buf,1,m_rx_buf,1);
    
    *data = m_rx_buf[0];
   
    return MEMS_SUCCESS;
}

uint8_t pah8002_burst_read_reg(uint8_t addr, uint8_t *data, uint32_t rx_size)
{
    m_tx_buf[0] = addr;

    PAH8002_IF_WriteThenRead(m_tx_buf,1,data,rx_size);
    
    //*data = m_rx_buf[0];
   
    return MEMS_SUCCESS;
}

#endif

