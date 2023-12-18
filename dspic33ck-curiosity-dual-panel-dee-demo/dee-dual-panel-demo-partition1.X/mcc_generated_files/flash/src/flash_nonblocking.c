/**
 * FLASH Generated Driver Source File
 *
 * @file      flash.c
 *
 * @ingroup   Flash Driver
 *
 * @brief     This is the generated driver source file for FLASH driver
 *
 * @skipline  @version   Firmware Driver Version 1.1.0
 *
 * @skipline  @version   PLIB Version 3.2.1
 *
 * @skipline  Device : dsPIC33CK256MP508
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

#include <stddef.h>
#include "../flash_nonblocking.h"

// User callback and context handles.
static void (*userCallbackHandler)(void *) = NULL;
static void *userContext = NULL;
bool lock = false;


// Section: Driver Interface
const struct FLASH_INTERFACE flash = {
    .PageAddressGet = FLASH_ErasePageAddressGet,
    .PageOffsetGet = FLASH_ErasePageOffsetGet,
    .NonBlockingPageErase = FLASH_NonBlockingPageErase,
    .NonBlockingBulkErase = FLASH_NonBlockingBulkErase,
    .NonBlockingRead = FLASH_NonBlockingRead,
    .NonBlockingWordWrite = FLASH_NonBlockingWordWrite,
    .NonBlockingRowWrite = FLASH_NonBlockingRowWrite,
    .NonBlockingPolledPageErase = NULL,
    .NonBlockingPolledBulkErase = NULL,
    .NonBlockingPolledRead = NULL,
    .NonBlockingPolledWordWrite = NULL,
    .NonBlockingPolledRowWrite = NULL,
    .OperationStatusGet = FLASH_OperationStatusGet

};

void FLASH_Initialize(void)
{
    // Flash Interrupt
    IFS0bits.NVMIF = 0U;
    IEC0bits.NVMIE = 1U;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _NVMInterrupt(void)
{
    IFS0bits.NVMIF = 0U;

    //Interrupt means operation completion, release the lock
    lock = false;

    if (NULL != userCallbackHandler) {
        (*userCallbackHandler)(userContext);
    }
}


enum FLASH_RETURN_STATUS FLASH_OperationStatusGet()
{
    enum FLASH_RETURN_STATUS flashReturnStatus = FLASH_NO_ERROR;
    if(NVMCONbits.WR)
    {
        flashReturnStatus = FLASH_OP_BUSY;
    }
    else
    {
        if(NVMCON & FLASH_ERROR_MASK)
        {
            flashReturnStatus = FLASH_WRITE_ERROR;
        }
        lock = false;
    }
    return flashReturnStatus;
}


enum FLASH_RETURN_STATUS FLASH_NonBlockingPageErase(uint32_t flashAddress, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context)
{
    enum FLASH_RETURN_STATUS flashReturnStatus = FLASH_OP_IN_PROGRESS;
    if(false == lock)
    {
        if (0U != ((uint16_t)flashAddress & ~FLASH_ERASE_PAGE_MASK))
        {
            flashReturnStatus = FLASH_INVALID_ADDRESS;
        }
        else if(NULL == callbackHandler)
        {
            flashReturnStatus  = FLASH_INVALID_CALLBACK_HANDLER;
        }
        else if(0U != NVMCONbits.WR)
        {
            flashReturnStatus = FLASH_OP_BUSY;
        }
        else
        {
            userCallbackHandler = callbackHandler;
            userContext = context;
            lock = true;
            bool status = FLASH_NonBlockingErasePage(flashAddress, unlockKey);
            if(!status)
            {
                lock = false;
                flashReturnStatus =  FLASH_WRITE_ERROR;
            }
        }
    }
    return flashReturnStatus;
}


enum FLASH_RETURN_STATUS FLASH_NonBlockingBulkErase(enum FLASH_PANEL panel, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context)
{
    enum FLASH_RETURN_STATUS flashReturnStatus = FLASH_OP_IN_PROGRESS;
    if(false == lock)
    {
        if ((panel < 0) || (panel >= FLASH_PANEL_MAX_PANELS))
        {
            flashReturnStatus = FLASH_INVALID_ADDRESS;
        }
        else if(NULL == callbackHandler)
        {
            flashReturnStatus  = FLASH_INVALID_CALLBACK_HANDLER;
        }
        else if(0U != NVMCONbits.WR)
        {
            flashReturnStatus = FLASH_OP_BUSY;
        }
        else
        {
            userCallbackHandler = callbackHandler;
            userContext = context;
            lock = true;
            bool status = FLASH_NonBlockingEraseBulk(panel, unlockKey);
            if(!status)
            {
                lock = false;
                flashReturnStatus =  FLASH_WRITE_ERROR;
            }
        }
    }
    return flashReturnStatus;
}

enum FLASH_RETURN_STATUS FLASH_NonBlockingWordWrite(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context)
{
    enum FLASH_RETURN_STATUS flashReturnStatus = FLASH_OP_IN_PROGRESS;
    if(false == lock)
    {
        if (NULL == data)
        {
            flashReturnStatus = FLASH_INVALID_DATA;
        }
        else if (0U != ((uint16_t)flashAddress & FLASH_ODD_ADDRESS_MASK))
        {
            flashReturnStatus = FLASH_INVALID_ADDRESS;
        }
        else if(NULL == callbackHandler)
        {
            flashReturnStatus = FLASH_INVALID_CALLBACK_HANDLER;
        }
        else if(0U != NVMCONbits.WR)
        {
            flashReturnStatus = FLASH_OP_BUSY;
        }
        else
        {
            // Save the handler and context to call for completion of this operation.
            userCallbackHandler = callbackHandler;
            userContext = context;
            lock = true;
            bool status = FLASH_NonBlockingWriteDoubleWord24(flashAddress, *data, *(data+1), unlockKey);
            if(!status)
            {
                lock = false;
                flashReturnStatus =  FLASH_WRITE_ERROR;
            }
        }
    }
    return flashReturnStatus;
}

enum FLASH_RETURN_STATUS FLASH_NonBlockingRowWrite(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey, FLASH_CALLBACK callbackHandler, void* context)
{
    enum FLASH_RETURN_STATUS flashReturnStatus = FLASH_OP_IN_PROGRESS;
    if(false == lock)
    {
        if (NULL == data)
        {
            flashReturnStatus = FLASH_INVALID_DATA;
        }
        else if (0U != ((uint16_t)flashAddress & FLASH_ODD_ADDRESS_MASK))
        {
            flashReturnStatus = FLASH_INVALID_ADDRESS;
        }
        else if(NULL == callbackHandler)
        {
            flashReturnStatus = FLASH_INVALID_CALLBACK_HANDLER;
        }
        else if(0U != NVMCONbits.WR)
        {
            flashReturnStatus = FLASH_OP_BUSY;
        }
        else
        {
            // Save the handler and context to call for completion of this operation.
            userCallbackHandler = callbackHandler;
            userContext = context;
            lock = true;
            bool status = FLASH_NonBlockingWriteRow24(flashAddress, data, unlockKey);
            if(!status)
            {
                lock = false;
                flashReturnStatus =  FLASH_WRITE_ERROR;
            }
        }
    }
    return flashReturnStatus;
}


enum FLASH_RETURN_STATUS FLASH_NonBlockingRead(flash_adr_t flashAddress, size_t count, flash_data_t *data)
{
    enum FLASH_RETURN_STATUS flashReturnStatus = FLASH_OP_BUSY;
    if(false == lock)
    {
        if ((NULL == data) || (0 == count))
        {
            flashReturnStatus = FLASH_INVALID_DATA;
        }
        else if (0U != ((uint16_t)flashAddress & FLASH_ODD_ADDRESS_MASK))
        {
            flashReturnStatus = FLASH_INVALID_ADDRESS;
        }
        else if(0U == NVMCONbits.WR)
        {
            lock = true;
            do
            {
                *data++ = FLASH_NonBlockingReadWord24(flashAddress);
                flashAddress = flashAddress + FLASH_PC_UNIT_SIZE;
            } while(--count);

            flashReturnStatus = FLASH_NO_ERROR;

            // Now that the read is done, the lock can be cleared.
            lock = false;
        }
    }
    return flashReturnStatus;
}
