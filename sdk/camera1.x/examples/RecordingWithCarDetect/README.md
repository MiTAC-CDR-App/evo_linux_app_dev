# Recording and car detection example
This example will use CameraSDK `mitacCamConfigCamera`, `mitacCamConfigMainStream`, `mitacCamConfigAiStream`, `mitacCamStartOperation`, and `mitacCamStopOperation`.

After get encoded data from the callback of CameraSDK, use FFMPEG to mux encoded video samples to test.mp4; After get raw data from the callback of CameraSDK, use SNPE library to perform car detection.

## How to build:
Please execute
```sh
./build
```
It will generate executable project/RecordingWithCarDetect.

## How to excute RecordingWithCarDetect:
If you run `ADB` from MS Windows, please execute
```sh
pushAndRun.bat
```
It will start video recording and car detect by using front camera and recording duration is 60 seconds. If car is detected, it will output like below,
Ex:Found a car at (24, 9), (291, 296), 267 x 287

under the examples/RecordingWithCarDetect/ directory. After the recording is successful, the number of detected cars will be printed and test.mp4 will be generated, and then `pushAndRun.bat` will pull test.mp4 to windows. You can play test.mp4 directly.

## Known issue
When processing the example, a warning message may be displayed. This warning will be resolved in the future.

The warning message example:
```sh
(Recording:1402): GStreamer-WARNING **: 00:12:10.081: ../gstreamer-1.16.3/gst/gstpad.c:4544:gst_pad_push_data:<h264parse:src> Got data flow before stream-start event
```
