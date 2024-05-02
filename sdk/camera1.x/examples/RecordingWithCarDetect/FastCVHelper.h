/*
 * Copyright (c) 2023 MiTAC International Corp.
 * -----
 * File: FastCVHelper.h
 * Project: aiTest
 * Created Date: 14 Mar 2023
 * Author: John
 * -----
 * Last Modified: Tue Mar 14 2023
 * Modified By: John
 * -----
 * Description: FastCV helper
 */

#ifndef FASTCVHELPER_H_
#define FASTCVHELPER_H_

class FastCVHelper
{
public:
    static void init();
    static void shutdown();
    static void *calloc(size_t num, size_t size);
    static void free(void *ptr);
    /**
     * Convert image from NV12 (YUV 420, UV interleaved) to RGB color format.
     * Caller must allocate dst buffer with size of at least srcWidth * srcWidth * 3 bytes.
     * Y and UV planes must share the same stride which equals srcWidth.
     */
    static void NV12toRGB(const uint8_t *planeY,
                          const uint8_t *planeUV,
                          uint32_t srcWidth,
                          uint32_t srcHeight,
                          uint8_t *dst);

    /**
     * Convert image from RGB to BGR and scale to 300 * 300 for SNPE processing.
     * Caller must allocate dst buffer with size of at least srcWidth * srcWidth * 3 * 4 bytes.
     */
    static void RGBtoBGRSNPE(const uint8_t *rgb,
                             uint32_t srcWidth,
                             uint32_t srcHeight,
                             float *dst);
private:
    FastCVHelper() = default;
    FastCVHelper(FastCVHelper &&) = default;
    FastCVHelper(const FastCVHelper &) = default;
    FastCVHelper &operator=(FastCVHelper &&) = default;
    FastCVHelper &operator=(const FastCVHelper &) = default;
};

#endif /* FASTCVHELPER_H_ */