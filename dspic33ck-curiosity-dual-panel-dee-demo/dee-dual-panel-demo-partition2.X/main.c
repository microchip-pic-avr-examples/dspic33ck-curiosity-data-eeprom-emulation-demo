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
#define EXPECTED_UPDATED_VALUE_AT_ADDR2 513
#define EXPECTED_VALUE_AT_ADDR3 514
#define EXPECTED_VALUE_AT_ADDR4 515
/*
    Main application
*/

int main(void)
{
    uint16_t addr0 = 0, addr1 = 1, addr2 = 2, addr3 = 3, addr4 = 4, i;
    uint16_t valueAtAddr0 = 0;
    uint16_t valueAtAddr1 = 0;
    uint16_t valueAtAddr2 = 0;
    uint16_t valueAtAddr3 = 0;
    uint16_t valueAtAddr4 = 0;
    SYSTEM_Initialize();
    printf("************* Application in Partition2 starts *******************\r\n");
    DEE_Init();
    
    DEE_Read(addr0, &valueAtAddr0);
    DEE_Read(addr1, &valueAtAddr1);
    
    if (valueAtAddr0 == EXPECTED_VALUE_AT_ADDR0 && valueAtAddr1 == EXPECTED_VALUE_AT_ADDR1) {
        LED1_SetHigh();
        printf("DEE Data comparison from previous iteration is successful.\r\n Value at address 0 is %d\r\n Value at address 1 is %d\r\n", valueAtAddr0, valueAtAddr1);
    } else {
        printf("DEE Data comparison from previous iteration is failed.\r\n Value at address 0 is %d\r\n Value at address 1 is %d\r\n", valueAtAddr0, valueAtAddr1);
    }
    printf("Address 2 value will be updated by Application in Partition2.\r\n");

    for (i = 0; i < 512; i++) {
        DEE_Write(addr2, i + 2);
        DEE_Write(addr3, i + 3);
        DEE_Write(addr4, i + 4);
    }


    DEE_Read(addr2, &valueAtAddr2);
    DEE_Read(addr3, &valueAtAddr3);
    DEE_Read(addr4, &valueAtAddr4);
    
    if (valueAtAddr2 == EXPECTED_UPDATED_VALUE_AT_ADDR2 && valueAtAddr3 == EXPECTED_VALUE_AT_ADDR3 && valueAtAddr4 == EXPECTED_VALUE_AT_ADDR4) {
        LED1_SetHigh();
        printf("Data EEPROM Emulation successful.\r\n Value at address 2 is %d\r\n Value at address 3 is %d\r\n Value at address 4 is %d\r\n",  valueAtAddr2, valueAtAddr3, valueAtAddr4);
    } else {
        printf("Data EEPROM Emulation failed.\r\n Value at address 2 is %d\r\n Value at address 3 is %d\r\n Value at address 4 is %d\r\n",  valueAtAddr2, valueAtAddr3, valueAtAddr4);
    }
    
    printf("************* Application in Partition2 ends *******************\r\n");
    while(1);
}