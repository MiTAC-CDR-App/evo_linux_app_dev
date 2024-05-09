/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  SmartCable                                                                                  *
 * BINARY NAME :  SmartCableReader                                                                            *
 * FILE NAME   :  SerialPort.h                                                                                *
 * CREATED BY  :  Jihung Li <jihung.li@mic.com.tw>                                                            *
 * CREATED DATE:  04/26/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  com port operation                                                                          *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef SerialPort_H_
#define SerialPort_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // close(), read(), write()
#include <thread>

#define BUF_LEN             1024
#define START_TAG_LENGTH    4
#define START_TAG_COM   0x5F4D4F43  // authentication url
#define START_TAG_DATA  0x41544144  // vehicle data
#define START_TAG_PB    0x5F5F4250  // Panic button event
#define START_TAG_LOG   0x3A474F4C  // logs

#define END_TAIL_DATA   0x5A5A5A5A  // ZZZZ
#define END_TAIL_PB__   0x5A5A      // ZZ

#define DATA_PACKET_LEN 312

#define DEBUG_CMD (char*)"CMD_1.000108\r\n"

class SerialPort
{
    public:
        SerialPort(const char *port, const int baudrate);
        virtual ~SerialPort();

        bool open();
        bool startRead();
        bool close();

    private:
        char *port;
        int baudrate;
        int fd = -1;
        char buf[BUF_LEN];
        int start = 0;
        int end = 0;
        bool stopped = true;
        std::thread *readThread = 0;
        bool urlPosting = false;
        std::thread *httpsThread = 0;
        std::string strUrl;

        void readThreadFun();
        void httpsThreadFun();
        int searchNextPacketBegin();
        int searchCmdTail();
        int searchDataTail();
        int searchPbTail();
        int searchLogTail();
        int searchNextStart();

        void parsePacket(int packetLen);
        void processCmdPacket(const char *data, int len);
        void processDataPacket(const char *data, int len);
        void processPbPacket(const char *data, int len);

        int sendData(const char *data);
};

#endif /* SerialPort_H_ */
