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
    uint8_t buff;

    // Reset device to defaults.
//    buff = 0x80;
//    status = iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    
    // Check chip ID to ensure IAM-20680 exists on board.
    status = iam20680_read_regs((uint8_t)IAM20680_WHO_AM_I, &buff, 1, dev);
    dev->chip_id = buff;

    // Configure int pin as data ready.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_INT_PIN_CFG, &buff, 1, dev);
    buff &= ~0x01;
    buff |= 1 << 0; // DATA_RDY_INT_EN
    status |= iam20680_write_regs((uint8_t)IAM20680_INT_PIN_CFG, &buff, 1, dev);
    
   
    // Set gyro low noise mode.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_LP_MODE_CFG, &buff, 1, dev);
    buff &= ~0x80;
    buff |= 1 << 7; // GYRO_CYCLE
    status |= iam20680_write_regs((uint8_t)IAM20680_LP_MODE_CFG, &buff, 1, dev);
    
    // Set accel low noise mode.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    buff &= ~0x20;
    buff |= 1 << 5; // ACCEL_CYCLE
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    
    // Wait 20 ms.
    for(uint32_t i = 0; i < 1000000; i++);  // >>> 20 ms

    // Bypass gyro DLPF.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_GYRO_CONFIG, &buff, 1, dev);
    buff &= ~0x03;
    buff |= 0 << 0;     // FCHOICE
    status |= iam20680_write_regs((uint8_t)IAM20680_GYRO_CONFIG, &buff, 1, dev);

    // Bypass accel DLPF.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);
    buff &= ~0x08;
    buff |= 0 << 3;     // ACCEL_FCHOICE_B
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);
    
    // Set DLPF_CFG.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_CONFIG, &buff, 1, dev);
    buff &= ~0x07;
    buff |= 1 << 0;     // DLPF_CFG 
    status |= iam20680_write_regs((uint8_t)IAM20680_CONFIG, &buff, 1, dev);
    
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);
    buff &= ~0x07;
    buff |= 1 << 0;     // A_DLPF_CFG
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);

    // Set averaging filter.
    // Gryo
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_LP_MODE_CFG, &buff, 1, dev);
    buff &= ~0x70;
    buff |= 0 << 4;      // G_AVGCFG
    status |= iam20680_write_regs((uint8_t)IAM20680_LP_MODE_CFG, &buff, 1, dev);
    // Accel
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);
    buff &= ~0x30;
    buff |= 0 << 4;     // DEC2_CFG
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);
    
    // Set SMPRT_DIV.
    buff = 20000/1000 - 1;  // 20 ms / 50 Hz
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG2, &buff, 1, dev);
    
    // Set full scale range.
    // Gyro
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_GYRO_CONFIG, &buff, 1, dev);
    buff &= ~0x18;
    buff |= 3 << 3;     // FS_SEL 
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG, &buff, 1, dev);
    // Accel
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_ACCEL_CONFIG, &buff, 1, dev);
    buff &= ~0x18;
    buff |= 0 << 3;     // ACCEL_FS_SEL 
    status |= iam20680_write_regs((uint8_t)IAM20680_ACCEL_CONFIG, &buff, 1, dev);

    // Enable accel.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_PWR_MGMT_2, &buff, 1, dev);
    buff &= ~0x38;
    buff |= 0 << 3;     // Enable x, y, and z.
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_2, &buff, 1, dev);
    for(uint32_t i = 0; i < 1000000; i++);  // >>> 20 ms
    

    // Enable gyro.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_PWR_MGMT_2, &buff, 1, dev);
    buff &= ~0x07;
    buff |= 0 << 0;     // Enable x, y, and z.
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_2, &buff, 1, dev);
    for(uint32_t i = 0; i < 1000000; i++);  // >>> 50 ms
    
    // Reset and enable FIFO.
    // Disable FIFO
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_USER_CTRL, &buff, 1, dev);
    buff &= ~0x40;
    buff |= 0 << 6;     // FIFO_EN
    status |= iam20680_write_regs((uint8_t)IAM20680_USER_CTRL, &buff, 1, dev);
    // Reset FIFO
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_USER_CTRL, &buff, 1, dev);
    buff &= ~0x04;
    buff |= 0 << 2;     // FIFO_RST
    status |= iam20680_write_regs((uint8_t)IAM20680_USER_CTRL, &buff, 1, dev);
    // Enable gyro FIFO
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_FIFO_EN, &buff, 1, dev);
    buff &= ~0x70;
    buff |= 7 << 4;     // Write x, y, and z to FIFO at data rate.
    status |= iam20680_write_regs((uint8_t)IAM20680_FIFO_EN, &buff, 1, dev);
    // Enable accel FIFO
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_FIFO_EN, &buff, 1, dev);
    buff &= ~0x08;
    buff |= 1 << 3;     // Write x, y, and z to FIFO at data rate.
    status |= iam20680_write_regs((uint8_t)IAM20680_FIFO_EN, &buff, 1, dev);
    // Enable FIFO
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_USER_CTRL, &buff, 1, dev);
    buff &= ~0x40;
    buff |= 1 << 6;     // FIFO_EN
    status |= iam20680_write_regs((uint8_t)IAM20680_USER_CTRL, &buff, 1, dev);
  
    return status;
}

/*!
 * @brief This API must be called before other APIs. It verifies the chip ID of the sensor.
 */
uint8_t iam20680_init2(struct iam20680_dev *dev)
{
    uint8_t status = 0x00;
    uint8_t buff;
    
    // Check WHO_AM_I register.
    status = iam20680_read_regs((uint8_t)IAM20680_WHO_AM_I, &buff, 1, dev);
    dev->chip_id = buff;
    
    // Reset driver states.
    status |= iam20680_read_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    buff |= 1 << 7; // DEVICE_RESET
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    for (uint32_t i = 0; i < 1000000; i++);
    buff = 0x00;
    while ((buff & (0x80)) != 0x00)
    {
        iam20680_read_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    }

    // Wake up.
    buff = 0x00;
    status |= iam20680_read_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);
    buff &= ~0x40;
    status |= iam20680_write_regs((uint8_t)IAM20680_PWR_MGMT_1, &buff, 1, dev);

    // Set up CLKSEL.
    // Disable gyro and accel.
    // Set full scale range.
    // Set bandwidth.
    // Set averaging filter.
    // Set sampling rate.
    // Disable FIFO.
    // Configure FIFO.
    // Enable Data Ready interrupt.

    
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

