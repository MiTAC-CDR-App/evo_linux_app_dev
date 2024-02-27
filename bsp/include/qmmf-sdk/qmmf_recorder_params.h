/*
* Copyright (c) 2016-2021, The Linux Foundation. All rights reserved.
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
*
* Changes from Qualcomm Innovation Center are provided under the following license:
*
* Copyright (c) 2021-2022 Qualcomm Innovation Center, Inc. All rights reserved.
*  
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*  
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*  
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*  
*     * Neither the name of Qualcomm Innovation Center, Inc. nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*  
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! @file qmmf_recorder_params.h
*/

#pragma once

#include <sys/types.h>

#include <cstddef>
#include <memory>
#include <iomanip>
#include <functional>
#include <sstream>
#include <string>
#include <cstdint>
#include <type_traits>
#include <vector>

#include <camera/CameraMetadata.h>

#include "qmmf-sdk/qmmf_buffer.h"

namespace qmmf {

namespace recorder {

typedef int32_t status_t;

enum class EventType : uint32_t {
  kUnknown       = 0, // Indicates a unknown event has occured.
  kServerDied    = 1, // Indicates un-recoverable service crash.
  kCameraError   = 2, // Indicates un-recoverable camera error.
  kCameraOpened  = 3, // Indicates camera that has been opened.
  kCameraClosing = 4, // Indicates camera that is about to be closed.
  kCameraClosed  = 5, // Indicates camera that has been closed.
  kFrameError    = 6, // Indicates a frame has been droped.
  kMetadataError = 7, // Indicates metadata for a frame has been droped.
};

enum class VideoFormat : uint32_t {
  kJPEG,
  kRGB,
  kNV12,
  kNV12UBWC,
  kNV16,
  kYUY2,
  kUYVY,
  kBayerIdeal,
  kBayerRDI8BIT,
  kBayerRDI10BIT,
  kBayerRDI12BIT,
  kBayerRDI16BIT,
};

enum class ImageFormat : uint32_t {
  kJPEG,
  kNV12,
  kNV21,
  kBayerIdeal,
  kBayerRDI8BIT,
  kBayerRDI10BIT,
  kBayerRDI12BIT,
  kBayerRDI16BIT,
};

enum class Rotation : uint32_t {
  kNone,
  k90,
  k180,
  k270,
};

/// @enum mapper::VideoFlags
/// @brief A strongly typed enum class representing video configuration flags.
enum class VideoFlags : uint64_t {
  kNone     = 0,      /// No active configuration flags.
  kIAEC     = 1 << 0, /// Wait Initial Auto Exposure Convergence.
  kUncashed = 1 << 1, /// Allocated buffers are not cached.
};

inline VideoFlags operator | (VideoFlags lhs, VideoFlags rhs) {
  using T = std::underlying_type_t<VideoFlags>;
  return static_cast<VideoFlags>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline VideoFlags& operator |= (VideoFlags& lhs, VideoFlags rhs) {
  lhs = lhs | rhs;
  return lhs;
}

inline VideoFlags operator & (VideoFlags lhs, VideoFlags rhs) {
  using T = std::underlying_type_t<VideoFlags>;
  return static_cast<VideoFlags>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline VideoFlags& operator &= (VideoFlags& lhs, VideoFlags rhs) {
  lhs = lhs & rhs;
  return lhs;
}

inline VideoFlags operator ^ (VideoFlags lhs, VideoFlags rhs) {
  using T = std::underlying_type_t<VideoFlags>;
  return static_cast<VideoFlags>(static_cast<T>(lhs) ^ static_cast<T>(rhs));
}

inline VideoFlags& operator ^= (VideoFlags& lhs, VideoFlags rhs) {
  lhs = lhs & rhs;
  return lhs;
}

inline VideoFlags operator ~ (VideoFlags& rhs) {
  using T = std::underlying_type_t<VideoFlags>;
  return static_cast<VideoFlags> (~static_cast<T>(rhs));
}

enum class VideoParam {
  kFrameRate,
  kEnableFrameRepeat,
};

typedef std::function<void(EventType event, void *payload, size_t size)> EventCb;

/// @brief Recorder callback is called to notify non track
/// and non session specific event notifications
///
/// Only error event types are expected as of now
struct RecorderCb {
  EventCb event_cb;
};

/// @brief Session cb is used to return state changes i.e. to indicate
/// start, stop, pause state transition completions
struct SessionCb {
  EventCb event_cb;
};

/// @brief Both data and event callbacks should be set by the client.
/// event_cb is called to notify track specific errors and data_cb
/// to notify availability of output data from track to clients
///
/// TrackMetaParam in data cb is an optional parameter. This parameter is
/// expected
/// to be used in case multiple frames are passed in the same buffer and
/// in that case meta_param can describe the respective frame offsets
/// and timestamps in the buffer
/// When the data cb is called by recorder, the buffer ownership is transfered
/// to
/// client. To return the buffer back to recoder, clients should call
/// ReturnTrackBuffer
/// API. However, clients needs to ensure that buffers returned within the frame
/// rate
/// of track - else recording pipeline will stall.
/// Track event_cb returns async error events and data_cb returns periodic
/// data
/// Note that both callback implementations need to be re-entrant.
struct TrackCb {
  std::function<void(uint32_t track_id, ::std::vector<BufferDescriptor> buffers,
                     ::std::vector<BufferMeta> metas)> data_cb;
  std::function<void(uint32_t track_id, EventType type, void *payload,
                     size_t size)> event_cb;
};

/// @brief Create time parameters for a video track
struct VideoTrackParam {
  /// Video Track camera id
  uint32_t    camera_id;
  /// Video Track width
  uint32_t    width;
  /// Video Track height
  uint32_t    height;
  /// Video Track frame rate
  float       framerate;
  /// Video Track format
  VideoFormat format;
  /// Video Track rotation angle
  Rotation    rotation;
  /// Additional buffers allocated for the track
  uint32_t    xtrabufs;
  /// Video Track addtional flags
  VideoFlags  flags;

  VideoTrackParam(uint32_t cam_id = 0, uint32_t w = 3840, uint32_t h = 2160,
                  float fps = 30, VideoFormat fmt = VideoFormat::kNV12,
                  Rotation rotate = Rotation::kNone, uint32_t extrabufs = 0,
                  VideoFlags flgs = VideoFlags::kNone)
      : camera_id(cam_id), width(w), height(h), framerate(fps), format(fmt),
        rotation(rotate), xtrabufs(extrabufs), flags(flgs) {}

  ::std::string ToString() const {
    ::std::stringstream stream;
    stream << "camera_id[" << camera_id << "] ";
    stream << "width[" << width << "] ";
    stream << "height[" << height << "] ";
    stream << "framerate[" << framerate << "] ";
    stream << "format["
           << static_cast<::std::underlying_type<VideoFormat>::type>(format)
           << "] ";
    stream << "rotation["
           << static_cast<::std::underlying_type<Rotation>::type>(rotation)
           << "] ";
    stream << "xtrabufs[" << xtrabufs << "] ";
    stream << "flags[" << ::std::setbase(16)
           << static_cast<::std::underlying_type<VideoFlags>::type>(flags)
           << ::std::setbase(10) << "]";
    return stream.str();
  }
};

/// @brief Result callback passed to StartCamera API
///
/// Optional result callback which will get triggered
/// by service once there is at least one started session
/// which includes a video track.
typedef std::function<void(uint32_t camera_id,
                           const android::CameraMetadata &res)> CameraResultCb;

/// @brief For thumbnail images only kJPEG is supported
/// For YUV and Bayer formats, quality is ignored
struct ImageParam {
  /// Image width
  uint32_t    width;
  /// Image height
  uint32_t    height;
  /// Image format
  ImageFormat format;
  /// Image quality (ignored for YUV and Bayer formats)
  uint32_t    quality;

  ImageParam(): width(0), height(0), format(ImageFormat::kJPEG), quality(95) {}

  ::std::string ToString() const {
    ::std::stringstream stream;
    stream << "width[" << width << "]";
    stream << "height[" << height << "] ";
    stream << "format["
           << static_cast<::std::underlying_type<ImageFormat>::type>(format)
           << "]";
    stream << "quality[" << quality << "] ";
    return stream.str();
  }
};

/// \brief ZSL queue parameters
///
/// Images in ZSL queue might have different dimension than final image.
struct ZslQueueParam {
  uint32_t    width;
  uint32_t    height;
  uint32_t    queue_depth;
  ImageFormat image_format;

  ZslQueueParam()
    : width(3840),
      height(2160),
      queue_depth(4),
      image_format(ImageFormat::kNV21) {}

  ::std::string ToString() const {
    ::std::stringstream stream;
    stream << "width[" << width << "]";
    stream << "height[" << height << "] ";
    stream << "queue_depth[" << queue_depth << "] ";
    stream << "image_format["
           << static_cast<::std::underlying_type<ImageFormat>::type>
                         (image_format)
           << "]";
    return stream.str();
  }
};

typedef std::function<void(uint32_t camera_id, uint32_t imgcount,
                           BufferDescriptor buffer, BufferMeta meta)>
    ImageCaptureCb;

typedef std::function<void(int32_t buf_fd, uint32_t encoded_size)>
    OfflineJpegCb;
};
};  // namespace qmmf::recorder
