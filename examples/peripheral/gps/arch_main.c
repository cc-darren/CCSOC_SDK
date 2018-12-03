/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ll.h"
#include "drvi_init.h"
#include "drvi_wktm.h"
#include "tracer.h"
#include "project.h"
#include "gps.h"

extern void sys_InitMain(void);

const GpsInterface *g_pCxd5603;
GpsCallbacks g_fpCxd5603Cb;

static int g_iPositioning = 0;

void DumpLocationInfo(GpsLocation *r)
{
    char   temp[256];
    char*  p   = temp;
    char*  end = p + sizeof(temp);
    struct tm   *utc;

    p += snprintf( p, end-p, "sending fix" );

    if (r->flags & GPS_LOCATION_HAS_LAT_LONG)
    {
        p += snprintf(p, end-p, " lat=%0.9f lon=%0.9f", r->latitude, r->longitude);
    }
    if (r->flags & GPS_LOCATION_HAS_ALTITUDE)
    {
        p += snprintf(p, end-p, " altitude=%0.9f", r->altitude);
    }
    if (r->flags & GPS_LOCATION_HAS_SPEED)
    {
        p += snprintf(p, end-p, " speed=%g", r->speed);
    }
    if (r->flags & GPS_LOCATION_HAS_BEARING)
    {
        p += snprintf(p, end-p, " bearing=%g", r->bearing);
    }
    if (r->flags & GPS_LOCATION_HAS_ACCURACY)
    {
        p += snprintf(p,end-p, " accuracy=%g", r->accuracy);
    }

    utc = localtime((time_t*) &r->timestamp);
    p += snprintf(p, end-p, " time=%s", asctime(utc) );

    TracerInfo("%s", temp);
}

void cxd5603_LocationCb(GpsLocation *pLocation)
{
    if (pLocation->flags != 0)
        DumpLocationInfo(pLocation);

    g_iPositioning = 1;
}


/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    int iResult = 0;

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START();

    drvi_initialize();

    TracerInfo("== CC6801 Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    g_pCxd5603 = get_gps_interface();
    g_fpCxd5603Cb.location_cb = cxd5603_LocationCb;

    iResult = g_pCxd5603->init(&g_fpCxd5603Cb);
    if (iResult != CC_SUCCESS)
        TracerInfo("GPS initialize fail...\r\n");

    TracerInfo("GPS start...\r\n");
    g_pCxd5603->start();
    if (iResult != CC_SUCCESS)
        TracerInfo("GPS start fail...\r\n");

    TracerInfo("GPS positioning...\r\n");
    while(!g_iPositioning)
    {
        g_pCxd5603->proc();
    }

    TracerInfo("GPS stop...\r\n");
    iResult = g_pCxd5603->stop();
    if (iResult != CC_SUCCESS)
        TracerInfo("GPS stop fail...\r\n");

    TracerInfo("GPS start...\r\n");
    g_pCxd5603->start();
    if (iResult != CC_SUCCESS)
        TracerInfo("GPS start fail...\r\n");
    while(1)
    {
        g_pCxd5603->proc();
    }
}

