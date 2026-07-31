/**
 * Logger utility for recording user actions
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
private:
    std::string logDir;
    std::string logFile;

    void initializeLogFile();

public:
    Logger(const std::string& dir = "outputs");
    ~Logger();

    void log(const std::string& action);
    std::string getLogPath() const;
};

#endif // LOGGER_H
