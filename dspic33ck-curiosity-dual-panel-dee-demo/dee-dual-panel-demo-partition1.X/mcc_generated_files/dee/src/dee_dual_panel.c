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

#include <stdlib.h>
#include <string.h>
#include "../../flash/flash_nonblocking.h"
#include "../dee.h"
#include "../dee_types.h"

/*Data EE info stored in PM in following format
  Status in first two locations of PM page,
  8-bit DEE Address (odd address, low byte) 16-bit DEE data (even address)*/
uint8_t activePartitionEmulationPages[DEE_DATA_SIZE_IN_BYTES]
    __attribute__ ((space(psv), address(DEE_START_ADDRESS_ACTIVE_PARTITION), aligned(NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2), noload));
#define DEE_PAGE_ADDRESS(bank, page)      ((__builtin_tbladdress(&activePartitionEmulationPages) + (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) & 0xFFFFFF)

#define GetAddrNotFound()                 activePartitionDataEEFlags.addrNotFound
#define SetAddrNotFound(x)                activePartitionDataEEFlags.addrNotFound = x
#define GetPageCorruptStatus()            activePartitionDataEEFlags.pageCorrupt
#define SetPageCorruptStatus(x)           activePartitionDataEEFlags.pageCorrupt = x
#define GetPageExpiredPage()              activePartitionDataEEFlags.expiredPage
#define SetPageExpiredPage(x)             activePartitionDataEEFlags.expiredPage = x
#define GetPageIllegalAddress()           activePartitionDataEEFlags.IllegalAddress
#define SetPageIllegalAddress(x)          activePartitionDataEEFlags.IllegalAddress = x
#define GetPagePackBeforeInit()           activePartitionDataEEFlags.packBeforeInit
#define SetPagePackBeforeInit(x)          activePartitionDataEEFlags.packBeforeInit = x
#define GetPagePackBeforePageFull()       activePartitionDataEEFlags.packBeforePageFull
#define SetPagePackBeforePageFull(x)      activePartitionDataEEFlags.packBeforePageFull = x
#define GetPagePackSkipped()              activePartitionDataEEFlags.packSkipped
#define SetPagePackSkipped(x)             activePartitionDataEEFlags.packSkipped = x
#define GetPageWriteError()               activePartitionDataEEFlags.writeError
#define SetPageWriteError(x)              activePartitionDataEEFlags.writeError = x

DATA_EE_FLAGS activePartitionDataEEFlags;

static uint8_t                  GetPageStatus(uint8_t bank, uint8_t page, uint8_t field);
static void                     ErasePage(uint8_t bank, uint8_t page);
static uint16_t                 GetNextAvailCount(uint8_t bank);
static DEE_RETURN_STATUS        DEE_ActivePartitionPack(uint8_t bank);
static bool                     DEE_IsDataCopyRequired();
static void                     DEE_FlashWrite(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey);
static void                     DEE_FlashRead(flash_adr_t flashAddress, size_t count, flash_data_t *data);
static void                     DEE_RetryDataWrite(flash_adr_t destAddr, flash_data_t *packedPageData);
static void                     DEE_PackedDataGet(flash_data_t *fullPageData, uint32_t fullPageDataSize, flash_data_t *packedPageData);

static void FlashEraseOpHandler(void* context) {
    // Doing nothing
}

static void FlashProgramOpHandler(void *context) {
    // Doing nothing
}


DEE_RETURN_STATUS DEE_ActivePartitionInit(void)
{
    uint8_t pageCnt;
    uint16_t erasePage;
    uint16_t currentPage;
    uint32_t pageAddress;
    uint8_t bank;
    uint32_t statusWord[2];

    activePartitionDataEEFlags.val = 0;
    for (bank = 0; bank < DATA_EE_BANKS; bank++)
    {
        pageCnt = 0;
        erasePage = 0;

        // Find unexpired page
        for (currentPage = 0;
                (currentPage < NUM_DATA_EE_PAGES) &&
                (GetPageStatus(bank, currentPage, STATUS_EXPIRED) == PAGE_EXPIRED);
                currentPage++)
        {
        }

        if (currentPage == NUM_DATA_EE_PAGES)
        {
            SetPageExpiredPage(1);
            return (DEE_EXPIRED_PAGE); // Error - All pages expired
        }
        // Count active page(s).
        for (currentPage = 0; currentPage < NUM_DATA_EE_PAGES; currentPage++)
        {
            if (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_CURRENT)
            {
                pageCnt++;
            }
        }
        //If no active pages found, initialize page 0
        if (pageCnt == 0)
        {
            ErasePage(bank, 0);
            pageAddress = DEE_PAGE_ADDRESS(bank, 0);
            //New page: unavailable, active, reset count
            statusWord[0] = 0xF3;
            statusWord[0] = statusWord[0] << 16;
            statusWord[1] = ERASE_STATE;
            DEE_FlashWrite(pageAddress, statusWord, FLASH_UNLOCK_KEY);
            continue;
        }
        //If one active page, do nothing
        else if (pageCnt == 1)
        {
            continue;
        }
        //If two active pages, erase second and repack first
        else if (pageCnt == 2)
        {
            if ((GetPageStatus(bank, NUM_DATA_EE_PAGES - 1, STATUS_CURRENT) == PAGE_CURRENT) &&
                    (GetPageStatus(bank, 0, STATUS_CURRENT) == PAGE_CURRENT))
            {
                currentPage = NUM_DATA_EE_PAGES - 1;
                erasePage = 0;
            }
            else
            {
                currentPage = 0;
                while ((GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT) &&
                        (currentPage < NUM_DATA_EE_PAGES))
                {
                    currentPage++;
                }
                erasePage = currentPage + 1;
                if (erasePage == NUM_DATA_EE_PAGES)
                {
                    erasePage = 0;
                }
            }
            ErasePage(bank, erasePage);
            if (!GetNextAvailCount(bank))
            {
                DEE_ActivePartitionPack(bank);
            }
            continue;
        }
        else
        {
            SetPageCorruptStatus(1);
            return (DEE_PAGE_CORRUPT);
        }
    }
    return (DEE_NO_ERROR);
}

static DEE_RETURN_STATUS DEE_ActivePartitionPack(uint8_t bank)
{
    uint16_t currentPage; //Array row (PM page) of active DEE page
    uint16_t packedPage; //Array row (PM page) of packed page
    uint32_t currentPageAddress; //Current page address
    uint32_t packedPageAddress; //Packed page address
    uint16_t latchAddr;
    uint16_t latchData;
    uint8_t dataEEFlags_sh;
    uint32_t deeItem[2];

    // Find the active page.
    for (currentPage = 0;
            (currentPage < NUM_DATA_EE_PAGES) &&
            (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT);
            currentPage++)
    {
    }


    if (currentPage == NUM_DATA_EE_PAGES)
    {
        SetPagePackBeforeInit(1);
        return (DEE_PACK_BEFORE_INIT); // Error - no active page
    }
    else
    {
        // Find the next unexpired page to use
        packedPage = currentPage + 1;
        if (packedPage == NUM_DATA_EE_PAGES)
        {
            packedPage = 0;
        }
        while (GetPageStatus(bank, packedPage, STATUS_EXPIRED) == PAGE_EXPIRED)
        {
            packedPage++;
            if (packedPage == NUM_DATA_EE_PAGES)
            {
                packedPage = 0;
            }
            if (packedPage == currentPage)
            {
                SetPageExpiredPage(1);
                return (DEE_EXPIRED_PAGE); // Error - all pages expired
            }
        }
    }

    packedPageAddress = DEE_PAGE_ADDRESS(bank, packedPage);

    if (GetNextAvailCount(bank))
    {
        SetPagePackBeforePageFull(1); // Pack called before the page was full
    }

    dataEEFlags_sh = activePartitionDataEEFlags.val;
    SetAddrNotFound(0); // Initialize flag


    deeItem[0] = ERASE_STATE;
    deeItem[1] = ERASE_STATE;
    DEE_FlashWrite(packedPageAddress, deeItem, FLASH_UNLOCK_KEY);

    packedPageAddress += 4; // Do not use odd Flash words on dsPIC33E devices with ECC or double word writes
    latchAddr = 0;

    do
    {
        DEE_Read((DATA_EE_SIZE * bank) + latchAddr, &latchData);
        if (GetAddrNotFound()) //if address is unwritten, skip to next address
        {
            SetAddrNotFound(0);
        }
        else
        {
            deeItem[0] = latchAddr;
            deeItem[0] = ((deeItem[0] << 16) | (latchData & 0xFFFF));
            deeItem[1] = ERASE_STATE;
            DEE_FlashWrite(packedPageAddress, deeItem, FLASH_UNLOCK_KEY);

            packedPageAddress += 4; // Do not use odd Flash words on devices with ECC or double word writes
        }
        latchAddr++;
    }
    while (latchAddr != DATA_EE_SIZE);

    activePartitionDataEEFlags.val = dataEEFlags_sh; //Restore status flags

    packedPageAddress = DEE_PAGE_ADDRESS(bank, packedPage) + 4; // Adjust offset to skip the odd word

    uint32_t data;
    DEE_FlashRead(packedPageAddress, 1, &data);
    latchAddr = (data >> 16) & 0xFF;
    latchData = data & 0xFFFF;
    packedPageAddress += 4; // Do not use odd Flash words on devices with ECC or double word writes

    while (latchAddr != 0xFF)
    {
        uint16_t dataRead;
        DEE_Read((DATA_EE_SIZE * bank) + latchAddr, &dataRead);
        if (dataRead != latchData)
        {
            SetPageWriteError(1);
            return (DEE_WRITE_ERROR); //Error - data does not match
        }
        DEE_FlashRead(packedPageAddress, 1, &data);
        latchAddr = (data >> 16) &0xFF;
        latchData = data & 0xFFFF;
        packedPageAddress += 4; // Do not use odd Flash words on devices with ECC or double word writes
    }

    //Program page status
    currentPageAddress = DEE_PAGE_ADDRESS(bank, currentPage);
    packedPageAddress = DEE_PAGE_ADDRESS(bank, packedPage);

    DEE_FlashRead(currentPageAddress, 1, &data);
    latchAddr = (data >> 16) & 0xFF;
    latchData = data & 0xFFFF;

    if (packedPage == 0)
    {
        latchData++; //Increment E/W counter
    }

    if (latchData >= ERASE_WRITE_CYCLE_MAX - 1)
    {
        SetPageExpiredPage(1);
        latchAddr &= 0xEF;
    }

    uint32_t dataItem[2];
    dataItem[0] = latchAddr;
    dataItem[0] = (dataItem[0] << 16) | latchData;
    dataItem[1] = ERASE_STATE;
    DEE_FlashWrite(packedPageAddress, dataItem, FLASH_UNLOCK_KEY);
    DEE_FlashRead(packedPageAddress, 1, &data);
    uint16_t readAddr = (data >> 16) & 0xFF;
    uint16_t readData = data & 0xFFFF;

    if ((latchAddr != readAddr) ||
            (latchData != readData))
    {
        SetPageWriteError(1);
        return (DEE_WRITE_ERROR);
    }

    //Erase active page
    ErasePage(bank, currentPage);

    return (GetPageExpiredPage());
}

static uint8_t GetPageStatus(uint8_t bank, uint8_t page, uint8_t field)
{
    uint32_t pageAddress;
    uint8_t statusByte;
    uint8_t status;
    uint32_t statusInfo;

    pageAddress = DEE_PAGE_ADDRESS(bank, page);
    DEE_FlashRead(pageAddress, 1, &statusInfo);
    statusByte = statusInfo >> 16 & 0xFF;
    switch (field)
    {
        case STATUS_AVAILABLE:
            status = ((statusByte & 4) >> 2);
            break;
        case STATUS_CURRENT:
            status = ((statusByte & 8) >> 3);
            break;
        case STATUS_EXPIRED:
            status = ((statusByte & 16) >> 4);
            break;
        default:
            status = 0;
            break;
    }
    return (status);
}

static void ErasePage(uint8_t bank, uint8_t page)
{
    uint32_t pageAddress = DEE_PAGE_ADDRESS(bank, page);
    FLASH_NonBlockingPageErase(pageAddress, FLASH_UNLOCK_KEY, FlashEraseOpHandler, NULL);
    while(FLASH_OperationStatusGet() == FLASH_OP_BUSY);
    return;
}

static uint16_t GetNextAvailCount(uint8_t bank)
{
    uint16_t nextAvailCount = 0;
    uint16_t currentPage;
    uint8_t dataEEval;
    uint32_t pageAddress;
    uint32_t latchAddrData;

    // Find the active page.
    for (currentPage = 0;
            (currentPage < NUM_DATA_EE_PAGES) &&
            (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT);
            currentPage++)
    {
    }

    pageAddress = DEE_PAGE_ADDRESS(bank, currentPage);

    do
    {
        nextAvailCount += 4;
        pageAddress += 4;
        DEE_FlashRead(pageAddress, 1, &latchAddrData);
        dataEEval = (latchAddrData >> 16) & 0xFF;
    }
    while ((nextAvailCount < NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2) && (dataEEval != 0xFF));

    if (nextAvailCount == NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2)
    {
        nextAvailCount = 0;
    }
    return (nextAvailCount);
}

static void DEE_FlashWrite(flash_adr_t flashAddress, flash_data_t *data, flash_key_t unlockKey)
{
    FLASH_NonBlockingWordWrite(flashAddress, data, unlockKey, FlashProgramOpHandler, NULL);
    while(FLASH_OperationStatusGet() == FLASH_OP_BUSY);
}

static void DEE_FlashRead(flash_adr_t flashAddress, size_t count, flash_data_t *data)
{
    while(FLASH_NonBlockingRead(flashAddress, count, data) == FLASH_OP_BUSY);
}

bool  DEE_ActiveToInactivePartitionDataCopy()
{
    bool dataCopied = false;
    uint16_t index = 0;
    uint16_t numberOfPagesCopied = 0;
    uint32_t dataInActivePartition;
    uint32_t dataInInactivePartition;
    flash_adr_t srcAddr = DEE_START_ADDRESS_ACTIVE_PARTITION;
    flash_adr_t dstAddr = DEE_START_ADDRESS_INACTIVE_PARTITION;
    //Check for status word at the start address of each page , if found copy is required
    bool copyRequired = DEE_IsDataCopyRequired();
    if (copyRequired)
    {
        while (numberOfPagesCopied < (DATA_EE_BANKS * NUM_DATA_EE_PAGES))
        {
            DEE_FlashRead(srcAddr, 1, &dataInActivePartition);
            // Pages with data , copy data to other partition
            if (((dataInActivePartition & ERASE_STATE) != ERASE_STATE))
            {
                /* Declare arrays for holding the entire page data and packed page data */
                flash_data_t fullPageData[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS];
                flash_data_t packedPageData[PACKED_PAGE_DATA_SIZE * FLASH_WORD_WRITE_SIZE];
                flash_data_t pageStatus[2];
                /* Initialize the arrays to 0xFFFFFFFF */
                memset(fullPageData, 0xFF, FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS * sizeof(flash_data_t));
                memset(packedPageData, 0xFF, PACKED_PAGE_DATA_SIZE * FLASH_WORD_WRITE_SIZE * sizeof(flash_data_t));
                /* Read entire page and pack the page*/
                DEE_FlashRead(srcAddr, FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS , fullPageData);
                DEE_PackedDataGet(fullPageData, FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS, packedPageData);
                /*Erase the destination page */
                FLASH_NonBlockingPageErase(dstAddr, FLASH_UNLOCK_KEY, FlashEraseOpHandler, NULL);
                while (FLASH_OperationStatusGet() == FLASH_OP_BUSY);
                /*Program the destination page with packed data */
                index = 0;
                flash_adr_t tempDestAddr = dstAddr;
                while (index < (PACKED_PAGE_DATA_SIZE * FLASH_WORD_WRITE_SIZE))
                {
                    FLASH_NonBlockingRowWrite(tempDestAddr, &packedPageData[index], FLASH_UNLOCK_KEY, FlashProgramOpHandler, NULL);
                    while (FLASH_OperationStatusGet() == FLASH_OP_BUSY);
                    /* Retry write in case of any write error */
                    if(FLASH_OperationStatusGet() == FLASH_WRITE_ERROR)
                    {
                       DEE_RetryDataWrite(tempDestAddr, &packedPageData[index]);
                    }
                    index = index + NUMBER_OF_INSTRUCTIONS_IN_ROW;
                    tempDestAddr = tempDestAddr + FLASH_WRITE_ROW_SIZE_IN_PC_UNITS;
                }
                /* copy page status from active partition page to inactive partition page */
                pageStatus[0] = fullPageData[0];
                pageStatus[1] = ERASE_STATE;
                DEE_FlashWrite(dstAddr, pageStatus, FLASH_UNLOCK_KEY);

                srcAddr = srcAddr + FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS;
                dstAddr = dstAddr + FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS;
                dataCopied = true;
            }
            else
            {
                /* Check if there is any valid DEE data , this condition occurs when the data copied correctly
                during the previous reboot , but there was some issue while erasing the page i.e. some pages are
                erased and some are not */

                DEE_FlashRead(dstAddr, 1, &dataInInactivePartition);
                if ((dataInInactivePartition & ERASE_STATE) == ERASE_STATE)
                {
                    FLASH_NonBlockingPageErase(dstAddr, FLASH_UNLOCK_KEY, FlashEraseOpHandler , NULL);
                    while (FLASH_OperationStatusGet() == FLASH_OP_BUSY);
                }
                srcAddr = srcAddr + FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS;
                dstAddr = dstAddr + FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS;
            }
            numberOfPagesCopied++;
        }
        /* Delete data from the active partition */
        if (dataCopied) {
            srcAddr = DEE_START_ADDRESS_ACTIVE_PARTITION;
            for (index = 0; index < numberOfPagesCopied; index++) {
               FLASH_NonBlockingPageErase(srcAddr, FLASH_UNLOCK_KEY, FlashEraseOpHandler, NULL);
               while (FLASH_OperationStatusGet() == FLASH_OP_BUSY);
               srcAddr = srcAddr + FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS;
            }
        }

    }
    return dataCopied;
}

bool DEE_IsDataCopyRequired()
{
    bool copyRequired = false;
    uint16_t numberOfPagesChecked = 0;
    uint32_t dataInActivePartition;
    flash_adr_t srcAddr = DEE_START_ADDRESS_ACTIVE_PARTITION;
    while (numberOfPagesChecked < (DATA_EE_BANKS * NUM_DATA_EE_PAGES))
    {
        /* Check whether data is present at address immediately after the page status */
        DEE_FlashRead(srcAddr + 4, 1, &dataInActivePartition);
        if ((dataInActivePartition & ERASE_STATE) != ERASE_STATE)
        {
            copyRequired = true;
            break;
        }
        srcAddr = srcAddr + FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS;
        numberOfPagesChecked++;
    }
    return copyRequired;
}

static void DEE_PackedDataGet(flash_data_t *fullPageData, uint32_t fullPageDataSize, flash_data_t *packedPageData)
{
    uint8_t deeAddress = MAX_DATA_SIZE_PER_PAGE;
    uint32_t exists[MAX_DATA_SIZE_PER_PAGE] = {0};
    uint32_t packedDataIndex = FLASH_WORD_WRITE_SIZE;
    for(int32_t index = fullPageDataSize - 1; index >= FLASH_WORD_WRITE_SIZE; index--)
    {
        deeAddress = (fullPageData[index] & DEE_ADDRESS_MASK) >> 16;
        if(!exists[deeAddress] && deeAddress != MAX_DATA_SIZE_PER_PAGE)
        {
            packedPageData[packedDataIndex] = fullPageData[index];
            packedDataIndex = packedDataIndex + FLASH_WORD_WRITE_SIZE;
            exists[deeAddress] = 1;
        }
    }
}

static void DEE_RetryDataWrite(flash_adr_t destAddr, flash_data_t *packedPageData) {
    uint16_t retryIndex = 0;
    for (retryIndex = 0; retryIndex < WRITE_RETRY_COUNT; retryIndex++) {
        FLASH_NonBlockingRowWrite(destAddr, packedPageData, FLASH_UNLOCK_KEY, FlashProgramOpHandler, NULL);
        while (FLASH_OperationStatusGet() == FLASH_OP_BUSY);
        if (FLASH_OperationStatusGet() != FLASH_WRITE_ERROR) {
            break;
        }
    }
}
