#!/bin/bash
insmod /lib/modules/5.4.191/extra/wlan.ko
sleep 3
ifconfig wlan0 up
sleep 3
wpa_supplicant -i wlan0 -D nl80211 -c /etc/misc/wifi/wpa_supplicant.conf -B
sleep 3
wpa_cli -iwlan0 add_network
wpa_cli -iwlan0 set_network 1 ssid '"TestWifiAp"'
wpa_cli -iwlan0 set_network 1 psk '"1234567890"'
wpa_cli -iwlan0 enable_network 1
dhcpcd