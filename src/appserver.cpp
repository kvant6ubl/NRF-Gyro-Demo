#include "appserver.h"
#include "mpu6050.h"

/**
 * @file appserver.cpp
 * 
 * Main service algo implementation.
 * 
 */ 

/* BLE service advertising period */
#define ADV_PERIOD 5000ms

/* MPU6050 device object */
MPU6050 mpu6050;

/**
 * @brief Application entry point
 *
 * Execution of the program starts here.
 * 
 * First create the reference to the BLE singleton corresponding to a given interface, 
 * initialize event queue for dispatching events start our service.
 * 
 * After that, make a process for handle basic BLE setup and advertising for clients.
 * 
 * Once it's done it will let continue with demo.
 *
 */
void ApplicationStart(void)
{
    BOARD_INFO();

    BLE &ble = BLE::Instance();
    events::EventQueue event_queue;

    GyroAndPeriphService GyroDemoService;

    GattServerProcess BLEProcess(event_queue, ble);

    BLEProcess.onInit(callback(&GyroDemoService, &GyroAndPeriphService::start));
    BLEProcess.start();
}

/**
 * @brief Start the application
 *
 * Initialize BLE peripheral of NRF52 
 * and MPU6050 device. 
 *
 * @param ble Reference to BLE-capable radio transceivers or SOC
 * @param event_queue Reference to event queue
 *
 * @return None
 */
void GyroAndPeriphService::start(BLE &ble, events::EventQueue &event_queue)
{
    _server = &ble.gattServer();
    _event_queue = &event_queue;

    LOGI("Registering demo service\r\n");
    ble_error_t err = _server->addService(_gyro_service);

    if (err)
    {
        LOGE("Error during service registration.\r\n");
        return;
    }

    /* register handlers */
    _server->setEventHandler(this);

    LOGI("Service registered\r\n");
    // printf("service handle: %u\r\n", _gyro_service.getHandle());
    // printf("hour characteristic value handle %u\r\n", _hour_char.getValueHandle());
    // printf("minute characteristic value handle %u\r\n", _minute_char.getValueHandle());
    // printf("second characteristic value handle %u\r\n", _second_char.getValueHandle());
    //
    _event_queue->call_every(ADV_PERIOD, callback(this, &GyroAndPeriphService::updateGyroCharacteristics));

    // // MPU part
    i2c.frequency(400000); // use fast (400 kHz) I2C

    // // Read the WHO_AM_I register, this is a good test of communication
    uint8_t mpu_whoami = mpu6050.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050); // Read WHO_AM_I register for MPU-6050

    if (mpu_whoami == 0x68) // WHO_AM_I should always be 0x68
        LOGI("MPU6050 device address is OK\r\n");
    else
        LOGE("MPU6050 device is not connected\r\n");

    mpu6050.reset();                        // Reset registers to default in preparation for device calibration
    mpu6050.calibrate(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers
    mpu6050.init();

    LOGI("MPU6050 device initialized for active data mode\r\n"); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
}

/**
 * @private 
 * 
 * @brief Handle GATT BLE action;
 *
 * Data Sent Attribute related 
 * event handling in GATT process.
 *
 * @param params Reference to GATT Data Attribute parameters 
 *
 * @return None
 */
void GyroAndPeriphService::onDataSent(const GattDataSentCallbackParams &params)
{
    LOGI("Sent updates\r\n");
}

/**
 * @private 
 * 
 * @brief Handle GATT BLE action;
 *
 * Calls whether the GattServer has received a write request 
 * or a GattClient has received a write response.
 *
 * @param params Reference to GATT Data Attribute parameters 
 *
 * @return None
 */
void GyroAndPeriphService::onDataWritten(const GattWriteCallbackParams &params)
{
    /* printf("Connection handle : %u\r\n", params.connHandle); */
    /* printf("Attribute handle  : %u", params.handle);         */

    if (params.handle == _accel_gX.getValueHandle())
    {
        LOGI("GX characteristic was written\r\n");
    }
    else if (params.handle == _accel_gY.getValueHandle())
    {
        LOGI("GY characteristic was written\r\n");
    }
    else if (params.handle == _accel_gZ.getValueHandle())
    {
        LOGI("GZ characteristic was written\r\n");
    }
    else
    {
        LOGI("No characteristic was written\r\n");
    }
}

/**
 * @private 
 * 
 * @brief Handle GATT BLE action
 *
 * Show to user registered callbacks whether 
 * the GattServer has received a read request 
 * or a GattClient has received a read response.
 *
 * @param params Reference to GATT Data Attribute parameters 
 *
 * @return None
 */
void GyroAndPeriphService::onDataRead(const GattReadCallbackParams &params)
{
    /* printf("Data read         : \r\n");                       */
    /* printf("Connection handle : %u\r\n", params.connHandle);  */
    /* printf("Attribute handle  : %u", params.handle);          */

    if (params.handle == _accel_gX.getValueHandle())
    {
        LOGI("AX characteristic was read\r\n");
    }
    else if (params.handle == _accel_gY.getValueHandle())
    {
        LOGI("AY characteristic was read\r\n");
    }
    else if (params.handle == _accel_gZ.getValueHandle())
    {
        LOGI("AZ characteristic was read\r\n");
    }
    else
    {
        LOGI("No characteristic was read\r\n");
    }
}

/**
 * @brief Handle GATT BLE action;
 *
 * Handler called after a client has subscribed to notification or indication.
 *
 * @param params Reference to GATT Data Attribute parameters 
 *
 * @return None
 */
void GyroAndPeriphService::onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params)
{
    LOGI("Update enabled on handle\r\n");
}

/**
 * @brief Handle GATT BLE action;
 *
 * Handler called after a client has cancelled his subscription from
 * notification or indication.
 *
 * @param params Reference to the characteristic value affected by the change.
 *
 * @return None
 */
void GyroAndPeriphService::onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params)
{
    LOGI("Update disabled on handle\r\n");
}

/**
 * @brief Handle GATT BLE action;
 *
 * Handler called when an indication confirmation has been received.
 *
 * @param params Handle of the GATT characteristic value that has emitted the indication.
 *  
 * @return None
 */
void GyroAndPeriphService::onConfirmationReceived(const GattConfirmationReceivedCallbackParams &params)
{
    LOGI("Confirmation received on handle\r\n");
}

/**
 * @brief Handler called when a GATT write authorization request is received.
 *
 * This handler verify the validity of value submitted by the client, before
 * authorizing the operation.
 *
 * @param params Pointer to write authorization parameters.
 *
 * @return None
 */
void GyroAndPeriphService::authorize_client_write(GattWriteAuthCallbackParams *write_auth_param)
{
    /* printf("characteristic %u write authorization\r\n", e->handle); */

    if (write_auth_param->offset != 0)
    {
        LOGE("Error invalid offset\r\n");
        write_auth_param->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
        return;
    }

    if (write_auth_param->len != 1)
    {
        LOGE("Error invalid len\r\n");
        write_auth_param->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
        return;
    }
    write_auth_param->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
}

/**
 * @brief Updating gyroscope characteristics
 *
 * Depending on the current MPU6050 address and interrupt value, read current angular velocity 
 * of rotation around the XYZ-axis and write it to BLE characteristics.
 *
 * @return None
 */
void GyroAndPeriphService::updateGyroCharacteristics(void)
{
    if (mpu6050.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01)
    {
        mpu6050.readGyroData(gyroCount);
        mpu6050.getGres();

        uint8_t gX;
        uint8_t gY;
        uint8_t gZ;

        ble_error_t err_gX = _accel_gX.get(*_server, gX);
        ble_error_t err_gY = _accel_gY.get(*_server, gY);
        ble_error_t err_gZ = _accel_gZ.get(*_server, gZ);

        if (err_gX && err_gY && err_gZ)
        {
            LOGW("Read of the AZ value returned error\r\n");
            return;
        }

        gX = mpu6050.getTinyGyroX();
        gY = mpu6050.getTinyGyroY();
        gZ = mpu6050.getTinyGyroZ();

        err_gX = _accel_gX.set(*_server, gX);
        err_gY = _accel_gY.set(*_server, gY);
        err_gZ = _accel_gZ.set(*_server, gZ);

        if (err_gX && err_gY && err_gZ)
        {
            LOGW("Write of accel values returned errors\r\n");
            return;
        }
    }
    else
    {
        LOGW("Reading from MPU returned errors\r\n");
        return;
    }
}