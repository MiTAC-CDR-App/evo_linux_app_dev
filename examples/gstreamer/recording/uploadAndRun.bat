adb root
adb shell mkdir /data/test
adb push project/GstreamerRecordingTest /data/test
adb shell "cd /data/test;chmod a+x GstreamerRecordingTest;./GstreamerRecordingTest"
adb pull /data/test/test.mp4