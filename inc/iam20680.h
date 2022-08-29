/**
 * @file    iam20680.h
 * @author  Joseph Gillispie
 * @date    22Aug2022
 * @brief   This is the header file for the TDK IAM-20680 acceleromter/gyrometer.
 */

#ifndef __IAM20680_H
#define __IAM20680_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * INCLUDES
 ****************************************************************************/ 
/**
 * @brief Required includes
 */
#include <stdio.h>
#include <stdint.h>

/*****************************************************************************
 * MACROS AND DEFINES 
 ****************************************************************************/ 
/**
 * @brief Register addresses
 */ 
#define IAM20680_SELF_TEST_X_GYRO   0x00
#define IAM20680_SELF_TEST_Y_GYRO   0x01
#define IAM20680_SELF_TEST_Z_GYRO   0x02
#define IAM20680_SELF_TEST_X_ACCEL  0x0D
#define IAM20680_SELF_TEST_Y_ACCEL  0x0E
#define IAM20680_SELF_TEST_Z_ACCEL  0x0F
#define IAM20680_XG_OFFS_USRH       0x13
#define IAM20680_XG_OFFS_USRL       0x14
#define IAM20680_YG_OFFS_USRH       0x15
#define IAM20680_YG_OFFS_USRL       0x16
#define IAM20680_ZG_OFFS_USRH       0x17
#define IAM20680_ZG_OFFS_USRL       0x18
#define IAM20680_SMPLRT_DIV         0x19
#define IAM20680_CONFIG             0x1A
#define IAM20680_GYRO_CONFIG        0x1B
#define IAM20680_ACCEL_CONFIG       0x1C
#define IAM20680_ACCEL_CONFIG2      0x1D
#define IAM20680_LP_MODE_CFG        0x1E
#define IAM20680_ACCEL_WOM_THR      0x1F
#define IAM20680_FIFO_EN            0x23
#define IAM20680_FSYNC_INT          0x36
#define IAM20680_INT_PIN_CFG        0x37
#define IAM20680_INT_ENABLE         0x38
#define IAM20680_INT_STATUS         0x3A
#define IAM20680_ACCEL_XOUT_H       0x3B
#define IAM20680_ACCEL_XOUT_L       0x3C
#define IAM20680_ACCEL_YOUT_H       0x3D
#define IAM20680_ACCEL_YOUT_L       0x3E
#define IAM20680_ACCEL_ZOUT_H       0x3F
#define IAM20680_ACCEL_ZOUT_L       0x40
#define IAM20680_TEMP_OUT_H         0x41
#define IAM20680_TEMP_OUT_L         0x42
#define IAM20680_GYRO_XOUT_H        0x43
#define IAM20680_GYRO_XOUT_L        0x44
#define IAM20680_GYRO_YOUT_H        0x45
#define IAM20680_GYRO_YOUT_L        0x46
#define IAM20680_GYRO_ZOUT_H        0x47
#define IAM20680_GYRO_ZOUT_L        0x48
#define IAM20680_SIGNAL_PATH_RESET  0x68
#define IAM20680_ACCEL_INTEL_CTRL   0x69
#define IAM20680_USER_CTRL          0x6A
#define IAM20680_PWR_MGMT_1         0x6B
#define IAM20680_PWR_MGMT_2         0x6C
#define IAM20680_FIFO_COUNTH        0x72
#define IAM20680_FIFO_COUNTL        0x73
#define IAM20680_FIFO_R_W           0x74
#define IAM20680_WHO_AM_I           0x75
#define IAM20680_XA_OFFSET_H        0x77
#define IAM20680_XA_OFFSET_L        0x78
#define IAM20680_YA_OFFSET_H        0x7A
#define IAM20680_YA_OFFSET_L        0x7B
#define IAM20680_ZA_OFFSET_H        0x7D
#define IAM20680_ZA_OFFSET_L        0x7E

/**\name Status */
#define IAM20680_OK     0x00 /*< OK */
#define IAM20680_ERR    0x01 /*< ERROR */
 
/**\name Who Am I */
#define IAM20680_CHIP_ID    0xA9

/*****************************************************************************
 * TYPEDEFS
 ****************************************************************************/ 
/**
 * @brief Type definitions
 */

/**
 * @brief Bus communication read function pointer. This should be mapped to
 * the platform-specific read function of the user application.
 *
 * @param[in] reg_addr      : Register address from which data is read.
 * @param[in] reg_data      : Pointer to data buffer where read data is stored.
 * @param[in] len           : Number of bytes of data to be read.
 * 
 * @retval 0 -> Success.
 * @retval Non-zero -> Fail.
 */
typedef uint8_t (*iam20680_read_fptr_typedef)(uint8_t reg_addr, uint8_t *reg_data, uint16_t len);


/**
 * @brief Bus communication write function pointer. Should be mapped to
 * the platform-specific write function of the user application.
 *
 * @param[in] reg_addr      : Register address to which data is written.
 * @param[in] reg_data      : Pointer to data buffer in which data to be written is stored.
 * @param[in] len           : Number of bytes of data to write.
 *
 * @retval 0 -> Success.
 * @retval Non-zero -> Fail.
 */
typedef uint8_t (*iam20680_write_fptr_typedef)(uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

/**
 * @brief IAM-20680 accelerometer and gyrometer data.
 */
struct iam20680_data {
    
    int32_t accel;  /*< Raw acclerometer data */
    int32_t gyro;   /*< Raw gyrometer data */
};

/**
 * @brief IAM-20680 register settings.
 */
struct iam20680_settings {
    
};

/**
 * @brief IAM-20680 device parameters.
 */
struct iam20680_dev {
    iam20680_read_fptr_typedef read;    /*< Read function pointer */
    iam20680_write_fptr_typedef write;  /*< Write function pointer */
    struct iam20680_settings settings;         /*< Sensor settings */
    uint8_t status;                     /*< Returned status of read/write functions */
};


/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 ****************************************************************************/ 
/**
 * \ingroup iam20680
 * \defgroup iam20680ApiInit Initialization
 * @brief Initialize the sensor and device structure
 */

/*!
 * \ingroup iam20680ApiInit
 * \page iam20680_api_iam20680_init iam20680_init
 * \code
 * uint8_t iam20680_init(struct iam20680_dev *dev);
 * \endcode
 * @details This API must be called before other APIs. It verifies the chip ID of the sensor.
 * 
 * @param[in, out] dev : Structure Instance of iam20680_dev
 * @return Result of API execution status.
 *
 * @retval 0 -> Success
 * @retval Non-zero -> Fail
 */
uint8_t iam20680_init(struct iam20680_dev *dev);

/**
 * \ingroup iam20680
 * \defgroup iam20680ApiRegister Registers
 * @brief Generic API for accessing sensor registers
 */

/*!
 * \ingroup iam20680ApiRegister
 * \page iam20680_api_iam20680_write_regs iam20680_write_regs
 * \code
 * uint8_t iam20680_write_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct iam20680_dev *dev);
 * \endcode
 * @details This API writes the given data to the register address of the sensor
 *
 * @param[in] reg_addr  : Register address to where the data is to be written.
 * @param[in] reg_data  : Pointer to data buffer which is to be written in the reg_addr of sensor.
 * @param[in] len       : Number of bytes of data to write.
 * @param[in, out]      : Structure instance of iam20680_dev.
 *
 * @regurn Result of API execution status.
 *
 * @retval 0 -> Success.
 * @retvan Non-zero -> Fail. 
 *
 */
uint8_t iam20680_write_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct iam20680_dev *dev);

/*!
 * \ingroup iam20680ApiRegister
 * \page iam20680_api_iam20680_read_regs iam20680_read_regs
 * \code
 * uint8_t iam20680_read_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct iam20680_dev *dev);
 * \endcode
 * @details This API writes the given data to the register address of the sensor
 *
 * @param[in] reg_addr  : Register address from where the data is to be read.
 * @param[in] reg_data  : Pointer to data buffer to store the read data.
 * @param[in] len       : Number of bytes of data to be read.
 * @param[in, out]      : Structure instance of iam20680_dev.
 *
 * @regurn Result of API execution status.
 *
 * @retval 0 -> Success.
 * @retvan Non-zero -> Fail. 
 *
 */
uint8_t iam20680_read_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, struct iam20680_dev *dev);



#ifdef __cplusplus
}
#endif

#endif /* __IAM20680_H */
