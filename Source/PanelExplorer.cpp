#include "PanelExplorer.h"
#include "Application.h"


PanelExplorer::PanelExplorer()
{
}

PanelExplorer::~PanelExplorer()
{
}


void PanelExplorer::Start(const char* root_path)
{
	this->root_path = root_path; 
	list<string> test = App->file_system->GetFilesInDirectory(root_path);
}

void PanelExplorer::Update()
{
	ImGui::Begin("Explorer"); 

	

	ImGui::End(); 
}

void PanelExplorer::Draw()
{
}
