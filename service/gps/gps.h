#include <stdint.h>

/** Milliseconds since January 1, 1970 */
typedef int64_t GpsUtcTime;

/** Maximum number of SVs for gps_sv_status_callback(). */
#define GPS_MAX_SVS 32

/** GpsLocation has valid latitude and longitude. */
#define GPS_LOCATION_HAS_LAT_LONG   0x0001
/** GpsLocation has valid altitude. */
#define GPS_LOCATION_HAS_ALTITUDE   0x0002
/** GpsLocation has valid speed. */
#define GPS_LOCATION_HAS_SPEED      0x0004
/** GpsLocation has valid bearing. */
#define GPS_LOCATION_HAS_BEARING    0x0008
/** GpsLocation has valid accuracy. */
#define GPS_LOCATION_HAS_ACCURACY   0x0010

/** Represents a location. */
typedef struct {
    /** set to sizeof(GpsLocation) */
    size_t          size;
    /** Contains GpsLocationFlags bits. */
    uint16_t        flags;
    /** Represents latitude in degrees. */
    double          latitude;
    /** Represents longitude in degrees. */
    double          longitude;
    /** Represents altitude in meters above the WGS 84 reference
     * ellipsoid. */
    double          altitude;
    /** Represents speed in meters per second. */
    float           speed;
    /** Represents heading in degrees. */
    float           bearing;
    /** Represents expected accuracy in meters. */
    float           accuracy;
    /** Timestamp for the location fix. */
    GpsUtcTime      timestamp;
} GpsLocation;

/** Represents SV information. */
typedef struct {
    /** set to sizeof(GpsSvInfo) */
    size_t          size;
    /** Pseudo-random number for the SV. */
    int     prn;
    /** Signal to noise ratio. */
    float   snr;
    /** Elevation of SV in degrees. */
    float   elevation;
    /** Azimuth of SV in degrees. */
    float   azimuth;
} GpsSvInfo;

/** Represents SV status. */
typedef struct {
    /** set to sizeof(GpsSvStatus) */
    size_t          size;

    /** Number of SVs currently visible. */
    int         num_svs;

    /** Contains an array of SV information. */
    GpsSvInfo   sv_list[GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs
     * have ephemeris data.
     */
    uint32_t    ephemeris_mask;

    /** Represents a bit mask indicating which SVs
     * have almanac data.
     */
    uint32_t    almanac_mask;

    /**
     * Represents a bit mask indicating which SVs
     * were used for computing the most recent position fix.
     */
    uint32_t    used_in_fix_mask;
} GpsSvStatus;

/** Callback with location information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (* gps_location_callback)(GpsLocation* location);

/** GPS callback structure. */
typedef struct {
    gps_location_callback location_cb;
} GpsCallbacks;


/** Represents the standard GPS interface. */
typedef struct {
    /** set to sizeof(GpsInterface) */
    size_t          size;
    /**
     * Opens the interface and provides the callback routines
     * to the implemenation of this interface.
     */
    int   (*init)( GpsCallbacks* callbacks );

    /** Starts navigating. */
    int   (*start)( void );

    /** Process navigation data. */
    int   (*proc)( void );

    /** Stops navigating. */
    int   (*stop)( void );

    /** Closes the interface. */
    void  (*cleanup)( void );
} GpsInterface;

const GpsInterface* get_gps_interface(void);

