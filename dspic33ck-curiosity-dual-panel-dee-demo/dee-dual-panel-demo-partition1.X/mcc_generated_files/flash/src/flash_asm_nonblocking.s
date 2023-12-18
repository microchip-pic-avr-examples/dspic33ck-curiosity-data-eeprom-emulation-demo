;/*******************************************************************************
;  flash.s for PIC24 and dsPIC33 Cores
;
;  Summary:
;    Assembly language optimized helper routines required flash functions
;
;  Description:
;    Assembly language optimized for flash access
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2016 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/
    .equ    WRITE_DWORD_CODE,                      0x4001
    .equ    ERASE_PAGE_CODE,                       0x4003
    .equ    ACTIVE_BULK_ERASE_CODE,                0x400E
    .equ    INACTIVE_BULK_ERASE_CODE,              0x4004
    .equ    FLASH_WRITE_ROW_CODE,                  0x4002
    .equ    ERASE_WRITE_OP_MASK,                   0x520F
    .equ    FLASH_ERASE_WRITE_OP_MASK,             0x520F
    .equ    FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS,  128
    .equ    FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS, 1024
    .equ    FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS,     1024 * 2
    .equ    FLASH_ERASE_PAGE_MASK,                 (~(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS - 1))
    .data
    _FlashKey: .long 0xFFFFFFFF
    .text
    .pushsection    .NVM_NonBlockingFlash, code


/**
 * ;void  FLASH_NonBlockingUnlock(uint32_t key);
 * Used to program the flash key in memory variable located in RAM
 * the valid key is 0x00AA0055
 *
 *
 * @param key       32-bit (unsigned long) for the key
 *
 *   Registers used:  w0 w1
 *
 * Inputs:
 *  w0,w1 = long data - Flash Key to save   (32 bits)
 *
 */

    .global         _FLASH_NonBlockingUnlock
    .type           _FLASH_NonBlockingUnlock, @function
    reset
 _FLASH_NonBlockingUnlock:
    mov     #_FlashKey, W2
    mov     W0, [W2++]
    mov     W1, [W2]
    return;

/**
 * ;void  FLASH_NonBlockingLock();
 * Locks the flash from programming by resetting the NVMKEY in memory
  *
 *
 * @param key       32-bit (unsigned long) for the key
 *
 *   Registers used:  w0 w1
 *
 * Inputs:
 *  w0,w1 = long data - Flash Key to save   (32 bits)
 *
 */

    .global         _FLASH_NonBlockingLock
    .type           _FLASH_NonBlockingLock, @function
    .extern         NVMKEY

    reset
 _FLASH_NonBlockingLock:
    clr W0
    clr W1
    rcall _FLASH_NonBlockingUnlock
    clr NVMKEY
    return;

 /**
 * ;void FLASH_NonBlockingSendNvmKey(uint32_t key);
 * Procedure to take the NVM key from memory and program the flash controller
 * with it.  A valid key is 0x00AA0055
 *
 *
 * @param key        32-bit (unsigned long) key
 *
 *   Registers used:    w0 w1 (Preserved)
 *
 *   Inputs:
 *   w0,w1 = long data - Flash Key to use   (32 bits)
 */
    reset
    .global         _FLASH_NonBlockingSendNvmKey
    .type           _FLASH_NonBlockingSendNvmKey, @function
    .extern         NVMKEY
    .extern         TBLPAG

    reset
_FLASH_NonBlockingSendNvmKey:
    push    W0
    push    W1

    mov    #_FlashKey, w1

    ; Disable interrupts
    push   INTCON2	        ; Save Global Interrupt Enable bit.
    bclr   INTCON2, #15	    ; Disable interrupts

    ; Write the KEY sequence
    mov    [W1++], W0
    mov    W0,     NVMKEY
    mov    [W1],   W0
    mov    W0,     NVMKEY
    bset   NVMCON, #15

    ; Insert two NOPs after programming
    nop
    nop
    ; Restore interrupts to original value.
    pop    INTCON2

    pop    W1
    pop    W0
    return


/**
 * ;uint32_t FLASH_NonBlockingReadWord24(uint32_t flashAddress);
 * Reads the 24 bit instruction located at the address passed to this function.
 *
 *
 * @param flashAddress       24-bit (unsigned long) specifying a target address
 *                      that needs to be read.  Needs to be aligned to an even
 *                      address.
 *
 *   Registers used:    w0 w1 w2
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to read   (24 bits)
 *
 *  outputs:
 *   w0,w1 = 32 bit data contains the flash data read.
 **/

    reset
    .global         _FLASH_NonBlockingReadWord24
    .type           _FLASH_NonBlockingReadWord24, @function
    .extern         TBLPAG


   _FLASH_NonBlockingReadWord24:
    mov         TBLPAG, W2
    mov         W1, TBLPAG    ; Little endian, w1 has MSW, w0 has LSX
    tblrdh      [W0], W1      ; read MSW of data to high latch
    tblrdl      [W0], W0      ; read LSW of data
    mov         W2, TBLPAG    ; Restore register,
    return

/**
 * ;uint16_t FLASH_NonBlockingReadWord16(uint32_t flashAddress);
 * Reads the 16 bit instruction located at the address passed to this function.
 * Address must be aligned to an even address.
 *
 *
 * @param flashAddress       24-bit (unsigned long) specifying a target address
 *                      that needs to be read.  Needs to be aligned to an even
 *                      address.
 *
 *   Registers used:    w0 w1 w2
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to read   (24 bits)
 *
 *  outputs:
 *   w0 = 16 bit data contains the flash data read.
 *
 **/

    reset
    .global         _FLASH_NonBlockingReadWord16
    .type           _FLASH_NonBlockingReadWord16, @function
    .extern         TBLPAG


   _FLASH_NonBlockingReadWord16:
    mov         TBLPAG, W2
    mov         W1, TBLPAG     ; Little endian, w1 has MSW, w0 has LSX
    tblrdl      [W0], W0       ; read LSW of data
    mov         W2, TBLPAG     ; restore register
    return


/**
 * void FLASH_NonBlockingErasePage(unsigned long flashAddress);
 * Erases the page containing the specified address. Be very careful if calling
 * this function as the function will erase any legal page,
 *
 * NOTE: This function can take upwards of 45ms on some device families and
 * target pages to complete. At frequencies appreciably slower than the maximum
 * device frequency, even longer may be required. Therefore, you may need to
 * issue a ClrWdt() call before calling this function, assuming the Watchdog
 * timer is enabled. This also means that you should not configure a watchdog
 * timeout of less than ~64ms, even when you pre-clear the timeout. This
 * function does NOT internally clear the watchdog for you as it is packaged as
 * a library routine where not everyone would desire such behavior.
 *
 * @param flashAddress  24-bit (unsigned long) specifying the first address on the page
 *                      to be erased. Must be page aligned.
 *
 *   Registers used:    w0 w1 w2 w3
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to erase   (24 bits)
 *   w2,w3 = long data - NVM Unlock Key
 *
 *  outputs:
 *   0 - Fail
 *   1 - Pass
 *
 **/

    .global         _FLASH_NonBlockingErasePage
    .type           _FLASH_NonBlockingErasePage, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         NVMADRU
    .extern         NVMADR
   reset



_FLASH_NonBlockingErasePage:
    push    w0
    push    w1
    push    w2
    mov     w2 , w0
    mov     w3 , w1
    call    _FLASH_NonBlockingUnlock
    pop     w2
    pop     w1
    pop     w0
    mov     #FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS-1, w2     ;    get mask and validate all lower bits = 0
    and     w2, w0, w2
    bra     NZ,3f

    mov     #ERASE_PAGE_CODE, w2
    mov     w2, NVMCON
    mov     w0, NVMADR
    mov     w1, NVMADRU        ; MSB

    call    _FLASH_NonBlockingSendNvmKey

    call    _FLASH_NonBlockingLock
    mov     #1, w0                 ; default return true
    btsc    NVMCON, #13            ; if error bit set,
3:  mov     #0, w0                 ;   return false
    return;


/**
 * void FLASH_NonBlockingEraseBulk(unsigned char inactive);
 * Erases the partition. Be very careful if calling
 * this function as the function will erase the partition!
 *
 * NOTE: This function can take upwards of 45ms on some device families and
 * target pages to complete. At frequencies appreciably slower than the maximum
 * device frequency, even longer may be required. Therefore, you may need to
 * issue a ClrWdt() call before calling this function, assuming the Watchdog
 * timer is enabled. This also means that you should not configure a watchdog
 * timeout of less than ~64ms, even when you pre-clear the timeout. This
 * function does NOT internally clear the watchdog for you as it is packaged as
 * a library routine where not everyone would desire such behavior.
 *
 * @param partition     (unsigned char) if 0, indicates the single partition (for single
 *                      partition devices) or active partition (for dual partition devices).
 *                      if 1, indicates the inactive partition (for dual partition devices only).
 *
 *   Registers used:    w0 w1 w2 w3
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0 =    char data - indicates active/inactive partition
 *   w1,w2 = long data - NVM Unlock Key
 *
 *  outputs:
 *   0 - Fail
 *   1 - Pass
 *
 **/
    .global         _FLASH_NonBlockingEraseBulk
    .type           _FLASH_NonBlockingEraseBulk, @function
    .extern         TBLPAG
    .extern         NVMCON
   reset

_FLASH_NonBlockingEraseBulk:
    push    w0
    push    w1
    push    w2
    push    w3
    mov     w2 , w0
    mov     w3 , w1
    call    _FLASH_NonBlockingUnlock
    pop     w3
    pop     w2
    pop     w1
    pop     w0

    mov     #ACTIVE_BULK_ERASE_CODE, w2   ; default to active/single partition
    mov     #0, w3
    cpseq   w3, w0                 ; if w0 == 0, skip the next instruction
    mov     #INACTIVE_BULK_ERASE_CODE, w2 ; load the inactive partition bulk-erase code

    mov     w2, NVMCON
    mov     w0, NVMADR
    mov     w1, NVMADRU        ; MSB

    call    _FLASH_NonBlockingSendNvmKey

    call    _FLASH_NonBlockingLock
    mov     #1, w0                 ; default return true
    btsc    NVMCON, #13            ; if error bit set,
3:  mov     #0, w0                 ;   return false
    return;


 /**
 * ;void FLASH_NonBlockingWriteDoubleWord24(uint32_t flashAddress, uint32_t instructionData0, uint32_t instructionData1);
 *  Writes two 24-bit instruction to the flash at the flashAdddress
 *  Only the lower 24 bits of each instruction will be written since the flash
 *  is only 24 bits wide and all data in the upper 8 bits will be lost.
 *
 *
 * @param flashAddress  32 bit value specifying a target address in flash
 *                      that this firmware will write to.  It needs to be on a
 *                      addresses divisible by 4.
 *
 *  @param uint32_t instructionData0  24 bit instruction  to be written first.
 *  @param uint32_t instructionData1  24 bit instruction  to be written second.
 *
 *
 *   Registers used:    w0 w1 w2 w3 w4 w5 w6 w7
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to write   (24 bits)
 *   w2,w3 = long data - 24 bits of data to write to flash   (24 bits)
 *   w4,w5 = long data - 24 bits of data to write to flash   (24 bits)
 *   w6,w7 = long data - NVM Unlock Key
 *
 *  outputs:
 *   none
 *
 **/

    .global         _FLASH_NonBlockingWriteDoubleWord24
    .type           _FLASH_NonBlockingWriteDoubleWord24, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         NVMADRU
    .extern         NVMADR
    reset

_FLASH_NonBlockingWriteDoubleWord24:
    push    w0
    push    w1
    push    w2
    mov     w6 , w0
    mov     w7 , w1
    call    _FLASH_NonBlockingUnlock
    pop     w2
    pop     w1
    pop     w0

    btsc    w0, #0          ; Check for a valid address Bit 0 and Bit 1 clear
    bra     3f
    btsc    w0, #1
    bra     3f

good24:
    mov     W1,NVMADRU
    mov     W0,NVMADR

    mov     #WRITE_DWORD_CODE, W0
    mov     W0, NVMCON

    mov     TBLPAG, W0          ; save it
    mov     #0xFA,W1
    mov     W1,TBLPAG
    mov     #0,W1

                                ; Perform the TBLWT instructions to write the latches
    tblwtl  W2,[W1]
    tblwth  W3,[W1++]
    tblwtl  W4,[W1]
    tblwth  w5,[W1++]

    call    _FLASH_NonBlockingSendNvmKey


    mov     W0, TBLPAG

    call    _FLASH_NonBlockingLock
    mov     #1, w0               ; default return true
    btsc    NVMCON, #13          ; if error bit set,
3:  mov     #0, w0               ;   return false
    return;



 /**
 * ;void FLASH_NonBlockingWriteDoubleWord16(uint32_t flashAddress, uint16_t Data0, uint16_t Data1, uint32_t unlockKey);
 *  Writes two 16-bit words to the flash at the flashAdddress
 *  The upper 8 bits of the 24 bit flash entry will have its  data programmed as
 *  0xFF.
  *
 *
 * @param flashAddress  32 bit value specifying a target address in flash
 *                      that this firmware will write to.  It needs to be on a
 *                      addresses divisible by 4.
 *
 *  @param uint16_t Data0  16 bit word to be written first.
 *  @param uint16_t Data1  16 bit word to be written second.
 *
 *   Registers used:    w0 w1 w2 w3 w4 w5
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to write   (24 bits)
 *   w2    = 16 bit data - 16 bits of data to write to flash
 *   w3    = 16 bit data - 16 bits of data to write to flash
 *   w4,w5 = long data   - NVM Unlock Key
 *
 *  outputs:
 *   none
 *
 **/

    .global    _FLASH_NonBlockingWriteDoubleWord16
    .type      _FLASH_NonBlockingWriteDoubleWord16, @function
    .extern    TBLPAG
    .extern    NVMCON
    .extern    NVMADRU
    .extern    NVMADR
    reset

 _FLASH_NonBlockingWriteDoubleWord16:
    push    w0
    push    w1
    push    w2
    mov     w4 , w0
    mov     w5 , w1
    call    _FLASH_NonBlockingUnlock
    pop     w2
    pop     w1
    pop     w0

    btsc    w0, #0          ; Check for a valid address Bit 0 and Bit 1 clear
    bra     3f
    btsc    w0, #1
    bra     3f

good16:
    mov     W1,NVMADRU
    mov     W0,NVMADR

    mov     TBLPAG, W1      ; save it

    mov     #WRITE_DWORD_CODE, W0
    mov     W0, NVMCON

    mov     #0xFA,W0
    mov     W0,TBLPAG
    mov     #0,W0

    tblwtl  W2,[W0]         ; Perform the TBLWT instructions to write the latches
    mov     #0xFF,W2
    tblwth  W2,[W0++]
    tblwtl  W3,[W0]
    tblwth  W2,[W0++]

    call    _FLASH_NonBlockingSendNvmKey


    mov     w1, TBLPAG

    call    _FLASH_NonBlockingLock
    mov     #1, w0          ; default return true
    btsc    NVMCON, #13     ; if error bit set,
3:  mov     #0, w0          ;   return false
    return;




/**
 * ;void FLASH_NonBlockingWriteRow24(uint32_t flashAddress, uint32_t *data, uint32_t unlockKey);
 *  Writes a single Row to the address flashAddress from the address of data
 *  Since this is passed as a 32 bit value
 *  and flash is only 24 bit wide, all data in the upper 8 bits will be lost.
 *
 *  This method uses the built in row load and program from a RAM buffer.
 *
 * @param flashAddress  32 bit value specifying a target address in flash
 *                      that this firmware will write to.  It needs to be on a
 *                      addresses divisible by 4.
 *
 *  @param *data        16 bit value of the SRAM address to read the data from.
 *
 *   Registers used:    w0 w1 w2 w3 w4 w5
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to write   (24 bits)
 *   w2,w3 = long data - 24 bits of data address to write to flash   (24 bits)
 *   w4,w5 = long data - NVM Unlock Key
 *
 *  outputs:
 *   none
 *
 **/

    .global         _FLASH_NonBlockingWriteRow24
    .type           _FLASH_NonBlockingWriteRow24, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         NVMADRU
    .extern         NVMADR
    .extern         NVMSRCADRL
    reset

    _FLASH_NonBlockingWriteRow24:
    push    w0
    push    w1
    push    w2
    push    w3
    mov     w4 , w0
    mov     w5 , w1
    call    _FLASH_NonBlockingUnlock
    pop     w3
    pop     w2
    pop     w1
    pop     w0

    mov     #((FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS*2)-1), w3     ;    get mask and validate all lower bits = 0
    and     w3, w0, w3
    bra     NZ,3f


    mov     W0,NVMADR
    mov     W1,NVMADRU

    mov     #FLASH_WRITE_ROW_CODE, W0 ; RPDF = 0 so not compressed
    mov     W0, NVMCON

    mov     W2, NVMSRCADRL

    call    _FLASH_NonBlockingSendNvmKey

    call    _FLASH_NonBlockingLock
    mov     #1, w0               ; default return true
    btsc    NVMCON, #13          ; if error bit set,
3:  mov     #0, w0               ;   return false
    return;





/**
 * ;void FLASH_NonBlockingWriteRow16(uint32_t flashAddress, uint16_t sourceRamAddress, uint32_t unlockKey);
 *  Writes a single Row to the address flashAddress from the sourceAddress
 *  Since this is passed as a 32 bit value
 *  and flash is only 24 bit wide, all data in the upper 8 bits will be lost.
 *
 *  This method uses the built in row load and program from a RAM buffer.
 *
 * @param flashAddress  32 bit value specifying a target address in flash
 *                      that this firmware will write to.  It needs to be on a
 *                      addresses divisible by 4.
 *
 *  @param sourceAddress  16 bit value of the address to read the data from.
 *
 *   Registers used:    w0 w1 w2
 *                      TBLPAG Preserved
 *
 *   Inputs:
 *   w0,w1 = long data - Address in flash to write   (24 bits)
 *   w2    = 16 bit - 16 bits of data address to write to flash   (16 bits)
 *   w4,w5 = long data - NVM Unlock Key
 *
 *  outputs:
 *   none
 *
 **/

    .global         _FLASH_NonBlockingWriteRow16
    .type           _FLASH_NonBlockingWriteRow16, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         NVMADRU
    .extern         NVMADR
    .extern         NVMSRCADRL
    reset

    _FLASH_NonBlockingWriteRow16:
    push    w0
    push    w1
    push    w2
    mov     w4 , w0
    mov     w5 , w1
    call    _FLASH_NonBlockingUnlock
    pop     w2
    pop     w1
    pop     w0

    mov     #((FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS*2)-1), w3     ;    get mask and validate all lower bits = 0
    and     w3, w0, w3
    bra     NZ,3f


    mov     W0,NVMADR
    mov     W1,NVMADRU

    mov     #WRITE_DWORD_CODE, W0
    mov     W0, NVMCON

    mov     #0xFA,W0
    mov     W0,TBLPAG
    mov     #0,W0

    mov     #FLASH_WRITE_ROW_SIZE_IN_INSTRUCTIONS, W1
    mov     #0xFF,   W4
1:
    tblwtl  [w2++], [W0]        ; Perform the TBLWT instructions to write the latches
    tblwth  W4,     [W0++]
    tblwtl  [w2++], [W0]        ; Perform the TBLWT instructions to write the latches
    tblwth  W4,     [W0--]

    call    _FLASH_NonBlockingSendNvmKey

prog_wait:
    btsc NVMCON, #15
    bra prog_wait


    inc2    NVMADR
    inc2    NVMADR
    sub     #2, W1
    bra     NZ, 1b

    call    _FLASH_NonBlockingLock
    mov     #1, w0              ; default return true
    btsc    NVMCON, #13         ; if error bit set,
3:  mov     #0, w0              ;   return false
    return;


;uint16_t FLASH_ErasePageOffsetGet(uint32_t flashAddress)
    .global     _FLASH_ErasePageOffsetGet
    .type       _FLASH_ErasePageOffsetGet, @function

_FLASH_ErasePageOffsetGet:
    mov     #((~FLASH_ERASE_PAGE_MASK) & 0xFFFF), W2
    and     w2, w0, w0
    return

;uint32_t FLASH_ErasePageAddressGet(uint32_t flashAddress);

    .global     _FLASH_ErasePageAddressGet
    .type       _FLASH_ErasePageAddressGet, @function

_FLASH_ErasePageAddressGet:

    mov     #(FLASH_ERASE_PAGE_MASK & 0xFFFF)	, W2	;LSW
    and     w2, w0, w0
    mov     #((FLASH_ERASE_PAGE_MASK >> 16) & 0xFFFF), W2 ; MSW
    and     w2, w1, w1

    return


   .popsection
