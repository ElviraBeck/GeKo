#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include "Utility.hpp"

enum class LogType{
  Info,
  Warning,
  Error,
  Critical
};
// Converts a LogType to a string.
std::string logTypeToString(LogType type);

struct LogMessage{
  std::time_t time;
  LogType type;
  std::string message;
};

//Writes the log message to a generic stream
template<class STREAM>
void writeLog(STREAM &stream, const LogMessage &lm){
  stream << put_time(&lm.time ,"%c")
    << ": " 
    <<  logTypeToString(lm.type) 
    << ": " << lm.message << std::endl;
}

//Handles the logging, should not be used directly
struct Log{
  std::vector<LogMessage> logs;
  std::ofstream outputFile;
  Log();
  ~Log();
  void addMessage(LogType type, std::string msg);
  void print();
};


