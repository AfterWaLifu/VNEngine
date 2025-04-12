#include <VNEngine/Core/Logger.hpp>

namespace VNEngine {

std::shared_ptr<Logger> Logger::instance_ = nullptr;

Logger::Logger(std::string NameOfLogFile) : logFilePath_(std::move(NameOfLogFile)), rawTime(0) {}

void Logger::setTag(std::string newMeta) { tag_ = "[" + newMeta + "]"; }

std::string Logger::tag() { return tag_; }

Logger& Logger::instance() {
    if (!instance_) {
        instance_ = std::shared_ptr<Logger>(new Logger("logs.txt"));
    }
    return *instance_;
}

} // namespace VNEngine