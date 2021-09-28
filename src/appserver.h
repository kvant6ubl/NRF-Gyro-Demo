#pragma once

#ifndef __APPSERVER_H__
#define __APPSERVER_H__

#include "gattserver.h"
#include "syslogger.h"

/**
 * @class GyroAndPeriphService 
 * 
 * @brief Gyroscope service demonstrating GattServer features of mbed OS 6
 * 
 * The service has 3 gyro-characteristics that contain the G-s values from MPU6050, gain via interrupt operation (MPU is pre-configured for this). A nRF Connect 
 * client can subscribe to updates of this characteristics and get notified when one of the value is changed. Clients can also change the value of the peripheral 
 * controled characteristic: set 0x01 to turn HIGH level of the appropriate output or 0x00 to set LOW pin level. 
 * The UUID of all characteristics was generated using python3 UUID module.
 * 
 */
class GyroAndPeriphService : public ble::GattServer::EventHandler
{
public:
    GyroAndPeriphService() : _accel_gX("90cb4365-2833-4541-a321-9437d9b38464", 0),
                     _accel_gY("df49a77c-4fd8-4327-aa5f-1410bce0d0ff", 0), 
                     _accel_gZ("a511aa3f-744e-4790-a225-8553838aa6ac", 0), 
                     _gyro_service(
                         /* uuid */                      "8c852eb9-8ec5-4ac0-a3eb-e9c375fbc756",
                         /* characteristics */           _gyro_characteristics,
                         /* number of characteristics */ sizeof(_gyro_characteristics) /
                             sizeof(_gyro_characteristics[0]))
    {
        /* Update internal pointers */
        _gyro_characteristics[0] = &_accel_gX;
        _gyro_characteristics[1] = &_accel_gY;
        _gyro_characteristics[2] = &_accel_gZ;

        /* Setup auth-handlers */
        _accel_gX.setWriteAuthorizationCallback(this, &GyroAndPeriphService::authorize_client_write);
        _accel_gY.setWriteAuthorizationCallback(this, &GyroAndPeriphService::authorize_client_write);
        _accel_gZ.setWriteAuthorizationCallback(this, &GyroAndPeriphService::authorize_client_write);
    }
    void start(BLE &, events::EventQueue &);

private:
    void onDataSent(const GattDataSentCallbackParams &) override;
    void onDataWritten(const GattWriteCallbackParams &) override;
    void onDataRead(const GattReadCallbackParams &)     override;
    void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &)   override;
    void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &) override;
    void onConfirmationReceived(const GattConfirmationReceivedCallbackParams &) override;

private:
    void authorize_client_write(GattWriteAuthCallbackParams *);
    void updateGyroCharacteristics(void);

private:
    /**
     * Custom Read-Only Characteristic declaration helper.
     *
     * @tparam T type of data held by the characteristic.
     */
    template <typename T>
    class ReadWriteNotifyIndicateCharacteristic : public GattCharacteristic
    {
    public:
        /**
         * Construct a characteristic that can be read.
         *
         * @param[in] uuid UUID of the characteristic.
         * @param[in] initial_value Initial value contained by the characteristic.
         */
        ReadWriteNotifyIndicateCharacteristic(const UUID &uuid, const T &initial_value) : GattCharacteristic(
                                                                                              /* UUID */ uuid,
                                                                                              /* Initial value */ &_value,
                                                                                              /* Value size */ sizeof(_value),
                                                                                              /* Value capacity */ sizeof(_value),
                                                                                              /* Properties */ GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
                                                                                              /* Descriptors */ nullptr,
                                                                                              /* Num descriptors */ 0,
                                                                                              /* variable len */ false),
                                                                                          _value(initial_value)
        {
        }

        /**
         * Get the value of this characteristic.
         *
         * @param[in] server GattServer instance that contain the characteristic
         * value.
         * @param[in] dst Variable that will receive the characteristic value.
         *
         * @return BLE_ERROR_NONE in case of success or an appropriate error code.
         */
        ble_error_t get(GattServer &server, T &dst) const
        {
            uint16_t value_length = sizeof(dst);
            return server.read(getValueHandle(), &dst, &value_length);
        }

        /**
         * Assign a new value to this characteristic.
         *
         * @param[in] server GattServer instance that will receive the new value.
         * @param[in] value The new value to set.
         * @param[in] local_only Flag that determine if the change should be kept
         * locally or forwarded to subscribed clients.
         */
        ble_error_t set(GattServer &server, const uint8_t &value, bool local_only = false) const
        {
            return server.write(getValueHandle(), &value, sizeof(value), local_only);
        }

    private:
        uint8_t _value;
    };

private:
    /**
     * @class Read-only Characteristic declaration helper.
     *
     * @tparam T type of data held by the characteristic.
     */
    template <typename T>
    class ReadOnlyAccelCharacteristic : public GattCharacteristic
    {
    public:
        /**
         * Construct a characteristic that can be read.
         *
         * @param[in] uuid The UUID of the characteristic.
         * @param[in] initial_value Initial value contained by the characteristic.
         */
        ReadOnlyAccelCharacteristic(const UUID &uuid, const T &initial_value) : GattCharacteristic(
                                                                                    /* UUID */ uuid,
                                                                                    /* Initial value */ &_value,
                                                                                    /* Value size */ sizeof(_value),
                                                                                    /* Value capacity */ sizeof(_value),
                                                                                    /* Properties */ GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
                                                                                    /* Descriptors */ nullptr,
                                                                                    /* Num descriptors */ 0,
                                                                                    /* variable len */ false),
                                                                                _value(initial_value)
        {
        }

        /**
         * Get the value of this characteristic.
         *
         * @param[in] server GattServer instance that contain the characteristic
         * value.
         * @param[in] dst Variable that will receive the characteristic value.
         *
         * @return BLE_ERROR_NONE in case of success or an appropriate error code.
         */
        ble_error_t get(GattServer &server, T &dst) const
        {
            uint16_t value_length = sizeof(dst);
            return server.read(getValueHandle(), &dst, &value_length);
        }

        /**
         * Assign a new value to this characteristic.
         *
         * @param[in] server GattServer instance that will receive the new value.
         * @param[in] value The new value to set.
         * @param[in] local_only Flag that determine if the change should be kept
         * locally or forwarded to subscribed clients.
         */
        ble_error_t set(GattServer &server, const uint8_t &value, bool local_only = false) const
        {
            return server.write(getValueHandle(), &value, sizeof(value), local_only);
        }

    private:
        uint8_t _value;
    };

private:
    GattServer *_server = nullptr;
    events::EventQueue *_event_queue = nullptr;

    GattService _gyro_service;
    GattCharacteristic *_gyro_characteristics[3];

    ReadOnlyAccelCharacteristic<uint8_t> _accel_gX;
    ReadOnlyAccelCharacteristic<uint8_t> _accel_gY;
    ReadOnlyAccelCharacteristic<uint8_t> _accel_gZ;
};

void ApplicationStart(void);

#endif