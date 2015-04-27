#include "../../libraries/Logging/Logging.h"

int main(int argc, char * argv[]){
  /* Don't use the Log class directly,
   * only use the LOG macros that are
   * used in the file.
   *
   * The log file will be saved in the
   * same path as the binary.
  */

  LOG_INFO("This is a Info");
  LOG_WARNING("This is a Warning");
  LOG_ERROR("This is a Error.");
  LOG(LogType::Warning,"This is also a Warning");

  ASSERT(false,LogType::Warning,"Log Assert");
  PRINT_LOG();
  CRITICAL_ASSERT(false,"Critical assert, will exit the application");
}
