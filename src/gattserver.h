#pragma once

#ifndef __GATT_SERVER_H__
#define __GATT_SERVER_H__

/**
 * @file gattserver.h
 * 
 * Additional compilation unit service class implementing.
 * 
 */

#include <stdint.h>
#include <math.h>

#include <events/mbed_events.h>
#include <platform/Callback.h>
#include <platform/NonCopyable.h>

#include <Gap.h>
#include <gap/AdvertisingDataParser.h>
#include <ble/common/FunctionPointerWithContext.h>

// #include <ble/BLE.h>
// #include <events/EventQueue.h>

/* Device name in nRF Connect */
#define DEV_NAME "Gyro & Peripheral Server"

/* Payload size */
#define MAX_ADVERTISING_PAYLOAD_SIZE 50

#include "syslogger.h"

/**
 * @class BLEProcess
 * 
 * @brief Inherit general handler of GAP-related events.
 * 
 */
class BLEProcess : private mbed::NonCopyable<BLEProcess>, public ble::Gap::EventHandler
{
public:
    /**
     * @brief Construct a BLEProcess from an event queue and a ble interface.
     */
    BLEProcess(events::EventQueue &event_queue, BLE &ble_interface) : _event_queue(event_queue),
                                                                      _ble(ble_interface),
                                                                      _gap(ble_interface.gap()),
                                                                      _adv_data_builder(_adv_buffer)
    {
    }

    ~BLEProcess()
    {
        stop();
    }

    /**
     * @brief Initialize the ble interface, configure it and start advertising.
     *
     * Function handle GAP events, and inform us off all events so we can schedule their handling using event queue. 
     * 
     * @return None
     */
    void start()
    {
        LOGI("BLE process started\r\n");

        if (_ble.hasInitialized())
        {
            LOGE("BLE instance has already been initialized\r\n");
            return;
        }

        _gap.setEventHandler(this);

        _ble.onEventsToProcess(
            makeFunctionPointer(this, &BLEProcess::schedule_ble_events));

        ble_error_t error = _ble.init(
            this, &BLEProcess::onInitComplete);

        if (error)
        {
            LOGE("Error returned by BLE::init\r\n");
            return;
        }
        _event_queue.dispatch_forever();
        return;
    }

    /**
     * @brief Close existing connections and stop the process.
     * 
     * @return None
     */
    void stop()
    {
        if (_ble.hasInitialized())
        {
            _ble.shutdown();
            LOGE("BLE process stopped\r\n");
        }
    }

    /**
     * @brief Subscription to the BLE interface initialization event.
     *
     * @param[in] cb The callback object that will be called when the ble interface is initialized.
     * 
     * @return None
     */
    void onInit(mbed::Callback<void(BLE &, events::EventQueue &)> cb)
    {
        _post_init_cb = cb;
    }

    /**
     * @brief Set callback for a succesful connection.
     *
     * @param[in] cb The callback object that will be called when we connect to a peer
     * 
     * @return None
     */
    void on_connect(mbed::Callback<void(BLE &, events::EventQueue &, const ble::ConnectionCompleteEvent &event)> cb)
    {
        _post_connect_cb = cb;
    }

    virtual const char *get_device_name()
    {
        static const char name[] = "BLE-Process";
        return name;
    }

protected:
    /**
     * @brief Sets up adverting payload and start advertising 
     * 
     * This function is invoked when the BLE interface is initialized
     * 
     * @param event Initialization complete event
     *
     * @return None
     */
    void onInitComplete(BLE::InitializationCompleteCallbackContext *event)
    {
        if (event->error)
        {
            LOGE("Error during the initialisation\r\n");
            return;
        }

        LOGI("BLE instance initialized\r\n");

        start_activity();

        if (_post_init_cb)
        {
            _post_init_cb(_ble, _event_queue);
        }
    }

    /**
     * @brief Start the gatt client process when a connection event is received 
     * 
     * This is called by Gap to notify the application we connected
     * 
     * @param event Connection complete event
     *
     * @return None
     */
    void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override
    {
        if (event.getStatus() == BLE_ERROR_NONE)
        {
            LOGI("Connected to BLE device in nRF Connect\r\n");

            if (_post_connect_cb)
            {
                _post_connect_cb(_ble, _event_queue, event);
            }
        }
        else
        {
            LOGE("Failed to connect\r\n");
            start_activity();
        }
    }

    /**
     * @brief Stop the gatt client process when the device is disconnected
     * 
     * This is called by Gap to notify the application we disconnected
     * 
     * @param event Disconnection complete event
     *
     * @return None
     */
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override
    {
        LOGE("Disconnected\r\n");
        start_activity();
    }

    /**
     * @brief Restarts main activity
     * 
     * @param event Advertising end complete event
     *
     * @return None
     */
    void onAdvertisingEnd(const ble::AdvertisingEndEvent &event)
    {
        start_activity();
    }

    /**
     * @brief Start advertising/scanning
     * 
     * Triggered by init or disconnection
     *
     * @return None
     */
    virtual void start_activity()
    {
        _event_queue.call([this]()
                          { start_advertising(); });
    }

    /**
     * @brief Start the advertising process, ends when a device connects
     * 
     * @return None
     */
    void start_advertising()
    {
        ble_error_t error;

        if (_gap.isAdvertisingActive(_adv_handle))
            return;

        ble::AdvertisingParameters adv_params(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(40)));

        error = _gap.setAdvertisingParameters(_adv_handle, adv_params);

        if (error)
        {
            LOGE("_ble.gap().setAdvertisingParameters() failed\r\n");
            return;
        }

        _adv_data_builder.clear();
        _adv_data_builder.setFlags();
        _adv_data_builder.setName(get_device_name());

        /* Set payload for the set */
        error = _gap.setAdvertisingPayload(
            _adv_handle, _adv_data_builder.getAdvertisingData());

        if (error)
        {
            LOGE("Gap::setAdvertisingPayload() failed\r\n");
            return;
        }

        error = _gap.startAdvertising(_adv_handle, ble::adv_duration_t(ble::millisecond_t(4000)));

        if (error)
        {
            LOGE("Gap::startAdvertising() failed\r\n");
            return;
        }

        LOGI("Advertising as Nordic device.\r\n");
    }

    /**
     * @brief Schedule processing of events from the BLE middleware in the event queue
     * 
     * @param event Reference of events to process event
     *
     * @return None
     */
    void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *event)
    {
        _event_queue.call(mbed::callback(&event->ble, &BLE::processEvents));
    }

protected:
    events::EventQueue &_event_queue;
    BLE &_ble;
    ble::Gap &_gap;

    uint8_t _adv_buffer[MAX_ADVERTISING_PAYLOAD_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;

    ble::advertising_handle_t _adv_handle = ble::LEGACY_ADVERTISING_HANDLE;

    mbed::Callback<void(BLE &, events::EventQueue &)> _post_init_cb;
    mbed::Callback<void(BLE &, events::EventQueue &, const ble::ConnectionCompleteEvent &event)> _post_connect_cb;
};

using mbed::callback;
using namespace std::literals::chrono_literals;

/**
 * @class GattServerProcess 
 * 
 * @brief Gatt Server Process for our service
 */
class GattServerProcess : public BLEProcess
{
public:
    GattServerProcess(events::EventQueue &event_queue, BLE &ble_interface) : BLEProcess(event_queue, ble_interface)
    {
    }

    /**
     * @brief Get device name for nRF Connect  
     * 
     * @return Pointer for device name
     */
    const char *get_device_name() override
    {
        static const char name[] = DEV_NAME;
        return name;
    }
};

#endif