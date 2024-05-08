/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  SmartCable                                                                                  *
 * BINARY NAME :  SmartCableReader                                                                            *
 * FILE NAME   :  DataPacket.h                                                                                *
 * CREATED BY  :  Jihung Li <jihung.li@mic.com.tw>                                                            *
 * CREATED DATE:  04/26/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Data Packet Struct                                                                          *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef DATA_PACKET_H_
#define DATA_PACKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class DataPacket
{
    public:

        static DataPacket *parse(const char *port, const int len);

        virtual ~DataPacket();

    private:
        DataPacket(const char *port, const int len);

    public:
        float totalVehicleDistance;
        float totalFuelUsed;
        float totalHoursofOperation;
        float vehicleSpeed;
        float engineSpeed;
        float engineFuelRate;
        int ignitionStatus;
        float fuelLevel;
        float batteryVoltage;
        float acceleratorPedalPosition;
        int cruiseControl;
        int brakePedalPosition;
        float totalTripHarshBrakes;
        float totalTripHarshAccelerationTime;
        float totalTripAccelerationTime;
        float totalTripAccelerationDistance;
        float totalTripDecelerationTime;
        float totalTripOverSpeedTime;
        float totalTripOverSpeedDistance;
        float retarderTotalTime;
        float retarderTotalDistance;
        int turnRightLights;
        int turnLeftLights;
        int trunHeadLights;
        int driverSeatBelt;
        int gearPosition;
        int malfunctionIndicatorLamp;
        float vehicleWeight;
        char vin[25] = {0};  // 24 bytes data and 1 byte end char for print
        float engineLoad;
        float engineTemperature;
        float instantFuelConsumption;
        char fuelName[13] = {0}; // 12 bytes data and 1 byte end char for print
        float batterySoc;
        float batterySoh;
        float batteryCurrent;
        int licenseStatus;
        int licenseExpirationTime;
        char deviceIdentifier[33] = {0}; // 32 bytes data and 1 byte end char for print
        char UID[33] = {0}; // 32 bytes data and 1 byte end char for print
        char bootcodeVer[17] = {0}; // 16 bytes data and 1 byte end char for print
        char firmwareVer[17] = {0}; // 16 bytes data and 1 byte end char for print
        int accIgnition;
};

#endif /* DATA_PACKET_H_ */
