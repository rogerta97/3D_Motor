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
		if (strcmp(path, (*it)->GetPath())==0)
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

json_file * JSON::CreateNewJSON(const char * path)
{
	json_file* doc = nullptr;

	//Check if the file already exists
	bool exist = false;
	for (std::list<json_file*>::iterator json_doc = j_files.begin(); json_doc != j_files.end(); ++json_doc)
	{
		if (path == (*json_doc)->GetPath())
		{
			exist = true;
			break;
		}
	}

	if (exist)
	{
		LOG("File %s already exists.", path);
	}
	else
	{
		JSON_Value* doc_data = json_value_init_object();
		JSON_Object* object_data = json_value_get_object(doc_data);

		doc = new json_file(doc_data, object_data, path);
		j_files.push_back(doc);
	}

	return doc;
}

json_file* JSON::GetFile(const char * name)
{
	json_file* ret = nullptr;

	for (std::list<json_file*>::iterator it = j_files.begin(); it != j_files.end(); it++)
	{
		if (strcmp(name, (*it)->GetPath()))
		{
			ret = (*it);
			break;
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

json_file::json_file(JSON_Value * value_, JSON_Object * object_, const char * path_)
{
	value = value_;
	object = object_;
	path = path_;
}
json_file::json_file(JSON_Object * Entry):object(Entry)
{

}
json_file::json_file()
{
}
json_file::~json_file()
{
}
int json_file::GetFileSize() const
{
	if (object != nullptr)
		return json_object_get_count(object);
	return 0;
	
}
void json_file::SetString(const char * set, const char * data)
{
	json_object_dotset_string(object, set, data);
}

void json_file::SetBool(const char * set, bool data)
{
	json_object_dotset_boolean(object, set, data);
}

void json_file::SetDouble(const char * set, double data)
{
	json_object_dotset_number(object, set, data);
}

void json_file::SetFloat(const char * set, double data)
{
	json_object_dotset_number(object, set, data);
}

void json_file::SetInt(const char * set, int data)
{
	json_object_dotset_number(object, set, data);
}

void json_file::SetUInt(const char * set, uint data)
{
	json_object_dotset_number(object, set, data);
}

void json_file::SetUID(const char * set, UID data)
{
	json_object_dotset_number(object, set, data);
}

void json_file::SetArray(const char * array_name)
{
	JSON_Value* val = json_value_init_array();
	JSON_Array* arra = json_value_get_array(val);
	json_object_set_value(object, array_name, val);
}

void json_file::SetArrayBool(const char * set, const bool * data, int size)
{
	if (data != nullptr && size > 0)
	{
		JSON_Value* va = json_value_init_array();
		array = json_value_get_array(va);
		json_object_set_value(object, set, va);

		for (int i = 0; i < size; ++i)
			json_array_append_boolean(array, data[i]);
	}
}

void json_file::SetArrayInt(const char * set, const int * data, int size)
{
	if (data != nullptr && size > 0)
	{
		JSON_Value* va = json_value_init_array();
		array = json_value_get_array(va);
		json_object_set_value(object, set, va);

		for (int i = 0; i < size; ++i)
			json_array_append_number(array, data[i]);
	}
}

void json_file::SetArrayUInt(const char * set, const uint * data, int size)
{
	if (data != nullptr && size > 0)
	{
		JSON_Value* va = json_value_init_array();
		array = json_value_get_array(va);
		json_object_set_value(object, set, va);

		for (int i = 0; i < size; ++i)
			json_array_append_number(array, data[i]);
	}
}

void json_file::SetArrayFloat(const char * set, const float * data, int size)
{
	if (data != nullptr && size > 0)
	{
		JSON_Value* va = json_value_init_array();
		array = json_value_get_array(va);
		json_object_set_value(object, set, va);

		for (int i = 0; i < size; ++i)
			json_array_append_number(array, data[i]);
	}
}

void json_file::SetArrayString(const char * set, const char ** data, int size)
{
	if (data != nullptr && size > 0)
	{
		JSON_Value* va = json_value_init_array();
		array = json_value_get_array(va);
		json_object_set_value(object, set, va);

		for (int i = 0; i < size; ++i)
			json_array_append_string(array, data[i]);
	}
}

void json_file::SetFloat3(const char * set, const float3 & data)
{
	return SetArrayFloat(set, &data.x, 3);
}
//useful for quaternions
void json_file::SetQuaternion(const char * set, const Quat & data)
{
	return SetArrayFloat(set, &data.x, 4);
}

void json_file::SetNodeEntry(const json_file & config)
{
	if (array != nullptr)
		json_array_append_value(array, json_value_deep_copy(config.value));
}

json_file json_file::GetEntry(const char * set) const
{
	return json_file(json_object_get_object(object, set));
}

json_file json_file::SetEntry(const char * set)
{
	json_object_set_value(object, set, json_value_init_object());
	return GetEntry(set);
}

json_file json_file::GetArray(const char * field, int index) const
{
	JSON_Array* array = json_object_get_array(object, field);
	if (array != nullptr)
		return json_file(json_array_get_object(array, index));
	return json_file((JSON_Object*) nullptr);
}
bool json_file::MoveToSectionFromArray(const char * field, int index)
{
	bool ret = false;

	JSON_Array* array = json_object_get_array(object, field);

	if (array != nullptr)
	{
		JSON_Object* obj = json_array_get_object(array, index);

		object = obj;
		ret = true;
	}

	return ret;
}
const char * json_file::GetString(const char * str, const char* defaul,int id)const
{
	const char* ret = defaul;

	if (FindValue(str, json_value_type::JSONString,id))
		ret = json_object_dotget_string(object, str);

	return ret;
}
bool json_file::GetBool(const char * str, bool defaul, int id)const
{
	bool ret = defaul;

	if (FindValue(str, json_value_type::JSONBoolean,id))
		ret = json_object_dotget_boolean(object, str);

	return ret;
}

double json_file::GetDouble(const char * str, double defaul, int id)const
{
	double ret = defaul;

	if (FindValue(str, json_value_type::JSONNumber,id))
		ret = json_object_dotget_number(object, str);

	return ret;
}

int json_file::GetInt(const char * set, int defaul, int id) const
{
	int ret = defaul;

	if (FindValue(set, json_value_type::JSONNumber,id))
		ret = json_object_dotget_number(object, set);

	return ret;
}

float json_file::GetFloat(const char * set, int defaul,int id) const
{
	float ret = defaul;

	if (FindValue(set, json_value_type::JSONNumber, id))
		ret = json_object_dotget_number(object, set);

	return ret;
}

uint json_file::GetUInt(const char * set, uint defaul, int id) const
{
	uint ret = defaul;

	if (FindValue(set, json_value_type::JSONNumber,id))
		ret = json_object_dotget_number(object, set);

	return ret;
}

UID json_file::GetUID(const char * set, UID defaul, int id) const
{
	UID ret = defaul;

	if (FindValue(set, json_value_type::JSONNumber,id))
		ret = json_object_dotget_number(object, set);

	return ret;
}
int json_file::GetArraySize(const char * field) const
{
	int ret = -1;
	JSON_Array* array = json_object_get_array(object, field);
	if (array != nullptr)
		ret = json_array_get_count(array);
	return ret;
}
float3 json_file::GetFloat3(const char * field, const float3 & default)
{
	return float3(
		GetFloat(field, default.x, 0),
		GetFloat(field, default.y, 1),
		GetFloat(field, default.z, 2));
}

Quat json_file::GetQuat(const char * field, const Quat & default)
{
	return Quat(
		GetFloat(field, default.x, 0),
		GetFloat(field, default.y, 1),
		GetFloat(field, default.z, 2),
		GetFloat(field,default.w,3));
}


void json_file::Save()
{
	if (json_serialize_to_file_pretty(value, path.c_str()) == JSONSuccess)
		LOG("New JSON saving!!");
}

void json_file::CleanUp()
{
	json_value_free(value);
}
void json_file::Delete()
{
		json_value_free(value);
		value = json_value_init_object();
		object = json_value_get_object(value);
}
bool json_file::FindValue(const char * str, json_value_type type,int index)const
{
	bool ret = false;

	JSON_Value* val = GetValue(str, index);

	if (val != nullptr && json_value_get_type(val) == type)
		ret = true;

	return ret;
}

JSON_Value * json_file::GetValue(const char * field, int index) const
{
	//index are -1 by default
	if (index < 0)
		return json_object_get_value(object, field);

	JSON_Array* array = json_object_get_array(object, field);
	if (array != nullptr)
		return json_array_get_value(array, index);

	return nullptr;
}
