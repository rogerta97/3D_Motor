#pragma once

#include "Functions.h"

using namespace std;

string GetLastPathCommand(const char* full_path, bool want_termination)
{
	string full_path_str(full_path);
	string final_str;

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

string TillLastBar(const char* full_path)
{
	string full_path_str(full_path);
	uint cut = full_path_str.find_last_of('\\');

	std::string final_str = full_path_str.substr(0, cut + 1);

	return final_str;
}

int GetPathTermination(const char* path)
{

	string full_path(path);
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