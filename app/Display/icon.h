/* Copyright (c) 2017 CloudChip Inc. All Rights Reserved.
 *
 * The information contained herein is property of CloudChip ASA.
 * Terms and conditions of usage are described in detail in CloudChip Inc
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */ 
#ifndef _ICON_H_
#define _ICON_H_

// for downsize code use:
/*
#define _accharging             _acbatteryfull
#define _acbattery_LowPower     _acbatteryfull
#define _acbatteryLevel4        _acbatteryfull
#define _acbatteryLevel3        _acbatteryfull
#define _acbatteryLevel2        _acbatteryfull
#define _acbatteryLevel1        _acbatteryfull
#define _acbattery              _acbatteryfull

#define _acpedometer1           _acbatteryfull
#define _acCalorie              _acbatteryfull
#define _acswiming              _acbatteryfull
#define _acmessage              _acbatteryfull
#define _acDistance             _acbatteryfull
#define _acincommingcall        _acbatteryfull
#define _acsms                  _acbatteryfull
#define _aclongsit              _acbatteryfull
#define _acAlarm                _acbatteryfull
*/




extern unsigned char _acPedometer[];
extern unsigned char _acpedometer1[];
extern unsigned char _acHRM[];
extern unsigned char _acCalorie[];
extern unsigned char _acswiming[];
extern unsigned char _acmessage[];
extern unsigned char _acDistance[];
extern unsigned char _acbattery[];
extern unsigned char _acincommingcall[];
extern unsigned char _acsms[];
extern unsigned char _aclongsit[];
extern unsigned char _acAlarm[];
extern unsigned char _acBatteryCharging24x24[];
extern unsigned char _acBatteryCharging16x16[];
extern unsigned char _achrmclose[];
extern unsigned char _accharging[];
extern unsigned char _acbattery_LowPower[];
extern unsigned char _acbattery[];
extern unsigned char _acbatteryfull[];
extern unsigned char _acbatteryLevel4[];
extern unsigned char _acbatteryLevel3[];
extern unsigned char _acbatteryLevel2[];
extern unsigned char _acbatteryLevel1[];

#endif //_ICON_H_

