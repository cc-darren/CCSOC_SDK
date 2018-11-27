/*
 * FreeRTOS+FAT Labs Build 160919 (C) 2016 Real Time Engineers ltd.
 * Authors include James Walmsley, Hein Tibosch and Richard Barry
 *
 *******************************************************************************
 ***** NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ***
 ***                                                                         ***
 ***                                                                         ***
 ***   FREERTOS+FAT IS STILL IN THE LAB:                                     ***
 ***                                                                         ***
 ***   This product is functional and is already being used in commercial    ***
 ***   products.  Be aware however that we are still refining its design,    ***
 ***   the source code does not yet fully conform to the strict coding and   ***
 ***   style standards mandated by Real Time Engineers ltd., and the         ***
 ***   documentation and testing is not necessarily complete.                ***
 ***                                                                         ***
 ***   PLEASE REPORT EXPERIENCES USING THE SUPPORT RESOURCES FOUND ON THE    ***
 ***   URL: http://www.FreeRTOS.org/contact  Active early adopters may, at   ***
 ***   the sole discretion of Real Time Engineers Ltd., be offered versions  ***
 ***   under a license other than that described below.                      ***
 ***                                                                         ***
 ***                                                                         ***
 ***** NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ***
 *******************************************************************************
 *
 * FreeRTOS+FAT can be used under two different free open source licenses.  The
 * license that applies is dependent on the processor on which FreeRTOS+FAT is
 * executed, as follows:
 *
 * If FreeRTOS+FAT is executed on one of the processors listed under the Special
 * License Arrangements heading of the FreeRTOS+FAT license information web
 * page, then it can be used under the terms of the FreeRTOS Open Source
 * License.  If FreeRTOS+FAT is used on any other processor, then it can be used
 * under the terms of the GNU General Public License V2.  Links to the relevant
 * licenses follow:
 *
 * The FreeRTOS+FAT License Information Page: http://www.FreeRTOS.org/fat_license
 * The FreeRTOS Open Source License: http://www.FreeRTOS.org/license
 * The GNU General Public License Version 2: http://www.FreeRTOS.org/gpl-2.0.txt
 *
 * FreeRTOS+FAT is distributed in the hope that it will be useful.  You cannot
 * use FreeRTOS+FAT unless you agree that you use the software 'as is'.
 * FreeRTOS+FAT is provided WITHOUT ANY WARRANTY; without even the implied
 * warranties of NON-INFRINGEMENT, MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. Real Time Engineers Ltd. disclaims all conditions and terms, be they
 * implied, expressed, or statutory.
 *
 * 1 tab == 4 spaces!
 *
 * http://www.FreeRTOS.org
 * http://www.FreeRTOS.org/plus
 * http://www.FreeRTOS.org/labs
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

/* FreeRTOS+FAT includes. */
#include "ff_headers.h"
#include "ff_spidisk.h"
#include "ff_sys.h"

/* CC6801 includes. */
#include "flash_winbond.h"

#define spiHIDDEN_SECTOR_COUNT      8
#define spiPRIMARY_PARTITIONS       1
#define spiHUNDRED_64_BIT           100ULL
#define spiSECTOR_SIZE              4096UL
#define spiPARTITION_NUMBER         0 /* Only a single partition is used. */
#define spiBYTES_PER_MB             ( 1024ull * 1024ull )
#define spiSECTORS_PER_MB           ( spiBYTES_PER_MB / 512ull )

/* Used as a magic number to indicate that an FF_Disk_t structure is a SPI
disk. */
#define spiSIGNATURE                0x41404342
#define spiIOMAN_MEM_SIZE           4096

/* Remembers if the card is currently considered to be present. */
static BaseType_t xSPIFlashStatus = pdFALSE;

/* Define a time-out for all DMA transactions in msec. */
#ifndef sdMAX_TIME_TICKS
    #define sdMAX_TIME_TICKS    pdMS_TO_TICKS( 2000UL )
#endif

/* Used to handle timeouts. */
static TickType_t xDMARemainingTime;
static TimeOut_t xDMATimeOut;

/* Used to unblock the task that calls prvEventWaitFunction() after an event has
occurred. */
static SemaphoreHandle_t xSDCardSemaphore = NULL;

/*-----------------------------------------------------------*/

/*
 * The function that writes to the media - as this is implementing a SPI disk
 * the media is just a SPI buffer.
 */
static int32_t prvWriteSPI( uint8_t *pucBuffer, uint32_t ulSectorNumber, uint32_t ulSectorCount, FF_Disk_t *pxDisk );

/*
 * The function that reads from the media - as this is implementing a SPI disk
 * the media is just a SPI buffer.
 */
static int32_t prvReadSPI( uint8_t *pucBuffer, uint32_t ulSectorNumber, uint32_t ulSectorCount, FF_Disk_t *pxDisk );

/*
 * This is the driver for a SPI disk.  Unlike most media types, SPI disks are
 * volatile so are created anew each time the system is booted.  As the disk is
 * new and just created, it must also be partitioned and formatted.
 */
#if 0
static FF_Error_t prvPartitionAndFormatDisk( FF_Disk_t *pxDisk );
#endif

/*
 * Check if the card is present, and if so, print out some info on the card.
 */
static BaseType_t prvSPIFLASHInit( BaseType_t xDriveNumber );

/*
 * A function will be called at the start of a DMA action.
 */
void prvEventSetupFunction( void );

/*
 * This function is supposed to wait for an event: SDIO or DMA.
 * Return non-zero if a timeout has been reached.
 */
uint32_t prvEventWaitFunction( void );


static int32_t prvReadSPI( uint8_t *pucDestination, uint32_t ulSectorNumber, uint32_t ulSectorCount, FF_Disk_t *pxDisk )
{
int32_t lReturn;
uint32_t ulSource;

    if( pxDisk != NULL )
    {
        if( pxDisk->ulSignature != spiSIGNATURE )
        {
            /* The disk structure is not valid because it doesn't contain a
            magic number written to the disk when it was created. */
            lReturn = FF_ERR_IOMAN_DRIVER_FATAL_ERROR | FF_ERRFLAG;
        }
        else if( pxDisk->xStatus.bIsInitialised == pdFALSE )
        {
            /* The disk has not been initialised. */
            lReturn = FF_ERR_IOMAN_OUT_OF_BOUNDS_WRITE | FF_ERRFLAG;
        }
        else if( ulSectorNumber >= pxDisk->ulNumberOfSectors )
        {
            /* The start sector is not within the bounds of the disk. */
            lReturn = ( FF_ERR_IOMAN_OUT_OF_BOUNDS_WRITE | FF_ERRFLAG );
        }
        else if( ( pxDisk->ulNumberOfSectors - ulSectorNumber ) < ulSectorCount )
        {
            /* The end sector is not within the bounds of the disk. */
            lReturn = ( FF_ERR_IOMAN_OUT_OF_BOUNDS_WRITE | FF_ERRFLAG );
        }
        else
        {
            /* Move to the start of the sector being read. */
            ulSource = ( 512ul * ulSectorNumber );

            /* Copy the data from the disk.  As this is a SPI disk this can be
            done using FLASH_Read(). */
            FLASH_Read( ( void * ) pucDestination,
                    ( uint32_t ) ulSource,
                    ( size_t ) ( ulSectorCount * 512ul ) );

            lReturn = FF_ERR_NONE;
        }
    }
    else
    {
        lReturn = FF_ERR_NULL_POINTER | FF_ERRFLAG;
    }

    return lReturn;
}
/*-----------------------------------------------------------*/

static int32_t prvWriteSPI( uint8_t *pucSource, uint32_t ulSectorNumber, uint32_t ulSectorCount, FF_Disk_t *pxDisk )
{
int32_t lReturn = FF_ERR_NONE;
uint32_t ulDestination = 0;

    if( pxDisk != NULL )
    {
        if( pxDisk->ulSignature != spiSIGNATURE )
        {
            /* The disk structure is not valid because it doesn't contain a
            magic number written to the disk when it was created. */
            lReturn = FF_ERR_IOMAN_DRIVER_FATAL_ERROR | FF_ERRFLAG;
        }
        else if( pxDisk->xStatus.bIsInitialised == pdFALSE )
        {
            /* The disk has not been initialised. */
            lReturn = FF_ERR_IOMAN_OUT_OF_BOUNDS_WRITE | FF_ERRFLAG;
        }
        else if( ulSectorNumber >= pxDisk->ulNumberOfSectors )
        {
            /* The start sector is not within the bounds of the disk. */
            lReturn = ( FF_ERR_IOMAN_OUT_OF_BOUNDS_WRITE | FF_ERRFLAG );
        }
        else if( ( pxDisk->ulNumberOfSectors - ulSectorNumber ) < ulSectorCount )
        {
            /* The end sector is not within the bounds of the disk. */
            lReturn = ( FF_ERR_IOMAN_OUT_OF_BOUNDS_WRITE | FF_ERRFLAG );
        }
        else
        {
            uint32_t ulSPISectorCount = 0;
            uint8_t *pucBuffer;

            ulSPISectorCount = (((ulSectorCount * 512ul) - 1) / spiSECTOR_SIZE ) + 1;

            pucBuffer = ffconfigMALLOC( ( size_t ) ulSPISectorCount * ( size_t ) spiSECTOR_SIZE );

            if( pucBuffer != NULL )
            {
                uint32_t ulDestAlign4K = 0;
                uint32_t ulDestOffset = 0;

                /* Move to the sector being written to. */
                ulDestination = ( 512ul * ulSectorNumber );
                ulDestAlign4K = ulDestination & 0xFFFFF000;
                ulDestOffset = ulDestination & 0x00000FFF;
#if 0
                FLASH_Write( ( uint32_t ) ulDestination,
                        ( void * ) pucSource,
                        ( size_t ) ulSectorCount * ( size_t ) 512 );
#else
                FLASH_Read( ( void * ) pucBuffer,
                         ulDestAlign4K,
                        ( size_t ) ulSPISectorCount * ( size_t ) spiSECTOR_SIZE );

                FLASH_Erase( ulDestAlign4K,
                        ( size_t ) ulSPISectorCount);

                /* Write to the disk.  As this is a SPI disk the write can use a
                FLASH_Write(). */
                memcpy( ( void * ) (pucBuffer+ulDestOffset),
                        ( void * ) pucSource,
                        ( size_t ) ulSectorCount * ( size_t ) 512ul );

                FLASH_Write( ulDestAlign4K,
                        ( void * ) pucBuffer,
                        ( size_t ) ulSPISectorCount * ( size_t ) spiSECTOR_SIZE );
#if 0
                memset(pucBuffer, 0, ( size_t ) ulSPISectorCount * ( size_t ) spiSECTOR_SIZE);

                FLASH_Read( ( void * ) pucBuffer,
                        ( uint32_t ) ulDestination,
                        ( size_t ) ulSectorCount * ( size_t ) 512ul );

                result = memcmp(pucBuffer, pucSource, ( size_t ) ulSectorCount * ( size_t ) 512ul);
                configASSERT( result == 0 );
#endif
#endif
                lReturn = FF_ERR_NONE;

                ffconfigFREE( pucBuffer );
            }
            else
            {
                FF_PRINTF( "prvFFWrite: malloc failed\n" );
                lReturn = FF_ERR_NOT_ENOUGH_MEMORY | FF_ERRFLAG;
            }
        }
    }
    else
    {
        lReturn = FF_ERR_NULL_POINTER | FF_ERRFLAG;
    }

    return lReturn;
}

/*-----------------------------------------------------------*/
#if 0
static FF_Error_t prvPartitionAndFormatDisk( FF_Disk_t *pxDisk )
{
FF_PartitionParameters_t xPartition;
FF_Error_t xError;

    /* Create a single partition that fills all available space on the disk. */
    memset( &xPartition, '\0', sizeof( xPartition ) );
    xPartition.ulSectorCount = pxDisk->ulNumberOfSectors;
    xPartition.ulHiddenSectors = spiHIDDEN_SECTOR_COUNT;
    xPartition.xPrimaryCount = spiPRIMARY_PARTITIONS;
    xPartition.eSizeType = eSizeIsQuota;

    /* Partition the disk */
    xError = FF_Partition( pxDisk, &xPartition );
    FF_PRINTF( "FF_Partition: %s\n", ( const char * ) FF_GetErrMessage( xError ) );

    if( FF_isERR( xError ) == pdFALSE )
    {
        /* Format the partition. */
        xError = FF_Format( pxDisk, spiPARTITION_NUMBER, pdTRUE, pdTRUE );
        FF_PRINTF( "FF_SPIDiskInit: FF_Format: %s\n", ( const char * ) FF_GetErrMessage( xError ) );
    }

    return xError;
}
#endif
/*-----------------------------------------------------------*/

void FF_SDDiskFlush( FF_Disk_t *pxDisk )
{
	if( ( pxDisk != NULL ) &&
		( pxDisk->xStatus.bIsInitialised != pdFALSE ) &&
		( pxDisk->pxIOManager != NULL ) )
	{
		FF_FlushCache( pxDisk->pxIOManager );
	}
}
/*-----------------------------------------------------------*/

/* This is the prototype of the function used to initialise the SPI disk driver.
Other media drivers do not have to have the same prototype.

In this example:
 + pcName is the name to give the disk within FreeRTOS+FAT's virtual file system.
 + pucDataBuffer is the start of the SPI to use as the disk.
 + ulSectorCount is effectively the size of the disk, each sector is 512 bytes.
 + xIOManagerCacheSize is the size of the IO manager's cache, which must be a
   multiple of the sector size, and at least twice as big as the sector size.
*/
FF_Disk_t *FF_SPIDiskInit( char *pcName, uint32_t ulSectorCount )
{
FF_Error_t xError;
FF_Disk_t *pxDisk = NULL;
BaseType_t xPartitionNumber = 0;
FF_CreationParameters_t xParameters;

    xSPIFlashStatus = prvSPIFLASHInit( 0 );

    if( xSPIFlashStatus != pdPASS )
    {
        FF_PRINTF( "FF_SDDiskInit: prvSPIFLASHInit failed\n" );
        pxDisk = NULL;
        return pxDisk;
    }

    /* Attempt to allocated the FF_Disk_t structure. */
    pxDisk = ( FF_Disk_t * ) ffconfigMALLOC( sizeof( FF_Disk_t ) );

    if( pxDisk != NULL )
    {
        /* Start with every member of the structure set to zero. */
        memset( pxDisk, '\0', sizeof( FF_Disk_t ) );

        /* The signature is used by the disk read and disk write functions to
        ensure the disk being accessed is a SPI disk. */
        pxDisk->ulSignature = spiSIGNATURE;

        /* The number of sectors is recorded for bounds checking in the read and
        write functions. */
        pxDisk->ulNumberOfSectors = ulSectorCount;

        /* Create the IO manager that will be used to control the SPI disk. */
        memset( &xParameters, '\0', sizeof( xParameters ) );
        xParameters.pucCacheMemory = NULL;
        xParameters.ulMemorySize = spiIOMAN_MEM_SIZE;
        xParameters.ulSectorSize = 512ul;
        xParameters.fnWriteBlocks = prvWriteSPI;
        xParameters.fnReadBlocks = prvReadSPI;
        xParameters.pxDisk = pxDisk;

        /* Driver is reentrant so xBlockDeviceIsReentrant can be set to pdTRUE.
        In this case the semaphore is only used to protect FAT data
        structures. */
        xParameters.pvSemaphore = ( void * ) xSemaphoreCreateRecursiveMutex();
        xParameters.xBlockDeviceIsReentrant = pdFALSE;

        pxDisk->pxIOManager = FF_CreateIOManger( &xParameters, &xError );

#if 0
        if( ( pxDisk->pxIOManager != NULL ) && ( FF_isERR( xError ) == pdFALSE ) )
        {
            /* Record that the RAM disk has been initialised. */
            pxDisk->xStatus.bIsInitialised = pdTRUE;

            /* Create a partition on the RAM disk.  NOTE!  The disk is only
            being partitioned here because it is a new RAM disk.  It is
            known that the disk has not been used before, and cannot already
            contain any partitions.  Most media drivers will not perform
            this step because the media will have already been partitioned. */
            xError = prvPartitionAndFormatDisk( pxDisk );

            if( FF_isERR( xError ) == pdFALSE )
            {
                /* Record the partition number the FF_Disk_t structure is, then
                mount the partition. */
                pxDisk->xStatus.bPartitionNumber = spiPARTITION_NUMBER;

                /* Mount the partition. */
                xError = FF_Mount( pxDisk, spiPARTITION_NUMBER );
                FF_PRINTF( "FF_SPIDiskInit: FF_Mount: %s\n", ( const char * ) FF_GetErrMessage( xError ) );
            }

            if( FF_isERR( xError ) == pdFALSE )
            {
                /* The partition mounted successfully, add it to the virtual
                file system - where it will appear as a directory off the file
                system's root directory. */
                FF_FS_Add( pcName, pxDisk );
            }
        }
        else
        {
            FF_PRINTF( "FF_RAMDiskInit: FF_CreateIOManger: %s\n", ( const char * ) FF_GetErrMessage( xError ) );

            /* The disk structure was allocated, but the disk's IO manager could
            not be allocated, so free the disk again. */
            FF_SPIDiskDelete( pxDisk );
            pxDisk = NULL;
        }
#else
        if( pxDisk->pxIOManager == NULL )
        {
            FF_PRINTF( "FF_SDDiskInit: FF_CreateIOManger: %s\n", (const char*)FF_GetErrMessage( xFFError ) );
            FF_SPIDiskDelete( pxDisk );
            pxDisk = NULL;
        }
        else
        {
            pxDisk->xStatus.bIsInitialised = pdTRUE;
            pxDisk->xStatus.bPartitionNumber = xPartitionNumber;
            if( FF_SPIDiskMount( pxDisk ) == 0 )
            {
                FF_SPIDiskDelete( pxDisk );
                pxDisk = NULL;
            }
            else
            {
                if( pcName == NULL )
                {
                    pcName = "/";
                }
                FF_FS_Add( pcName, pxDisk );
                FF_PRINTF( "FF_SDDiskInit: Mounted SD-card as root \"%s\"\n", pcName );
                FF_SPIDiskShowPartition( pxDisk );
            }
        }
#endif
    }
    else
    {
        FF_PRINTF( "FF_SPIDiskInit: Malloc failed\n" );
    }

    return pxDisk;
}
/*-----------------------------------------------------------*/

BaseType_t FF_SPIDiskFormat( FF_Disk_t *pxDisk, BaseType_t xPartitionNumber )
{
FF_Error_t xError;
BaseType_t xReturn = pdFAIL;

	xError = FF_Unmount( pxDisk );

	if( FF_isERR( xError ) != pdFALSE )
	{
		FF_PRINTF( "FF_SPIDiskFormat: unmount fails: %08x\n", ( unsigned ) xError );
	}
	else
	{
		/* Format the drive - try FAT32 with large clusters. */
		xError = FF_Format( pxDisk, xPartitionNumber, pdFALSE, pdFALSE);

		if( FF_isERR( xError ) )
		{
			FF_PRINTF( "FF_SPIDiskFormat: %s\n", (const char*)FF_GetErrMessage( xError ) );
		}
		else
		{
			FF_PRINTF( "FF_SPIDiskFormat: OK, now remounting\n" );
			pxDisk->xStatus.bPartitionNumber = xPartitionNumber;
			xError = FF_SPIDiskMount( pxDisk );
			FF_PRINTF( "FF_SPIDiskFormat: rc %08x\n", ( unsigned )xError );
			if( FF_isERR( xError ) == pdFALSE )
			{
				xReturn = pdPASS;
			}
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t FF_SPIDiskUnmount( FF_Disk_t *pxDisk )
{
FF_Error_t xFFError;
BaseType_t xReturn = pdPASS;

	if( ( pxDisk != NULL ) && ( pxDisk->xStatus.bIsMounted != pdFALSE ) )
	{
		pxDisk->xStatus.bIsMounted = pdFALSE;
		xFFError = FF_Unmount( pxDisk );

		if( FF_isERR( xFFError ) )
		{
			FF_PRINTF( "FF_SDDiskUnmount: rc %08x\n", ( unsigned )xFFError );
			xReturn = pdFAIL;
		}
		else
		{
			FF_PRINTF( "Drive unmounted\n" );
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t FF_SPIDiskMount( FF_Disk_t *pxDisk )
{
FF_Error_t xFFError;
BaseType_t xReturn;

    /* Mount the partition */
    xFFError = FF_Mount( pxDisk, pxDisk->xStatus.bPartitionNumber );

    if( FF_isERR( xFFError ) )
    {
        FF_PRINTF( "FF_SPIDiskMount: %08lX\n", xFFError );
        xReturn = pdFAIL;
    }
    else
    {
        pxDisk->xStatus.bIsMounted = pdTRUE;
        FF_PRINTF( "****** FreeRTOS+FAT initialized %lu sectors\n", pxDisk->pxIOManager->xPartition.ulTotalSectors );
        xReturn = pdPASS;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/* Release all resources */
BaseType_t FF_SPIDiskDelete( FF_Disk_t *pxDisk )
{
	if( pxDisk != NULL )
	{
		pxDisk->ulSignature = 0;
		pxDisk->xStatus.bIsInitialised = 0;
		if( pxDisk->pxIOManager != NULL )
		{
			if( FF_Mounted( pxDisk->pxIOManager ) != pdFALSE )
			{
				FF_Unmount( pxDisk );
			}
			FF_DeleteIOManager( pxDisk->pxIOManager );
		}

		vPortFree( pxDisk );
	}
	return 1;
}
/*-----------------------------------------------------------*/

BaseType_t FF_SPIDiskShowPartition( FF_Disk_t *pxDisk )
{
FF_Error_t xError;
uint64_t ullFreeSectors;
uint32_t ulTotalSizeMB, ulFreeSizeMB;
int iPercentageFree;
FF_IOManager_t *pxIOManager;
const char *pcTypeName = "unknown type";
BaseType_t xReturn = pdPASS;

    if( pxDisk == NULL )
    {
        xReturn = pdFAIL;
    }
    else
    {
        pxIOManager = pxDisk->pxIOManager;

        FF_PRINTF( "Reading FAT and calculating Free Space\n" );

        switch( pxIOManager->xPartition.ucType )
        {
            case FF_T_FAT12:
                pcTypeName = "FAT12";
                break;

            case FF_T_FAT16:
                pcTypeName = "FAT16";
                break;

            case FF_T_FAT32:
                pcTypeName = "FAT32";
                break;

            default:
                pcTypeName = "UNKOWN";
                break;
        }

        FF_GetFreeSize( pxIOManager, &xError );

        ullFreeSectors = pxIOManager->xPartition.ulFreeClusterCount * pxIOManager->xPartition.ulSectorsPerCluster;
        iPercentageFree = ( int ) ( ( spiHUNDRED_64_BIT * ullFreeSectors + pxIOManager->xPartition.ulDataSectors / 2 ) /
            ( ( uint64_t )pxIOManager->xPartition.ulDataSectors ) );

        ulTotalSizeMB = pxIOManager->xPartition.ulDataSectors / spiSECTORS_PER_MB;
        ulFreeSizeMB = ( uint32_t ) ( ullFreeSectors / spiSECTORS_PER_MB );

        /* It is better not to use the 64-bit format such as %Lu because it
        might not be implemented. */
        FF_PRINTF( "Partition Nr   %8u\n", pxDisk->xStatus.bPartitionNumber );
        FF_PRINTF( "Type           %8u (%s)\n", pxIOManager->xPartition.ucType, pcTypeName );
        FF_PRINTF( "VolLabel       '%8s' \n", pxIOManager->xPartition.pcVolumeLabel );
        FF_PRINTF( "TotalSectors   %8lu\n", pxIOManager->xPartition.ulTotalSectors );
        FF_PRINTF( "SecsPerCluster %8lu\n", pxIOManager->xPartition.ulSectorsPerCluster );
        FF_PRINTF( "Size           %8lu MB\n", ulTotalSizeMB );
        FF_PRINTF( "FreeSize       %8lu MB ( %d perc free )\n", ulFreeSizeMB, iPercentageFree );
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

void FF_SPIDiskFlush( FF_Disk_t *pxDisk )
{
    if( ( pxDisk != NULL ) && ( pxDisk->xStatus.bIsInitialised != 0 ) && ( pxDisk->pxIOManager != NULL ) )
    {
        FF_FlushCache( pxDisk->pxIOManager );
    }
}
/*-----------------------------------------------------------*/

static BaseType_t prvSPIFLASHInit( BaseType_t xDriveNumber )
{
BaseType_t xReturn;

    /* 'xDriveNumber' not yet in use. */
    ( void ) xDriveNumber;

    if( xSDCardSemaphore == NULL )
    {
        xSDCardSemaphore = xSemaphoreCreateBinary();
    }
    /* Check if the SD card is plugged in the slot */
    if( FLASH_Init() == pdFALSE )
    {
        FF_PRINTF( "No Spi Flash detected\n" );
        xReturn = pdFAIL;
    }
    else
    {
        FF_PRINTF( "FLASH_Init: type: %s Capacity: %lu MB\n",
            "SPI",
            WINBOND_FLASH_SIZE/1024/1024 );

        xReturn = pdPASS;
    }
    return xReturn;
}
/*-----------------------------------------------------------*/

//void SPI0_M_IRQHandler(void)
//{
//BaseType_t xHigherPriorityTaskWoken = 0;

//    cc6801_SPI0_M_IRQHandler();

//    if( xSDCardSemaphore != NULL )
//    {
//        xSemaphoreGiveFromISR( xSDCardSemaphore, &xHigherPriorityTaskWoken );
//    }
//    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//}

void prvEventSetupFunction( void )
{
    /* A DMA transfer to or from the SD-card is about to start.
    Reset the timers that will be used in prvEventWaitFunction() */
    xDMARemainingTime = sdMAX_TIME_TICKS;
    vTaskSetTimeOutState( &xDMATimeOut );
}

uint32_t prvEventWaitFunction( void )
{
    uint32_t ulReturn;

    /*
     * It was measured how quickly a DMA interrupt was received. It varied
     * between 0 and 4 ms.
     * <= 1 ms : 8047
     * <= 2 ms : 1850
     * <= 3 ms :   99
     * <= 4 ms :   79
     * >= 5 ms :    0 times
     */

    if( xTaskCheckForTimeOut( &xDMATimeOut, &xDMARemainingTime ) != pdFALSE )
    {
        /* The timeout has been reached, no need to block. */
        ulReturn = 1UL;
    }
    else
    {
        /* The timeout has not been reached yet, block on the semaphore. */
        xSemaphoreTake( xSDCardSemaphore, xDMARemainingTime );
        if( xTaskCheckForTimeOut( &xDMATimeOut, &xDMARemainingTime ) != pdFALSE )
        {
            ulReturn = 1UL;
        }
        else
        {
            ulReturn = 0UL;
        }
    }

    return ulReturn;
}
