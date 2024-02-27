/*
* Copyright (c) 2017-2021, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of The Linux Foundation nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! @file qmmf_recorder_extra_param_tags.h
*/

#pragma once

#include "qmmf_recorder_params.h"
#include "qmmf_recorder_extra_param.h"

namespace qmmf {

namespace recorder {

enum ParamTag {
  QMMF_SOURCE_VIDEO_TRACK_ID = (1 << 16),
  QMMF_SNAPSHOT_TYPE,
  QMMF_VIDEO_HDR_MODE,
  QMMF_TRACK_CROP,
  QMMF_FORCE_SENSOR_MODE,
  QMMF_EIS,
  QMMF_PARTIAL_METADATA,
  QMMF_CAMERA_SLAVE_MODE,
  QMMF_USE_LINKED_TRACK_IN_SLAVE_MODE,
  QMMF_LDC,
  QMMF_LCAC,
  QMMF_FRAME_RATE_CONTROL,
};

enum class SnapshotMode {
  /**< this is not valid mode */
  kNone,
  /**< High quality snapshot. This snapshot */
  /**< will interrupt video streaming if any */
  kStill,
  /**< High quality snapshot plus RAW dump. If kStillPlusRaw is enabled then */
  /**< RAW reprocessing (RAW plugins) cannot be supported.                   */
  kStillPlusRaw,
  /**< Video snapshot is captured with video settings. Video recording will  */
  /**< not be interrupted in this mode. */
  kVideo,
  /**< Continuous capture. QMMF will take images until CancelCaptureImage.   */
  /**< Capture rate could be set by QMMF_POSTPROCESS_FRAME_SKIP tag.         */
  kContinuous,
  /**< Zero Shutter Lag capture. QMMF starts ZSL continuous stream. Frames   */
  /**< from continuous stream are stored in ZSL queue. Last good frame in    */
  /**< ZSL queue will be used when user call CaptureImage API. ZSL stream    */
  /**< will be stopped when mode is changed or CancelCaptureImage is called. */
  kZsl,
  /**< It is the combination of kVideo and RAW snapshot. */
  kVideoPlusRaw
};

enum class SlaveMode {
  /**< this is not valid mode */
  kNone,
  /**< Camera Master mode */
  kMaster,
  /**< Camera Slave mode */
  kSlave,
};


enum class FrameRateControlMode {
  /**< control stream frame rate by frame skip */
  kFrameSkip,
  /**< control stream frame rate by HAL3 capture requests */
  kCaptureRequest
};

struct SourceVideoTrack : DataTagBase {
  int32_t source_track_id;  // Default: -1
  SourceVideoTrack()
    : DataTagBase(QMMF_SOURCE_VIDEO_TRACK_ID),
      source_track_id(-1) {}
};

struct SnapshotType : DataTagBase {
  /**< This is to change the Snapshot type */
  /**< Supported Modes are: kStill, kStillPlusRaw, kVideo, kContinuous. */
  /**< Default snapshot mode is kVideo. */
  SnapshotMode type;
  /**< RAW format takes place only if snapshot type is kStillPlusRaw. */
  /**< Default RAW format is kBayerRDI10BIT. */
  ImageFormat raw_format;
  /**< This is ZSL queue configuration. */
  ZslQueueParam zsl_queue_params;
  /**< This is output images configuration in kZsl snapshot mode. */
  ImageParam    zsl_image_param;

  SnapshotType()
    : DataTagBase(QMMF_SNAPSHOT_TYPE),
      type(SnapshotMode::kVideo),
      raw_format(ImageFormat::kBayerRDI10BIT),
      zsl_queue_params{},
      zsl_image_param{} {}

};

struct VideoHDRMode : DataTagBase {
  bool enable;  // Default: false to disable HDR
  VideoHDRMode()
    : DataTagBase(QMMF_VIDEO_HDR_MODE),
      enable(false) {}
};

struct TrackCrop : DataTagBase {
  // Y-axis coordinate of the crop rectangle top left starting point.
  // The coordinate system begins from the top left corner of the source.
  uint32_t x;         // Default: 0
  // X-axis coordinate of the crop rectangle top left starting point.
  // The coordinate system begins from the top left corner of the source.
  uint32_t y;         // Default: 0
  // Width in pixels of the crop rectangle.
  uint32_t width;     // Default: 0
  // Height in pixels of the crop rectangle.
  uint32_t height;    // Default: 0

  TrackCrop()
    : DataTagBase(QMMF_TRACK_CROP),
       x(0), y(0), width(0), height(0) {}
};

struct ForceSensorMode : DataTagBase {
  int32_t  mode;    // Default: -1 to disable ForceSensorMode
  ForceSensorMode()
    : DataTagBase(QMMF_FORCE_SENSOR_MODE),
      /**< Index of sensor mode to be passed by the application. */
      /**< Application needs to set the mode only once, attach this tag */
      /**< to only one of the tracks. Once all tracks are deleted, */
      /**< framework will return to auto mode selection. */
      /**< Force Sensor Mode index starts with 0. To disable this feature, */
      /**< set it to -1 in any one track, to allow auto mode selection. */
      mode(-1) {}
};

struct EISSetup : DataTagBase {
  bool enable; // Default: false to disable EIS
  EISSetup() :
    DataTagBase(QMMF_EIS),
    enable(false) {
  }
};

struct PartialMetadata : DataTagBase {
  /**< Client can configure whether it requires partial Metadata or not. */
  /**< Camera Adaptor will send the partial data to camera context */
  /**< irrespective of clients needs it or not.Its responsibility  */
  /**< of context to check whether to propagate the partial data to */
  /**< client or not. Default: false to disable PartialMetadata*/
  bool enable;
  PartialMetadata() :
    DataTagBase(QMMF_PARTIAL_METADATA),
    enable(false) {
  }
};

struct CameraSlaveMode : DataTagBase {
  /**< Add support for multi client support for same camera. */
  /**< Client can open a given camera in slave mode. */
  /**< The camera being opened as slave needs to be already opened by */
  /**< another client which uses it as master. If this requirement is */
  /**< not fulfilled then the slave client needs to wait until signaled */
  /**< on event from the service. Default: SlaveMode::kNone*/
  SlaveMode mode;
  CameraSlaveMode() :
    DataTagBase(QMMF_CAMERA_SLAVE_MODE),
    mode(SlaveMode::kNone) {
  }
};

struct LinkedTrackInSlaveMode : DataTagBase {
  /**< Add support for client to enable/disable linked track in slave mode. */
  /**< Default: True*/
  bool enable;
  LinkedTrackInSlaveMode() :
    DataTagBase(QMMF_USE_LINKED_TRACK_IN_SLAVE_MODE),
    enable(false) {
  }
};

struct LDCMode : DataTagBase {
  /**< Add support for client to enable/disable */
  /**< LDC (Lens Distortion Correction). */
  /**< Default: False */
  bool enable;
  LDCMode() :
    DataTagBase(QMMF_LDC), enable(false) {
  }
};

struct LCACMode : DataTagBase {
  /**< Add support for client to enable/disable */
  /**< LCAC (Lateral Chromatic Aberration Correction). */
  /**< Default: False */
  bool enable;
  LCACMode() :
    DataTagBase(QMMF_LCAC), enable(false) {
  }
};

struct FrameRateControl : DataTagBase {
  /**< Add support for stream frame rate control mode  */
  FrameRateControlMode mode;
  FrameRateControl() :
    DataTagBase(QMMF_FRAME_RATE_CONTROL),
    mode(FrameRateControlMode::kFrameSkip) {
  }
};

}; //namespace recorder.

}; //namespace qmmf.
