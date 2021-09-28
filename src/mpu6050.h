#pragma once

#ifndef __MPU6050_H__
#define __MPU6050_H__

/**
 * @file mpu6050.h
 * 
 * @brief Additional compilation unit contains a lightweight library for MPU6050.
 * 
 * Header consist of register's defines for MPU6050, implementing in 
 * MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2:
 * 
 * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 *  
 */

#include <mbed.h>
#include <math.h>

#define XGOFFS_TC 0x00
#define YGOFFS_TC 0x01
#define ZGOFFS_TC 0x02

#define X_FINE_GAIN 0x03
#define Y_FINE_GAIN 0x04
#define Z_FINE_GAIN 0x05

#define XA_OFFSET_H 0x06
#define XA_OFFSET_L_TC 0x07
#define YA_OFFSET_H 0x08
#define YA_OFFSET_L_TC 0x09
#define ZA_OFFSET_H 0x0A
#define ZA_OFFSET_L_TC 0x0B

#define SELF_TEST_X 0x0D
#define SELF_TEST_Y 0x0E
#define SELF_TEST_Z 0x0F
#define SELF_TEST_A 0x10

#define XG_OFFS_USRH 0x13
#define XG_OFFS_USRL 0x14
#define YG_OFFS_USRH 0x15
#define YG_OFFS_USRL 0x16
#define ZG_OFFS_USRH 0x17
#define ZG_OFFS_USRL 0x18

#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C

#define FF_THR 0x1D
#define FF_DUR 0x1E
#define MOT_THR 0x1F
#define MOT_DUR 0x20
#define ZMOT_THR 0x21
#define ZRMOT_DUR 0x22
#define FIFO_EN 0x23

#define I2C_MST_CTRL 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27
#define I2C_SLV1_ADDR 0x28
#define I2C_SLV1_REG 0x29
#define I2C_SLV1_CTRL 0x2A
#define I2C_SLV2_ADDR 0x2B
#define I2C_SLV2_REG 0x2C
#define I2C_SLV2_CTRL 0x2D
#define I2C_SLV3_ADDR 0x2E
#define I2C_SLV3_REG 0x2F
#define I2C_SLV3_CTRL 0x30
#define I2C_SLV4_ADDR 0x31
#define I2C_SLV4_REG 0x32
#define I2C_SLV4_DO 0x33
#define I2C_SLV4_CTRL 0x34
#define I2C_SLV4_DI 0x35
#define I2C_MST_STATUS 0x36

#define INT_PIN_CFG 0x37 
#define INT_ENABLE 0x38

#define DMP_INT_STATUS 0x39 

#define INT_STATUS 0x3A

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60

#define MOT_DETECT_STATUS 0x61

#define I2C_SLV0_DO 0x63
#define I2C_SLV1_DO 0x64
#define I2C_SLV2_DO 0x65
#define I2C_SLV3_DO 0x66
#define I2C_MST_DELAY_CTRL 0x67

#define SIGNAL_PATH_RESET 0x68

#define MOT_DETECT_CTRL 0x69

#define USER_CTRL 0x6A

#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C

#define DMP_BANK 0x6D
#define DMP_RW_PNT 0x6E
#define DMP_REG 0x6F
#define DMP_REG_1 0x70
#define DMP_REG_2 0x71

#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74

#define WHO_AM_I_MPU6050 0x75

#define MPU6050_INTCFG_INT_LEVEL_BIT 7
#define MPU6050_INTCFG_INT_OPEN_BIT 6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT 5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT 4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT 3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT 2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT 1
#define MPU6050_INTCFG_CLKOUT_EN_BIT 0

#define ADO 0
#if ADO
#define MPU6050_ADDRESS 0x69 << 1 /* Device address when ADO = 1 */
#else
#define MPU6050_ADDRESS 0x68 << 1 /* Device address when ADO = 0 */
#endif

#define MPU_I2C_SDA P0_26
#define MPU_I2C_SCL P0_27

I2C i2c(MPU_I2C_SDA, MPU_I2C_SCL);

enum Ascale
{
    AFS_2G = 0,
    AFS_4G,
    AFS_8G,
    AFS_16G
};

enum Gscale
{
    GFS_250DPS = 0,
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

static int Gscale = GFS_250DPS; /* Set gyro  scale */
static int Ascale = AFS_2G;     /* Set accel scale */
static float aRes, gRes;                                        /* Scale resolutions per LSB for the sensors           */
// static float ax, ay, az;                                        /* Stores the real accel value in g's                  */
static int16_t gyroCount[3];                                    /* Stores the 16-bit signed gyro sensor output         */
// static float gx, gy, gz;                                        /* Stores the real gyro value in degrees per seconds   */
static float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0}; /* Bias corrections for gyro and accelerometer         */

/**
 * @brief Mapping two values, based on ranges
 *
 * @param value    Integer number, out variable
 * @param from_min Minimal bound of value
 * @param from_max Maximum bound of value
 * @param to_min   Minimal bound of new value
 * @param to_max   Minimal bound of new value
 *   
 * @return long output value 
 */
long fromADCtoInt(long value, long from_min, long from_max, long to_min, long to_max)
{
    return (value - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}

class MPU6050
{
protected:
public:
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
    {
        char data_write[2];
        data_write[0] = subAddress;
        data_write[1] = data;
        i2c.write(address, data_write, 2, 0);
    }

    char readByte(uint8_t address, uint8_t subAddress)
    {
        char data[1]; 
        char data_write[1];
        data_write[0] = subAddress;
        i2c.write(address, data_write, 1, 1);
        i2c.read(address, data, 1, 0);
        return data[0];
    }

    void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest)
    {
        char data[14];
        char data_write[1];
        data_write[0] = subAddress;
        i2c.write(address, data_write, 1, 1); // no stop
        i2c.read(address, data, count, 0);
        for (int ii = 0; ii < count; ii++)
        {
            dest[ii] = data[ii];
        }
    }

    /**
     * @brief Possible gyro scales.
     *
     * Their register bit settings are:
     * 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS (11).
     * Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value.
     */
    void getGres()
    {
        switch (Gscale)
        {
        case GFS_250DPS:
            gRes = 250.0 / 32768.0;
            break;
        case GFS_500DPS:
            gRes = 500.0 / 32768.0;
            break;
        case GFS_1000DPS:
            gRes = 1000.0 / 32768.0;
            break;
        case GFS_2000DPS:
            gRes = 2000.0 / 32768.0;
            break;
        }
    }

    /**
     * @brief Possible accelerometer scales.
     *
     * Their register bit settings) are:
     * 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
     * Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value.
     */
    void getAres()
    {
        switch (Ascale)
        {
        case AFS_2G:
            aRes = 2.0 / 32768.0;
            break;
        case AFS_4G:
            aRes = 4.0 / 32768.0;
            break;
        case AFS_8G:
            aRes = 8.0 / 32768.0;
            break;
        case AFS_16G:
            aRes = 16.0 / 32768.0;
            break;
        }
    }

    /**
     * @brief Read x/y/z accel data
     *
     * The x/y/z accel register data stored in <rawData>. Read the six raw data registers into data array.
     * It turn the MSB and LSB into a signed 16-bit value.
     * 
     * @return None
     */
    void readAccelData(int16_t *destination)
    {
        uint8_t rawData[6];
        readBytes(MPU6050_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);            
        destination[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]); 
        destination[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]);
        destination[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]);
    }

    /**
     * @brief Read x/y/z gyro data
     *
     * The x/y/z gyro register data stored here <rawData>. Read the six raw data registers sequentially into data array.
     * It turn the MSB and LSB into a signed 16-bit value.
     * 
     * @return None
     */
    void readGyroData(int16_t *destination)
    {
        uint8_t rawData[6];
        readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);             
        destination[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]); 
        destination[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]);
        destination[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]);
    }

    /**
     * @brief Gettin gyro-X value
     *
     * Calculate and convert to uint8_t small range gyro X value. 
     * 
     * @return <uint8_t> converted value.  
     */
    uint8_t getTinyGyroX(void)
    {
        return fromADCtoInt(gyroCount[0], -32768, +32767, -250, +250);
    }

    /**
     * @brief Gettin gyro-Y value
     *
     * Calculate and convert to uint8_t small range gyro Y value. 
     * 
     * @return <uint8_t> converted value.  
     */
    uint8_t getTinyGyroY(void)
    {
        return fromADCtoInt(gyroCount[1], -32768, +32767, -250, +250);
    }

    /**
     * @brief Gettin gyro-Z value
     *
     * Calculate and convert to uint8_t small range gyro Z value. 
     * 
     * @return <uint8_t> converted value.  
     */
    uint8_t getTinyGyroZ(void)
    {
        return fromADCtoInt(gyroCount[2], -32768, +32767, -250, +250);
    }

    /**
     * @brief Configure the motion detection control for low power accelerometer mode
     *
     * The sensor has a high-pass filter necessary to invoke to allow the sensor motion detection algorithms work properly
     * Motion detection occurs on free-fall (acceleration below a threshold for some time for all axes), motion (acceleration
     * above a threshold for some time on at least one axis), and zero-motion toggle (acceleration on each axis less than a
     * threshold for some time sets this flag, motion above the threshold turns it off). The high-pass filter takes gravity out
     * consideration for these threshold evaluations.
     * 
     * @return None
     */
    void lowPowerAccelOnly()
    {
        uint8_t c = readByte(MPU6050_ADDRESS, PWR_MGMT_1);
        /* Clear sleep and cycle bits [5:6] */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c & ~0x30);
        /* Set sleep and cycle bits [5:6] to zero to make sure accelerometer is running */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c | 0x30);

        c = readByte(MPU6050_ADDRESS, PWR_MGMT_2);
        /* Clear standby XA, YA, and ZA bits [3:5] */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c & ~0x38);
        /* Set XA, YA, and ZA bits [3:5] to zero to make sure accelerometer is running */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c | 0x00);

        c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
        /* Clear high-pass filter bits [2:0] */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x07);
        /* Set ACCEL_HPF to 0; reset mode disbaling high-pass filter */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | 0x00);

        c = readByte(MPU6050_ADDRESS, CONFIG);
        /* Clear low-pass filter bits [2:0] */
        writeByte(MPU6050_ADDRESS, CONFIG, c & ~0x07);
        /* Set DLPD_CFG to 0; 260 Hz bandwidth, 1 kHz rate */
        writeByte(MPU6050_ADDRESS, CONFIG, c | 0x00);

        c = readByte(MPU6050_ADDRESS, INT_ENABLE);
        /* Clear all interrupts */
        writeByte(MPU6050_ADDRESS, INT_ENABLE, c & ~0xFF);
        /* Enable motion threshold (bits 5) interrupt only */
        writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x40);
        
        /* Set motion detection to 0.256 g; LSB = 2 mg */
        writeByte(MPU6050_ADDRESS, MOT_THR, 0x80);
        /* Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate */
        writeByte(MPU6050_ADDRESS, MOT_DUR, 0x01);

        /* Add delay for accumulation of samples */
        thread_sleep_for(100);

        c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
        /* Clear high-pass filter bits [2:0] */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x07);
        /* Set ACCEL_HPF to 7; hold the initial accleration value as a referance */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | 0x07);

        c = readByte(MPU6050_ADDRESS, PWR_MGMT_2);
        /* Clear standby XA, YA, and ZA bits [3:5] and LP_WAKE_CTRL bits [6:7] */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c & ~0xC7);
        /* Set wakeup frequency to 5 Hz, and disable XG, YG, and ZG gyros (bits [0:2]) */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c | 0x47);

        c = readByte(MPU6050_ADDRESS, PWR_MGMT_1);
        /* Clear sleep and cycle bit 5 */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c & ~0x20);
        /* Set cycle bit 5 to begin low power accelerometer motion interrupts */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c | 0x20);
    }

    /**
     * @brief Reset MPU device
     *
     * Write a one to bit 7 reset bit and toggle reset device
     * 
     * @return None
     */
    void reset()
    {
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); 
        thread_sleep_for(100);
    }

    /**
     * @brief Initialize MPU device
     *
     * Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
     * DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both.
     * Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate.
     * 
     * @return None
     */
    void init()
    {
        /* Clear sleep mode bit (6), enable all sensors */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);
        thread_sleep_for(100);

        /* Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001 */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
        writeByte(MPU6050_ADDRESS, CONFIG, 0x03);

        /* Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV) */
        /* Use a 200 Hz rate; the same rate set in CONFIG above */
        writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x04);

        /* Set gyroscope full scale range */
        /* Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3 */
        uint8_t c = readByte(MPU6050_ADDRESS, GYRO_CONFIG);
        /* Clear self-test bits [7:5] */
        writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0xE0);
        /* Clear AFS bits [4:3] */
        writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0x18);
        /* Set full scale range for the gyro */
        writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c | Gscale << 3);

        /* Set accelerometer configuration */
        c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
        /* Clear self-test bits [7:5] */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0xE0);
        /* Clear AFS bits [4:3] */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x18);
        /* Set full scale range for the accelerometer */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | Ascale << 3);

        /* Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN */
        writeByte(MPU6050_ADDRESS, INT_PIN_CFG, 0x22);
        /* 0x01 Enable data ready (bit 0) interrupt */
        writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x01);
    }

    /**
     * @brief Calibrate device before adress advertising
     *
     * Function which accumulates gyro and accelerometer data after device initialization. 
     * It calculates the average of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
     * Reset device, reset all registers, clear gyro and accelerometer bias registers.
     * 
     * @return None
     */
    void calibrate(float *dest1, float *dest2)
    {
        uint8_t data[12];
        uint16_t p, packet_count, fifo_count;
        int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

        /* Write a one to bit 7 reset bit; toggle reset device */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80);
        thread_sleep_for(100);

        /* Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001 */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
        writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x00);
        thread_sleep_for(100);

        writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);   /* Disable all interrupts            */
        writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);      /* Disable FIFO                      */
        writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   /* Turn on internal clock source     */
        writeByte(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); /* Disable I2C master                */
        writeByte(MPU6050_ADDRESS, USER_CTRL, 0x00);    /* Disable FIFO and I2C master modes */
        writeByte(MPU6050_ADDRESS, USER_CTRL, 0x0C);    /* Reset FIFO and DMP                */
        thread_sleep_for(150);

        writeByte(MPU6050_ADDRESS, CONFIG, 0x01);       /* Set low-pass filter to 188 Hz                                      */
        writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);   /* Set sample rate to 1 kHz                                           */
        writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  /* Set gyro full-scale to 250 degrees per second, maximum sensitivity */
        writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); /* Set accelerometer full-scale to 2 g, maximum sensitivity           */

        uint16_t gyrosensitivity = 131;    /* 131 LSB/degrees/sec */
        uint16_t accelsensitivity = 16384; /* 16384 LSB/g         */

        /* Configure FIFO to capture accelerometer and gyro data for bias calculation */
        writeByte(MPU6050_ADDRESS, USER_CTRL, 0x40); /* Enable FIFO                                    */
        writeByte(MPU6050_ADDRESS, FIFO_EN, 0x78);   /* Enable gyro and accelerometer sensors for FIFO */
        thread_sleep_for(80);

        /* Disable gyro and accelerometer sensors for FIFO */
        writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);
        
        /* Read FIFO sample count */
        readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]);
        fifo_count = ((uint16_t)data[0] << 8) | data[1];
        packet_count = fifo_count / 12;

        for (p = 0; p < packet_count; p++)
        {
            int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};

            /* Read data for averaging */
            readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]);
            
            /* Form signed 16-bit integer for each sample in FIFO */
            accel_temp[0] = (int16_t)(((int16_t)data[0] << 8) | data[1]);
            accel_temp[1] = (int16_t)(((int16_t)data[2] << 8) | data[3]);
            accel_temp[2] = (int16_t)(((int16_t)data[4] << 8) | data[5]);
            gyro_temp[0] = (int16_t)(((int16_t)data[6] << 8) | data[7]);
            gyro_temp[1] = (int16_t)(((int16_t)data[8] << 8) | data[9]);
            gyro_temp[2] = (int16_t)(((int16_t)data[10] << 8) | data[11]);

            /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
            accel_bias[0] += (int32_t)accel_temp[0];
            accel_bias[1] += (int32_t)accel_temp[1];
            accel_bias[2] += (int32_t)accel_temp[2];
            gyro_bias[0] += (int32_t)gyro_temp[0];
            gyro_bias[1] += (int32_t)gyro_temp[1];
            gyro_bias[2] += (int32_t)gyro_temp[2];
        }

        /* Normalize sums to get average count biases */
        accel_bias[0] /= (int32_t)packet_count;
        accel_bias[1] /= (int32_t)packet_count;
        accel_bias[2] /= (int32_t)packet_count;
        gyro_bias[0] /= (int32_t)packet_count;
        gyro_bias[1] /= (int32_t)packet_count;
        gyro_bias[2] /= (int32_t)packet_count;

        if (accel_bias[2] > 0L)
            /* Remove gravity from the z-axis accelerometer bias calculation */
            accel_bias[2] -= (int32_t)accelsensitivity;
        else
            accel_bias[2] += (int32_t)accelsensitivity;

        /* Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup */
        /* Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format */
        data[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF;

        /* Biases are additive, so change sign on calculated average gyro biases */
        data[1] = (-gyro_bias[0] / 4) & 0xFF;
        data[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
        data[3] = (-gyro_bias[1] / 4) & 0xFF;
        data[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
        data[5] = (-gyro_bias[2] / 4) & 0xFF;

        // Push gyro biases to hardware registers
        writeByte(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]);
        writeByte(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
        writeByte(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
        writeByte(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
        writeByte(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
        writeByte(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);

        /* Construct gyro bias in deg/s for later manual subtraction */
        dest1[0] = (float)gyro_bias[0] / (float)gyrosensitivity;
        dest1[1] = (float)gyro_bias[1] / (float)gyrosensitivity;
        dest1[2] = (float)gyro_bias[2] / (float)gyrosensitivity;

        int32_t accel_bias_reg[3] = {0, 0, 0};
        /* Read factory accelerometer trim values */
        readBytes(MPU6050_ADDRESS, XA_OFFSET_H, 2, &data[0]);
        accel_bias_reg[0] = (int16_t)((int16_t)data[0] << 8) | data[1];
        readBytes(MPU6050_ADDRESS, YA_OFFSET_H, 2, &data[0]);
        accel_bias_reg[1] = (int16_t)((int16_t)data[0] << 8) | data[1];
        readBytes(MPU6050_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
        accel_bias_reg[2] = (int16_t)((int16_t)data[0] << 8) | data[1];

        /* Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers */
        uint32_t mask = 1uL;
        /* Define array to hold mask bit for each accelerometer bias axis */
        uint8_t mask_bit[3] = {0, 0, 0};

        for (p = 0; p < 3; p++)
        {
            /* If temperature compensation bit is set, record that fact in mask_bit */
            if (accel_bias_reg[p] & mask)
                mask_bit[p] = 0x01;
        }

        /* Construct total accelerometer bias, including calculated average accelerometer bias from above */
        /* Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale) */
        accel_bias_reg[0] -= (accel_bias[0] / 8);
        accel_bias_reg[1] -= (accel_bias[1] / 8);
        accel_bias_reg[2] -= (accel_bias[2] / 8);
        data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
        data[1] = (accel_bias_reg[0]) & 0xFF;
        data[1] = data[1] | mask_bit[0];
        data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
        data[3] = (accel_bias_reg[1]) & 0xFF;
        data[3] = data[3] | mask_bit[1];
        data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
        data[5] = (accel_bias_reg[2]) & 0xFF;
        data[5] = data[5] | mask_bit[2];

        dest2[0] = (float)accel_bias[0] / (float)accelsensitivity;
        dest2[1] = (float)accel_bias[1] / (float)accelsensitivity;
        dest2[2] = (float)accel_bias[2] / (float)accelsensitivity;
    }
};

#endif