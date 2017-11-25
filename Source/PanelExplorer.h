#pragma once

#include "imgui.h"
#include "Globals.h"

class ExplorerNode
{

public: 
	ExplorerNode(); 
	~ExplorerNode(); 

	bool IsFolder() const;
	void SetIsFolder(bool set); 

	string GetNameStr() const;
	const char* GetName() const;
	void SetName(const char* new_name);

	const char* GetPath() const;
	void SetPath(const char* new_path);

	void AddChild(ExplorerNode* new_child); 

	void DrawNode(); 

private:
	vector<ExplorerNode*> childs; 
	string name; 
	string path; 
	bool is_folder; 
};

class PanelExplorer
{
public:

	PanelExplorer();
	~PanelExplorer(); 

	void Start(); 
	void Update(); 


	// UTILITY

	// This will load all the hierarchy of folders and files inside root directory (Game)
	void CreateTreeRecursive(const char* curr_path, ExplorerNode* node); 

private:
	const char* root_path = ""; 
	ExplorerNode* root_node; 
};
