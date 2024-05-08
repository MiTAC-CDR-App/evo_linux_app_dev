/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  SmartCable                                                                                  *
 * BINARY NAME :  SmartCableReader                                                                            *
 * FILE NAME   :  SmartCableReader.cpp                                                                              *
 * CREATED BY  :  Jihung Li <jihung.li@mic.com.tw>                                                            *
 * CREATED DATE:  04/26/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  SmartCable example                                                                          *
 *------------------------------------------------------------------------------------------------------------*/

#include "SerialPort.h"

#define SMART_CABLE_PORT "/dev/ttyHS1"
#define BAURDRATE        230400

int main(int argc, char *argv[])
{
    SerialPort *port = new SerialPort(SMART_CABLE_PORT, BAURDRATE);
    if (!port->open())
    {
        return 0;
    }

    port->startRead();
    sleep(1200);
    port->close();

    return 0;
}
