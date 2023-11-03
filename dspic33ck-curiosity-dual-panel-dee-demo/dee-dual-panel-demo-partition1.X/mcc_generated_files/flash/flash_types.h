
/**
 * flash Generated Driver Types Header File
 *
 * @file        flash_types.h
 *
 * @ingroup     Flash Driver
 *
 * @brief       FLASH Driver using dsPIC MCUs
 *
 * @skipline  @version   Firmware Driver Version 1.1.0
 *
 * @skipline @version     PLIB Version 3.2.1
 *
 * @skipline    Device : dsPIC33CK256MP508
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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

#ifndef FLASH_TYPES_H
#define FLASH_TYPES_H

typedef void (*FLASH_CALLBACK)(void *);

typedef uint32_t flash_adr_t;
typedef uint32_t flash_data_t;
typedef uint32_t flash_key_t;

/**
 @ingroup  flashdriver
 @enum     FLASH_PANEL
 @brief    This enum is be used for the flash panel numbers and maximum flash panels.
*/
enum FLASH_PANEL
{
    FLASH_PANEL_1 = 0,
    FLASH_PANEL_2 = 1,
    FLASH_PANEL_MAX_PANELS
};

/**
 @ingroup  flashdriver
 @enum     FLASH_RETURN_STATUS
 @brief    This enum is be used to return the status of write, read and erase operation.
*/
enum FLASH_RETURN_STATUS {
    FLASH_NO_ERROR,                     /**< No error occurred */
    FLASH_INVALID_ADDRESS,              /**< Invalid address */
    FLASH_INVALID_DATA,                 /**< Invalid data */
    FLASH_WRITE_ERROR,                  /**< Write error has occurred */
    FLASH_READ_ERROR,                   /**< Read error has occurred */
    FLASH_ERASE_ERROR,                  /**< Flash erase error occurred */
    FLASH_INVALID_CALLBACK_HANDLER,     /**< Invalid parameter to operation */
    FLASH_OP_BUSY,                      /**< Flash is physically busy */
    FLASH_OP_IN_PROGRESS                /**< Flash operation is in progress */
};

#endif //FLASH_TYPES_H
