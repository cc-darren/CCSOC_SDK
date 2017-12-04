/* Includes ------------------------------------------------------------------*/
#include "project.h"

#if (MODULE_ADC == ADC_NULL)
/******************************************************************
*   place all function and private variables in this part
******************************************************************/
#define AdcNull_IF_Read(rbuf,rlen)
#define AdcNull_IF_Write(buf,len)

#endif //#if (MODULE_ADC == ADC_NULL)
