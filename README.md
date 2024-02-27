# Development Package for EVO Linux
This repository is used to pack required
* BSP/3rd-party header files
* BSP/3rd-party shared libraries

to develop a typical CDR (Connected Drive Recorder) application.  And it also include sample codes for
* gstreamer (check `/examples/gstreamer/*`)
* mqtt (check [/examples/mqtt](https://github.com/MiTAC-CDR-App/evo_linux_app_dev/tree/main/examples/mqtt))

## Build Environment
Our original development environment is Ubuntu Linux, to prepare required development compilers/tools, please execute

```sh
sudo apt install cmake
sudo apt install gcc-aarch64-linux-gnu
sudo apt install g++-aarch64-linux-gnu
sudo apt install build-essential
sudo apt install openjdk-8-jdk
```

## Run Time Tool
To push files into EVO device, or to run executible on EVO device, `ADB` is required.  It can be downloaded from
https://developer.android.com/tools/releases/platform-tools.

## Pofiling Tools
Under construction...