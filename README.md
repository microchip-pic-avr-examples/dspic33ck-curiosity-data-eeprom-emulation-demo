![image](images/microchip.jpg) 

## DSPIC33CK CURIOSITY DATA EEPROM EMULATION DEMO


## Summary

Data EEPROM Emulation demo.

## Related Documentation

- dsPIC33CK256MP508 microcontroller (https://www.microchip.com/dsPIC33CK256MP508)


## Software Used 

- MPLAB® X IDE v6.00 or newer (https://www.microchip.com/mplabx)
- MPLAB® XC16 v2.00 or newer (https://www.microchip.com/xc) 
- Any of the serial terminal application. Example: Tera Term (https://ttssh2.osdn.jp/index.html.en)


## Hardware Used

- DSPIC33CK CURIOSITY DEVELOPMENT BOARD (https://www.microchip.com/DM330030)
- Micro USB Cable



## Operation

This demo runs the Data EEPROM Emulation , if emulation is succesful then prints a message saying "Data EEPROM Emulation successful. Value at address 0 is 1024 and address1 is 1025"
If emulation fails it prints a message saying "Data EEPROM Emulation failed".

## Accessing 16-Bit Data EEPROM Emulation Library

*Note:* More MCC Melody devices support will be added in next releases.

| Device Family         | Procedure                  |
| -----------           | -----------                |
| "CK" Family of devices| 1. Create project <br> 2. Launch MCC <br> 3. Select "MCC Melody" <br>   <img src="images/melodySelection.jpg" width="600"> <br> 4. "16-Bit Data EEPROM Emulation Library "will be available under "Libraries" <br>     <img src="images/DEEMelody.jpg" width="600">       |
|   All other devices except "CK" device family                    | 1. Create project <br> 2. Launch MCC <br> 3. Select "Classic MCC"Text  <br>  <img src="images/classicSelection.jpg" width="600"><br> 4. Switch core version to "5.2.1"     <br>  <img src="images/coreSwitch.jpg" width="600">  <br> 5. "16-Bit Data EEPROM Emulation Library" will be available under "Libraries" <br>   <img src="images/DEEClassic.jpg" width="600">         |





