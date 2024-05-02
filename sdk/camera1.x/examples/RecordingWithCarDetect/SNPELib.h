/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2021 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Android tests/FrontDMSLib                                                                   *
 * BINARY NAME :                                                                                              *
 * FILE NAME   :  SNPELIB.cpp                                                                                 *
 * CREATED BY  :  Sang Trinh <sang.trinh@mic.com.tw>                                                          *
 * CREATED DATE:  02/25/21 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  snpe using                                                                                  *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef SNPELIB_H
#define SNPELIB_H

#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iterator>
#include <unordered_map>
#include <algorithm>

// SNPE includes
#include <DlSystem/DlEnums.hpp>
#include <DlSystem/DlVersion.hpp>
#include <DlSystem/UserBufferMap.hpp>
#include <DlSystem/RuntimeList.hpp>
//#include <DlSystem/UDLFunc.hpp>
#include <DlSystem/IUserBuffer.hpp>
#include <DlSystem/PlatformConfig.hpp>
#include <DlSystem/IUserBufferFactory.hpp>
#include <DlSystem/StringList.hpp>
#include <DlSystem/TensorShape.hpp>
#include <DlContainer/IDlContainer.hpp>
#include <DlSystem/ITensor.hpp>
#include <DlSystem/TensorMap.hpp>
#include <DlSystem/ITensorFactory.hpp>
#include <DlSystem/RuntimeList.hpp>  // added for v2.13
#include <SNPE/SNPE.hpp>
#include <SNPE/SNPEFactory.hpp>
#include <SNPE/SNPEBuilder.hpp>
#include <DiagLog/IDiagLog.hpp>
#include <DlContainer/IDlContainer.hpp>
#include <DiagLog/IDiagLog.hpp>

#include "IAIModel.h"

static const int FAILURE = -1;
static const int SUCCEED_NO_OBJ = 0;
static const int SUCCEED = 1;

class SNPELib : public IAIModel
{
public:
    int process(void *data, uint32_t width, uint32_t height, Rect &result);
    int processYolo(void *data, uint32_t width, uint32_t height, Rect &result);
    int init();
    int deinit();
    int initSNPE();

private:
    bool mExecStatus = false;
    float mConfThreshold = 0.5;
    int mModelInputH;
    int mModelInputW;
    int mOrigImageH = 480;
    int mOrigImageW = 640;
    std::string mDlcPath;
    zdl::DlSystem::StringList mOutputLayerNames;
    std::unique_ptr<zdl::SNPE::SNPE> mSNPE;
    zdl::DlSystem::Runtime_t mRuntime;
    zdl::DlSystem::RuntimeList mRuntimeList;  // added for v2.14.2
    std::unique_ptr<zdl::DlContainer::IDlContainer> mContainer;
    bool logStarted = false; // added for v2.14.2

    void loadContainerFromFile(const std::string &containerPath);
};

#endif // SNPELIB_H
