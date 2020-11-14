/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <string.h>


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define RX_DATA_LENGTH 7

typedef struct
{
    /* currentAddrPtr - to allow for sequential read (from the current address) */
    uint16_t                    currentAddrPtr;
    /* addrIndex - used to copy 2 bytes of EEPROM memory address */
    uint8_t                     addrIndex;        
} CONTAINER_DATA;

CONTAINER_DATA contData;
uint8_t I2C_RxData[RX_DATA_LENGTH];
uint8_t internalData[] = {
    'C', 'H', 'R', 'I', 'S'
};

bool APP_I2C_CLIENT_Callback (I2C_SLAVE_TRANSFER_EVENT event, uintptr_t contextHandle) {
    
    bool isSuccess = true;
    
    switch (event) {
        case I2C_SLAVE_TRANSFER_EVENT_ADDR_MATCH:
            contData.addrIndex = 2;
            LED1_Set();
            break;
            
        case I2C_SLAVE_TRANSFER_EVENT_RX_READY:
            if (contData.addrIndex > 0) {
                ((uint8_t*)&contData.currentAddrPtr)[--contData.addrIndex] = I2C1_ReadByte();
            } else {
                I2C_RxData[contData.currentAddrPtr++] = I2C1_ReadByte();
                LED3_Set();
            }
            event = I2C_SLAVE_TRANSFER_EVENT_NONE;
            break;
        case I2C_SLAVE_TRANSFER_EVENT_NONE:
            if (memcmp(&internalData[0], &I2C_RxData[2], RX_DATA_LENGTH) == 0) {
                LED2_Set();
            }

            break;
        default:
            break;
    }
    
    return isSuccess;
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    I2C1_CallbackRegister(APP_I2C_CLIENT_Callback, 0);

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

