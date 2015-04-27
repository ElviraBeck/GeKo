#include "Log.h"
std::string logTypeToString(LogType type) {
  if(type == LogType::Info){
    return "[Info]";
  }
  if(type == LogType::Warning){
    return "[Warning]";
  }
  if(type == LogType::Error){
    return "[Error]";
  }
  if(type == LogType::Critical){
    return "[Critical]";
  }
  return "";
}
Log::Log(){
  using namespace std::chrono;
  auto time = system_clock::to_time_t(system_clock::now());
  auto date = put_time(&time , "%F");
  std::string path = "GeKo-"+date + ".log";
  //FIXME: Need default log location.
  outputFile.open(path);
}
Log::~Log(){
  outputFile.close();
}
void Log::addMessage(LogType type, std::string msg){
  auto time = std::chrono::system_clock::now();
  auto lm = LogMessage{std::chrono::system_clock::to_time_t(time)
    ,type
      ,msg};
  logs.push_back(lm);
  //FIXME: Blocks the current thread
  writeLog(outputFile,lm);
}
auto writeLogToConsole = [](const LogMessage &lm){
  writeLog(std::cout,lm);
};
void Log::print(){
  for(const LogMessage &lm : logs ){
    writeLogToConsole(lm);
  }
}
