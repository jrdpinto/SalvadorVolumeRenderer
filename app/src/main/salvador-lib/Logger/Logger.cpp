//
// Created by jp on 26/06/2017.
//

#include <memory>

#include "Logger.h"
#include "LoggerAPI.h"
#include "AndroidLogger.h"

enum class Severity : char
{
    VERBOSE = 0,
    DEBUG = 1,
    INFO = 2,
    ERROR = 3
};

class LoggerImpl
{
public:

    LoggerImpl() : severityFilter_(Severity::DEBUG)
    {
        // TODO: Determine the current platform before instantiating the appropriate logger.
        // For now, use the android logger.
        api_ = std::make_unique<AndroidLogger>();
    }

    ~LoggerImpl(){}

    void log(const Severity s, const std::string& msg) const
    {
        if (s >= severityFilter_)
        {
            switch (s)
            {
                case Severity::VERBOSE :
                    api_->logv(msg);
                break;
                case Severity::DEBUG :
                    api_->logd(msg);
                break;
                case Severity::INFO :
                    api_->logi(msg);
                break;
                case Severity::ERROR :
                    api_->loge(msg);
                break;
            }
        }
    }

    static const LoggerImpl* getInstance()
    {
        static LoggerImpl logger;
        return &logger;
    }

private:
    std::unique_ptr<LoggerAPI> api_;

    // Limits the logger to outputting messages of a specified severity or higher.
    Severity severityFilter_;
};

void Logger::logd(const std::string &msg)
{
    LoggerImpl::getInstance()->log(Severity::DEBUG, msg);
}

void Logger::loge(const std::string &msg)
{
    LoggerImpl::getInstance()->log(Severity::ERROR, msg);
}

void Logger::logv(const std::string &msg)
{
    LoggerImpl::getInstance()->log(Severity::VERBOSE, msg);
}

void Logger::logi(const std::string &msg)
{
    LoggerImpl::getInstance()->log(Severity::INFO, msg);
}
