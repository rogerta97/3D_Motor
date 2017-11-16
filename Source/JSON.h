#ifndef _JSON_H_
#define _JSON_H_

#include "Module.h"
#include "Parson\parson.h"
#include<string>
#include<list>

class json_file
	 {
	 public:
		 json_file(JSON_Value* value, JSON_Object* object, const char* path);
		 json_file(JSON_Object* Entry);
		 json_file();
		 ~json_file();
		 int GetFileSize()const;
		 void SetString(const char* set, const char* data);
		 void SetBool(const char* set, bool data);
		 void SetDouble(const char* set, double data);
		 void SetFloat(const char* set, double data);
		 void SetInt(const char* set, int data);
		 void SetUInt(const char* set, uint data);
		 void SetUID(const char* set, UID data);
		 void SetArray(const char* set);
		 void SetArrayBool(const char* set, const bool* data, int size);
		 void SetArrayInt(const char* set, const int* data, int size);
		 void SetArrayUInt(const char* set, const uint* data, int size);
		 void SetArrayFloat(const char* set, const float* data, int size);
		 void SetArrayString(const char* set, const char** data, int size);
		 void SetFloat3(const char* set, const float3& data);
		 void SetQuaternion(const char * set, const Quat & data);
		 void SetNodeEntry(const json_file& config);

		 json_file GetEntry(const char* set) const;
		 json_file SetEntry(const char* set);
		 int GetArraySize(const char * field) const;
		 json_file GetArray(const char * field, int index) const;
		 const char* GetString(const char* str, const char* defaul = "", int id = -1)const;
		 bool GetBool(const char* bo, bool defaul = false, int id = -1)const;
		 double GetDouble(const char* nu, double defaul = 0, int id = -1)const;
		 int GetInt(const char* set, int defaul, int id = -1)const;
		 float GetFloat(const char* set, int defaul,int id = -1)const;
		 uint GetUInt(const char* set, uint defaul, int id = -1)const;
		 UID GetUID(const char* set, UID defaul, int id = -1)const;
		 float3 GetFloat3(const char* field, const float3& default = float3::zero);
		 Quat GetQuat(const char* field, const Quat& default = Quat::identity);

		 const char* GetPath();
		 void Save();
		 void CleanUp();

	 private:
		 bool FindValue(const char* str, json_value_type type,int index)const;
		 JSON_Value * GetValue(const char * field, int index) const;

	 private:
		JSON_Value* value = nullptr;
		JSON_Object* object = nullptr;
		JSON_Array* array = nullptr;
		std::string path;
};


class JSON : public Module
{
public:
	JSON(bool enabled);
	~JSON();
	bool Awake();
	json_file* LoadJSONFile(const char* path);
	json_file* GetFile(const char* name);
	bool CleanUp();
private:
	std::list<json_file*> j_files;
};


#endif // !_JSON_H_
