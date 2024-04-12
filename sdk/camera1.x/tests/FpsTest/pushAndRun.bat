adb root
adb shell mkdir /data/test
adb push ../../project/libMitacCameraSdk.so /data/test
adb push project/FpsTest /data/test
adb shell "cd /data/test;chmod a+x FpsTest;export LD_LIBRARY_PATH=/data/test;./FpsTest"