#include "Utility.hpp"
std::string put_time(const std::time_t *time, std::string format){
  char mbstr[100];
  std::strftime(mbstr,sizeof(mbstr), format.c_str(), std::localtime(time));
  return std::string(mbstr);
}
