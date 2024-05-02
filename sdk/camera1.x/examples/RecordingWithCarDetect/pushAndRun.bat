adb root
adb shell mkdir /data/test
adb push ../../project/libMitacCameraSdk.so /data/test
adb push ../../../../bsp/lib/snpe/libSNPE.so /data/test
adb push mobilenet_ssd.dlc /data/test
adb push project/RecordingWithCarDetect /data/test
adb shell "cd /data/test;chmod a+x RecordingWithCarDetect;export LD_LIBRARY_PATH=/data/test;./RecordingWithCarDetect"
adb pull /data/test/test.mp4