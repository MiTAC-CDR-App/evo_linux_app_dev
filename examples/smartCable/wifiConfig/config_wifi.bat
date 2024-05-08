adb devices
adb root
adb remount
adb push connect_wifi.sh /data/test/
adb shell chmod +x /data/test/connect_wifi.sh
adb shell /data/test/connect_wifi.sh
pause