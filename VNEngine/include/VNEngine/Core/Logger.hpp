#pragma once

#include <ctime>
#include <fstream>

namespace VNEngine {

class Logger {
public:
    void setTag(std::string newMeta);
    std::string tag();

    static Logger& instance();

    template <class type>
    Logger& operator<<(type logIt) {
        auto logFile = std::ofstream(logFilePath_);
        if (!logFile) {
            return *this;
        }

        time(&rawTime);
        tm notRawTime;
        localtime_s(&notRawTime, &rawTime);

        logFile << "[" << notRawTime.tm_hour << ":" << notRawTime.tm_min <<  /*writting time*/
            ":" << notRawTime.tm_sec << "]" << tag_ << " " << logIt << "\n"; // writting meta&logs

        logFile.close();

        return *this;
    }

private:
    Logger(std::string NameOfLogFile);

    std::string logFilePath_;

    time_t rawTime;

    std::string tag_ = "[INFO]";

    static std::shared_ptr<Logger> instance_;
};

} // namespace VNEngine

#define VN_LOGS_INFO(x)                                                                                                \
    if (Logger::instance().tag() != "INFO")                                                                            \
        Logger::instance().setTag("INFO");                                                                             \
    Logger::instance() << x;
#define VN_LOGS_ERROR(x)                                                                                               \
    if (Logger::instance().tag() != "ERROR")                                                                           \
        Logger::instance().setTag("ERROR");                                                                            \
    Logger::instance() << x;
#define VN_LOGS_WARNING(x)                                                                                             \
    if (Logger::instance().tag() != "WARNING")                                                                         \
        Logger::instance().setTag("WARNING");                                                                          \
    Logger::instance() << x;