#include <string>
#include <ctime>

//Singleton class for a unique global object.
template<class T>
struct Singleton
{
  static T& getInstance()
  {
    static T instance;
    return instance;
  }
  Singleton() = delete;
  Singleton(Singleton const&)= delete;
  void operator=(Singleton const&)= delete;
};
//Converts time to a string
std::string put_time(const std::time_t *time, std::string format);

