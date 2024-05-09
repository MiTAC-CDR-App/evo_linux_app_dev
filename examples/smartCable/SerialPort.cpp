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

#include <string.h>
#include <time.h>
#include <termios.h>    // struct termios, tcgetattr(), tcsetattr()
#include "SerialPort.h"
#include "DataPacket.h"
#include <curl.h>

void printData(char *ptr, int startIdx, int endIdx)
{
    int count = 0;
    int line = 0;
    printf("%02d\t", line);
    for (int i = startIdx; i <= endIdx; i++)
    {
        count++;
        printf("%02X ", ptr[i]);
        if (count == 16)
        {
            printf("\n%02d\t", ++line);
            count = 0;
        }
    }
    printf("\n------------------------------------------------------\n");
}

SerialPort::SerialPort(const char *port, const int baudrate)
{
    int len = strlen(port) + 1;
    this->port = new char[len];
    memcpy(this->port, port, len);
    this->baudrate = baudrate;
}

SerialPort::~SerialPort()
{
    delete [] port;
    delete readThread;
    if (httpsThread)
    {
        delete httpsThread;
    }
}

static speed_t getBaudrate(int baudrate)
{
    switch(baudrate)
    {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return -1;
    }
}

bool SerialPort::open()
{
    if (fd != -1)
    {
        return true;
    }

    speed_t speed = getBaudrate(baudrate);
    fd = ::open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        printf("open() failed\n");
        return false;
    }

    printf("open() success\n");

    struct termios cfg;
    if (tcgetattr(fd, &cfg))
    {
        printf("tcgetattr() failed\n");
        ::close(fd);
        return false;
    }
    cfmakeraw(&cfg);
    cfsetispeed(&cfg, speed);
    cfsetospeed(&cfg, speed);
    cfg.c_cflag |= (CLOCAL | CREAD);
    cfg.c_cflag |= CRTSCTS;
    cfg.c_cc[VMIN] = 0;
    cfg.c_cc[VTIME] = 10;

    if (tcsetattr(fd, TCSANOW, &cfg))
    {
        printf("tcsetattr() failed\n");
        ::close(fd);
        return false;
    }

    return true;
}

bool SerialPort::startRead()
{
    if (fd < 0)
    {
        return false;
    }

    stopped = false;
    readThread = new std::thread(&SerialPort::readThreadFun, this);
    sendData(DEBUG_CMD);
    return true;
}

void SerialPort::readThreadFun()
{
    printf("* * * readLineFun start * * *\n");

    while (!stopped)
    {
        int nextPacketBegin = searchNextPacketBegin();
        if (nextPacketBegin > 0)
        {
            int len = (nextPacketBegin - start);
            parsePacket(len);
            start = nextPacketBegin;
            continue;
        }

        open();

        int unprocessed = end - start;
        if (unprocessed == 0)
        {
            // all data processed completed, read data directly
            end = 0;
            start = 0;
        }
        else
        {
            // move unprocessed data to head
            for (int i = start; i < end; i++)
            {
                buf[i - start] = buf[i];
            }
            end -= start;
            start = 0;
        }

        int size = read(fd, &buf[end], (BUF_LEN - unprocessed));
        if (size <= 0)
        {
            perror("read");
            ::close(fd);
            fd = -1;
            continue;
        }
        end += size;
    }

    printf("* * * readLineFun end * * *\n");
}

int SerialPort::sendData(const char *data)
{
    if (fd == -1)
    {
        return -1;
    }

    int result = write(fd, data, strlen(data));
    if (result == -1)
    {
        printf("send \"%s\" cmd failed.\n", data);
    }
    else
    {
        printf("send \"%s\" cmd success.\n", data);
    }
    return result;
}

static int getInt32(void *ptr)
{
    return *((int32_t*) ptr);
}

static int getInt16(void *ptr)
{
    return *((int16_t*) ptr);
}

int SerialPort::searchNextPacketBegin()
{
    if (start >= end)
    {
        start = end = 0;
        return 0;
    }

    if ((end - start) < START_TAG_LENGTH)
    {
        return 0;
    }

    char *ptr = &buf[start];
    int startTag = getInt32(ptr);
    switch (startTag)
    {
        case START_TAG_COM:
        {
            return searchCmdTail();
        }
        case START_TAG_DATA:
        {
            return searchDataTail();
        }
        case START_TAG_PB:
        {
            return searchPbTail();
        }
        case START_TAG_LOG:
        {
            return searchLogTail();
        }
        default:
        {
            return searchNextStart();
        }
    }
}

int SerialPort::searchCmdTail()
{
    // tail is '\n'
    for (int i = (start + 4); i < end; i++)
    {
        if (buf[i] == 0x0A)
        {
            return (i + 1);
        }
    }

    return 0;
}

int SerialPort::searchDataTail()
{
    // tail is 'ZZZZ'
    for (int i = (start + 4); (i + 3) < end; i++)
    {
        char *ptr = &buf[i];
        if (getInt32(ptr) == END_TAIL_DATA)
        {
            return (i + 4);
        }
    }

    return 0;
}

int SerialPort::searchPbTail()
{
    // tail is 'ZZ'
    for (int i = (start + 4); (i + 1) < end; i++)
    {
        char *ptr = &buf[i];
        if (getInt16(ptr) == END_TAIL_PB__)
        {
            return (i + 2);
        }
    }

    return 0;
}

int SerialPort::searchLogTail()
{
    // tail is '\n'
    for (int i = (start + 4); i < end; i++)
    {
        if (buf[i] == 0x0A)
        {
            return (i + 1);
        }
    }

    return 0;
}

int SerialPort::searchNextStart()
{
    for (int i = (start + 1); (i + 3) < end; i++)
    {
        char *ptr = &buf[i];
        int startTag = getInt32(&buf[i]);
        if ((startTag == START_TAG_COM) || (startTag == START_TAG_DATA) || (startTag == START_TAG_PB) || 
                                                                           (startTag == START_TAG_LOG))
        {
            return i;
        }
    }

    return 0;
}

void SerialPort::parsePacket(int packetLen)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *localTime = localtime(&tv.tv_sec);

    char *ptr = &buf[start];
    int startTag = getInt32(ptr);
    switch (startTag)
    {
        case START_TAG_COM:
        {
            printf("%04d-%02d-%02d %02d:%02d:%02d.%03ld--------------- CMD (%d bytes)-----------------------\n",
                    localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
                    localTime->tm_hour, localTime->tm_min, localTime->tm_sec, tv.tv_usec / 1000, packetLen);
            processCmdPacket(ptr, packetLen);
            break;
        }
        case START_TAG_DATA:
        {
            printf("%04d-%02d-%02d %02d:%02d:%02d.%03ld--------------- DATA (%d bytes)----------------------\n",
                    localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
                    localTime->tm_hour, localTime->tm_min, localTime->tm_sec, tv.tv_usec / 1000, packetLen);
            if (packetLen != DATA_PACKET_LEN)
            {
                printf("received invalid data packet (wrong length).\n");
            }
            else
            {
                processDataPacket(ptr, packetLen);
            }
            break;
        }
        case START_TAG_PB:
        {
            printf("%04d-%02d-%02d %02d:%02d:%02d.%03ld--------------- PB (%d bytes)------------------------\n",
                    localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
                    localTime->tm_hour, localTime->tm_min, localTime->tm_sec, tv.tv_usec / 1000, packetLen);
            processPbPacket(ptr, packetLen);
            break;
        }
        case START_TAG_LOG:
        {
            printf("%04d-%02d-%02d %02d:%02d:%02d.%03ld--------------- LOG (%d bytes)-----------------------\n",
                    localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
                    localTime->tm_hour, localTime->tm_min, localTime->tm_sec, tv.tv_usec / 1000, packetLen);
            char *ptrLog = new char[packetLen + 1];
            memcpy(ptrLog, ptr, packetLen);
            ptrLog[packetLen] = '\0';
            printf("%s\n", ptrLog);
            delete [] ptrLog;
            break;
        }
        default:
        {
            printf("%04d-%02d-%02d %02d:%02d:%02d.%03ld--------------- unknown (%d bytes)-------------------\n",
                    localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
                    localTime->tm_hour, localTime->tm_min, localTime->tm_sec, tv.tv_usec / 1000, packetLen);
            if (packetLen <= 1)
            {   // probably, '0x0A'
                break;
            }
            printData(ptr, 0, packetLen - 1);
            break;
        }
    }
}

size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *buffer) 
{
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

void SerialPort::processCmdPacket(const char *data, int len)
{
    // The data will be similar:
    // COM_https://apps.cangomobility.com/device/encryption/init?buffer=15b1e36d\n
    printf("Cango request: %s\n", data);

    if (httpsThread)
    {
        if (urlPosting)
        {
            printf("The previous request is still in progress.  This request will be ignored.\n");
            return;
        }
        httpsThread->join();
        delete httpsThread;
    }

    strUrl = std::string(data, len);
    httpsThread = new std::thread(&SerialPort::httpsThreadFun, this);
    urlPosting = true;
}

void SerialPort::httpsThreadFun()
{
    size_t found = strUrl.find('?');
    if (found == std::string::npos)
    {
        printf("Unexpected url.\n");
        return;
    }

    if (found < 9) // Make sure there has a complete beginning (COM_https)
    {
        printf("Can not parse out param\n");
        return;
    }

    std::string strPath = strUrl.substr(4, (found - 4)); // remove "CMD_"
    std::string strParam = strUrl.substr(found + 1);

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        printf("curl initial failed.\n");
        return;
    }

    std::string responseData;
    curl_easy_setopt(curl, CURLOPT_URL, strPath.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_DOH_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    CURLcode res = curl_easy_perform(curl);
    int statusCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    std::string txMsg = "COM_" + std::to_string(statusCode);
    if (res != CURLE_OK)
    {
        printf("performing curl request failed. [%s] err= %s\n", txMsg.c_str(), curl_easy_strerror(res));
    }
    else
    {
        txMsg += responseData;
        printf("statusCode: %d\n", statusCode);
        printf("responseData: %s\n", responseData.c_str());
        printf("performing curl request response: %s\n", txMsg.c_str());
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    sendData(txMsg.c_str());
    urlPosting = false;
}

void SerialPort::processDataPacket(const char *data, int len)
{
    DataPacket *dataPacket = DataPacket::parse(data, len);
    printf("Device ID            : %s\n", dataPacket->deviceIdentifier);
    printf("UID                  : %s\n", dataPacket->UID);
    printf("License status                   : %d\n", dataPacket->licenseStatus);
    printf("Engine speed                     : %f\n", dataPacket->engineSpeed);
    printf("Vehicle distance                 : %f\n", dataPacket->totalVehicleDistance);
    printf("Hours                            : %f\n", dataPacket->totalHoursofOperation);
    printf("Vehicle speed                    : %f\n", dataPacket->vehicleSpeed);
    printf("Fuel level                       : %f\n", dataPacket->fuelLevel);
    printf("Engine temperature               : %f\n", dataPacket->engineTemperature);
    printf("Accelerator pedal position       : %f\n", dataPacket->acceleratorPedalPosition);
    printf("ACC Ignition status              : %d\n", dataPacket->accIgnition);
    printf("Ignition status                  : %d\n", dataPacket->ignitionStatus);
    printf("Battery SOC                      : %f\n", dataPacket->batterySoc);
    printf("Battery SOH                      : %f\n", dataPacket->batterySoh);
    printf("Engine load                      : %f\n", dataPacket->engineLoad);
    printf("VIN                              : %s\n", dataPacket->vin);
    printf("Engine fuel rate                 : %f\n", dataPacket->engineFuelRate);
    printf("Fuel consumption                 : %f\n", dataPacket->instantFuelConsumption);
    printf("Total fuel used                  : %f\n", dataPacket->totalFuelUsed);
    printf("Battery current                  : %f\n", dataPacket->batteryCurrent);
    printf("Vehicle weight                   : %f\n", dataPacket->vehicleWeight);
    printf("Fuel name                        : %s\n", dataPacket->fuelName);
    printf("Bootloader version               : %s\n", dataPacket->bootcodeVer);
    printf("Firmware version                 : %s\n", dataPacket->firmwareVer);
    delete dataPacket;
}

void SerialPort::processPbPacket(const char *data, int len)
{
    int state = getInt16((void*)&data[10]);
    if (state == 0x31)
    {
        printf("Panic Button event: pressed\n");
    }
    else if (state == 0x30)
    {
        printf("Panic Button event: released\n");
    }
    else
    {
        printf("Panic Button event: unknown(0x%02X)", state);
    }
}

bool SerialPort::close()
{
    stopped = true;
    readThread->join();
    if (urlPosting)
    {
        httpsThread->join();
    }
    if (fd != -1)
    {
        printf("close %s.\n", port);
        ::close(fd);
        fd = -1;
    }
    return true;
}
