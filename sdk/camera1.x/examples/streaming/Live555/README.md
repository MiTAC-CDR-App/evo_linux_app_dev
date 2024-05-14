# live555 test example
This example will demonstrate video streaming with CameraSDK and live555 library.

## How to build:
Please execute
```sh
./build
```
It will generate executable project/Live555Test

## How to play RTSP streaming:
1. Please make sure the device and PC are in the same local network and install VLC player on the PC first.
2. If you run `ADB` from MS Windows, please execute
   ```sh
   pushAndRun.bat
   ```
3. Play RTSP streaming with VLC player on the PC:
   1. Select [File] -> [Open Network].
   2. Enter RTSP url "rtsp://[device IP address]:8554/live555Test". 
   3. Push [Play] button.

## How to exit Live555Test:
Ctrl+c or enter "end" to exit Live555Test.

## Known issue
When processing the example, a warning message may be displayed. This warning will be resolved in the future.

The warning message looks like:
```sh
(Recording:1402): GStreamer-WARNING **: 00:12:10.081: ../gstreamer-1.16.3/gst/gstpad.c:4544:gst_pad_push_data:<h264parse:src> Got data flow before stream-start event
```
