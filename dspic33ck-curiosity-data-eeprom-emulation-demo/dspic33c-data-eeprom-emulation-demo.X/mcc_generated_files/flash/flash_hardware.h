
/**
 * FLASH Generated Driver Header File 
 * 
 * @file        flash.h
 * 
 * @ingroup     flashdriver
 * 
 * @brief       This is the generated driver header file for the FLASH driver
 *
 * @version     Driver Version 1.0.1
 *
 * @skipline    Device : dsPIC33CK256MP508
*/

/*
© [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef FLASH_HARDWARE_H
#define FLASH_HARDWARE_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "flash_types.h"

/**
 @ingroup  flashdriver
 @brief    Defines FLASH unlock key value
*/
#define FLASH_UNLOCK_KEY  0x00AA0055
/**
 @ingroup  flashdriver
 @brief    Defines FLASH write row size in instruction counts
*/
#define FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS  128U
/**
 @ingroup  flashdriver
 @brief    Defines FLASH write row size in PC counts
*/
#define FLASH_WRITE_ROW_SIZE_IN_PC_UNITS (FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS*2U)
/**
 @ingroup  flashdriver
 @brief    Defines FLASH erase page size in instruction counts
*/
#define FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS 1024U
/**
 @ingroup  flashdriver
 @brief    Defines FLASH erase page size in PC counts
*/
#define FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS  (FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS*2U)
/**
 @ingroup  flashdriver
 @brief    Defines if FLASH has ECC
*/
#define FLASH_HAS_ECC  1
/**
 @ingroup  flashdriver
 @brief    Defines FLASH erase page mask value
*/
#define FLASH_ERASE_PAGE_MASK (~((FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS*2UL) - 1U)) 


/**
 * @ingroup flashdriver
 * @brief This routine erases a page indicated by the page aligned address.
 * @param[in] address - Flash page aligned address.  
 * @return bool - Returns True if operation successful else returns false.  
 */
bool FLASH_ErasePage(flash_adr_t address, flash_key_t unlockKey);

/* The address in the Read and Write word functions below must be even */

/**
 * @ingroup flashdriver
 * @brief This routine reads a 24 bit instruction from the indicated address in flash.
 * @param[in] address - Flash address.  
 * @return uint32_t - 24 bit data.  
 */
uint32_t FLASH_ReadWord24(flash_adr_t address);


/**
 * @ingroup flashdriver
 * @brief This routine reads a 16 bit instruction from the indicated address in flash.
 * @param[in] address - Flash address.  
 * @return uint16_t - 16 bit data.  
 */
uint16_t FLASH_ReadWord16(flash_adr_t address);


/**
 * @ingroup flashdriver
 * @brief This routine writes two 24 bit instructions to the indicated address in flash.
 * @param[in] address - Flash address.
 * @param[in] data1   - First 24 bit data to be written(First word).
 * @param[in] data2   - Second 24 bit data to be written(Second word).  
 * @return bool - Returns true if operation is successful else returns false.  
 */
bool FLASH_WriteDoubleWord24(flash_adr_t address, flash_data_t Data0, flash_data_t Data1, flash_key_t unlockKey);


/**
 * @ingroup flashdriver
 * @brief This routine writes two ,16 bit instructions to the indicated address in flash.
 * @param[in] address - Flash address.
 * @param[in] data1   - First 16 bit data to be written(First word).
 * @param[in] data2   - Second 16 bit data to be written(Second word).  
 * @return bool - Returns true if operation is successful else returns false.  
 */
bool FLASH_WriteDoubleWord16(flash_adr_t flashAddress, uint16_t Data0, uint16_t Data1, flash_key_t unlockKey);


/* Program the flash one row at a time. */


/**
 * @ingroup flashdriver
 * @brief
 *    This routine writes a single row of data from the location given in *data to the flash location in address. 
 *    Since the flash is only 24 bits wide all data in the upper 8 bits of the source will be lost.The address in 
 *    *data must be row aligned.
 * @param[in] address - Flash address.
 * @param[in] *data    - Address of the data to be written.  
 * @return bool - Returns true if operation is successful else returns false.  
 */
bool FLASH_WriteRow24(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey);


/**
 * @ingroup flashdriver
 * @brief
 *    This routine writes a single row of data from the location given in *data to the flash location in address. 
 *    Each 16 bit source data word is stored in the lower 16 bits of each flash entry and the upper 8 bits of the 
 *    flash is not programmed.The address in *data must be row aligned.
 * @param[in] address - Flash address.
 * @param[in] *data   - Address of the data to be written.  
 * @return bool - Returns true if operation is successful else returns false.  
 */
bool FLASH_WriteRow16(flash_adr_t address, uint16_t *data, flash_key_t unlockKey);


/**
 * @ingroup flashdriver
 * @brief This routiner returns the page offset the given flash address.
 * @param[in] address - Flash address.  
 * @return uint32_t - returns offset.  
 */
uint16_t FLASH_ErasePageOffsetGet(flash_adr_t address);


/**
 * @ingroup flashdriver
 * @brief This routine returns the page aligned address for a given flash address..
 * @param[in] address - Flash address.  
 * @return uint32_t - Returns page aligned flash address.  
 */
uint32_t FLASH_ErasePageAddressGet(flash_adr_t address);

#endif    /* FLASH_HARDWARE_H */



