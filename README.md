## Demonstrating BLE GATT server features of mbed OS 6 (6.9) using NRF52-DK and MPU6050 sensor.

### Release
Current stage of this project includes ``MPU6050`` header (lightweight version of MPU6050IMU lib), GATT service and log-tool header.

### Hardware
Device consist of ``NRF52-DK`` development board and ``MPU6050`` connected to it. 

[NRF52-DK](https://www.nordicsemi.com/Products/Development-hardware/nrf52-dk), is (in this project) a board with nRF52832 flash-based ANT/ANT+, Bluetooth Low Energy SoC solution:

<p align="center">
    <img src="https://infocenter.nordicsemi.com/topic/ug_nrf52832_dk/UG/nrf52_DK/images/nrf52_dk_kit_content.png"
        height="400">
</p>


``MPU6050`` is 6-axis motion-tracking device that combines a 3-axis
gyroscope, 3-axis accelerometer, and a Digital Motion Processor (more about it here).


### Logic of the work
When SoC is started-up, the device make GAP advertising every 5 seconds. User could find the device by [NRF Connect](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop) (iOS, Android). Once it connected, user can see custom service with several characteristics: ``GX``, ``GY``, ``GZ``. User could read the last accelerometer values (values update due to external interrupts from the sensor) from the gyro characteristic.


### GATT Service
The GATT service has 3 gyro-characteristics that contain the small ``uint8_t`` gyroscopes values from ``MPU6050``, gain via external interrupt operation (MPU is pre-configured for this). All characteristics have read-only acces. The UUID of all characteristics was generated using python3 UUID module (file uniconverter.py):
```
$ python3 uniconverter.py -n 3
Take your 1 UUID: 90cb4365-2833-4541-a321-9437d9b38464
Take your 2 UUID: df49a77c-4fd8-4327-aa5f-1410bce0d0ff
Take your 3 UUID: a511aa3f-744e-4790-a225-8553838aa6ac
Take your 4 UUID: 8c852eb9-8ec5-4ac0-a3eb-e9c375fbc756
```


### Installation dependencies

* Install ``doxygen`` for generating documentation:
```
$ sudo apt-get install doxygen
```
* Download and install ``J-Link`` for j-link board interface to debuging and prototyping:
```
$ wget https://www.segger.com/downloads/jlink/JLink_Linux_x86_64.deb
$ sudo dpkg -i JLink_Linux_x86_64.deb
``` 
* Install tool for build & flash tool - ``pio``: 
```
$ wget https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -O get-platformio.py python3 get-platformio.py
```