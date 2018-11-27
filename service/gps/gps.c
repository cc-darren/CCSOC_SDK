#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gps.h"
#include "tracer.h"

#define NMEA_GGA 0x01
#define NMEA_GLL 0x02
#define NMEA_GSA 0x04
#define NMEA_GSV 0x08
#define NMEA_GNS 0x10
#define NMEA_RMC 0x20
#define NMEA_VTG 0x40
#define NMEA_ZDA 0x80
#define NMEA_MASK 0xFF

#define SLEEP_0 0
#define SLEEP_1 1
#define SLEEP_2 2


#define CMD_GCD              "GCD"
#define CMD_GSP              "GSP"
#define CMD_GSTP             "GSTP"
#define CMD_SLP              "SLP"
#define CMD_VER              "VER"
#define CMD_WUP              "WUP"
#define CMD_BSSL             "BSSL"

#define GPS_CMD(_x)          "@"##_x##"\r\n"
#define GPS_CMD_ARG(_x,_y)   "@"##_x##" "STRINGIZE(_y)"\r\n"

#define GPS_TIMEOUT          (SYSTEM_CLOCK_MHZ*10000)

#define  GPS_EMULATION  0
#define  GPS_DEBUG  0

#if GPS_DEBUG
#  define  D(...)   TracerInfo(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       N M E A   T O K E N I Z E R                     *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

typedef struct {
    const char*  p;
    const char*  end;
} Token;

#define  MAX_NMEA_TOKENS  16

typedef struct {
    int     count;
    Token   tokens[ MAX_NMEA_TOKENS ];
} NmeaTokenizer;

GpsSvStatus gps_sv_status;
unsigned char gps_sv_status_flag = 0;

typedef struct
{
    //used satellites total number
    unsigned int num;
    //used satellites's prn
    int NO[12];
}UsedSatellites;

UsedSatellites used_satellites;

static int
nmea_tokenizer_init( NmeaTokenizer*  t, const char*  p, const char*  end )
{
    int    count = 0;

    // the initial '$' is optional
    if (p < end && p[0] == '$')
        p += 1;

    // remove trailing newline
    if (end > p && end[-1] == '\n') {
        end -= 1;
        if (end > p && end[-1] == '\r')
            end -= 1;
    }

    // get rid of checksum at the end of the sentecne
    if (end >= p+3 && end[-3] == '*') {
        end -= 3;
    }

    while (p < end) {
        const char*  q = p;

        q = memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q > p) {
            if (count < MAX_NMEA_TOKENS) {
                t->tokens[count].p   = p;
                t->tokens[count].end = q;
                count += 1;
            }
        }
        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

static Token
nmea_tokenizer_get( NmeaTokenizer*  t, int  index )
{
    Token  tok;
    static const char*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}


static int
str2int( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;

    for ( ; len > 0; len--, p++ )
    {
        int  c;

        if (p >= end)
            goto Fail;

        c = *p - '0';
        if ((unsigned)c >= 10)
            goto Fail;

        result = result*10 + c;
    }
    return  result;

Fail:
    return -1;
}

static double
str2float( const char*  p, const char*  end )
{
    int   len    = end - p;
    char  temp[16];

    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy( temp, p, len );
    temp[len] = 0;
    return strtod( temp, NULL );
}

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       N M E A   P A R S E R                           *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

#define  NMEA_MAX_SIZE  83

typedef struct {
    int     pos;
    int     overflow;
    int     utc_year;
    int     utc_mon;
    int     utc_day;
    int     utc_diff;
    GpsLocation  fix;
    gps_location_callback  callback;
    char    in[ NMEA_MAX_SIZE+1 ];
} NmeaReader;


//static void
//nmea_reader_update_utc_diff( NmeaReader*  r )
//{
//    time_t         now = time(NULL);
//    struct tm      tm_local;
//    struct tm      tm_utc;
//    long           time_local, time_utc;

//    gmtime_r( &now, &tm_utc );
//    localtime_r( &now, &tm_local );

//    time_local = tm_local.tm_sec +
//                 60*(tm_local.tm_min +
//                 60*(tm_local.tm_hour +
//                 24*(tm_local.tm_yday +
//                 365*tm_local.tm_year)));

//    time_utc = tm_utc.tm_sec +
//               60*(tm_utc.tm_min +
//               60*(tm_utc.tm_hour +
//               24*(tm_utc.tm_yday +
//               365*tm_utc.tm_year)));

//    r->utc_diff = time_utc - time_local;
//}


static void
nmea_reader_init( NmeaReader*  r )
{
    memset( r, 0, sizeof(*r) );

    r->pos      = 0;
    r->overflow = 0;
    r->utc_year = -1;
    r->utc_mon  = -1;
    r->utc_day  = -1;
    r->callback = NULL;
    r->fix.size = sizeof(r->fix);

    //nmea_reader_update_utc_diff( r );
}


static void
nmea_reader_set_callback( NmeaReader*  r, gps_location_callback  cb )
{
    r->callback = cb;
    if (cb != NULL && r->fix.flags != 0) {
        D("%s: sending latest fix to new callback\r\n", __FUNCTION__);
        r->callback( &r->fix );
        r->fix.flags = 0;
    }
}


static int
nmea_reader_update_time( NmeaReader*  r, Token  tok )
{
    int        hour, minute;
    double     seconds;
    struct tm  tm;
    time_t     fix_time;

    if (tok.p + 6 > tok.end)
        return -1;

//TODO: Implement time function
//    if (r->utc_year < 0) {
//        // no date yet, get current one
//        time_t  now = time(NULL);
//        gmtime_r( &now, &tm );
//        r->utc_year = tm.tm_year + 1900;
//        r->utc_mon  = tm.tm_mon + 1;
//        r->utc_day  = tm.tm_mday;
//    }

    hour    = str2int(tok.p,   tok.p+2);
    minute  = str2int(tok.p+2, tok.p+4);
    seconds = str2float(tok.p+4, tok.end);

    tm.tm_hour  = hour;
    tm.tm_min   = minute;
    tm.tm_sec   = (int) seconds;
    tm.tm_year  = r->utc_year - 1900;
    tm.tm_mon   = r->utc_mon - 1;
    tm.tm_mday  = r->utc_day;
    tm.tm_isdst = -1;

    fix_time = mktime( &tm ) + r->utc_diff;
//    r->fix.timestamp = (long long)fix_time * 1000;
    r->fix.timestamp = (long long)fix_time;

    return 0;
}

static int
nmea_reader_update_date( NmeaReader*  r, Token  date, Token  time )
{
    Token  tok = date;
    int    day, mon, year;

    if (tok.p + 6 != tok.end) {
        D("date not properly formatted: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }
    day  = str2int(tok.p, tok.p+2);
    mon  = str2int(tok.p+2, tok.p+4);
    year = str2int(tok.p+4, tok.p+6) + 2000;

    if ((day|mon|year) < 0) {
        D("date not properly formatted: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }

    r->utc_year  = year;
    r->utc_mon   = mon;
    r->utc_day   = day;

    return nmea_reader_update_time( r, time );
}


static double
convert_from_hhmm( Token  tok )
{
    double  val     = str2float(tok.p, tok.end);
    int     degrees = (int)((val == (int)val ? val : (int)val-1) / 100);
    double  minutes = val - degrees*100.;
    double  dcoord  = degrees + minutes / 60.0;
    return dcoord;
}


static int
nmea_reader_update_latlong( NmeaReader*  r,
                            Token        latitude,
                            char         latitudeHemi,
                            Token        longitude,
                            char         longitudeHemi )
{
    double   lat, lon;
    Token    tok;

    tok = latitude;
    if (tok.p + 6 > tok.end) {
        D("latitude is too short: '%.*s'\r\n", tok.end-tok.p, tok.p);
        return -1;
    }
    lat = convert_from_hhmm(tok);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end) {
        D("longitude is too short: '%.*s'\r\n", tok.end-tok.p, tok.p);
        return -1;
    }
    lon = convert_from_hhmm(tok);
    if (longitudeHemi == 'W')
        lon = -lon;

    r->fix.flags    |= GPS_LOCATION_HAS_LAT_LONG;
    r->fix.latitude  = lat;
    r->fix.longitude = lon;
    return 0;
}


static int
nmea_reader_update_altitude( NmeaReader*  r,
                             Token        altitude,
                             Token        units )
{
    Token   tok = altitude;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_ALTITUDE;
    r->fix.altitude = str2float(tok.p, tok.end);
    return 0;
}


static int
nmea_reader_update_bearing( NmeaReader*  r,
                            Token        bearing )
{
    Token   tok = bearing;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_BEARING;
    r->fix.bearing  = str2float(tok.p, tok.end);
    return 0;
}


static int
nmea_reader_update_speed( NmeaReader*  r,
                          Token        speed )
{
    Token   tok = speed;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_SPEED;
    r->fix.speed    = str2float(tok.p, tok.end);
    return 0;
}


static void
nmea_reader_parse( NmeaReader*  r )
{
   /* we received a complete sentence, now parse it to generate
    * a new GPS fix...
    */
    NmeaTokenizer  tzer[1];
    Token          tok;

    D("Received: '%.*s'", r->pos, r->in);

    if (r->pos < 9) {
        D("Too short. discarded.\r\n");
        return;
    }

    nmea_tokenizer_init(tzer, r->in, r->in + r->pos);
#if GPS_DEBUG
    {
        int  n;
        D("Found %d tokens\r\n", tzer->count);
        for (n = 0; n < tzer->count; n++) {
            Token  tok = nmea_tokenizer_get(tzer,n);
            D("%2d: '%.*s'\r\n", n, tok.end-tok.p, tok.p);
        }
    }
#endif

    tok = nmea_tokenizer_get(tzer, 0);
    if (tok.p + 5 > tok.end) {
        D("sentence id '%.*s' too short, ignored.\r\n", tok.end-tok.p, tok.p);
        return;
    }

    // ignore first two characters.
    tok.p += 2;
    if ( !memcmp(tok.p, "GGA", 3) ) {
        // GPS fix
        Token  tok_time          = nmea_tokenizer_get(tzer,1);
        Token  tok_latitude      = nmea_tokenizer_get(tzer,2);
        Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,3);
        Token  tok_longitude     = nmea_tokenizer_get(tzer,4);
        Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,5);
        Token  tok_altitude      = nmea_tokenizer_get(tzer,9);
        Token  tok_altitudeUnits = nmea_tokenizer_get(tzer,10);

        nmea_reader_update_time(r, tok_time);
        nmea_reader_update_latlong(r, tok_latitude,
                                      tok_latitudeHemi.p[0],
                                      tok_longitude,
                                      tok_longitudeHemi.p[0]);
        nmea_reader_update_altitude(r, tok_altitude, tok_altitudeUnits);
    }
    else if ( !memcmp(tok.p, "GSA", 3) )
    {
        Token  tok_accuracy = nmea_tokenizer_get(tzer,15);
        r->fix.accuracy = str2float(tok_accuracy.p,tok_accuracy.end);
        r->fix.flags |= GPS_LOCATION_HAS_ACCURACY;

        int i;
        int used_satellite_NO;
        for(i = 0;i < 12;i++)
        {
            Token tok_used_satellite_NO = nmea_tokenizer_get(tzer,3 + i);
            used_satellite_NO = str2int(tok_used_satellite_NO.p,tok_used_satellite_NO.end);
            if(used_satellite_NO == 0)
                break;
            used_satellites.num++;
            used_satellites.NO[i] = used_satellite_NO;
        }
        gps_sv_status_flag |= 0x01;
    }
    else if ( !memcmp(tok.p, "GSV", 3) )
    {
        Token tok_message_total_num = nmea_tokenizer_get(tzer,1);
        volatile int messages_total_num = str2int(tok_message_total_num.p,tok_message_total_num.end);
        Token tok_message_NO = nmea_tokenizer_get(tzer,2);
        volatile int messages_NO = str2int(tok_message_NO.p,tok_message_NO.end);
        Token tok_satellites_in_view = nmea_tokenizer_get(tzer,3);
        volatile int satellites_in_view = str2int(tok_satellites_in_view.p,tok_satellites_in_view.end);

        if(gps_sv_status_flag & (0x01 << messages_NO))
        {
        //    return;
        }

        gps_sv_status_flag |= (1 << messages_NO);

        gps_sv_status.size    = sizeof(GpsSvStatus);
        gps_sv_status.num_svs = satellites_in_view;

        volatile int tmp;
        if((satellites_in_view / 4) < messages_NO)
        {
            tmp = satellites_in_view % 4;
        }
        else
        {
            tmp = 4;
        }

        D("satellites_in_view = %d messages_NO = %d tmp = %d\n",satellites_in_view,messages_NO,tmp);

        int i;
        for(i = 0;i < tmp;i++)
        {
            gps_sv_status.sv_list[i + 4 * (messages_NO - 1)].size = sizeof(GpsSvInfo);

            Token tok_prn = nmea_tokenizer_get(tzer,4 + i * 4);
            int prn = str2int(tok_prn.p,tok_prn.end);
            gps_sv_status.sv_list[i + 4 * (messages_NO - 1)].prn = prn;

            Token tok_elevation = nmea_tokenizer_get(tzer,5 + i * 4);
            float elevation = str2float(tok_elevation.p,tok_elevation.end);
            gps_sv_status.sv_list[i + 4 * (messages_NO - 1)].elevation = elevation;

            Token tok_azimuth = nmea_tokenizer_get(tzer,6 + i * 4);
            float azimuth = str2float(tok_azimuth.p,tok_azimuth.end);
            gps_sv_status.sv_list[i + 4 * (messages_NO - 1)].azimuth = azimuth;

            Token tok_snr = nmea_tokenizer_get(tzer,7 + i * 4);
            float snr = str2float(tok_snr.p,tok_snr.end);
            gps_sv_status.sv_list[i + 4 * (messages_NO - 1)].snr = snr;

            D("prn:%d elevation:%f azimuth:%f snr:%f\n",prn,elevation,azimuth,snr);
        }

        D("gps_sv_status_flag = %d\n", gps_sv_status_flag);
    }
    else if ( !memcmp(tok.p, "RMC", 3) )
    {
        Token  tok_time          = nmea_tokenizer_get(tzer,1);
        Token  tok_fixStatus     = nmea_tokenizer_get(tzer,2);
        Token  tok_latitude      = nmea_tokenizer_get(tzer,3);
        Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,4);
        Token  tok_longitude     = nmea_tokenizer_get(tzer,5);
        Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,6);
        Token  tok_speed         = nmea_tokenizer_get(tzer,7);
        Token  tok_bearing       = nmea_tokenizer_get(tzer,8);
        Token  tok_date          = nmea_tokenizer_get(tzer,9);

        D("in RMC, fixStatus=%c\r\n", tok_fixStatus.p[0]);
        if (tok_fixStatus.p[0] == 'A')
        {
            nmea_reader_update_date( r, tok_date, tok_time );

            nmea_reader_update_latlong( r, tok_latitude,
                                           tok_latitudeHemi.p[0],
                                           tok_longitude,
                                           tok_longitudeHemi.p[0] );

            nmea_reader_update_bearing( r, tok_bearing );
            nmea_reader_update_speed  ( r, tok_speed );
        }
    } else {
        tok.p -= 2;
        D("unknown sentence '%.*s\r\n", tok.end-tok.p, tok.p);
    }
    if (r->fix.flags != 0) {
#if GPS_DEBUG
        char   temp[256];
        char*  p   = temp;
        char*  end = p + sizeof(temp);
        //struct tm   utc;

        p += snprintf( p, end-p, "sending fix" );
        if (r->fix.flags & GPS_LOCATION_HAS_LAT_LONG) {
            p += snprintf(p, end-p, " lat=%g lon=%g", r->fix.latitude, r->fix.longitude);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_ALTITUDE) {
            p += snprintf(p, end-p, " altitude=%g", r->fix.altitude);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_SPEED) {
            p += snprintf(p, end-p, " speed=%g", r->fix.speed);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_BEARING) {
            p += snprintf(p, end-p, " bearing=%g", r->fix.bearing);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_ACCURACY) {
            p += snprintf(p,end-p, " accuracy=%g", r->fix.accuracy);
        }
        //gmtime_r( (time_t*) &r->fix.timestamp, &utc );
        //p += snprintf(p, end-p, " time=%s", asctime( &utc ) );
        D("%s\r\n", temp);
#endif
        if (r->callback) {
            r->callback( &r->fix );
            r->fix.flags = 0;
        }
        else {
            D("no callback, keeping data until needed !\r\n");
        }
    }
}

static void
nmea_reader_addc( NmeaReader*  r, int  c )
{
    if (r->overflow) {
        r->overflow = (c != '\n');
        return;
    }

    if (r->pos >= (int) sizeof(r->in)-1 ) {
        r->overflow = 1;
        r->pos      = 0;
        return;
    }

    r->in[r->pos] = (char)c;
    r->pos       += 1;

    if (c == '\n') {
        //TODO: change issue a flag
        nmea_reader_parse( r );
        r->pos = 0;
    }
}

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       C O N N E C T I O N   S T A T E                 *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

/* commands sent to the gps thread */
enum {
    CMD_QUIT  = 0,
    CMD_START = 1,
    CMD_STOP  = 2
};

NmeaReader  reader[1];

/* this is the state of our connection to the qemu_gpsd daemon */
typedef struct {
    int                     init;
    GpsCallbacks            callbacks;
} GpsState;

static GpsState  _gps_state[1];

__align(4) static char g_cUartBuf[64] = {0};
static int g_iUartBufIdx = 0;

static void uart_cb(T_UartEvent * p_event)
{
    memcpy(&g_cUartBuf[g_iUartBufIdx], p_event->pRxBuf, p_event->bRxBytes);
    g_iUartBufIdx += p_event->bRxBytes;
}

static void uart_init(void)
{
    uint8_t bCr = 0;

    drvi_UartRxDoneRegister(GPS_IF_ID, uart_cb);
    drvi_UartReceive(GPS_IF_ID, &bCr, sizeof(bCr));
}

static int read_data(char *buf, int size)
{
    int ret = 0;

    if (g_iUartBufIdx <= 0)
        return ret;

    if (size > g_iUartBufIdx)
        ret = g_iUartBufIdx;
    else
        ret = size;

    memcpy(buf, g_cUartBuf, ret);
    g_iUartBufIdx = 0;

    D("READ:%s", buf);

    return ret;
}

int wait_response(char *cmd, int size)
{
#if GPS_EMULATION
    return CC_SUCCESS;
#else
    char buff[16], in[16];
    int pos = 0, started =0;
    int nn, ret, timeout;

    for(timeout = 0;timeout <= GPS_TIMEOUT; timeout++)
    {
        ret = read_data(buff, sizeof(buff));

        for (nn = 0; nn < ret; nn++)
        {
            if (pos > sizeof(in))
                return CC_ERROR_INTERNAL;

            if ((buff[nn] == '[') || (started))
                started = 1;
            else
                continue;

            in[pos++] = buff[nn];

            if (buff[nn] == '\n')
            {
                char response[8] = {0}, result[8] = {0};
                sscanf(in, "[%[^]]] %[^\r\n]", response, result);

                if (strcmp(cmd, response))
                    return CC_ERROR_INTERNAL;

                if (!strcmp(result, "Done"))
                    return CC_SUCCESS;
            }
        }
    }

    return CC_ERROR_INTERNAL;
#endif
}

static int write_cmd(char *cmd, int size)
{
    int ret;
    __align(4) char tmp[16] = {0};

    memset(tmp, 0, sizeof(tmp));
    memcpy(tmp, cmd, size);
    ret = drvi_UartTx(GPS_IF_ID, (uint8_t *)tmp, strlen(tmp));
    D("WRITE:%s", buf);

    return ret;
}

static int
send_command_to_module(const int cmd)
{
    D("%s[%d] enter!",__FUNCTION__,__LINE__);
    int   res;

    switch(cmd)
    {
        case CMD_QUIT:
            write_cmd(GPS_CMD(CMD_GSTP), sizeof(GPS_CMD(CMD_GSTP)));
            res = wait_response(CMD_BSSL, sizeof(CMD_BSSL));
            if (res != CC_SUCCESS)
                break;
            write_cmd(GPS_CMD_ARG(CMD_SLP, SLEEP_2), sizeof(GPS_CMD_ARG(CMD_SLP, SLEEP_2)));
            res = wait_response(CMD_BSSL, sizeof(CMD_BSSL));
            break;
        case CMD_START:
            write_cmd(GPS_CMD(CMD_GSP), sizeof(GPS_CMD(CMD_GSP)));
            res = wait_response(CMD_BSSL, sizeof(CMD_BSSL));
            break;
        case CMD_STOP:
            write_cmd(GPS_CMD(CMD_GSTP), sizeof(GPS_CMD(CMD_GSTP)));
            res = wait_response(CMD_BSSL, sizeof(CMD_BSSL));
            if (res != CC_SUCCESS)
                break;
            write_cmd(GPS_CMD_ARG(CMD_SLP, SLEEP_1), sizeof(GPS_CMD_ARG(CMD_SLP, SLEEP_1)));
            res = wait_response(CMD_BSSL, sizeof(CMD_BSSL));
        default:
            break;
    }

    D("%s[%d] exit!",__FUNCTION__,__LINE__);
    return res;
}

static void
gps_state_done( GpsState*  s )
{
    send_command_to_module(CMD_QUIT);
    s->init = 0;
}

static int
gps_state_start( GpsState*  s )
{
    nmea_reader_set_callback(reader, s->callbacks.location_cb);
    return send_command_to_module(CMD_START);
}


static int
gps_state_stop( GpsState*  s )
{
    nmea_reader_set_callback(reader, NULL);
    return send_command_to_module(CMD_STOP);
}

void gps_state_proc(GpsState *state)
{
    char  buff[32];
    int  nn, ret;

    ret = read_data(buff, sizeof(buff));
    D("received %d bytes: %.*s\r\n", ret, ret, buff);
    for (nn = 0; nn < ret; nn++)
        nmea_reader_addc( reader, buff[nn] );
}

static int
gps_state_init( GpsState*  state, GpsCallbacks* callbacks )
{
    int res = 0;
    state->init       = 1;
    state->callbacks = *callbacks;

    uart_init();
    write_cmd(GPS_CMD(CMD_GCD), sizeof(GPS_CMD(CMD_GCD)));
    res = wait_response(CMD_GCD, sizeof(CMD_GCD));
    if (res != CC_SUCCESS)
        return res;

    write_cmd(GPS_CMD_ARG(CMD_BSSL, NMEA_RMC), sizeof(GPS_CMD_ARG(CMD_BSSL, NMEA_RMC)));
    res = wait_response(CMD_BSSL, sizeof(CMD_BSSL));
    if (res != CC_SUCCESS)
        return res;

    nmea_reader_init(reader);

    D("gps state initialized");
    return res;
}


/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       I N T E R F A C E                               *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/


static int
gps_init(GpsCallbacks* callbacks)
{
    int res = 0;
    GpsState*  s = _gps_state;

    if (!s->init)
        res = gps_state_init(s, callbacks);

    return res;
}

static void
gps_cleanup(void)
{
    GpsState*  s = _gps_state;

    if (s->init)
        gps_state_done(s);
}


static int
gps_start()
{
    GpsState*  s = _gps_state;

    if (!s->init) {
        D("%s: called with uninitialized state !!\r\n", __FUNCTION__);
        return -1;
    }

    D("%s: called", __FUNCTION__);
    gps_state_start(s);
    return 0;
}

static int
gps_proc()
{
    GpsState*  s = _gps_state;

    if (!s->init) {
        D("%s: called with uninitialized state !!\r\n", __FUNCTION__);
        return -1;
    }

    D("%s: called", __FUNCTION__);
    gps_state_proc(s);
    return 0;
}

static int
gps_stop()
{
    GpsState*  s = _gps_state;

    if (!s->init) {
        D("%s: called with uninitialized state !!\r\n", __FUNCTION__);
        return -1;
    }

    D("%s: called\r\n", __FUNCTION__);
    gps_state_stop(s);
    return 0;
}

static const GpsInterface  SonyGpsInterface = {
    sizeof(GpsInterface),
    //when system enable gps , it will be called
    /*
     * Opens the interface and provides the callback routines
     * to the implemenation of this interface.
     */
    gps_init,
    //when app use gps , this will be called second
    /*
     * Starts navigating.
     */
    gps_start,
    /*
     * Process navigation data.
     */
    gps_proc,
    //when app exit , and does not use  gps , this will be called first
    /*
     * Stop navigating.
     */
    gps_stop,
    //when system disable gps , it will be called
    /*
     * Close the interface.
     */
    gps_cleanup,
};

const GpsInterface* get_gps_interface(void)
{
    return &SonyGpsInterface;
}
