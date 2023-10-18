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
#include <stdio.h>
#include <stdbool.h>

#define EXPECTED_VALUE_AT_ADDR0 1024
#define EXPECTED_VALUE_AT_ADDR1 1025
#define EXPECTED_UPDATED_VALUE_AT_ADDR1 512
#define EXPECTED_VALUE_AT_ADDR2 513
#define EXPECTED_VALUE_AT_ADDR3 514
/*
    Main application
*/

int main(void)
{
    uint16_t addr0 = 0, addr1 = 1, addr2 = 2, addr3 = 3, i;
    uint16_t valueAtAddr0 = 0;
    uint16_t valueAtAddr1 = 0;
    uint16_t valueAtAddr2 = 0;
    uint16_t valueAtAddr3 = 0;
    bool dataFound = false;
    SYSTEM_Initialize();
    printf("************* Application in Partition2 starts *******************");
    DEE_Init();

    DEE_Read(addr0, &valueAtAddr0);
    DEE_Read(addr1, &valueAtAddr1); 
    
   
    if (valueAtAddr0 == EXPECTED_VALUE_AT_ADDR0 && valueAtAddr1 == EXPECTED_VALUE_AT_ADDR1) {
        dataFound = true;
        printf("Data EEPROM Emulation successful. Value at address 0 is %d and address 1 is %d \r\n", valueAtAddr0, valueAtAddr1);
    } else {
        printf("Data EEPROM Emulation failed. Value at address 0 is %d and address 1 is %d \r\n", valueAtAddr0, valueAtAddr1);
    }
    
    for (i = 0; i < 512; i++) {
        DEE_Write(addr1, i + 1);
        DEE_Write(addr2, i + 2);
        DEE_Write(addr3, i + 3);
    }

    DEE_Read(addr0, &valueAtAddr0);
    DEE_Read(addr1, &valueAtAddr1);
    DEE_Read(addr2, &valueAtAddr2);
    DEE_Read(addr3, &valueAtAddr3);
    
    if (valueAtAddr0 == EXPECTED_VALUE_AT_ADDR0 && valueAtAddr1 == EXPECTED_UPDATED_VALUE_AT_ADDR1 && valueAtAddr2 == EXPECTED_VALUE_AT_ADDR2 && valueAtAddr3 == EXPECTED_VALUE_AT_ADDR3) {
        if (dataFound) {
            LED1_SetHigh();
        }
        printf("Data EEPROM Emulation successful. Value at address 0 is %d , address 1 is %d , address 2 is %d, address 3 is %d\r\n", valueAtAddr0, valueAtAddr1, valueAtAddr2, valueAtAddr3);
    } else {
        printf("Data EEPROM Emulation failed. Value at address 0 is %d , address 1 is %d, address 2 is %d, address 3 is %d \r\n", valueAtAddr0, valueAtAddr1, valueAtAddr2 , valueAtAddr3);
    }
    
    printf("************* Application in Partition2 ends *******************");
    while(1);
}