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
#include "mcc_generated_files/dee/dee.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/flash/flash_nonblocking.h"
#include <stdio.h>

#define IBSEQ_INDEX 1022
#define PARTITION2_CONFIG_BITS_START_ADDRESS 0x415F00
#define PARTITION2_NEW_IBSEQ 0xFFE001
#define EXPECTED_VALUE_AT_ADDR0 1024
#define EXPECTED_VALUE_AT_ADDR1 1025
#define EXPECTED_VALUE_AT_ADDR2 1026

void UpdateConfigBitFBTSEQ();

volatile bool eraseDone = false;
volatile bool writeDone = false;

void FlashEraseOpHandler(void* context) {
    eraseDone = true;
}

void FlashProgramOpHandler(void *context) {
    writeDone = true;
}

/*
    Main application
 */

int main(void) {
    uint16_t addr0 = 0, addr1 = 1, addr2=2, index;
    uint16_t valueAtAddr0 = 0;
    uint16_t valueAtAddr1 = 0;
    uint16_t valueAtAddr2 = 0;

    SYSTEM_Initialize();
    printf("************* Application in Partition1 starts *******************\r\n");
    DEE_Init();
    for (index = 0; index < 1024; index++) {
        DEE_Write(addr0, index + 1);
        DEE_Write(addr1, index + 2);
        DEE_Write(addr2, index + 3);
    }

    DEE_Read(addr0, &valueAtAddr0);
    DEE_Read(addr1, &valueAtAddr1);
    DEE_Read(addr2, &valueAtAddr2);

    /*Update the partition2 FBTSEQ config bit so that after reboot partition2 becomes active partition*/
    UpdateConfigBitFBTSEQ();

    if (valueAtAddr0 == EXPECTED_VALUE_AT_ADDR0 && valueAtAddr1 == EXPECTED_VALUE_AT_ADDR1 && valueAtAddr2 == EXPECTED_VALUE_AT_ADDR2) {
        LED2_SetHigh();
        printf("Data EEPROM Emulation successful.\r\n Value at address 0 is %d\r\n Value at address 1 is %d\r\n Value at address 2 is %d\r\n", valueAtAddr0, valueAtAddr1, valueAtAddr2);
    } else {
        printf("Data EEPROM Emulation failed.\r\n Value at address 0 is %d\r\n Value at address 1 is %d\r\n Value at address 2 is %d\r\n", valueAtAddr0, valueAtAddr1, valueAtAddr2);
    }
    printf("************* Application in Partition1 ends *******************\r\n");
    while (1);
}

void UpdateConfigBitFBTSEQ() {
    int index = 0;
    flash_data_t data[FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS] = {0x00};
    flash_adr_t confAddress = flash.PageAddressGet(PARTITION2_CONFIG_BITS_START_ADDRESS);
    flash.NonBlockingRead(confAddress, FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS, data);
    flash.NonBlockingPageErase(confAddress, FLASH_UNLOCK_KEY, FlashEraseOpHandler, NULL);
    while (eraseDone == false);
    data[IBSEQ_INDEX] = PARTITION2_NEW_IBSEQ;
    while (index < FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS) {
        flash.NonBlockingRowWrite(confAddress, &data[index], FLASH_UNLOCK_KEY, FlashProgramOpHandler, NULL);
        while (flash.OperationStatusGet() == FLASH_OP_BUSY);
        index = index + FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS;
        confAddress = confAddress + FLASH_WRITE_ROW_SIZE_IN_PC_UNITS;
    }
}