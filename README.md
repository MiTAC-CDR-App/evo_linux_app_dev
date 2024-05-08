# Development Package for EVO Linux
This repository is used to pack required
* BSP/3rd-party header files
* BSP/3rd-party shared libraries

to develop a typical CDR (Connected Drive Recorder) application.  And it also include sample codes for
* gstreamer (check [/examples/gstreamer/*](https://github.com/MiTAC-CDR-App/evo_linux_app_dev/tree/main/examples/gstreamer))
* mqtt (check [/examples/mqtt](https://github.com/MiTAC-CDR-App/evo_linux_app_dev/tree/main/examples/mqtt))
* smartCable (check [/examples/smartCable](https://github.com/MiTAC-CDR-App/evo_linux_app_dev/tree/main/examples/smartCable))

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
To push files into EVO device, pull files from EVO device, or to run executible on EVO device, `ADB` is required.  It can be downloaded from
https://developer.android.com/tools/releases/platform-tools.

To ease the execution, we usually
* Setup samba server on Linux.
* Map Linux's home as Windows drive (fo example `Z:`).


When we test example, we will open a Windows command line and change directory to example's path to execute example specified DOS batch file.
```sh
z:
cd \evo_linux_app_dev\examples\mqtt
uploadAndRun
```
Inside the batch file (for example, `uploadAndRun.bat`), it usually push executible into EVO device, and `ADB shell` to execute commands on EVO device.

## Pofiling Tools
Under construction...