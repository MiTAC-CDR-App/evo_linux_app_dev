adb root
adb shell mkdir data/mqttTest
adb shell mkdir data/mqttTest/cert
adb push project/MqttTest data/mqttTest/MqttTest
adb push cert/039e1ece-ed28-4507-8c88-09dc7b6fa780.key data/mqttTest/cert
adb push cert/039e1ece-ed28-4507-8c88-09dc7b6fa780.pem data/mqttTest/cert
adb push cert/ca-certificates.crt data/mqttTest/cert
adb shell "cd data/mqttTest;chmod a+x MqttTest;./MqttTest"

