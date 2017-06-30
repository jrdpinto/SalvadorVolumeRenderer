//
// Created by jp on 26/06/2017.
//

#ifndef VOLUMERENDERER_LOGGER_H
#define VOLUMERENDERER_LOGGER_H

#include <string>

class Logger
{
public:
    enum class Severity : char
    {
        VERBOSE = 0,
        DEBUG = 1,
        INFO = 2,
        ERROR = 3
    };

    void log(const Severity s, const std::string& msg) const;
    static const Logger* getInstance();

    // Helper functions that are less verbose than 'getInstance()->log()'
    static void logd(const std::string& msg);
    static void loge(const std::string& msg);
    static void logv(const std::string& msg);
    static void logi(const std::string& msg);

private:
    Logger();
    ~Logger();

    class impl;
    std::unique_ptr<impl> pimpl_;
};

#endif //VOLUMERENDERER_LOGGER_H