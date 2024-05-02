/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2021 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Android tests/FrontDMSLib                                                                   *
 * BINARY NAME :                                                                                              *
 * FILE NAME   :  SNPELib.cpp                                                                                 *
 * CREATED BY  :  Sang Trinh <sang.trinh@mic.com.tw>                                                          *
 * CREATED DATE:  02/25/21 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  snpe using                                                                                  *
 *------------------------------------------------------------------------------------------------------------*/

// Standard include
#include <stdio.h>
#include <math.h>
#include <map>
// POSIX include
#include <sys/stat.h>
// Local include
#include "SNPELib.h"
#include <stdlib.h>
#include "Log.h"
#include <string>


int SNPELib::init()
{
    mModelInputH = 300;
    mModelInputW = 300;
    mDlcPath = (char *)"./mobilenet_ssd.dlc";
    mOutputLayerNames.append("Postprocessor/BatchMultiClassNonMaxSuppression");
    mOutputLayerNames.append("add");
    
    // set SNPE log path and start log  (available log level:  FATAL, ERROR, WARN, INFO, and VERBOSE)
    const std::string logPath = std::string("./log");
    const zdl::DlSystem::LogLevel_t logLevel = zdl::DlSystem::LogLevel_t::LOG_ERROR;
    logStarted = zdl::SNPE::SNPEFactory::initializeLogging(logLevel, logPath);
    LOGI("SNPE library version:%s", zdl::SNPE::SNPEFactory::getLibraryVersion().toString().c_str());

    // set runtime order list
    mRuntimeList.clear();
    mRuntime = zdl::DlSystem::Runtime_t::DSP;
    mRuntimeList.add(mRuntime);
    mRuntime = zdl::DlSystem::Runtime_t::GPU;
    mRuntimeList.add(mRuntime);
    mRuntime = zdl::DlSystem::Runtime_t::CPU;
    mRuntimeList.add(mRuntime);
    /*
    if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(mRuntime))
    {
        LOGW("Selected runtime not present. Falling back to GPU.");
        mRuntime = zdl::DlSystem::Runtime_t::GPU;
        if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(mRuntime))
        {
            LOGW("Selected runtime not present. Falling back to CPU.");
            mRuntime = zdl::DlSystem::Runtime_t::CPU;
        }
    }
    */
    if (initSNPE() == SUCCEED)
    {
        LOGI("Init AIModel success");
        return 0;
    }
    return -1;
}

int SNPELib::initSNPE()
{
    std::ifstream dlcFile(mDlcPath);

    if (!dlcFile)
    {
        LOGE("dlc file not valid.");
        return FAILURE;
    }

    mContainer = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(mDlcPath.c_str()));

    if (mContainer == nullptr)
    {
        LOGE("Error while opening the container file.");
        LOGE("%s", zdl::DlSystem::getLastErrorString());
        return FAILURE;
    }

    // deprecated.
    // zdl::DlSystem::UDLFactoryFunc udlFunc = UdlExample::MyUDLFactory;
    // zdl::DlSystem::UDLBundle udlBundle;
    // udlBundle.cookie = (void *)0xdeadbeaf;
    // udlBundle.func = udlFunc;

    zdl::SNPE::SNPEBuilder snpeBuilder(mContainer.get());
    zdl::DlSystem::PerformanceProfile_t
    profile = zdl::DlSystem::PerformanceProfile_t::HIGH_PERFORMANCE;
    zdl::DlSystem::PlatformConfig platformConfig;

    mSNPE = snpeBuilder.setOutputLayers(mOutputLayerNames)
                .setRuntimeProcessorOrder(mRuntimeList)    // give runtime order list instead of give specified one
                //.setCPUFallbackMode(true)    // deprecated.
                .setUseUserSuppliedBuffers(false)
                .setPerformanceProfile(profile)
                .setPlatformConfig(platformConfig)
                .setInitCacheMode(false)
                .build();

    if (mSNPE == nullptr)
    {
        LOGE("Error while building SNPE object.");
        LOGE("%s", zdl::DlSystem::getLastErrorString());   // get last error string when needed.
        return FAILURE;
    }
    LOGI("Model version: %s", mSNPE->getModelVersion().c_str());

    LOGI("Init SNPE success");
    return SUCCEED;
}

int SNPELib::deinit()
{
    if (mSNPE != nullptr)
    {
        mSNPE.reset();
    }

    if (mContainer.get() != nullptr)
    {
        mContainer.reset();
    }

    // stop SNPE logging
    if(logStarted)
    {
        zdl::SNPE::SNPEFactory::terminateLogging();
        logStarted = false;
    } 

    return SUCCEED;
}

int SNPELib::process(void *data, uint32_t width, uint32_t height, Rect &result)
{
    const auto strList = mSNPE->getInputTensorNames();
    auto inputDims = mSNPE->getInputDimensions((*strList).at(0));

    zdl::DlSystem::TensorMap outputTensorMap;
    zdl::DlSystem::TensorMap inputTensorMap;
    std::unique_ptr<zdl::DlSystem::ITensor> inputTensor = zdl::SNPE::SNPEFactory::getTensorFactory().createTensor(inputDims);
    inputTensorMap.add((*strList).at(0), inputTensor.get());
    
    // float* dataFloat = (float*) data;
    int memSize = mModelInputH * mModelInputW * 3;
    std::copy((float *)data, (float *)data + memSize, inputTensor->begin());
    
    // cv::Mat input;
    // cv::Mat resizeMat;
    // cv::resize(src, resizeMat, cv::Size(mModelInputW, mModelInputH));

    // cv::cvtColor(resizeMat, input, cv::COLOR_BGR2RGB);
    // cv::Mat input_norm(mModelInputH, mModelInputW, CV_32FC3, inputTensor.get()->begin().dataPointer());
    // input.convertTo(input, CV_32F);
    // cv::normalize(input, input_norm, -1.0f, 1.0f, cv::NORM_MINMAX);

    if (!inputTensor)
    {
        LOGE("Error while creating input tensor.");
        return FAILURE;
    }
    zdl::DlSystem::ITensor *outBoxes = nullptr;
    zdl::DlSystem::ITensor *outScores = nullptr;
    zdl::DlSystem::ITensor *outClasses = nullptr;
    mExecStatus = mSNPE->execute(inputTensorMap, outputTensorMap);
    // LOGI("after execute()");
    
    if (mExecStatus != true)
    {
        LOGE("Error while executing SNPE object.");
        return FAILURE;
    }

    //outBoxes = outputTensorMap.getTensor("Postprocessor/BatchMultiClassNonMaxSuppression_boxes");
    outBoxes = outputTensorMap.getTensor("detection_boxes:0");
    // LOGI("after getTensor() outBoxes");
    //outScores = outputTensorMap.getTensor("Postprocessor/BatchMultiClassNonMaxSuppression_scores");
    outScores = outputTensorMap.getTensor("detection_scores:0");
    // LOGI("after getTensor() outScores");
    outClasses = outputTensorMap.getTensor("detection_classes:0");
    // LOGI("after getTensor() outClasses");
  
    
    zdl::DlSystem::TensorShape boxesShape = outBoxes->getShape();
    // LOGI("after getShape() outBoxes");

    // int boxesRank = boxesShape.rank();
    int boxesBatch = boxesShape[0];
    int boxesChannel = boxesShape[1];
    // int boxesLen = boxesShape[2];
    // float x_scale = mOrigImageW / (float)mModelInputW;
    // float y_scale = mOrigImageH / (float)mModelInputH;
    const float *score_data = &*(outScores->cbegin());
    const float *class_data = &*(outClasses->cbegin());
    const float *box_data = &*(outBoxes->cbegin());
    // LOGI("boxesBatch %d", boxesBatch);
    // LOGI("boxesChannel %d", boxesChannel);
    // LOGI("boxesLen %d", boxesLen);

    std::vector<Rect> rects;
    for (int i = 0; i < boxesBatch; ++i)
    {
        for (int j = 0; j < boxesChannel; ++j)
        {
            float confidence = *score_data++;
            float label = *class_data++;
            float yMin = *box_data++;
            float xMin = *box_data++;
            float yMax = *box_data++;
            float xMax = *box_data++;
            // LOGI("confidence %f", confidence);
            if ((confidence > mConfThreshold) && label == 3.0f) // 3 = car coco
            {
                LOGI("label %f, confidence: %f", label, confidence);
                float xStart = xMin * mModelInputW;
                float yStart = yMin * mModelInputH;
                float xEnd = xMax * mModelInputW;
                float yEnd = yMax * mModelInputH;
                rects.push_back(Rect(yStart, xStart, yEnd, xEnd));
            }
        }
    }

    if (rects.size() == 0)
    {
        return SUCCEED_NO_OBJ;
    }

    Rect biggestRect(0, 0, 0, 0);
    int biggestArea = 0;
    for (Rect &rect : rects)
    {
        int width = rect.getWidth();
        int height = rect.getHeight();
        int area = width * height;
        if (area > biggestArea)
        {
            biggestArea = area;
            biggestRect = rect;
        }
    }

    result = biggestRect;
    // LOGI("process success");
    return SUCCEED;
}
