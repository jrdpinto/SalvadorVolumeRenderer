//
// Created by jp on 26/06/2017.
// Description: Platform agnostic API for logging functionality.
//

#ifndef VOLUMERENDERER_LOGGERAPI_H
#define VOLUMERENDERER_LOGGERAPI_H

#include <string>

class LoggerAPI
{
public:
    LoggerAPI() {}
    virtual ~LoggerAPI() {}

    virtual void loge(const std::string& msg) = 0;
    virtual void logd(const std::string& msg) = 0;
    virtual void logi(const std::string& msg) = 0;
    virtual void logv(const std::string& msg) = 0;
};

#endif //VOLUMERENDERER_LOGGERAPI_H
