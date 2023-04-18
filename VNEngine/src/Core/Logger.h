#pragma once

#include <fstream>
#include <ctime>

namespace VNEngine {

	/**
	* @brief	Logger class, used as singletone
	* Logs everything you need (or not) into logs.txt file.
	* Writes also timestamps and labels for every line
	*/
	class Logger
	{
		std::ofstream* logs;		/// Logs file
		
		time_t rawTime;				/// Time in secs
		struct tm notRawTime;		/// Default C time struct

		std::string meta = "[INFO]";	/// Meta string like INFO, WARNING, ERROR or whtwr u want

		Logger(std::string NameOfLogFile);	/// Private constructor, don't touch that
		~Logger();
		static Logger* s_pInstance;			/// The only instance of the logger (its ptr)

	public:
		/**
		* @brief	Sets 'meta' string
		* @param	newMeta	New meta, that will be written into logs
		*/
		void setMeta(std::string newMeta);
		std::string getMeta();	///	Returns current 'meta' state

		static Logger& Instance();	///	The only way to get an access to the logger
		static void Terminate();	///	Function, closing logger (and file)

		/**
		* @brief	Template method for logging like into casual stream
		* @param	logIt	Whatever it is - log it, please
		*/
		template <class type>
		Logger& operator<<(type logIt) {
			time(&rawTime);					//getting time
			localtime_s(&notRawTime, &rawTime);
			*logs << "[" << notRawTime.tm_hour << ":" << notRawTime.tm_min << 		/*writting time*/ 
				":" << notRawTime.tm_sec << "]" << meta << " " << logIt << "\n";	//writting meta&logs
			return *this;
		}
	};
}


//	This things are used to log smth in easier way

#define VN_LOGS_INFO(x)		if (Logger::Instance().getMeta() != "INFO")		Logger::Instance().setMeta("INFO"); Logger::Instance() << x;
#define VN_LOGS_ERROR(x)	if (Logger::Instance().getMeta() != "ERROR")\
							Logger::Instance().setMeta("ERROR"); Logger::Instance() << x;\
							Logger::Terminate();
#define VN_LOGS_WARNING(x)	if (Logger::Instance().getMeta() != "WARNING")	Logger::Instance().setMeta("WARNING"); Logger::Instance() << x;