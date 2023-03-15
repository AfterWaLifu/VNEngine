#include "Logger.h"
#include "vnepch.h"

namespace VNEngine {

	Logger* Logger::s_pInstance = nullptr;

	Logger::Logger(std::string NameOfLogFile)
		: rawTime(0), notRawTime({})
	{
		logs = new std::ofstream(NameOfLogFile);
		if (!logs) exit(1);
	}

	Logger::~Logger()
	{
		if (logs) {
			logs->close();
		}
	}

	void Logger::setMeta(std::string newMeta)
	{
		meta = "[" + newMeta + "]";
	}

	std::string Logger::getMeta()
	{
		return meta;
	}

	Logger& Logger::Instance()
	{
		return s_pInstance ? *s_pInstance : *(s_pInstance = new Logger("logs.txt"));
	}
	void Logger::Terminate()
	{
		Logger* logs = &Logger::Instance();
		logs->logs->close();
	}
}