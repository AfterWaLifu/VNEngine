#pragma once

#include <fstream>
#include <ctime>

namespace VNEngine {

	static class Logger
	{
		std::ofstream* logs;		//logs file
		
		time_t rawTime;				//time in secs
		struct tm notRawTime;		//time struct

		std::string meta = "[INFO]";	//meta string like INFO ERROR etc

	public:
		Logger(std::string NameOfLogFile);
		~Logger();

		void setMeta(std::string newMeta);
		std::string getMeta();

		template <class type>				//template method for logging like into casual stream
		Logger& operator<<(type logIt) {
			time(&rawTime);					//getting time
			localtime_s(&notRawTime, &rawTime);
			*logs << "[" << notRawTime.tm_hour << ":" << notRawTime.tm_min << 		/*writting time*/ 
				":" << notRawTime.tm_sec << "]" << meta << " " << logIt << "\n";	//writting meta&logs
			return *this;
		}
	} logs("logs.txt");

}

