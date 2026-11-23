// multimod_OPT3001.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for OPT3001 functions

/************************************Includes***************************************/

#include "../multimod_OPT3001.h"

#include <stdint.h>
#include "../multimod_i2c.h"

/************************************Includes***************************************/

#define OPT3001_CONFIG_ADDR     0x01

/********************************Public Functions***********************************/

// OPT3001_Init
// Initializes OPT3001, configures it to continuous conversion mode.
//0x47
// Return: void
void OPT3001_Init(void) {
    // your code here
    OPT3001_WriteRegister(OPT3001_CONFIG_ADDR, OPT3001_CONFIG_CC_BM);
    return;
}

// OPT3001_WriteRegister
// Writes to a register in the OPT3001.
// Param uint8_t "addr": Register address of the OPT3001.
// Param uint16_t "data": 16-bit data to write to the register.
// Return: void
void OPT3001_WriteRegister(uint8_t addr, uint16_t data) {
    // your code here
    uint8_t command[3] = {addr, (uint8_t)(data >> 8), (uint8_t)(data)};
    I2C_WriteMultiple(I2C_A_BASE, OPT3001_ADDR, command, 3);
    return;
}

// OPT3001_ReadRegister
// Reads from a register in the OPT3001.
// Param uint8_t "addr": Register address of the OPT3001.
// Return: uint16_t
uint16_t OPT3001_ReadRegister(uint8_t addr) {
    // your code here
    I2C_WriteSingle(I2C_A_BASE, OPT3001_ADDR, addr);
    uint8_t data[2] = {};
    I2C_ReadMultiple(I2C_A_BASE, OPT3001_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[0]) << 8 | (uint16_t)(data[1]);
    return result;
}

// OPT3001_GetResult
// Gets conversion result, calculates byte result based on datasheet
// and configuration settings.
// Return: uint32_t
uint32_t OPT3001_GetResult(void) {
    // your code here
    uint32_t result = OPT3001_ReadRegister(OPT3001_RESULT_ADDR);
    result = LUX((result >> 12 & 0xF), (result & 0x0FFF));
    return result;
}

// OPT3001_SetLowLimit
// Sets the low limit register.
// Param uint16_t "exp": Exponential bound
// Param uint16_t "result": Conversion bound
// Return: void
void OPT3001_SetLowLimit(uint16_t exp, uint16_t result) {
    // your code here
    OPT3001_WriteRegister(OPT3001_LOWLIMIT_ADDR, (exp << OPT3001_RESULT_E_S | (result & 0xFFF)));
    return;
}

// OPT3001_SetHighLimit
// Sets the high limit register.
// Param uint16_t "exp": Exponential bound
// Param uint16_t "result": Conversion bound
// Return: void
void OPT3001_SetHighLimit(uint16_t exp, uint16_t result) {
    // your code here
    OPT3001_WriteRegister(OPT3001_HIGHLIMIT_ADDR, (exp << OPT3001_RESULT_E_S | (result & 0xFFF)));
    return;
}

// OPT3001_GetChipID
// Gets the chip ID of the OPT3001.
// Return: uint16_t
uint16_t OPT3001_GetChipID(void) {
    //return OPT3001_ReadRegister(/* where? */);
    // hint go look in the header files and datasheet for where the device ID is located
    uint16_t device_id = OPT3001_ReadRegister(OPT3001_DEVICEID_ADDR);
    return (device_id);
}

/********************************Public Functions***********************************/
