# MqttTest example
This example will connect to MiTAC VisonMax server as specified pre-setup CDR device (SN:L2023110004), submit a startup event and then recive event ticket.  And it will print out the event ticket and then exit.

## How to build:
Please execute
```sh
./build
```
It will generate executable project/MqttTest.

## How to setup environment and excute MqttTest:
To run MqttTest successfully,
* The device should be connected to Internet, either by LTE or WiFi.
* The device time should be roughly correct.  If the device is already connected to Internet, NTP will calibrate the device time.  Or you can set device time manually by `date`.
* MQTT certificate files should be copied to device.

If you run `ADB` from MS Windows, please execute
```sh
./uploadAndRun.bat
```
under the mqtt example directory.
