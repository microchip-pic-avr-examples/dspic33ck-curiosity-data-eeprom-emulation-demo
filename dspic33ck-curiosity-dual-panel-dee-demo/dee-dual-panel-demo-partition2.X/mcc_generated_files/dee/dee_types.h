/**
  DEE Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    dee_types.h

  @Summary
    This is the generated header file for the DEE library using Melody Code Configurator

  @Description
    This header file provides APIs for library DEE.
    The generated drivers are tested against the following:
*/

#ifndef DEE_TYPES_H
#define DEE_TYPES_H

#define DATA_EE_BANKS                     1
#define DATA_EE_SIZE                      255
#define DATA_EE_TOTAL_SIZE                (DATA_EE_BANKS * DATA_EE_SIZE)
#define NUM_DATA_EE_PAGES                 2
#define	NUMBER_OF_INSTRUCTIONS_IN_PAGE    1024
#define	NUMBER_OF_INSTRUCTIONS_IN_ROW     128
// If the device has ECC
#define __HAS_ECC	                        1
#define ERASE_WRITE_CYCLE_MAX             10000
#define NUMBER_OF_ROWS_IN_PAGE            (_FLASH_PAGE / _FLASH_ROW)
#define PAGE_AVAILABLE                    1
#define PAGE_CURRENT                      0
#define PAGE_EXPIRED                      0
#define PAGE_NOT_AVAILABLE                0
#define PAGE_NOT_CURRENT                  1
#define PAGE_NOT_EXPIRED                  1
#define STATUS_AVAILABLE                  18
#define STATUS_CURRENT                    19
#define STATUS_EXPIRED                    20
#define ERASE_STATE                       0xFFFFFF
#define DEE_STATUS_MASK                   0xE3
#define PACKED_PAGE_DATA_SIZE             0x100
#define MAX_DATA_SIZE_PER_PAGE            0xFF
#define DEE_ADDRESS_MASK                  0x00FF0000
#define WRITE_RETRY_COUNT                 2
#define FLASH_WORD_WRITE_SIZE             2
#define DEE_START_ADDRESS_INACTIVE_PARTITION 0x414800
#define DEE_START_ADDRESS_ACTIVE_PARTITION   0x14800

/*Data EE info stored in PM in following format
  Status in first two locations of PM page,
  8-bit DEE Address (odd address, low byte) 16-bit DEE data (even address)*/
#define BYTES_PER_DATA_EE                 2
#define DEE_BANK_SIZE                     (NUMBER_OF_INSTRUCTIONS_IN_PAGE * BYTES_PER_DATA_EE * NUM_DATA_EE_PAGES)
#define DEE_PAGE_SIZE                     (NUMBER_OF_INSTRUCTIONS_IN_PAGE * BYTES_PER_DATA_EE)
#define DEE_DATA_SIZE_IN_BYTES            (DATA_EE_BANKS * NUM_DATA_EE_PAGES * NUMBER_OF_INSTRUCTIONS_IN_PAGE * BYTES_PER_DATA_EE)


typedef union
{
    uint8_t val;
    struct
    {
        unsigned addrNotFound:1;
        unsigned expiredPage:1;
        unsigned packBeforePageFull:1;
        unsigned packBeforeInit:1;
        unsigned packSkipped:1;
        unsigned IllegalAddress:1;
        unsigned pageCorrupt:1;
        unsigned writeError:1;
    };
} DATA_EE_FLAGS;


// User constant validation
#if DATA_EE_BANKS == 0
    #error Minimum data EE banks is 1
#endif

#if DATA_EE_SIZE > 255
    #error Maximum data EE size is 255
#endif

#if NUM_DATA_EE_PAGES < 2
    #error Minimum number of program memory pages is 2
#endif

#if ERASE_WRITE_CYCLE_MAX > 65535
    #error Maximum number of erase/write cycles is 65,535
#endif

#endif	/* DEE_TYPES_H */