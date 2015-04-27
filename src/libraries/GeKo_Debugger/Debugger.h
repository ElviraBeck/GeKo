#pragma once
#include "GeKo_Graphics/Defs.h"
#include <fstream>

/** To use the debugger in a main-method you have to initialize the debugger-object before creating the window.
  The debug-callback has to be set after calling "glewInit();". 
  You can define your own error messages using void DebugMessageInsert(enum source, enum type, uint id, enum severity, sizei length, const char* buf).
  For further information have a look at https://www.opengl.org/registry/specs/KHR/debug.txt */
class Debugger{
public:
  /// Standard constructor
  /** Sets window-context (esp. the debug-context) and opens the output-stream for the error-messages ("OGLdebugLog.txt")*/
  Debugger();
  ~Debugger();
  /// A method to set the default debug-callback defined in "Debugger.cpp"
  /***/
  void setDebugCallback();
  /// A method to set a non-default debug-callback 
  /***/
  void setDebugCallback(GLDEBUGPROC debugCallback);
};
