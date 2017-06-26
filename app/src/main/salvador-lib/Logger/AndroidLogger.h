//
// Created by jp on 26/06/2017.
//

#ifndef VOLUMERENDERER_ANDROIDLOGGER_H
#define VOLUMERENDERER_ANDROIDLOGGER_H

#include "LoggerAPI.h"

class AndroidLogger : public LoggerAPI
{
public:
    AndroidLogger();
    ~AndroidLogger();

    void loge(const std::string& msg) override;
    void logd(const std::string& msg) override;
    void logi(const std::string& msg) override;
    void logv(const std::string& msg) override;
};


#endif //VOLUMERENDERER_ANDROIDLOGGER_H
