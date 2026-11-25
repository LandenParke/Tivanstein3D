// multimod_i2c.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for I2C functions

/************************************Includes***************************************/

#include "../multimod_i2c.h"

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <inc/hw_i2c.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// I2C_Init
// Initializes specified I2C module
// Param uint32_t "mod": base address of module
// Return: void
void I2C_Init(uint32_t mod) {
    // your code here
    switch (mod) {
        case I2C_A_BASE:
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
            while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

            // does it require GPIO pins? if so, how do we configure them?
            GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
            GPIOPinConfigure(GPIO_PA6_I2C1SCL);

            GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
            GPIOPinConfigure(GPIO_PA7_I2C1SDA);

            // you get this for free
            I2CMasterInitExpClk(mod, SysCtlClockGet(), false);
            I2CMasterEnable(mod);
            break;
   }
}

// I2C_WriteSingle
// Writes a single byte to an address.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Param uint8_t "byte": byte to send
// Return: void
void I2C_WriteSingle(uint32_t mod, uint8_t addr, uint8_t byte) {
    // your code here
    // which module and where to?
        I2CMasterSlaveAddrSet(mod, addr, false);
        // what data?
        I2CMasterDataPut(mod, byte);
        // what mode?
        I2CMasterControl(mod,I2C_MASTER_CMD_SINGLE_SEND);
        // should we wait till finished?
        while(I2CMasterBusy(mod));
        return;
}

// I2C_ReadSingle
// Reads a single byte from address.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Return: uint8_t
uint8_t I2C_ReadSingle(uint32_t mod, uint8_t addr) {
    // your code here
    // which module and where to?
       I2CMasterSlaveAddrSet(mod, addr, true);
       // what data?
       // none
       // what mode?
       I2CMasterControl(mod, I2C_MASTER_CMD_SINGLE_RECEIVE);
       // should we wait till finished?
       while(I2CMasterBusy(mod));
       // return data from module
       uint8_t data = (uint8_t)(I2CMasterDataGet(mod));
       return (data);
}

// I2C_WriteMultiple
// Write multiple bytes to a device.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Param uint8_t* "data": pointer to an array of bytes
// Param uint8_t "num_bytes": number of bytes to transmit
// Return: void
void I2C_WriteMultiple(uint32_t mod, uint8_t addr, uint8_t* data, uint8_t num_bytes) {
   // your code here
   uint8_t num = 0;

   // which module and where to?
   I2CMasterSlaveAddrSet(mod, addr, false);

   // what data?
   I2CMasterDataPut(mod, data[num++]);

   // what mode?
   I2CMasterControl(mod, I2C_MASTER_CMD_BURST_SEND_START);

   // should be wait till finished?
   while(I2CMasterBusy(mod));

   // hint: how many times do we need to write?
   for (; num < num_bytes - 1; num++) {
       I2CMasterDataPut(mod, data[num]);
       I2CMasterControl(mod, I2C_MASTER_CMD_BURST_SEND_CONT);
       // should be wait till finished?
       while(I2CMasterBusy(mod));
   }
   // hint 2: how do we tell the module we're done sending data?
   I2CMasterDataPut(mod, data[num]);
   I2CMasterControl(mod, I2C_MASTER_CMD_BURST_SEND_FINISH);
   // should be wait till finished?
   while(I2CMasterBusy(mod));
   return;
}

// I2C_ReadMultiple
// Read multiple bytes from a device.
// Param uint32_t "mod": base address of module
// Param uint8_t "addr": address to device
// Param uint8_t* "data": pointer to an array of bytes
// Param uint8_t "num_bytes": number of bytes to read
// Return: void
void I2C_ReadMultiple(uint32_t mod, uint8_t addr, uint8_t* data, uint8_t num_bytes) {
    // your code here
    uint8_t num = 0;

    // which module and where to?
    I2CMasterSlaveAddrSet(mod, addr, true);

    // what data?
    //none

    // what mode?
    I2CMasterControl(mod, I2C_MASTER_CMD_BURST_RECEIVE_START);

    // should be wait till finished?
    while(I2CMasterBusy(mod));
    data[num++] = (uint8_t)(I2CMasterDataGet(mod));

    // hint: how many times do we need to read?
    for(; num < num_bytes - 1; num++) {
        I2CMasterControl(mod, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while(I2CMasterBusy(mod));
        data[num] = (uint8_t)(I2CMasterDataGet(mod));
    }

    // hint 2: how do we tell the module we're done receiving data?
    I2CMasterControl(mod, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(mod));
    data[num] = (uint8_t)(I2CMasterDataGet(mod));

    return;
}

/********************************Public Functions***********************************/

