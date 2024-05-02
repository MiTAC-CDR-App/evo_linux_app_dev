/*
 * Copyright (c) 2023 MiTAC International Corp.
 * -----
 * File: FastCVHelper.cpp
 * Project: aiTest
 * Created Date: 14 Mar 2023
 * Author: John
 * -----
 * Last Modified: Tue Mar 14 2023
 * Modified By: John
 * -----
 * Description: FastCV helper
 */

 // Standard includes
#include <stdlib.h>
#include <string.h>
#include <string>
#include <algorithm>
// FastCV include
#include <fastcv.h>
#include <fastcvExt.h>

#include "Log.h"
#include "FastCVHelper.h"
#include "IAIModel.h"

/*
 * FastCV operation mode (see fcvOperationMode in fastcv.h)
 * 0: FASTCV_OP_LOW_POWER
 * 1: FASTCV_OP_PERFORMANCE
 * 2: FASTCV_OP_CPU_OFFLOAD
 * 3: FASTCV_OP_CPU_PERFORMANCE
 */
#define FASTCV_OP_MODE FASTCV_OP_CPU_PERFORMANCE
/*
 * FastCV extended operation mode (see fcvOperationModeExt in fastcvExt.h)
 * 0: FASTCV_OP_EXT_QDSP
 * 1: FASTCV_OP_EXT_GPU
 * 2: FASTCV_OP_EXT_ADSP
 * 3: FASTCV_OP_EXT_CDSP
 * 4: FASTCV_OP_EXT_MDSP
 */
#define FASTCV_OP_MODE_EXT FASTCV_OP_EXT_GPU

// Uncomment this line to use ext mode
// #define USE_EXT_MODE

void FastCVHelper::init()
{
    char version[80];
    fcvGetVersion(version, 80);
    LOGI("fastcv version is %s.", version);

#ifdef USE_EXT_MODE
    int opMode = FASTCV_OP_MODE_EXT;
    int ret = fcvSetOperationModeExt((fcvOperationModeExt)opMode);
    LOGI("Set fastcv operation mode (extended): %d, result: %d", opMode, ret);
#else
    int opMode = FASTCV_OP_MODE;
    int ret = fcvSetOperationMode((fcvOperationMode)opMode);
    LOGI("Set fastcv operation mode: %d, result: %d", opMode, ret);
#endif
    fcvMemInit();
}

void FastCVHelper::shutdown()
{
    fcvMemDeInit();
    fcvCleanUp();
}

void *FastCVHelper::calloc(size_t num, size_t size)
{
    void *buf = fcvMemAlloc(num * size, 16);
    memset(buf, 255, num * size);
    return buf;
}

void FastCVHelper::free(void *ptr)
{
    fcvMemFree(ptr);
}

void FastCVHelper::NV12toRGB(const uint8_t *planeY, const uint8_t *planeUV, uint32_t srcWidth, uint32_t srcHeight, uint8_t *dst)
{
    fcvColorYCbCr420PseudoPlanarToRGB888u8(planeY, planeUV, srcWidth, srcHeight, srcWidth, srcWidth, dst, srcWidth * 3);
}

void FastCVHelper::RGBtoBGRSNPE(const uint8_t *rgb, uint32_t srcWidth, uint32_t srcHeight, float *dst)
{
    // 1. Scale to 300 x 300 plane by plane
    uint32_t srcStride = srcWidth;
    uint8_t *planeR = (uint8_t *)calloc(srcWidth * srcHeight, sizeof(uint8_t));
    uint8_t *planeG = (uint8_t *)calloc(srcWidth * srcHeight, sizeof(uint8_t));
    uint8_t *planeB = (uint8_t *)calloc(srcWidth * srcHeight, sizeof(uint8_t));
    fcvChannelExtractu8(rgb, srcWidth, srcHeight, srcStride * 3, 0, 0, 0, 0, FASTCV_CHANNEL_R, FASTCV_RGB, planeR, srcStride);
    fcvChannelExtractu8(rgb, srcWidth, srcHeight, srcStride * 3, 0, 0, 0, 0, FASTCV_CHANNEL_G, FASTCV_RGB, planeG, srcStride);
    fcvChannelExtractu8(rgb, srcWidth, srcHeight, srcStride * 3, 0, 0, 0, 0, FASTCV_CHANNEL_B, FASTCV_RGB, planeB, srcStride);

    uint32_t dstWidth = IAIModel::MODEL_WIDTH;
    uint32_t dstHeight = IAIModel::MODEL_HEIGHT;
    uint32_t stride = 304; // multiple of 8, for better performance
    uint8_t *planeR1 = (uint8_t *)calloc(stride * dstHeight, sizeof(uint8_t));
    uint8_t *planeG1 = (uint8_t *)calloc(stride * dstHeight, sizeof(uint8_t));
    uint8_t *planeB1 = (uint8_t *)calloc(stride * dstHeight, sizeof(uint8_t));
    uint8_t *rgbScaled = (uint8_t *)calloc(stride * dstHeight * 3, sizeof(uint8_t));
    fcvScaleu8(planeR, srcWidth, srcHeight, srcStride, planeR1, dstWidth, dstHeight, stride,
               FASTCV_INTERPOLATION_TYPE_BILINEAR);
    fcvScaleu8(planeG, srcWidth, srcHeight, srcStride, planeG1, dstWidth, dstHeight, stride,
               FASTCV_INTERPOLATION_TYPE_BILINEAR);
    fcvScaleu8(planeB, srcWidth, srcHeight, srcStride, planeB1, dstWidth, dstHeight, stride,
               FASTCV_INTERPOLATION_TYPE_BILINEAR);

    fcvChannelCombine3Planesu8(planeB1, dstWidth, dstHeight, stride, planeG1, stride, planeR1, stride, rgbScaled, stride * 3);

    // 2. Normalize RGB values from [min, max] to [-1.0f, 1.0f]
    uint8_t minVal = 255;
    uint8_t maxVal = 0;
    // find min, max, row by row search
    for (uint r = 0; r < dstHeight; r++)
    {
        uint8_t min = *std::min_element(rgbScaled + r * stride * 3,
                                        rgbScaled + r * stride * 3 + dstWidth * 3);
        uint8_t max = *std::max_element(rgbScaled + r * stride * 3,
                                        rgbScaled + r * stride * 3 + dstWidth * 3);
        if (min < minVal)
        {
            minVal = min;
        }
        if (max > maxVal)
        {
            maxVal = max;
        }
    }
    uint8_t range = maxVal - minVal;
    float MAPPING_TBL[256]; // mapping table
    for (int i = minVal; i <= maxVal; i++)
    {
        MAPPING_TBL[i] = -1.0f + 2.0f * (i - minVal) / range;
    }
    // LOGI("minVal: %d, maxVal: %d, range: %d", minVal, maxVal, range);

    for (uint r = 0; r < dstHeight; r++)
    {
        for (uint c = 0; c < dstWidth; c++)
        {
            dst[r * dstWidth * 3 + c * 3] = MAPPING_TBL[rgbScaled[r * stride * 3 + c * 3]];
            dst[r * dstWidth * 3 + c * 3 + 1] = MAPPING_TBL[rgbScaled[r * stride * 3 + c * 3 + 1]];
            dst[r * dstWidth * 3 + c * 3 + 2] = MAPPING_TBL[rgbScaled[r * stride * 3 + c * 3 + 2]];
        }
    }

    fcvMemFree(planeR);
    fcvMemFree(planeG);
    fcvMemFree(planeB);
    fcvMemFree(planeR1);
    fcvMemFree(planeG1);
    fcvMemFree(planeB1);
    fcvMemFree(rgbScaled);
}
