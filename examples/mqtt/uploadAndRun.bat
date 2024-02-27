adb root
adb shell mkdir data/mqttTest
adb shell mkdir data/mqttTest/cert
adb push project/MqttTest data/mqttTest/MqttTest
adb push cert data/mqttTest/cert
adb shell "cd data/mqttTest;chmod a+x MqttTest;./MqttTest"

