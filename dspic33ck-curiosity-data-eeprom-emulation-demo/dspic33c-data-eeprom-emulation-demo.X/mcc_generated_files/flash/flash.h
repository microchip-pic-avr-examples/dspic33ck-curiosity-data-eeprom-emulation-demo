
/**
 * FLASH Generated Driver Header File 
 * 
 * @file         flash.h
 * 
 * @ingroup      flashdriver Flash Driver
 * 
 * @brief        FLASH driver using dsPIC MCUs
 *
 * @version      Firmware Driver Version 1.0.3
 *
 * @version      PLIB Version 3.1.0-dev.1
 *
 * @skipline     Device : dsPIC33CK256MP508
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

#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "flash_hardware.h"
#include "flash_interface.h"

// Section: Data Type Definitions

/**
 @ingroup   flashdriver
 @brief     Structure object of type FLASH_INTERFACE assigned with name 
            displayed in the Melody Driver User interface. A structure pointer can be used to achieve portability
            across the FLASH having same interface structure.
*/

extern const struct FLASH_INTERFACE flash;

/**
 @ingroup  flashdriver
 @enum     FLASH_RETURN_STATUS
 @brief    This enum is be used to return the status of write, read and erase opertation.
*/
enum FLASH_RETURN_STATUS {
    FLASH_NO_ERROR,                     /**< No error occurred */
    FLASH_INVALID_ADDRESS,              /**< Invalid address */
    FLASH_INVALID_DATA,                 /**< Invalid data */
    FLASH_WRITE_ERROR,                  /**< Write error has occurred */
    FLASH_READ_ERROR,                   /**< Read error has occurred */
};

/**
 * @ingroup flashdriver
 * @brief This function erases a page.
 * @param[in]  flashAddress : Flash address 
 * @param[in]  unlockKey : Pointer of the data to be written  
 * @return  FLASH_RETURN_STATUS  
 */
enum FLASH_RETURN_STATUS FLASH_PageErase(flash_adr_t flashAddress, flash_key_t unlockKey);


/**
 * @ingroup flashdriver
 * @brief This function writes the specified data to the specified address.
 * @param[in]  flashAddress : Flash address 
 * @param[in]  data : Pointer of the data to be written.  
 *                    This will write the TWO WORDS pointed to by the pointer.                       
 * @return  FLASH_RETURN_STATUS  
 */
enum FLASH_RETURN_STATUS FLASH_WordWrite(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey);


/**
 * @ingroup flashdriver
 * @brief This function writes the specified data to the specified address.
 * @param[in]  flashAddress : Flash address 
 * @param[in]  data : Pointer of the data to be written  
 * @return  FLASH_RETURN_STATUS  
 */
enum FLASH_RETURN_STATUS FLASH_RowWrite(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey);


/**
 * @ingroup flashdriver
 * @brief This function reads the data from the specified address.
 * @param[in]  flashAddress : Flash address of 
 * @param[out]  data : Pointer to read the data  
 * @return  FLASH_RETURN_STATUS  
 */
enum FLASH_RETURN_STATUS FLASH_Read(flash_adr_t flashAddress, size_t count, flash_data_t *data);


#endif    /* FLASH_H */



