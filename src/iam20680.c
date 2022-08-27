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
    uint8_t rslt;
    uint8_t chip_id;

    rslt = iam20680_read_regs((uint8_t)IAM20680_CHIP_ID, &chip_id, 1, dev);

    return rslt;
}

