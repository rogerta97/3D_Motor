#pragma once

#include <string>
#include <map>
#include "Globals.h"

using namespace std;

string GetLastPathCommand(const char* full_path, bool want_termination);

string TillLastBar(const char* full_path);
string FindMeshes(const char* full_path);

int GetPathTermination(const char* path);

template <typename TYPE1, typename TYPE2>
vector<TYPE2> MapToVector(map<TYPE1, TYPE2> map);

template<typename TYPE1, typename TYPE2>
vector<TYPE2*> MapToVector(map<TYPE1, TYPE2*> candidate_list)
{
	vector<TYPE2*> to_return; 

	for (map<TYPE1, TYPE2*>::const_iterator it = candidate_list.begin(); it != candidate_list.end(); ++it)
	{
		to_return.push_back(it->second);
	}

	return to_return;
}
