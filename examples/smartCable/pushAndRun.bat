adb root
adb shell mkdir /data/test
adb push project/SmartCableReader /data/test
adb shell "cd /data/test;chmod a+x SmartCableReader;./SmartCableReader"