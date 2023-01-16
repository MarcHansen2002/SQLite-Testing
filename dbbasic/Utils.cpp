#include <windows.h>

#include "Utils.h"

using namespace std;

void DebugPrint(const string& mssg1, const string& mssg2)
{
	OutputDebugString(mssg1.c_str());
	OutputDebugString("\n");
	if (!mssg2.empty()) {
		OutputDebugString(mssg2.c_str());
		OutputDebugString("\n");
	}
}