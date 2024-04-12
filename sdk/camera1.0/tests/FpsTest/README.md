# FPS measure example
This example will use CameraSDK `mitacCamConfigCamera`, `mitacCamConfigMainStream`, `mitacCamConfigAiStream`, `mitacCamStartOperation`, and `mitacCamStopOperation`.

After get frame data from the callback of CameraSDK, calculate fps of the stream.

## How to build:
Please execute
```sh
./build
```
It will generate executable project/FpsTest.

## How to excute FpsTest:
If you run `ADB` from MS Windows, please execute
```sh
pushAndRun.bat
```
under the tests/FpsTest/ directory.  After the recording is run for more than 10 seconds, FPS will output on screen every second. Key in "end" and press <Enter> to stop it.

## Known issue
When processing the example, a warning message may be displayed. This warning will be resolved in the future.

The warning message example:
```sh
(Recording:1402): GStreamer-WARNING **: 00:12:10.081: ../gstreamer-1.16.3/gst/gstpad.c:4544:gst_pad_push_data:<h264parse:src> Got data flow before stream-start event
```
