/**
 ****************************************************************************************
 *
 * @file nvds.c
 *
 * @brief Non Volatile Data Storage (NVDS) driver
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup NVDS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>      // string definitions
#include <stddef.h>      // standard definitions
#include "nvds.h"        // nvds definitions
#include "arch.h"        // main
#include "co_math.h"     // math operations
//#include "flash.h"       // flash definition
#include "jump_table.h"
/*
 * DEFINES
 ****************************************************************************************
 */

#if 0
/// NVDS parameter data maximum length
#define NVDS_PARAMETER_MAX_LENGTH   NVDS_LEN_BASIC_THRESHOLD

/// TAG STATUS bit assignment
#define NVDS_STATUS_VALID_MASK   0x01
#define NVDS_STATUS_VALID        0x00
#define NVDS_STATUS_NOT_VALID    0x01
#define NVDS_STATUS_LOCKED_MASK  0x02
#define NVDS_STATUS_LOCKED       0x00
#define NVDS_STATUS_NOT_LOCKED   0x02
#define NVDS_STATUS_ERASED_MASK  0x04
#define NVDS_STATUS_ERASED       0x00
#define NVDS_STATUS_NOT_ERASED   0x04

#if (NVDS_READ_WRITE == 1)
/// Max storage for the NVDS device which can be used for tags
#define NVDS_MAX_STORAGE_SIZE    0x0800 // 2KB
#endif //(NVDS_READ_WRITE == 1)

// NVDS Mapping

/// Magic number offset
#define NVDS_MAGIC_NUMBER_ADDRESS         0x0000
/// Size of magic number
#define NVDS_MAGIC_NUMBER_LENGTH               4

/// Start of NVDS data
#if (NVDS_PACKED == 1)
#define NVDS_START_STORAGE_AREA_ADDRESS   \
    NVDS_MAGIC_NUMBER_ADDRESS + NVDS_MAGIC_NUMBER_LENGTH
#else //(NVDS_PACKED == 0)
#define NVDS_START_STORAGE_AREA_ADDRESS   \
    CO_ALIGN4_HI(NVDS_MAGIC_NUMBER_ADDRESS) + \
    CO_ALIGN4_HI(NVDS_MAGIC_NUMBER_LENGTH)
#endif //(NVDS_PACKED == 1)

/// Value found in flash when nothing has been written
#define NVDS_NO_TAG              0xFF


/*
 * MACROS
 ****************************************************************************************
 */

/// Check is tag is the last one
#define NVDS_IS_TAG_LAST(h) \
    ((h).tag == NVDS_NO_TAG)
/// Check is tag is valid
#define NVDS_IS_TAG_OK(h) \
    ((((h).status) & (NVDS_STATUS_VALID_MASK|NVDS_STATUS_ERASED_MASK)) == \
     (NVDS_STATUS_VALID|NVDS_STATUS_NOT_ERASED))
/// Check is tag is locked
#define NVDS_IS_TAG_LOCKED(h) \
    ((((h).status) & NVDS_STATUS_LOCKED_MASK) == NVDS_STATUS_LOCKED)
/// Set tag as erased
#define NVDS_SET_TAG_ERASED(h) \
    ((((h).status) & (~NVDS_STATUS_ERASED_MASK)) | NVDS_STATUS_ERASED)
/// Set tag as locked
#define NVDS_SET_TAG_LOCKED(h) \
    ((((h).status) & (~NVDS_STATUS_LOCKED_MASK)) | NVDS_STATUS_LOCKED)
/// Set tag as valid
#define NVDS_SET_TAG_OK(h) \
    (NVDS_STATUS_VALID | NVDS_STATUS_NOT_LOCKED | NVDS_STATUS_NOT_ERASED)


/// Macro for alignment
#if (NVDS_PACKED == 1)
#define NVDS_ALIGNMENT(p) (p)
#else //(NVDS_PACKED == 0)
#define NVDS_ALIGNMENT(p) CO_ALIGN4_HI(p)
#endif //(NVDS_PACKED == 1)

/// Length of tag header
#define NVDS_TAG_HEADER_LENGTH \
    NVDS_ALIGNMENT(sizeof(struct nvds_tag_header))
/// Length of tag data
#define NVDS_TAG_CONTENT_LENGTH(h) \
    NVDS_ALIGNMENT((h).length)
/// Full length of tag (header+data)
#define NVDS_TAG_FULL_LENGTH(h) \
    NVDS_TAG_HEADER_LENGTH + NVDS_TAG_CONTENT_LENGTH(h)

/*
 * STRUCT DEFINITIONS
 ****************************************************************************************
 */

/// Structure defining the header of a TAG.  It is very important that the TAG remains
/// the first element of the structure because it defines the LAST TAG of the NVDS when
/// set the oxFF.
struct nvds_tag_header
{
    /// current TAG identifier
    uint8_t  tag;
    /// status of the TAG (erased, locked ...)
    uint8_t  status;
    /// length of the TAG
    nvds_tag_len_t length;
};


/// Environment structure of the NVDS module
struct nvds_env_tag
{
    /// Function to read the device Address being in the NVDS memory space
    void  (*read)(uint32_t const                    address,
                  uint32_t const                    length,
                  uint8_t* const                    buf);
    /// Function to write the device Address being in the NVDS memory space
    void (*write)(uint32_t const                    address,
                  uint32_t const                    length,
                  uint8_t* const                    buf);
    /// Function to erase the entire NVDS memory space
    void (*erase)(uint32_t const                    address,
                  uint32_t const                    length);

    /// NVDS base pointer
    uint8_t *nvds_space;

    /// Total size of the NVDS area
    uint32_t   total_size;

    /// Flash ID
    uint8_t flash_id;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

#if (NVDS_READ_WRITE == 1)
/// temporary buffer used for purging
//static uint8_t nvds_temp_buf[NVDS_MAX_STORAGE_SIZE];
#endif //(NVDS_READ_WRITE == 1)

/// NVDS magic number keyword
//static const uint8_t nvds_magic_number[NVDS_MAGIC_NUMBER_LENGTH] = {'N', 'V', 'D', 'S'};

/// NVDS environment
//static struct nvds_env_tag nvds_env;

/*
 * LOCAL FUNCTION DECLARATION
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Check if the current NVDS has the correct magic number set.
 *
 * Implementation notes:we do not put an assert on the read access because we could be in
 * the situation of a dummy read (returns always NVDS_FAIL) and we want to return
 * correctly the FALSE.
 *
 * @return  True if the NVDS has the Magic Number set, false otherwise.
 ****************************************************************************************
 */
//static bool nvds_is_magic_number_ok(void);

/**
 ****************************************************************************************
 * @brief Look for a specific TAG.
 *
 * If found, it returns the address and the header, otherwise the TAG address returned
 * points to a location where it is possible to store a new TAG.
 * The TAG is returned only if it is valid (not erased).  This function is useful to find
 * a single valid TAG element or find the next available space for a new TAG.
 *
 * @param[in] tag                  TAG to look for
 * @param[out] nvds_tag_header_ptr Pointer to the TAG header structure allocated by the
 *                                 caller to contain the searched TAG header
 * @param[out] tag_address_ptr     Pointer to the NVDS address at which TAG was found
 *                                 (returned) or if the TAG was not found, first address
 *                                 free for storing new TAG in NVDS
 *
 * @return  Return codes from the @ref nvds_walk_tag function call
 ****************************************************************************************
 */
//static uint8_t nvds_browse_tag(uint8_t tag,
//                           struct nvds_tag_header *nvds_tag_header_ptr,
//                          uint32_t *tag_address_ptr);

/**
 ****************************************************************************************
 * @brief Read the TAG header that MUST be present at NVDS address cur_tag_addr and fill
 * the TAG header structure that is allocated by the caller and referenced by
 * nvds_tag_header_ptr.
 *
 * Upon completion of the read, the next TAG address is computed and returned to the
 * caller through nxt_tag_addr_ptr (except if the current TAG is the LAST one).
 * If the caller wishes to read the first TAG of the NVDS, the value
 * NVDS_START_STORAGE_AREA_ADDRESS can be used as the cur_tag_addr.
 * If the current Address specified is pointing at the position of the last element of
 * the NVDS the function returns NVDS_TAG_NOT_DEFINED.  In this case, there is NO VALUE
 * returned through nxt_tag_addr_ptr.  The cur_tag_addr is already pointing to an empty
 * TAG.
 * The TAG read is not check for validity, this information should be handled by the
 * caller if he wishes to use the TAG information correctly.
 *
 * @param[in] cur_tag_addr         Address of the current TAG in NVDS memory space
 * @param[out] nvds_tag_header_ptr A pointer to an allocated space for the parameter header
 * @param[out] nxt_tag_addr_ptr    A pointer to the next TAG address in the NVDS memory space
 *
 * @return  NVDS_OK                 TAG read, header filled and next TAG address filled
 *          NVDS_TAG_NOT_DEFINED    Last TAG reached, header filled with garbage
 *          NVDS_CORRUPT            Current TAG is overcoming the NVDS size limit
 ****************************************************************************************
 */
//static uint8_t nvds_walk_tag (uint32_t cur_tag_addr,
//                           struct nvds_tag_header *nvds_tag_header_ptr,
//                           uint32_t *nxt_tag_addr_ptr);

/**
 ****************************************************************************************
 * @brief Hook a dummy driver to the NVDS.
 * If no valid NVDS media was found, to avoid incorrect behavior a dummy driver should
 * be hooked to the NVDS.
 *
 * @return NVDS_OK
 ****************************************************************************************
 */
//static uint8_t nvds_null_init(void);

/**
 ****************************************************************************************
 * @brief Dummy function to safely replace Read function
 *
 * @param[in]  address  Start address of the data to read from NVDS
 * @param[in]  length   Length of the data to read from NVDS
 * @param[in]  buf      Pointer to the buffer containing the DATA to read from the NVDS
 ****************************************************************************************
 */
//static void nvds_null_read(uint32_t address, uint32_t length, uint8_t *buf);

/**
 ****************************************************************************************
 * @brief Dummy function to safely replace Write function
 *
 * @param[in]  address NVDS address at which the write operation must be performed
 * @param[in]  length  Length of the write operation to perform
 * @param[in]  buf     Pointer to a buffer containing the data to write
 ****************************************************************************************
 */
//static void nvds_null_write(uint32_t address, uint32_t length, uint8_t *buf);

/**
 ****************************************************************************************
 * @brief Dummy function to safely replace Erase function
 * @param[in]  address NVDS address at which the erase operation must be performed
 * @param[in]  length  Length of the erase operation to perform
 ****************************************************************************************
 */
//static void nvds_null_erase(uint32_t address, uint32_t length);

/**
 ****************************************************************************************
 * @brief Read data from NVDS.
 *
 * @param[in]  address  Start address of the data to read from NVDS
 * @param[in]  length   Length of the data to read from NVDS
 * @param[in]  buf      Pointer to the buffer containing the DATA to read from the NVDS
 ****************************************************************************************
 */
//static void nvds_read(uint32_t address, uint32_t length, uint8_t *buf);

#if (NVDS_READ_WRITE == 1)
/**
 ****************************************************************************************
 * @brief Write data into NVDS
 *
 * @param[in]  address  Start address of the data to write to NVDS
 * @param[in]  length   Length of the data to write to NVDS
 * @param[in]  buf      Pointer to the buffer containing the DATA to write to the NVDS
 ****************************************************************************************
 */
//static void nvds_write(uint32_t address, uint32_t length, uint8_t *buf);

/**
 ****************************************************************************************
 * @brief Erase data in NVDS
 *
 * @param[in]  address  Start address of the data to read from NVDS
 * @param[in]  length   Length of the data to read from NVDS
 ****************************************************************************************
 */
//static void nvds_erase(uint32_t address, uint32_t length);

/**
 ****************************************************************************************
 * @brief Initialize the NVDS memory.
 *
 * This function clears the entire memory content and writes the MagicNumber
 ****************************************************************************************
 */
//static void nvds_init_memory(void);

/**
 ****************************************************************************************
 * @brief Purge NVDS memory
 *
 * This function performs a read of all the valid TAGs of the NVDS, stores them in
 * the temporary buffer allocated by the caller, flushes the NVDS and then rewrites all
 * the valid TAGs.
 *
 * It is used to purge the NVDS when there is no more space to store a new TAG for
 * example or regularly to save TAG browse time.
 *
 * @param[in]  length  Length of the buffer allocated to perform the temporary storage of
 *                     the NVDS while purging (erase and compress)
 * @param[in]  buf     A pointer to the buffer allocated by the caller for the temporary
 *                     storage of the NVDS while purging
 ****************************************************************************************
 */
//static void nvds_purge(uint32_t length, uint8_t* buf);
#endif  //(NVDS_READ_WRITE == 1)

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
#if 0
static bool nvds_is_magic_number_ok(void)
{
    bool is_magic_number_ok = false;
    uint8_t read_magic_number[NVDS_MAGIC_NUMBER_LENGTH];

    // Look for the magic number
    nvds_env.read(NVDS_MAGIC_NUMBER_ADDRESS, sizeof(read_magic_number), read_magic_number);

    // Compare the read magic number with the correct value
    if (memcmp(read_magic_number, nvds_magic_number, NVDS_MAGIC_NUMBER_LENGTH)==0)
    {
        is_magic_number_ok = true;
    }
    return is_magic_number_ok;
}

static uint8_t nvds_walk_tag (uint32_t cur_tag_addr,
                                  struct nvds_tag_header *nvds_tag_header_ptr,
                                  uint32_t *nxt_tag_addr_ptr)
{
    uint8_t status = NVDS_OK;



    // Read the current parameter header
    nvds_env.read((uint32_t)cur_tag_addr,
                  (uint32_t)sizeof(struct nvds_tag_header),
                  (uint8_t*)nvds_tag_header_ptr);

    // Check if the read operation completed successfully
    if (!NVDS_IS_TAG_LAST(*nvds_tag_header_ptr))
    {
        // Calculate the address of the next tag
        *nxt_tag_addr_ptr = cur_tag_addr + NVDS_TAG_FULL_LENGTH(*nvds_tag_header_ptr);

        // Check if there is enough space to read next header
        // the limit is set minus 1 because we need to leave at least an end marker
        if (*nxt_tag_addr_ptr > (nvds_env.total_size - 1))
        {
            // Going above NVDS limit, probably an error occurred
            ASSERT_ERR(0);
            status = NVDS_CORRUPT;
        }
    }
    else
    {
        // this is beyond the last TAG
        status = NVDS_TAG_NOT_DEFINED;
    }
    return(status);
}

static uint8_t nvds_browse_tag (uint8_t tag,
                                    struct nvds_tag_header *nvds_tag_header_ptr,
                                    uint32_t *tag_address_ptr)
{
    uint8_t status;
    uint32_t cur_tag_addr, nxt_tag_addr;



    // set the address to the first data byte of the NVDS
    nxt_tag_addr = NVDS_START_STORAGE_AREA_ADDRESS;

    do
    {
        // go to the next tag
        cur_tag_addr = nxt_tag_addr;

        // retrieve the parameter header
        status = nvds_walk_tag(cur_tag_addr, nvds_tag_header_ptr, &nxt_tag_addr);

    } while ((status == NVDS_OK) &&
             !((nvds_tag_header_ptr->tag == tag) && NVDS_IS_TAG_OK(*nvds_tag_header_ptr)));

    // the returned address is the last address found
    *tag_address_ptr = cur_tag_addr;

    return(status);
}

static void nvds_null_read(uint32_t address, uint32_t length, uint8_t *buf){}

static void nvds_null_write(uint32_t address, uint32_t length, uint8_t *buf){}

static void nvds_null_erase(uint32_t address, uint32_t length){}

static uint8_t nvds_null_init(void)
{

	if(jump_table_struct[JT_POS_PARA_NVDS_ENABLE] == 0)
	{
		return NVDS_FAIL;
	}

    // init all the structure
    memset( &nvds_env, 0, sizeof(nvds_env));
    nvds_env.read  = nvds_null_read;
    nvds_env.write = nvds_null_write;
    nvds_env.erase = nvds_null_erase;

    return NVDS_OK;
}

static void nvds_read(uint32_t address, uint32_t length, uint8_t *buf)
{

    // Test the validity of address + length
    ASSERT_ERR(((address + length) <= nvds_env.total_size));

    // Read the memory
    flash_read(nvds_env.flash_id, (uint32_t)nvds_env.nvds_space + address, length, buf, NULL);
}

#if (NVDS_READ_WRITE == 1)
static void nvds_write(uint32_t address, uint32_t length, uint8_t *buf)
{


    // Test the validity of address + length
    ASSERT_ERR(((address + length) <= nvds_env.total_size));

    // Read the memory
    flash_write(nvds_env.flash_id, (uint32_t)nvds_env.nvds_space + address, length, buf, NULL);
}

static void nvds_erase(uint32_t address, uint32_t length)
{

    // Erase NVDS
    flash_erase(nvds_env.flash_id, (uint32_t)nvds_env.nvds_space + address, length, NULL);
}

static void nvds_init_memory(void)
{

    // clear the device
    nvds_env.erase((uint32_t)NVDS_MAGIC_NUMBER_ADDRESS, nvds_env.total_size);

    // Write the magic number at address 0
    nvds_env.write((uint32_t)NVDS_MAGIC_NUMBER_ADDRESS,
                   (uint32_t)NVDS_MAGIC_NUMBER_LENGTH,
                   (uint8_t*)nvds_magic_number);
}

static void nvds_purge(uint32_t length, uint8_t* buf)
{
    uint8_t status;
    struct nvds_tag_header tag_hdr;
    uint32_t cur_tag_addr, nxt_tag_addr;
    uint32_t total_length;
    uint8_t *walk_ptr;


    // store all the valid TAG elements in the locally allocated buffer
    total_length = 0;
    nxt_tag_addr = NVDS_START_STORAGE_AREA_ADDRESS;
    walk_ptr = buf;
    do
    {
        // go to the next tag
        cur_tag_addr = nxt_tag_addr;

        status = nvds_walk_tag(cur_tag_addr, (struct nvds_tag_header*)&tag_hdr, &nxt_tag_addr);

        if ((status == NVDS_OK) && NVDS_IS_TAG_OK(tag_hdr))
        {
            // check that the current size is not overcoming the buffer
            total_length += NVDS_TAG_FULL_LENGTH(tag_hdr);
            ASSERT_ERR(total_length <= length);

            // copy the header content
            *((struct nvds_tag_header*)walk_ptr) = tag_hdr;

            // increment the pointer to the data part
            walk_ptr += NVDS_TAG_HEADER_LENGTH;
            cur_tag_addr += NVDS_TAG_HEADER_LENGTH;

            // retrieve all the data part
            nvds_env.read((uint32_t)cur_tag_addr, (uint32_t)tag_hdr.length, walk_ptr);

            // increment the walking pointer
            walk_ptr += NVDS_TAG_CONTENT_LENGTH(tag_hdr);
        }

    } while (status == NVDS_OK);

    // reinitialize the flash
    nvds_init_memory();

    // rewrite the NVDS once cleaned
    nvds_env.write((uint32_t)NVDS_START_STORAGE_AREA_ADDRESS,
                   (uint32_t)total_length,
                   buf);
}

#endif  //(NVDS_READ_WRITE == 1)
#endif


#endif

/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */


uint8_t app_nvds_init(uint8_t *base, uint32_t len){uint8_t ret = NVDS_FAIL; return ret;}
uint8_t app_nvds_del(uint8_t tag){uint8_t ret = NVDS_FAIL; return ret;} 
uint8_t app_nvds_put(uint8_t tag, uint8_t length, uint8_t *buf){uint8_t ret = NVDS_FAIL; return ret;}
uint8_t app_nvds_get(uint8_t tag, uint8_t * lengthPtr, uint8_t *buf){uint8_t ret = NVDS_FAIL; return ret;}







/// @} NVDS

