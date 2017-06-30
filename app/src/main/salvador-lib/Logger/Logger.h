//
// Created by jp on 26/06/2017.
// Description: Platform agnostic logger that displays messages in a log/console
// and filters them based on the current build type and severity. By default, in
// debug mode all messages with a severity of 'debug' or higher are displayed.
// In release mode, only error messages are displayed.
//

#ifndef VOLUMERENDERER_LOGGER_H
#define VOLUMERENDERER_LOGGER_H

#include <string>

class Logger
{
    Logger();
    ~Logger();

public:
    // Logs a debug message
    static void logd(const std::string &msg);

    // Logs an error message
    static void loge(const std::string &msg);

    // Logs a message that is only displayed in 'verbose' mode
    static void logv(const std::string &msg);

    // Logs a message that is only displayed in 'info' mode
    static void logi(const std::string &msg);
};

#endif //VOLUMERENDERER_LOGGER_H