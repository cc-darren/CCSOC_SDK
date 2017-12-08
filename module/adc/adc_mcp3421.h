

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADC_NULL_DRIVER_H
#define _ADC_NULL_DRIVER_H



/* Exported macro ------------------------------------------------------------*/
//ADC sensor interface
#define ADC_Init()                             MCP3421Init()
#define ADC_ReadRaw(val)                       MCP3421ReadRaw(val)
#define ADC_ReadScales(val)                    MCP3421ReadSampleRate(val)
#define ADC_ReadSampleRate(rate)               MCP3421ReadSacle(rate)
#define ADC_WritePga(pga)                      MCP3421WritePga(pga)
#define ADC_WriteSampleRate(rate)              MCP3421WriteSampleFreq(rate)


/* Exported constants --------------------------------------------------------*/







int MCP3421Init(void);
int MCP3421ReadRaw(int *piVal);
int MCP3421ReadSacle(int *piVal);
int MCP3421ReadSampleRate(int *piVal);
int MCP3421WritePga(int iVal);
int MCP3421WriteSampleFreq(int iVal);



#endif //_ADC_NULL_DRIVER_H
