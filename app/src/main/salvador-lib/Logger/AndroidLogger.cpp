//
// Created by jp on 26/06/2017.
//

#include "AndroidLogger.h"
#include <android/log.h>

const char* TAG = "SALVADOR-LIB";

AndroidLogger::AndroidLogger()
{
}

AndroidLogger::~AndroidLogger()
{
}

void AndroidLogger::loge(const std::string& msg)
{
    __android_log_print(ANDROID_LOG_ERROR, TAG, "%s", msg.c_str());
}

void AndroidLogger::logd(const std::string& msg)
{
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "%s", msg.c_str());
}

void AndroidLogger::logi(const std::string& msg)
{
    __android_log_print(ANDROID_LOG_INFO, TAG, "%s", msg.c_str());
}

void AndroidLogger::logv(const std::string& msg)
{
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "%s", msg.c_str());
}
