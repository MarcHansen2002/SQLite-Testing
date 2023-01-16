#pragma once

#include <string>

/*
Send messages to the debug output window
Minimum of one string, but you can send two.
*/
void DebugPrint(const std::string& mssg1, const std::string& mssg2 = "");
