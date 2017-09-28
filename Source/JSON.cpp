#include "JSON.h"
#include "Globals.h"

JSON::JSON(bool enabled) : Module(enabled)
 {
	}

JSON::~JSON()
{
}

bool JSON::Awake()
 {
	bool ret = true;
	
		LOG("JSON loading...");
	return ret;
	}

json_file* JSON::LoadJSONFile(const char * path)
 {
	json_file* ret = nullptr;

	bool exists = false;
	for (std::list<json_file*>::iterator it = j_files.begin(); it != j_files.end(); it++)
	{
		if (strcmp(path, (*it)->GetPath()))
		{
			ret = (*it);
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		JSON_Value *user_data = json_parse_file(path);
		JSON_Object *root_object = json_value_get_object(user_data);

		if (user_data == nullptr)
		{
			LOG("Error loading %s", path);
		}
		else
		{
			LOG("Succes loading %s", path);

			json_file* new_doc = new json_file(user_data, root_object, path);
			j_files.push_back(new_doc);

			ret = new_doc;
		}
	}

	return ret;
	}



bool JSON::CleanUp()
 {
	bool ret = true;
	
		LOG("Cleaning JSON ");
	
		for (std::list<json_file*>::iterator item = j_files.begin(); item != j_files.end();)
		 {
			(*item)->CleanUp();
			delete (*item);

			item = j_files.erase(item);
		}
	
	return ret;
}

const char * json_file::GetPath()
{
	return path.c_str();
}

json_file::json_file(JSON_Value * value, JSON_Object * object, const char * path)
{
	this->value = value;
	this->object = object;
	this->path = path;
}
json_file::~json_file()
{
}
void json_file::SetString(const char * set, const char * str)
{
	json_object_dotset_string(object, set, str);
}

void json_file::SetBool(const char * set, bool bo)
{
	json_object_dotset_boolean(object, set, bo);
}

void json_file::SetNumber(const char * set, double nu)
{
	json_object_dotset_number(object, set, nu);
}
const char * json_file::GetString(const char * str, const char* defaul)
{
	const char* ret = defaul;

	if (FindValue(str, json_value_type::JSONString))
		ret = json_object_dotget_string(object, str);

	return ret;
}

bool json_file::GetBool(const char * str, bool defaul)
{
	bool ret = defaul;

	if (FindValue(str, json_value_type::JSONBoolean))
		ret = json_object_dotget_boolean(object, str);

	return ret;
}

double json_file::GetNumber(const char * str, double defaul)
{
	double ret = defaul;

	if (FindValue(str, json_value_type::JSONNumber))
		ret = json_object_dotget_number(object, str);

	return ret;
}

void json_file::Save()
{
	json_serialize_to_file_pretty(value, path.c_str());
}

void json_file::CleanUp()
{
	json_value_free(value);
}
bool json_file::FindValue(const char * str, json_value_type type)
{
	bool ret = false;

	JSON_Value* val = json_object_dotget_value(object, str);

	if (val != nullptr && json_value_get_type(val) == type)
		ret = true;

	return ret;
}
