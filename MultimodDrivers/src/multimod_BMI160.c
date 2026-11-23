// multimod_BMI160.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for BMI160 functions

/************************************Includes***************************************/

#include "../multimod_BMI160.h"

#include <stdint.h>
#include "../multimod_i2c.h"

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// BMI160_Init
// Initializes the BMI160. Currently enables the accelerometer
// in full-power mode.
//0x69
// Return: void
void BMI160_Init() {
    // your code here
    I2C_Init(I2C_A_BASE);
    BMI160_WriteRegister(BMI160_CMD_ADDR, BMI160_ACCEL_ENABLE);
    BMI160_WriteRegister(BMI160_CMD_ADDR, BMI160_GYRO_ENABLE);
}

// BMI160_WriteRegister
// Writes to a register address in the BMI160.
// Param uint8_t "addr": Register address
// Param uint8_t "data": data to write
// Return: void
void BMI160_WriteRegister(uint8_t addr, uint8_t data) {
    // your code here
    uint8_t command[2] = {addr, data};
    I2C_WriteMultiple(I2C_A_BASE, BMI160_ADDR, command, 2);
}

// BMI160_ReadRegister
// Reads from a register address in the BMI160.
// Param uint8_t "addr": Register address
// Return: void
uint8_t BMI160_ReadRegister(uint8_t addr) {
    // your code here
    I2C_WriteSingle(I2C_A_BASE, BMI160_ADDR, addr);
    uint8_t value = I2C_ReadSingle(I2C_A_BASE, BMI160_ADDR);
    return value;
}

// BMI160_MultiReadRegister
// Uses the BMI160 auto-increment function to read from multiple registers.
// Param uint8_t "addr": beginning register address
// Param uint8_t* "data": pointer to an array to store data in
// Param uint8_t "num_bytes": number of bytes to read
// Return: void
void BMI160_MultiReadRegister(uint8_t addr, uint8_t* data, uint8_t num_bytes) {
    // your code here
    I2C_WriteSingle(I2C_A_BASE, BMI160_ADDR, addr);
    I2C_ReadMultiple(I2C_A_BASE, BMI160_ADDR, data, num_bytes);
}

// BMI160_AccelXGetResult
// Gets the 16-bit x-axis acceleration result.
// Return: uint16_t
int16_t BMI160_AccelXGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_ACCELX_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_AccelYGetResult
// Gets the 16-bit y-axis acceleration result.
// Return: uint16_t
int16_t BMI160_AccelYGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_ACCELY_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_AccelZGetResult
// Gets the 16-bit z-axis acceleration result.
// Return: uint16_t
int16_t BMI160_AccelZGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_ACCELZ_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_GyroXGetResult
// Gets the 16-bit x-axis gyroscope result.
// Return: uint16_t
int16_t BMI160_GyroXGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_GYROX_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_GyroYGetResult
// Gets the 16-bit y-axis gyroscope result.
// Return: uint16_t
int16_t BMI160_GyroYGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_GYROY_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_GyroZGetResult
// Gets the 16-bit z-axis gyroscope result.
// Return: uint16_t
int16_t BMI160_GyroZGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_GYROZ_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_MagXGetResult
// Gets the 16-bit x-axis magnetometer result.
// Return: uint16_t
int16_t BMI160_MagXGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_MAGX_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_MagYGetResult
// Gets the 16-bit y-axis magnetometer result.
// Return: uint16_t
int16_t BMI160_MagYGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_MAGY_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_MagZGetResult
// Gets the 16-bit z-axis magnetometer result.
// Return: uint16_t
int16_t BMI160_MagZGetResult() {
    // your code here
    uint8_t data[2] = {};
    BMI160_MultiReadRegister(BMI160_MAGZ_ADDR, data, 2);
    uint16_t result = (uint16_t)(data[1]) << 8 | (uint16_t)(data[0]);
    return result;
}

// BMI160_AccelXYZGetResult
// Stores the 16-bit XYZ accelerometer results in an array.
// Param uint16_t* "data": pointer to an array of 16-bit data.
// Return: void
void BMI160_AccelXYZGetResult(uint16_t* data) {
    // your code here
    data[0] = BMI160_AccelXGetResult();
    data[1] = BMI160_AccelYGetResult();
    data[2] = BMI160_AccelZGetResult();
}

// BMI160_GyroXYZGetResult
// Stores the 16-bit XYZ gyroscope results in an array.
// Param uint16_t* "data": pointer to an array of 16-bit data.
// Return: void
void BMI160_GyroXYZGetResult(uint16_t* data) {
    // your code here
    data[0] = BMI160_GyroXGetResult();
    data[1] = BMI160_GyroYGetResult();
    data[2] = BMI160_GyroZGetResult();
}

// BMI160_MagXYZGetResult
// Stores the 16-bit XYZ magnetometer results in an array.
// Param uint16_t* "data": pointer to an array of 16-bit data.
// Return: void
void BMI160_MagXYZGetResult(uint16_t* data) {
    // your code here
    data[0] = BMI160_MagXGetResult();
    data[1] = BMI160_MagYGetResult();
    data[2] = BMI160_MagZGetResult();

}

// BMI160_GetDataStatus
// Gets the status register to determine if data is ready to read.
// Return: uint8_t
uint8_t BMI160_GetDataStatus() {
    return BMI160_ReadRegister(BMI160_STATUS_ADDR);
}

/********************************Public Functions***********************************/
