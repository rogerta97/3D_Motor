#include "PanelExplorer.h"
#include "Application.h"
#include "Functions.h"

//PANEL

PanelExplorer::PanelExplorer()
{
	root_path = ""; 
	root_node = nullptr; 
}

PanelExplorer::~PanelExplorer()
{
}


void PanelExplorer::Start()
{
	this->root_path = App->file_system->game_assets_dir.c_str(); 

	//Create root node 
	ExplorerNode* node = new ExplorerNode();

	node->SetPath(root_path);
	node->SetName(GetLastPathCommand(root_path, true).c_str());
	node->SetIsFolder(App->file_system->IsFolder(root_path));

	root_node = node; 

	CreateTreeRecursive(root_path, node);

}

void PanelExplorer::Update()
{
	ImGui::Begin("Explorer"); 
	
	root_node->DrawNode(); 
		
	ImGui::End(); 
}


void PanelExplorer::CreateTreeRecursive(const char* curr_path, ExplorerNode* node)
{
	vector<string> filenames_list = App->file_system->GetFilesInDirectory(curr_path);

	for (int i = 0; i < filenames_list.size(); i++)
	{
		ExplorerNode* new_node = new ExplorerNode(); 

		string node_path(curr_path + filenames_list[i] + '\\'); 

		new_node->SetPath(node_path.c_str());
		new_node->SetName(GetLastPathCommand(node_path.c_str(), true).c_str());
		new_node->SetIsFolder(App->file_system->IsFolder(node_path.c_str()));

		if (new_node->IsFolder())
		{
			CreateTreeRecursive(node_path.c_str(), new_node);
		}				

		node->AddChild(new_node); 		
	}
}

string PanelExplorer::GetPending()
{
	return to_load;
}

void PanelExplorer::SetPending(string pending)
{
	to_load = pending;
}


// EXPLORER NODE

ExplorerNode::ExplorerNode()
{
	childs.clear();
	name = ""; 
	is_folder = false; 
}


void ExplorerNode::DrawNode()
{
	if (is_folder)
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			for (int i = 0; i < childs.size(); i++)
			{
				childs[i]->DrawNode();
			}

			ImGui::TreePop();
		}
	}
	else
	{
		if (ImGui::MenuItem(name.c_str()))
		{		
			App->imgui->StartModalWindow("Load element ?", "YES", "NO", "LOAD WINDOW");
			App->imgui->panel_explorer.SetPending(name); 
		}

		if (App->imgui->GetModalState() == MODAL_YES && name == App->imgui->panel_explorer.GetPending())
		{
			DeleteEndBars(path);
			App->resource_manager->Load(path.c_str());
			App->imgui->SetModalState(MODAL_NULL); 
		}
		
	}
}

ExplorerNode::~ExplorerNode()
{
}

bool ExplorerNode::IsFolder() const
{
	return is_folder;
}

void ExplorerNode::SetIsFolder(bool set)
{
	is_folder = set;
}

string ExplorerNode::GetNameStr() const
{
	return name;
}

const char * ExplorerNode::GetName() const
{
	return name.c_str();
}

void ExplorerNode::SetName(const char* new_name)
{
	name = new_name; 
}

const char * ExplorerNode::GetPath() const
{
	return path.c_str();
}

void ExplorerNode::SetPath(const char * new_path)
{
	path = new_path; 
}

void ExplorerNode::AddChild(ExplorerNode * new_child)
{
	childs.push_back(new_child); 
}

