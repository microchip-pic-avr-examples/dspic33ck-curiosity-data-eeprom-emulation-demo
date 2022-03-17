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
#include "stdio.h"
#include "mcc_generated_files/dee/dee.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/uart/uart1.h"

/*
    Main application
 */

int main(void) {
    int addr0 = 0, addr1 = 1, i;
    uint16_t valueAtAddr0 = 0;
    uint16_t valueAtAddr1 = 0;
    uint16_t expectedValueAtAddr0 = 1024;
    uint16_t expectedValueAtAddr1 = 1025;

    SYSTEM_Initialize();
    DEE_Init();

    for (i = 0; i < 1024; i++) {
        DEE_Write(addr0, i + 1);
        DEE_Write(addr1, i + 2);
    }

    DEE_Read(addr0, &valueAtAddr0);
    DEE_Read(addr1, &valueAtAddr1);

    if (valueAtAddr0 == expectedValueAtAddr0 && valueAtAddr1 == expectedValueAtAddr1) {
        printf("Data EEPROM Emulation successful. Value at address 0 is %d and address1 is %d \r\n", valueAtAddr0, valueAtAddr1);
    } else {
        printf("Data EEPROM Emulation failed. Value at address 0 is %d and address1 is %d \r\n", valueAtAddr0, valueAtAddr1);
    }
    return 0;

}