#pragma once

#include <string>
#include "Globals.h"

using namespace std;

string GetLastPathCommand(const char* full_path, bool want_termination);

string TillLastBar(const char* full_path);

int GetPathTermination(const char* path);

