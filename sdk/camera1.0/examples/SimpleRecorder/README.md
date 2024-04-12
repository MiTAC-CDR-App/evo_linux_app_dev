# Recording example
This example will use CameraSDK `mitacCamConfigCamera`, `mitacCamConfigMainStream`, `mitacCamStartOperation`, and `mitacCamStopOperation`.

After get encoded data from the callback of CameraSDK, use FFMPEG to mux encoded video samples to test.mp4.

## How to build:
Please execute
```sh
./build
```
It will generate executable project/SimpleRecorder.

## How to excute SimpleRecorder:
If you run `ADB` from MS Windows, please execute
```sh
pushAndRun.bat
```
under the examples/SimpleRecorder/ directory.  After the recording is successful, test.mp4 will be generated, and then `pushAndRun.bat` will pull test.mp4 to windows.  You can play test.mp4 directly.

## Known issue
When processing the example, a warning message may be displayed. This warning will be resolved in the future.

The warning message example:
```sh
(Recording:1402): GStreamer-WARNING **: 00:12:10.081: ../gstreamer-1.16.3/gst/gstpad.c:4544:gst_pad_push_data:<h264parse:src> Got data flow before stream-start event
```
