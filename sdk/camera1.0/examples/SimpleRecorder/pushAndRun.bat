adb root
adb shell mkdir /data/test
adb push ../../project/libMitacCameraSdk.so /data/test
adb push project/SimpleRecorder /data/test
adb shell "cd /data/test;chmod a+x SimpleRecorder;export LD_LIBRARY_PATH=/data/test;./SimpleRecorder"
adb pull /data/test/test.mp4