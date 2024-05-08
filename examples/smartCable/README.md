# CAN bus data example with SmartGO

This example will demo how to read the data packet from SmartGO, parse it and then output the result on the console

## How to build:

Please execute
```sh
./build
```

It will generate executable project/SmartCableReader.

## How to setup environment:
SmartGO needs authentication via internet prior to decode and output data.  Please make sure that the device is connected to internet either by LTE or WiFi.

If you run `ADB` from MS Windows, you can follow steps to setup WiFi:
1. modify WiFi SSID and password in wifiConfig/connect_wifi.sh
2. execute
    ```sh
    wifiConfig/config_wifi.bat
    ```

## Excute SmartCableReader:
If you run `ADB` from MS Windows, please execute
```sh
pushAndRun.bat
```
under the `SmartCable` example directory

## Known issue
1. Some vehicle models are not supported by SmartGO
2. For the supported vehile models, there might be part of OBD2 data not supported by SmartGO
3. Sometimes the data packet will be dropped due to missing byte
