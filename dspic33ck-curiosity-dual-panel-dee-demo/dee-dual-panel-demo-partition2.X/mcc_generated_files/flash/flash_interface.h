
/**
 * flash Generated Driver Interface Header File
 *
 * @file        flash_interface.h
 *
 * @defgroup    Flash Driver
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

#ifndef FLASH_INTERFACE_H
#define FLASH_INTERFACE_H

// Section: Included Files
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "flash_types.h"

// Section: Data Type Definitions


/**
 @ingroup flashdriver
 @struct FLASH_INTERFACE
 @brief Structure containing the function pointers of FLASH driver.
*/
struct FLASH_INTERFACE
{
    enum FLASH_RETURN_STATUS (*PageErase)(flash_adr_t address, flash_key_t unlockKey);
    ///< Pointer to FLASH_PageErase e.g. \ref FLASH_PageErase

    enum FLASH_RETURN_STATUS (*Write)(flash_adr_t address, flash_data_t* data, flash_key_t unlockKey);
    ///< Pointer to FLASH_WordWrite e.g. \ref FLASH_WordWrite

    enum FLASH_RETURN_STATUS (*RowWrite)(flash_adr_t address, flash_data_t* data, flash_key_t unlockKey);
    ///< Pointer to FLASH_RowWrite e.g. \ref FLASH_RowWrite

    enum FLASH_RETURN_STATUS (*Read)(flash_adr_t address, size_t count, uint32_t *data);
    ///< Pointer to FLASH_Read e.g. \ref FLASH_Read

    uint32_t (*PageAddressGet)(flash_adr_t address);
    ///< Pointer to FLASH_ErasePageAddressGet e.g. \ref FLASH_ErasePageAddressGet

    uint16_t (*PageOffsetGet)(flash_adr_t address);
    ///< Pointer to FLASH_ErasePageOffsetGet e.g. \ref FLASH_ErasePageOffsetGet

    enum FLASH_RETURN_STATUS (*OperationStatusGet)(void);
    ///< Pointer to FLASH_OperationStatusGet e.g. \ref FLASH_OperationStatusGet

    enum FLASH_RETURN_STATUS (*NonBlockingPageErase)(flash_adr_t address, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context);
    ///< Pointer to FLASH_NonBlockingPageErase e.g. \ref FLASH_NonBlockingPageErase

    enum FLASH_RETURN_STATUS (*NonBlockingBulkErase)(enum FLASH_PANEL panel, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context);
    ///< Pointer to FLASH_NonBlockingBulkErase e.g. \ref FLASH_NonBlockingBulkErase

    enum FLASH_RETURN_STATUS (*NonBlockingWordWrite)(flash_adr_t address, flash_data_t* data, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context);
    ///< Pointer to FLASH_NonBlockingWordWrite e.g. \ref FLASH_NonBlockingWordWrite

    enum FLASH_RETURN_STATUS (*NonBlockingRowWrite)(flash_adr_t address, flash_data_t* data, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context);
    ///< Pointer to FLASH_NonBlockingRowWrite e.g. \ref FLASH_NonBlockingRowWrite

    enum FLASH_RETURN_STATUS (*NonBlockingRead)(flash_adr_t address, size_t count, uint32_t *data);
    ///< Pointer to FLASH_NonBlockingRead e.g. \ref FLASH_NonBlockingRead

    enum FLASH_RETURN_STATUS (*NonBlockingPolledPageErase)(flash_adr_t address, flash_key_t unlockKey);
    ///< Pointer to FLASH_NonBlockingPolledPageErase e.g. \ref FLASH_NonBlockingPolledPageErase

    enum FLASH_RETURN_STATUS (*NonBlockingPolledBulkErase)(enum FLASH_PANEL panel, flash_key_t unlockKey);
    ///< Pointer to FLASH_NonBlockingPolledBulkErase e.g. \ref FLASH_NonBlockingPolledBulkErase

    enum FLASH_RETURN_STATUS (*NonBlockingPolledWordWrite)(flash_adr_t address, flash_data_t* data, flash_key_t unlockKey);
    ///< Pointer to FLASH_NonBlockingPolledWordWrite e.g. \ref FLASH_NonBlockingPolledWordWrite

    enum FLASH_RETURN_STATUS (*NonBlockingPolledRowWrite)(flash_adr_t address, flash_data_t* data, flash_key_t unlockKey);
    ///< Pointer to FLASH_NonBlockingPolledRowWrite e.g. \ref FLASH_NonBlockingPolledRowWrite

    enum FLASH_RETURN_STATUS (*NonBlockingPolledRead)(flash_adr_t address, size_t count, uint32_t *data);
    ///< Pointer to FLASH_NonBlockingPolledRead e.g. \ref FLASH_NonBlockingPolledRead
};

#endif //FLASH_INTERFACE_H
