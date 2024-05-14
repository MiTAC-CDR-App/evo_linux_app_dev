adb root
adb shell mkdir /data/test
adb push ../../../project/libMitacCameraSdk.so /data/test
adb push project/Live555Test /data/test
adb shell "cd /data/test;chmod a+x Live555Test;export LD_LIBRARY_PATH=/data/test;./Live555Test"