#pragma once

#include <fstream>
#include <ctime>

namespace VNEngine {

	class Logger
	{
		std::ofstream* logs;		//logs file
		
		time_t rawTime;				//time in secs
		struct tm notRawTime;		//time struct

		std::string meta = "[INFO]";	//meta string like INFO ERROR etc

		Logger(std::string NameOfLogFile);
		~Logger();
		static Logger* s_pInstance;

	public:
		void setMeta(std::string newMeta);
		std::string getMeta();

		static Logger& Instance();
		static void Terminate();

		template <class type>				//template method for logging like into casual stream
		Logger& operator<<(type logIt) {
			time(&rawTime);					//getting time
			localtime_s(&notRawTime, &rawTime);
			*logs << "[" << notRawTime.tm_hour << ":" << notRawTime.tm_min << 		/*writting time*/ 
				":" << notRawTime.tm_sec << "]" << meta << " " << logIt << "\n";	//writting meta&logs
			return *this;
		}
	};
}

#define VN_LOGS_INFO(x)		if (Logger::Instance().getMeta() != "INFO")		Logger::Instance().setMeta("INFO"); Logger::Instance() << x;
#define VN_LOGS_ERROR(x)	if (Logger::Instance().getMeta() != "ERROR")\
							Logger::Instance().setMeta("ERROR"); Logger::Instance() << x;\
							Logger::Terminate();
#define VN_LOGS_WARNING(x)	if (Logger::Instance().getMeta() != "WARNING")	Logger::Instance().setMeta("WARNING"); Logger::Instance() << x;