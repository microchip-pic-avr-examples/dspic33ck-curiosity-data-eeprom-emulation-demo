/**
  DEE Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    dee.h

  @Summary
    This is the generated header file for the DEE library using Melody Code Configurator

  @Description
    This header file provides APIs for library DEE.
    The generated drivers are tested against the following:
*/

#ifndef DEE_H
#define DEE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum{
    DEE_NO_ERROR,
    DEE_EXPIRED_PAGE,
    DEE_PACK_BEFORE_PAGE_FULL,
    DEE_PACK_BEFORE_INIT,
    DEE_PACK_SKIPPED,
    DEE_ILLEGAL_ADDRESS,
    DEE_PAGE_CORRUPT,
    DEE_WRITE_ERROR,
    DEE_READ_ERROR,
}DEE_RETURN_STATUS;


/**
  @Summary
    This function initializes all of the EEPROM banks

  @Description
    This routine finds an unexpired page to become an active page. It then
    counts the number of active pages. If no active pages are found, the
    first unexpired page is initialized for emulation. If one active page is
    found, it is assumes a reset occurred and the function does nothing. If
    two active pages are found, it is assumes a reset occurred during a pack.
    The second page is erased and a pack is called. If three, an error code
    is returned as the allocated memory is assumed to be corrupted. This
    function must be called prior to any other operation.

  @Param
    None.

  @Returns
    Returns DEE_RETURN_STATUS
*/
DEE_RETURN_STATUS  DEE_Init(void);

/**
  @Summary
    This function is used to retrieve data EEPROM information.

  @Description
    This routine verifies the address is valid. If not, the Illegal Address
    flag is set and return DEE_ILLEGAL_ADDRESS. It then finds the active page,if an
    active page can not be found, the Page Corrupt status bit is set and
    DEE_PAGE_CORRUPT is returned. A reverse search of the active page attempts to find
    the matching address in the program memory MSB (odd address). If a match
    is found, the corresponding data EEPROM data (even address) is returned,
    otherwise DEE_READ_ERROR is returned. This function can be called by the user.

  @Param
     Data EE address and pointer to hold the read flash data.

  @Returns
    Returns DEE_RETURN_STATUS
*/
DEE_RETURN_STATUS DEE_Read(uint16_t address, uint16_t *data);

/**
  @Summary
    This function is used to write emulated data EEPROM.

  @Description
    This routine verifies the address is valid. If not, the Illegal Address
    flag is set and an error code is returned. It then finds the active page.
    If an active page can not be found, the Page Corrupt status bit is set
    and an error code is returned. A read is performed, if the data was not
    changed, the function exits. If the last location is programmed, the Pack
    Skipped error flag is set (one location should always be available). The
    data EE information (MSB = address, LSW = data) is programmed and
    verified. If the verify fails, the Write Error flag is set. If the write
    went into the last location of the page, pack is called. This function
    can be called by the user.

  @Param
    Data EE address and data to be written.

  @Returns
    Returns DEE_RETURN_STATUS
*/
DEE_RETURN_STATUS DEE_Write(uint16_t address, uint16_t data);


#endif	/* DEE_H */