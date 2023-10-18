/**
  DEE Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    dee_helper.h

  @Summary
    This is the generated header file for the DEE library using Melody Code Configurator

  @Description
    This header file provides APIs for library DEE.
    The generated drivers are tested against the following:
*/

#ifndef DEE_HELPER_H
#define DEE_HELPER_H

#include "dee.h"

/**
  @Summary
    This function initializes all of the EEPROM banks

  @Description
    This functions performs the same functionality as DEE_Init() , but this function works on the
    active partition in devices with dual partition support.

  @Param
    None.

  @Returns
    Returns DEE_RETURN_STATUS
*/
DEE_RETURN_STATUS  DEE_ActivePartitionInit(void);

/**
  @Summary
    This function copies the DEE data from active partition to in-active partition

  @Description
    This function initializes the DEE data structure in the active partition and check if there is any data available
    in the active partition from the previous execution if so it copies the data to the inactive partition.

  @Param
    None.

  @Returns
    Returns True or False.
*/
bool  DEE_ActiveToInactivePartitionDataCopy(void);

#endif	/* DEE_HELPER_H */