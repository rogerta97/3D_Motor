#pragma once
#include <string>

const char* GetLastPathCommand(const char* full_path)
{
	return ""; 
}

void GetPathTermination(const char* path, char* termination)
{
	std::string full_path(path); 
	std::string term = full_path.substr(full_path.size() - 3, 3);
	strcpy(termination, term.c_str()); 
}		

