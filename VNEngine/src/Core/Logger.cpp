#include "Logger.h"
#include "vnepch.h"

namespace VNEngine {

	Logger::Logger(std::string NameOfLogFile)
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

	static Logger& logger = *(new Logger("logs.txt"));
}