//
// Created by jp on 26/06/2017.
//

#include <memory>

#include "Logger.h"
#include "LoggerAPI.h"
#include "AndroidLogger.h"

class Logger::impl
{
    std::unique_ptr<LoggerAPI> api_;

    // Limits the logger to outputting messages of a specified severity or higher.
    Severity severityFilter_;

public:
    impl() : severityFilter_(Severity::DEBUG)
    {
        // TODO: Determine the current platform before instantiating the appropriate logger.
        // For now, use the android logger.
        api_ = std::make_unique<AndroidLogger>();
    }

    ~impl(){}

    void log(const Logger::Severity s, const std::string& msg)
    {
        if (s >= severityFilter_)
        {
            switch (s)
            {
                case Logger::Severity::VERBOSE :
                    api_->logv(msg);
                break;
                case Logger::Severity::DEBUG :
                    api_->logd(msg);
                break;
                case Logger::Severity::INFO :
                    api_->logi(msg);
                break;
                case Logger::Severity::ERROR :
                    api_->loge(msg);
                break;
            }
        }
    }
};

Logger::Logger()
{
    pimpl_ = std::make_unique<impl>();
}

Logger::~Logger()
{
}

void Logger::log(const Logger::Severity s, const std::string& msg) const
{
    pimpl_->log(s, msg);
}

const Logger *Logger::getInstance()
{
    static Logger logger;
    return &logger;
}