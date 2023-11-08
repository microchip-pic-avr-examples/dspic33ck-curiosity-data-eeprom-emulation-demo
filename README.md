![image](images/microchip.jpg)

## dsPIC33CK Curiosity Data EEPROM Emulation Demo


## Summary

Microchip Technology Inc., has expanded its product portfolio to include a wide variety of cost-effective PIC® Microcontrollers (MCUs) without an internal data EEPROM.

Many applications store nonvolatile information in the Flash program memory using table read and write operations. Applications that need to frequently update these data may have greater endurance requirements
than the specified Flash endurance for the MCU/Digital Signal Controller (DSC) devices.

The alternate solution of using an external, serial EEPROM device may not be appropriate for cost-sensitive or pin-constrained applications.

This 16-Bit Data EEPROM Emulation library presents a third alternative that addresses these issues. This library features an interface similar to an internal data EEPROM, which
uses available program memory and can improve endurance by a factor as high as 500.

This code example demonstrates the usage of 16-Bit Data EEPROM Emulation Library.

## Related Documentation

- dsPIC33CK256MP508 microcontroller (https://www.microchip.com/dsPIC33CK256MP508)
- Emulating Data EEPROM for PIC18 and PIC24 MCUs and dsPIC33 DSCs (https://www.microchip.com/en-us/application-notes/an1095)


## Software Used

- MPLAB® X IDE v6.15 or newer (https://www.microchip.com/mplabx)
- MPLAB® XC16 v2.00 or newer (https://www.microchip.com/xc)
- Any of the serial terminal application. Example: Tera Term (https://ttssh2.osdn.jp/index.html.en)
- 16-bit Data EEPROM Emulation Library (DEE) for PIC24 MCUs and dsPIC33 DSCs (https://www.microchip.com/en-us/software-library/16-bit_data_eeprom_emulation)


## Hardware Used

- dsPIC33CK Curiosity Development Board (https://www.microchip.com/DM330030)
- Micro USB Cable



## Operation
There are 2 examples in this repository. One example uses single panel and another demonstrates dual panel capability of DEE.

## Example1:
This demo runs the Data EEPROM Emulation , if emulation is successful then prints a message to the serial terminal saying "Data EEPROM Emulation successful. Value at address 0 is 1024 and address1 is 1025"
If emulation fails it prints a message saying "Data EEPROM Emulation failed".

## Example2:
There are 2 MPLABX projects under folder "dspic33ck-curiosity-dual-panel-dee-demo". One project for partition1 another for partition2.
These projects are already linked so that when dee-dual-panel-demo-partition1.X is built and programmed, it programs both the partitions.

After programming , user will see the following data on the serial terminal on successful emulation,

![image](images/partition1.jpg)

After some time , once the emulation is completed and output is displayed, reset the board.

The following data will be displayed on successful emulation.

![image](images/partition2.jpg)

## Accessing 16-Bit Data EEPROM Emulation Library

*Note:* More MCC Melody devices support will be added in future releases.

| Device Family         | Procedure                  |
| -----------           | -----------                |
| Devices supported on Melody| 1. Create project <br> 2. Launch MCC <br> 3. Select "MCC Melody" <br>   <img src="images/melodySelection.jpg" width="600"> <br> 4. "16-Bit Data EEPROM Emulation Library "will be available under "Libraries" <br>     <img src="images/DEEMelody.jpg" width="600">       |
|   Devices not supported on Melody                   | 1. Make sure https://ww1.microchip.com/downloads/en/DeviceDoc/script-1.29.3.mc3lib is installed <br> 2. Create project <br> 3. Launch MCC <br> 4. Select "Classic MCC"  <br>  <img src="images/classicSelection.jpg" width="600"><br> 5. Switch core version to "5.2.1"     <br>  <img src="images/coreSwitch.jpg" width="600">  <br> 6. "16-Bit Data EEPROM Emulation Library" will be available under "Libraries" <br>   <img src="images/DEEClassic.jpg" width="600">         |

For more information refer: https://www.microchip.com/en-us/software-library/dspic33-pic24-data-eeprom-emulation
