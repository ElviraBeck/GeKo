#include "Log.h"

//TODO: Only log in debug mode

// Just logs a message with a LogType. LogType can be Info, Warning, Error or Critical
#define LOG(type,message) Singleton<Log>::getInstance().addMessage(type,message);
#define LOG_INFO(message) LOG(LogType::Info,message)
#define LOG_WARNING(message) LOG(LogType::Warning,message)
#define LOG_ERROR(message) LOG(LogType::Error,message)
// Same as LOG but with a precondition.
#define ASSERT(cond,type,message) if(!(cond)){LOG(type,message)}
// Same as ASSERT but will abort the application.
#define CRITICAL_ASSERT(cond,message) if(!(cond)){LOG(LogType::Critical,message);std::exit(EXIT_FAILURE);}
// Prints the whole log to std::cout.
#define PRINT_LOG() Singleton<Log>::getInstance().print();
