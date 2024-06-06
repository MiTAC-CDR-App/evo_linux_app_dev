# Camera SDK Package for EVO Linux
The SDK is created for supporting the camera functions(to offer H264 encoded frames, RGB data for AI) on EVO Linux platform

## The camera SDK contains
* camera1.x/include/mitacCameraSdk.h SDK header file including the API description
* camera1.x/project/libMitacCameraSdk.so SDK library

## Sample codes for using camera SDK
* camera1.x/examples/SimpleRecorder sample code to generate the mp4 file with the camera SDK
* camera1.x/tests/FpsTest sample code for the fps measurement

## Release Note
### SDK v1.1.0.1
New feature
* Some internal fix to maintain reference count correctly
* Add userData to better support clients
* Support to pass encoded frames, AI data from client threads
* Timestamp is aligned to Linux CLOCK_BOOTTIME

### SDK v1.1.0.2
Bug fix
* The 'gop' argument of mitacCamConfigSubStream() is not applied
* When the stream setting parameter 'byUserThread' is true, the user thread cannot be stopped after calling mitacCamStopOperation()

### SDK v1.1.0.3
Bug fix
* Fix the black image issue for aiStream

### SDK v1.5.0.1
New feature
* Support TVI recording

### SDK v1.5.0.2
New feature
* Support TVI AI streams (cannot work with TVI recording)
Bug fix
* Fix TVI recording mirror issue
Limitation
* For TVI cameras, cannot support both recording and AI

