/**
 * @file    iam20680.c
 * @author  Joseph Gillispie
 * @date    22Aug2022
 * @brief   This is the source file for the TDK IAM-20680 acceleromter/gyrometer.
 */

/*! @file iam20680.c
 * @brief Driver for IAM-20680 sensor
 */
#include "iam20680.h"

/**\name Internal macros */

/**\name Internal APIs */

// Ex: power modes, calibration checks, etc

/*!
 * @brief This API must be called before other APIs. It verifies the chip ID of the sensor.
 */
uint8_t iam20680_init(struct iam20680_dev *dev)
{
    uint8_t status;
    uint8_t chip_id;
    uint8_t buff;

    // Reset device to defaults.
    buff = 0x80;
    status = iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);

    // Check chip ID to ensure IAM-20680 exists on board.
    status = iam20680_read_regs((uint8_t)IAM20680_WHO_AM_I, &chip_id, 1, dev);

	// Perform a reset and let IAM-20680 select best clock source.
    buff = 0x6B;
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);

    // Output data rate selection.
    // SAMPLE_RATE = INTERNAL_SAMPLE_RATE/(1 + SMPLRT_DIV), INTERNAL_SAMPLE_RATE = 1 kHz
    buff = 0x09;    // 100 Hz sample rate
    status |= iam20680_write_regs((uint8_t)IAM20680_SMPLRT_DIV, &buff, 1, dev);

    // Full scale range selection.
    buff = 0x01; // Acceleromter = 4 g
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG, &buff, 1, dev);
    
    buff = 0x01; // Gyrometer = 500 dps
    status |= iam20680_write_regs((uint8_t)IAM20680_GYRO_CONFIG, &buff, 1, dev);

    // Filter frequency selection.
    buff = 0x05; // 10.2 Hz 
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG, &buff, 1, dev);

    // Power mode selection.
    buff = 0x3F; // Acceleromter and gyrometer x, y, and z enabled
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_2, &buff, 1, dev);
  
    return status;
}

/*!
 * @brief This API writes the data to the given register address of the sensor.
 */
uint8_t iam20680_write_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct iam20680_dev *dev)
{
	// Write the data.
	dev->status = dev->write(reg_addr, reg_data, len);

	return dev->status;
}

/*!
 * @brief This API reads the data from the given register address of the sensor.
 */
uint8_t iam20680_read_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct iam20680_dev *dev)
{
	// Check if SPI is used.
	if (dev->interface == IAM20680_SPI)
	{
		reg_addr |= 0x80;
	}

	// Read the data.
    dev->status = dev->read(reg_addr, reg_data, len);

	return dev->status;
}

