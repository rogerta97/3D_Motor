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
		 ~json_file();

		 void SetString(const char* set, const char* str);
		 void SetBool(const char* set, bool bo);
		 void SetNumber(const char* set, double nu);
		 const char* GetString(const char* str, const char* defaul = "");
		 bool GetBool(const char* bo, bool defaul = false);
		 double GetNumber(const char* nu, double defaul = 0);

		 const char* GetPath();
		 void Save();
		 void CleanUp();

	 private:
		 bool FindValue(const char* str, json_value_type type);
	 private:
		 JSON_Value* value = nullptr;
		JSON_Object* object = nullptr;
		std::string path;
};


class JSON : public Module
{
public:
	JSON(bool enabled);
	~JSON();
	bool Awake();
	json_file* LoadJSONFile(const char* path);

	bool CleanUp();
private:
	std::list<json_file*> j_files;
};


#endif // !_JSON_H_
