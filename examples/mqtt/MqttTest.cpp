/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  EVO Linxu Example Support                                                                   *
 * BINARY NAME :  MqttTest                                                                                    *
 * FILE NAME   :  MqttTest.cpp                                                                                *
 * CREATED BY  :  Brian Wu <tzung.home@mic.com.tw>                                                            *
 * CREATED DATE:  02/21/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Example for MQTT.                                                                           *
 *------------------------------------------------------------------------------------------------------------*/

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include <signal.h>
// POSIX include
#include <unistd.h>
// C++ include
#include <memory>
#include <exception>
// Linux include
#include <ucontext.h>
#include <execinfo.h>
// AWS IoT SDK(Cpp) include
#include <aws/common/logging.h>
#include <aws/io/io.h>
#include <aws/io/logging.h>
#include <aws/io/event_loop.h>
#include <aws/io/channel_bootstrap.h>
#include <aws/io/host_resolver.h>
#include <aws/io/socket.h>
#include <aws/io/tls_channel_handler.h>
#include <aws/mqtt/mqtt.h>
#include <aws/mqtt/client.h>

static aws_allocator *allocator;
static aws_event_loop_group elg;
static aws_host_resolver resolver;
static aws_client_bootstrap *bootstrap;
static aws_tls_ctx *tlsCtx;
static aws_tls_connection_options optTlsConn;
static aws_socket_options optSocket;
static aws_mqtt_client client;
static aws_mqtt_client_connection *mqttConn = 0;
static aws_mqtt_connection_options optMqttConn;

static const char *CA = "cert/ca-certificates.crt";
static const char *HOST = "a1c5bvsmdje7oz-ats.iot.us-east-1.amazonaws.com";
static std::string topicRoot = "mdt-VisionMax-dev"; //B2B
static std::string topicCategory = topicRoot + "/EVO/K265/"; //B2B
static std::string devID = "039e1ece-ed28-4507-8c88-09dc7b6fa780";
static std::string eventTopic = topicCategory + devID + "/event";
static std::string serverEventTopic = topicRoot + "/command/" + devID + "/event";
static const char *testEventPayload = "{\"payload\":{\"spd\":0.000000,\"alt\":0,\"lng\":121.374800,"
                 "\"lat\":25.046950},\"version\":{\"app\":\"1.0.0.0\",\"cam1\":\"1.0.0.0\",\"speedcam\":\"1\"},"
                "\"device_id\":\"039e1ece-ed28-4507-8c88-09dc7b6fa780\",\"timestamp\":%d,\"type\":\"startup\"}";

static void _onMqttConnected(aws_mqtt_client_connection *conn, int errorCode,
                                   aws_mqtt_connect_return_code returnCode, bool sessionPresent, void *context);
static void _onPublishDone(aws_mqtt_client_connection *conn, uint16_t pktID, int errorCode, void *context);
static void _onEventTicketReceived(aws_mqtt_client_connection *conn, const aws_byte_cursor *topic,
                                                                 const aws_byte_cursor *payload, void *context);
static void initMqtt();
static void triggerStartupEvent();

static bool connecting = false;
static bool connected = false;
static bool transmitting = false;

int main(int argc, char *argv[])
{
    connecting = true;
    initMqtt();
    //Wait for mqtt connection to complete.
    while (connecting)
    {
        usleep(100 * 1000);
    }
    if (connected)
    {
        transmitting = true;
        triggerStartupEvent();
        //Wait for mqtt message transmitting to complete.
        while (transmitting)
        {
            usleep(100 * 1000);
        }
    }
}

static void triggerStartupEvent()
{
    aws_byte_cursor topicCur = aws_byte_cursor_from_c_str(eventTopic.c_str());
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    int utcSec =  ((ts.tv_sec * 1000LL + (ts.tv_nsec + 500000) / 1000000) / 1000);
    char buff[1024];
    snprintf(buff, sizeof(buff), testEventPayload, utcSec);
    printf("send event, payload = %s\n" , buff);
    aws_byte_cursor payloadCur = aws_byte_cursor_from_c_str(buff);
    int pktID = aws_mqtt_client_connection_publish(mqttConn, &topicCur, AWS_MQTT_QOS_AT_LEAST_ONCE, false,
                                                                             &payloadCur, _onPublishDone, NULL);
    if(pktID != 0)
    {
        printf("Succeed to sendMessage, pktID : %d\n" , pktID);
    }
    else
    {
        printf("Failed to MQTT publish!\n");
    }
    fflush(stdout);
}

static void _onPublishDone(aws_mqtt_client_connection *conn, uint16_t pktID, int errorCode, void *context)
{
    if(errorCode)
    {
        printf("Failed to MQTT publish, errorCode: %d, pktID: %d!\n", errorCode, pktID);
        transmitting = false;
    }
    else
    {
        printf("Succeed to MQTT publish, pktID: %d!\n", pktID);
        printf("Waitting for server return the event ticket.\n");
    }
    fflush(stdout);
}

static void initMqtt()
{
    allocator = aws_mem_tracer_new(aws_default_allocator(), NULL, AWS_MEMTRACE_BYTES, 0);
    aws_mqtt_library_init(allocator);
    aws_io_library_init(aws_default_allocator());

    int result = aws_event_loop_group_default_init(&elg, aws_default_allocator(), 1);
    if(result == AWS_OP_SUCCESS)
    {
        printf("Succeed to initialize AWS event loop group.\n");
    }
    else
    {
        printf("Failed to initialize AWS event loop group, result: %d.\n", result);
    }
    result = aws_host_resolver_init_default(&resolver, allocator, 8, &elg);
    if(result == AWS_OP_SUCCESS)
    {
        printf("Succeed to initialize AWS default host resolver.\n");
    }
    else
    {
        printf("Failed to initialize AWS default host resolver, result: %d.\n", result);
    }
    aws_client_bootstrap_options optBootstrap = {&elg, &resolver, 0, 0, 0};
    bootstrap = aws_client_bootstrap_new(allocator, &optBootstrap);
    aws_tls_ctx_options optTlsCtx;
    std::string certFile = "cert/";
    certFile += devID;
    certFile += ".pem";
    std::string keyFile = "cert/";
    keyFile += devID;
    keyFile += ".key";
    result = aws_tls_ctx_options_init_client_mtls_from_path(&optTlsCtx, allocator, certFile.c_str(),
                                                                                               keyFile.c_str());
    if(result == AWS_OP_SUCCESS)
    {
        printf("Succeed to initialize AWS TLS ctx option.\n");
    }
    else
    {
        printf("Failed to initialize AWS TLS ctx option, result: %d.\n", result);
    }
    if(!aws_tls_is_cipher_pref_supported(optTlsCtx.cipher_pref))
    {
        printf("cipher not supported!\n");
    }
        aws_tls_ctx_options_override_default_trust_store_from_path(&optTlsCtx, 0, CA);

    tlsCtx = aws_tls_client_ctx_new(allocator, &optTlsCtx);
    aws_tls_ctx_options_clean_up(&optTlsCtx);
    aws_tls_connection_options_init_from_ctx(&optTlsConn, tlsCtx);

    result = aws_mqtt_client_init(&client, allocator, bootstrap);
    if(result == AWS_OP_SUCCESS)
    {
        printf("Succeed to initialize AWS MQTT client.\n");
    }
    else
    {
        printf("Failed to initialize AWS MQTT client, result: %d.\n", result);
    }

    memset(&optSocket, 0, sizeof(optSocket));
    optSocket.connect_timeout_ms = 10000;
    optSocket.type = AWS_SOCKET_STREAM;
    optSocket.domain = AWS_SOCKET_IPV4;
    printf("AWS MQTT connection  HOST: %s.\n", HOST);
    optMqttConn.host_name = aws_byte_cursor_from_c_str(HOST);
    optMqttConn.port = 8883;
    optMqttConn.socket_options = &optSocket;
    optMqttConn.tls_options = &optTlsConn;
    optMqttConn.client_id = aws_byte_cursor_from_c_str(devID.c_str());
    optMqttConn.keep_alive_time_secs = 0;
    optMqttConn.ping_timeout_ms = 0;
    optMqttConn.on_connection_complete = _onMqttConnected;
    optMqttConn.user_data = 0;
    optMqttConn.clean_session = true;
    mqttConn = aws_mqtt_client_connection_new(&client);
    result = aws_mqtt_client_connection_connect(mqttConn, &optMqttConn);

    if(result == AWS_OP_SUCCESS)
    {
        printf("Succeed to initiate MQTT connection.\n");
    }
    else
    {
        printf("Failed to initiate MQTT connection, result: %d!\n.", result);
    }
}

static void _onMqttConnected(aws_mqtt_client_connection *conn, int errorCode,
                                    aws_mqtt_connect_return_code returnCode, bool sessionPresent, void *context)
{
    if(errorCode == 0)
    {
        printf("Connect to MDT MQTT broker successfully.\n");
        if (mqttConn)
        {
            aws_byte_cursor topicCur = aws_byte_cursor_from_c_str(serverEventTopic.c_str());
            int pktID = aws_mqtt_client_connection_subscribe(conn, &topicCur, AWS_MQTT_QOS_AT_LEAST_ONCE,
                                                                            _onEventTicketReceived, 0, 0, 0, 0);
            if(pktID == 0)
            {
                printf("Failed to MQTT subscribe for event ticket, pktID == 0!\n");
            }
            else
            {
                printf("Subscribe for event ticket successfully. pktID == %d\n" , pktID);
                connected = true;
                connecting = false;
            }
        }
    }
    else
    {
        printf("Failed to connect to MDT MQTT broker, error: %d, return: %d!\n"
                             "Please confirm whether the network is connected and the system time is correct.\n"
                                                                                       , errorCode, returnCode);
        connecting = false;
    }
    fflush(stdout);
}

static void _onEventTicketReceived(aws_mqtt_client_connection *conn, const aws_byte_cursor *topic,
                                                                  const aws_byte_cursor *payload, void *context)
{
    printf("Received server event ticket, payload = %s\n" , (const char *) (payload->ptr));
    transmitting = false;
    fflush(stdout);
}
