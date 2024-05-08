/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  SmartCable                                                                                  *
 * BINARY NAME :  SmartCableReader                                                                            *
 * FILE NAME   :  DataPacket.cpp                                                                              *
 * CREATED BY  :  Jihung Li <jihung.li@mic.com.tw>                                                            *
 * CREATED DATE:  04/26/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Data Packet Struct                                                                          *
 *------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "DataPacket.h"

DataPacket* DataPacket::parse(const char *data, const int len)
{
    return new DataPacket(data, len);
}

static int getInt32(const void *ptr)
{
    return *((int32_t*)ptr);
}

static float getFloat(const void *ptr)
{
    return *((float*)ptr);
}

DataPacket::DataPacket(const char *data, const int len)
{
    // 0 ~ 23 is packet info, such as head, type, size, version and valid flags.

    // Valid flags, 8 bytes, 16 ~ 23
    // In this example code, we do not parse out flags.
    // It is recommaned to check it before use each data.

    // Total Vehicle Distance, 4 bytes, 24 ~ 27
    totalVehicleDistance = getFloat(&data[24]);
    // Total Fuel Used, 4 bytes, 28 ~ 31
    totalFuelUsed = getFloat(&data[28]);
    // Total Hours of Operation, 4 bytes, 32 ~ 35
    totalHoursofOperation = getFloat(&data[32]);
    // Vehicle Speed, 4 bytes, 36 ~ 39
    vehicleSpeed = getFloat(&data[36]);
    // Engine Speed, 4 bytes, 40 ~ 43
    engineSpeed = getFloat(&data[40]);
    // Engine Fuel Rate, 4 bytes, 44 ~ 47
    engineFuelRate = getFloat(&data[44]);
    // Ignition Status, 4 bytes, 48 ~ 51
    ignitionStatus = getInt32(&data[48]);
    // Fuel Level, 4 bytes, 52 ~ 55
    fuelLevel = getFloat(&data[52]);
    // Battery Voltage, 4 bytes, 56 ~ 59
    batteryVoltage = getFloat(&data[56]);
    // Accelerator Pedal Position, 4 bytes, 60 ~ 63
    acceleratorPedalPosition = getFloat(&data[60]);
    // Cruise Control, 4 bytes, 64 ~ 67
    cruiseControl = getInt32(&data[64]);
    // Brake Pedal Position, 4 bytes, 68 ~ 71
    brakePedalPosition = getInt32(&data[68]);
    // Total Trip Harsh Brakes, 4 bytes, 72 ~ 75
    totalTripHarshBrakes = getFloat(&data[72]);
    // Total Trip Harsh Acceleration Time, 4 bytes, 76 ~ 79
    totalTripHarshAccelerationTime = getFloat(&data[76]);
    // Total Trip Acceleration Time, 4 bytes, 80 ~ 83
    totalTripAccelerationTime = getFloat(&data[80]);
    // Total Trip Acceleration Distance, 4 bytes, 84 ~ 87
    totalTripAccelerationDistance = getFloat(&data[84]);
    // Total Trip Deceleration Time, 4 bytes, 88 ~ 91
    totalTripDecelerationTime = getFloat(&data[88]);
    // Total Trip Over Speed Time, 4 bytes, 92 ~ 95
    totalTripOverSpeedTime = getFloat(&data[92]);
    // Total Trip Over Speed Distance, 4 bytes, 96 ~ 99
    totalTripOverSpeedDistance = getFloat(&data[96]);
    // Retarder Info - total time, 4 bytes, 100 ~ 103
    retarderTotalTime = getFloat(&data[100]);
    // Retarder Info - total distance , 4 bytes, 104 ~ 107
    retarderTotalDistance = getFloat(&data[104]);
    // Turn Right Lights, 4 bytes, 108 ~ 111
    turnRightLights = getInt32(&data[108]);
    // Turn Left Lights, 4 bytes, 112 ~ 115
    turnLeftLights = getInt32(&data[112]);
    // Trun Head Lights, 4 bytes, 116 ~ 119
    trunHeadLights = getInt32(&data[116]);
    // Driver Seat Belt, 4 bytes, 120 ~ 123
    driverSeatBelt = getInt32(&data[120]);
    // Gear Position, 4 bytes, 124 ~ 127
    gearPosition = getInt32(&data[124]);
    // Malfunction Indicator Lamp, 4 bytes, 128 ~ 131
    malfunctionIndicatorLamp = getInt32(&data[128]);
    // Vehicle Weight, 4 bytes, 132 ~ 135
    vehicleWeight = getFloat(&data[132]);
    // VIN, 24 bytes, 136 ~ 159
    memcpy(vin, &data[136], 24);
    // Engine load, 4 bytes, 160 ~ 163
    engineLoad = getFloat(&data[160]);
    // Engine temperature, 4 bytes, 164 ~ 167
    engineTemperature = getFloat(&data[164]);
    // Instant_Fuel_Consumption, 4 bytes, 168 ~ 171
    instantFuelConsumption = getFloat(&data[168]);
    // Fuel_Name, 12 bytes, 172 ~ 183
    memcpy(fuelName, &data[172], 12);
    // Battery soc, 4 bytes, 184 ~ 187
    batterySoc = getFloat(&data[184]);
    // Battery soh, 4 bytes, 188 ~ 191
    batterySoh = getFloat(&data[188]);
    // Battery current, 4 bytes, 192 ~ 195
    batteryCurrent = getFloat(&data[192]);
    // License Status, 4 bytes, 196 ~ 199
    licenseStatus = getInt32(&data[196]);
    // License Expiration Time, 4 bytes, 200 ~ 203
    licenseExpirationTime = getInt32(&data[200]);
    // Device Identifier, 32 bytes, 204 ~ 235
    memcpy(deviceIdentifier, &data[204], 32);
    // UID, 32 bytes, 236 ~ 267
    memcpy(UID, &data[236], 32);
    // bootcode_version, 16 bytes, 268 ~ 283
    memcpy(bootcodeVer, &data[268], 16);
    // firmware_version, 16 bytes, 284 ~ 299
    memcpy(firmwareVer, &data[284], 16);
    // ACC ignition, 4 bytes, 300 ~ 303
    accIgnition = getInt32(&data[300]);

    // CRC, 4 bytes, 304 ~ 307
    // Tail, 4 bytes, 308 ~ 311
}

DataPacket::~DataPacket()
{
}
