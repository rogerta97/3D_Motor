#pragma once

#include "Functions.h"

using namespace std;

string GetLastPathCommand(const char* full_path, bool want_termination)
{
	string full_path_str(full_path);
	string final_str;

	DeleteEndBars(full_path_str);

	if (full_path_str.find('\\') != 0)
	{
		uint cut = full_path_str.find_last_of('\\');

		if(!want_termination)
			final_str = full_path_str.substr(cut + 1, full_path_str.length() - cut - 5);
		else
			final_str = full_path_str.substr(cut + 1, full_path_str.length() - cut);
	}

	return final_str;
}

void DeleteLastPathCommand(string& full_path)
{
	string final_str;

	DeleteEndBars(full_path);

	if (full_path.find('\\') != 0)
	{
		uint cut = full_path.find_last_of('\\');

		uint to_kill = full_path.size() - cut - 1; 
		
		full_path = full_path.substr(0, full_path.size() - to_kill);
		
	}
}

string TillLastBar(const char* full_path)
{
	string full_path_str(full_path);
	uint cut = full_path_str.find_last_of('\\');

	std::string final_str = full_path_str.substr(0, cut + 1);

	return final_str;
}
string FindMeshes(const char* full_path)
{
	string full_path_str(full_path);
	uint cut = full_path_str.find_last_of("Meshes");

	std::string final_str = full_path_str.substr(0, cut + 1);

	return final_str;
}

void DeleteEndBars(string & full_path)
{
	if (full_path.at(full_path.size() - 1) == '\\')
		full_path.erase(full_path.size() - 1);
}

int GetPathTermination(const char* path)
{
	string full_path(path);
	
	DeleteEndBars(full_path); 

	string term = full_path.substr(full_path.size() - 3, 3);

	if (term == "fbx" || term == "FBX")
		return 0;

	else if (term == "png" || term == "PNG")
		return 1;

	else if (term == "tga" || term == "TGA")
		return 2;

	else
		return 3;
}