/*******************************************************************************
* File Name          : AK09912_MAG.c
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "project.h"

#if (MODULE_MAG == MAG_AKM_AK09912C)

/* Imported constants --------------------------------------------------------*/

/* Imported macro ------------------------------------------------------------*/
#if (MAG_IF_TYPE == IF_SPI)
    #include "drvi_spi.h"
    #define AK09912_IF_WriteThenRead(tbuf,tlen,rbuf,rlen)   drvi_SpiWriteThenRead(MAG_IF_ID,tbuf,tlen,rbuf,rlen)
    #define AK09912_IF_Write(buf,len)                       drvi_SpiWrite(MAG_IF_ID,buf,len)
#elif (MAG_IF_TYPE == IF_I2C)
    #include "drvi_i2c.h"
    #define AK09912_IF_WriteThenRead(tbuf,tlen,rbuf,rlen)   drvi_I2cWriteThenRead(MAG_IF_ID,tbuf,tlen,rbuf,rlen)
    #define AK09912_IF_Write(buf,len)                       drvi_I2cWrite(MAG_IF_ID,buf,len)
#else
    #define AK09912_IF_WriteThenRead(tbuf,tlen,rbuf,rlen)      
    #define AK09912_IF_Write(buf,len)                            
#endif


/* Private macro -------------------------------------------------------------*/
#define AK09912_IF_ADDRESS      0x0C
/* Private variables ---------------------------------------------------------*/
static uint8_t m_rx_buf[32];
static uint8_t m_tx_buf[32];
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name		: AK09912_MAG_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*					: I2C or SPI reading functions					
* Input				: Register Address
* Output			: Data REad
* Return			: None
*******************************************************************************/
u8_t AK09912_MAG_ReadReg(u8_t Reg, u8_t* Data) {
  
  //To be completed with either I2c or SPI reading function
  //i.e.: *Data = SPI_Mems_Read_Reg( Reg );
  //*Data = i2c_sensor_read_byte((uint32_t)Reg);
  //i2c_data_read(Reg, Data, 1);
  m_tx_buf[0] = Reg;

  AK09912_IF_WriteThenRead(m_tx_buf,1,Data,1);
    
  *Data = m_rx_buf[0];
   
  return MEMS_SUCCESS;
  //EXAMPLE
  //if(!I2C_BufferRead(Data, AK09912_MAG_MEMS_I2C_ADDRESS, Reg, 1)) return MEMS_ERROR;
  //else return MEMS_SUCCESS; 
}

/*******************************************************************************
* Function Name		: AK09912_MAG_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, Data to be written
* Output			: None
* Return			: None
*******************************************************************************/
u8_t AK09912_MAG_WriteReg(u8_t Reg, u8_t Data) {
    
  //To be completed with either I2c or SPI writing function
  //i.e.: SPI_Mems_Write_Reg(Reg, Data);
  //i2c_sensor_write_byte((uint32_t)Reg, Data);
  //i2c_data_write(Reg, &Data);

  m_tx_buf[0] = Reg;
  m_tx_buf[1] = Data;

  AK09912_IF_Write(m_tx_buf,2);
  return MEMS_SUCCESS;
  //EXAMPLE  
  //I2C_ByteWrite(&Data,  AK09912_MAG_MEMS_I2C_ADDRESS,  Reg); 
  //return 1;
}
/* Private functions ---------------------------------------------------------*/
status_t AK09912_MAG_GetAKMID(u8_t *id)
{
    if( !AK09912_MAG_ReadReg(AK09912_AKM_ID, id))
        return MEMS_ERROR;
    else
        return MEMS_SUCCESS;
}

status_t AK09912_MAG_GetID(u8_t *id)
{
    if( !AK09912_MAG_ReadReg(AK09912_MAG_ID, id))
        return MEMS_ERROR;
    else
        return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AK09912_MAG_SetODR
* Description    : Sets AK09912_MAG working mode
* Input          : Set mode
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_SetMode(AK09912_MAG_MODE_t ov){
  u8_t value;

  if( !AK09912_MAG_ReadReg(AK09912_MAG_CNTL2, &value) )
    return MEMS_ERROR;
	
  value &= ~AK09912_MAG_ROM_ACCESS; //mask
  value |= ov;		

  if( !AK09912_MAG_WriteReg(AK09912_MAG_CNTL2, value) )
    return MEMS_ERROR;
  
  if( !AK09912_MAG_ReadReg(AK09912_MAG_CNTL2, &value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AK09912_MAG_TemperatureEN
* Description    : Enable/Disable AK09912_MAG Temperature
* Input          : Temperature Enable/Disable [AK09912_MAG_TEMP_EN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_TemperatureEN(AK09912_MAG_TEMP_EN_t ov){
  u8_t value;

  if( !AK09912_MAG_ReadReg(AK09912_MAG_CNTL1, &value) )
    return MEMS_ERROR;

  value &= ~AK09912_MAG_TEMP_EN_ENABLE; //mask
  value |= ov;	

  if( !AK09912_MAG_WriteReg(AK09912_MAG_CNTL1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : AK09912_MAG_SoftReset
* Description    : Enable/Disable AK09912_MAG SoftReset
* Input          : SoftReset Enable/Disable [AK09912_MAG_SOFT_RST_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_SoftReset(AK09912_MAG_SOFT_RST_t ov){
  u8_t value;

  if( !AK09912_MAG_ReadReg(AK09912_MAG_CNTL3, &value) )
    return MEMS_ERROR;

  value &= ~AK09912_MAG_SOFT_RST_ENABLE; //mask
  value |= ov;	

  if( !AK09912_MAG_WriteReg(AK09912_MAG_CNTL3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : AK09912_MAG_SerialInterfaceMode
* Description    : set AK09912_MAG SerialInterfaceMode
* Input          : set AK09912_MAG SerialInterfaceMode [AK09912_MAG_SIM_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_I2CDisable(AK09912_MAG_I2CDIS_t ov){
  u8_t value;

  if( !AK09912_MAG_ReadReg(AK09912_MAG_I2CDIS, &value) )
    return MEMS_ERROR;
	
  value &= ~0xFF; //mask
  value |= ov;	

  if( !AK09912_MAG_WriteReg(AK09912_MAG_I2CDIS, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : AK09912_MAG_SerialInterfaceMode
* Description    : set AK09912_MAG SerialInterfaceMode
* Input          : set AK09912_MAG SerialInterfaceMode [AK09912_MAG_SIM_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_I2CHS_En(AK09912_MAG_I2CHS_EN_t ov){
  u8_t value;

  if( !AK09912_MAG_ReadReg(AK09912_MAG_ST1, &value) )
    return MEMS_ERROR;
	
  value &= ~AK09912_MAG_I2C_HS_ENABLE; //mask
  value |= ov;	

  if( !AK09912_MAG_WriteReg(AK09912_MAG_ST1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : AK09912_MAG_MeasOverFlow
* Description    : AK09912_MAG  measure overflow
* Input          : pointer to AK09912_MAG_MROI_t
* Output         : AK09912_MAG Interrupt Flag [AK09912_MAG_MROI_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t AK09912_MAG_MeasOverFlow(AK09912_MAG_HOFL_t *value){

  if( !AK09912_MAG_ReadReg(AK09912_MAG_ST2, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= AK09912_MAG_OVERFLOW; //mask

  return MEMS_SUCCESS;
}

status_t AK09912_MAG_GetST1Stat(u8_t *value){

  if( !AK09912_MAG_ReadReg(AK09912_MAG_ST1, (u8_t *)value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

status_t AK09912_MAG_GetST2Stat(u8_t *value){

  if( !AK09912_MAG_ReadReg(AK09912_MAG_ST2, (u8_t *)value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : AK09912_MAG_MeasOverRun
* Description    : AK09912_MAG  measure overflow
* Input          : pointer to AK09912_MAG_MROI_t
* Output         : AK09912_MAG Interrupt Flag [AK09912_MAG_MROI_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t AK09912_MAG_MeasOverRun(AK09912_MAG_DOR_DRDY_t *value){

  if( !AK09912_MAG_ReadReg(AK09912_MAG_ST1, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= AK09912_MAG_DOR; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : AK09912_MAG_Data ready
* Description    : AK09912_MAG  measure ready
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t AK09912_MAG_Data_Ready(AK09912_MAG_DOR_DRDY_t *value){

  if( !AK09912_MAG_ReadReg(AK09912_MAG_ST1, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= AK09912_MAG_DRDY; //mask

  return MEMS_SUCCESS;
}

status_t  AK09912_MAG_GetMagRawBurst(u8_t* buff, u8_t buffer_size)
{
    if((!buffer_size) || !buff)
    {
        return MEMS_ERROR;
    }
    else
    {
        //i2c_data_read(AK09912_MAG_HXL, buff, buffer_size);
        m_tx_buf[0] = AK09912_MAG_HXL;
        AK09912_IF_WriteThenRead(m_tx_buf,1,buff,buffer_size);
        return MEMS_SUCCESS;
    }
}

/*******************************************************************************
* Function Name  : AK09912_MAG_GetMagRaw
* Description    : Read magnetic output register
* Input          : pointer to AxesRaw_t
* Output         : Magnetic Output Registers buffer AxesRaw_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_GetMagRaw(AxesRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;
  u8_t stat = 0;

  //do {
  //  AK09912_MAG_Data_Ready(&stat);
  //} while (stat != AK09912_MAG_DRDY);

  //while (tmp != AK09912_MAG_DRDY && tmp != AK09912_MAG_DOR)
  //{
  //   AK09912_MAG_Data_Ready(&tmp);
  //}
	
//  do{  
//    AK09912_MAG_ReadReg(AK09912_MAG_ST1, &stat);
//  } while ((stat & 0x03) == 0);
  
	
  if( !AK09912_MAG_ReadReg(AK09912_MAG_HXL, &valueL) )
      return MEMS_ERROR;
  
  if( !AK09912_MAG_ReadReg(AK09912_MAG_HXH, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL );
  
  if( !AK09912_MAG_ReadReg(AK09912_MAG_HYL, &valueL) )
      return MEMS_ERROR;
  
  if( !AK09912_MAG_ReadReg(AK09912_MAG_HYH, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL );
  
   if( !AK09912_MAG_ReadReg(AK09912_MAG_HZL, &valueL) )
      return MEMS_ERROR;
  
  if( !AK09912_MAG_ReadReg(AK09912_MAG_HZH, &valueH) )
      return MEMS_ERROR;
  
  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL );

  AK09912_MAG_GetST1Stat(&stat);
  AK09912_MAG_GetST2Stat(&stat);

  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : AK09912_MAG_GetTemperatureRaw
* Description    : Read Temperature output register
* Input          : pointer to u8_t
* Output         : Temperature data row to u16_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_GetTemperatureRaw(u8_t* buff) {
  u8_t value;
	
  if( !AK09912_MAG_ReadReg(AK09912_MAG_TMPS, &value) )
      return MEMS_ERROR;
	  
  *buff = value;
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : AK09912_MAG_GetMagAdj
* Description    : Read magnetic output register
* Input          : pointer to AxesAdj_t
* Output         : Magnetic Output Registers buffer AxesAdj_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t AK09912_MAG_AdjValue(AxesAdj_t* buff) {
  u8_t value;
	
  if( !AK09912_MAG_ReadReg(AK09912_MAG_ASAX, &value) )
      return MEMS_ERROR;
  
  buff->AXIS_X = value;
  
  if( !AK09912_MAG_ReadReg(AK09912_MAG_ASAY, &value) )
      return MEMS_ERROR;
  
  buff->AXIS_Y = value;
  
   if( !AK09912_MAG_ReadReg(AK09912_MAG_ASAZ, &value) )
      return MEMS_ERROR;
  
  buff->AXIS_Z = value;
  return MEMS_SUCCESS;  
}


status_t AK09912_MAG_Init(void)
{
	status_t response;
    u8_t id;
    #if (MAG_IF_TYPE == IF_I2C)
    T_I2cDevice i2c_sensor_config = {
       .bBusNum          = MAG_IF_ID,
       .bAddr            = AK09912_IF_ADDRESS,
    };

    drvi_I2cDeviceRegister(&i2c_sensor_config);
    #endif
 response = AK09912_MAG_GetAKMID(&id);
 if(id != AKM_COMPANY_ID) {
     return MEMS_ERROR;
 }

 response = AK09912_MAG_GetID(&id);
 if(id != AK09912_MAG_WHO_AM_I) {
     return MEMS_ERROR;
 }
  
 //Disable I2C interface
 //AK09912_MAG_I2CDisable(AK09912_MAG_I2C_DISABLE);

 //Initialize Magnetometer 
 response = AK09912_MAG_SetMode(AK09912_MAG_DO_100_Hz);
 //response = AK09912_MAG_SetMode(AK09912_MAG_DO_10_Hz);
 if(response==MEMS_ERROR) return response; //manage here comunication error

 return MEMS_SUCCESS;

}

status_t AK09912_MAG_SLEEP(void)
{

	status_t response;

 response = AK09912_MAG_SetMode(AK09912_MAG_POWER_DOWN);
 if(response==MEMS_ERROR) return response; //manage here comunication error

 return MEMS_SUCCESS;

}


#endif //#if (MODULE_MAG == MAG_AKM_AK09912C)
