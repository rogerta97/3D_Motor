#include "PanelConsole.h"

void PanelConsole::AddToConsole(std::string new_line)
{
	console_buffer.push_back(new_line);	
}

void PanelConsole::ClearConsole()
{
	console_buffer.clear();
}

void PanelConsole::SetBoolean(bool _new)
{
	show_console = _new;
}

void PanelConsole::PrintConsole()
{
	if (!show_console)
		return; 

	ImGuiWindowFlags console_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::Begin("Console", &show_console, console_flags);

	for (int i = 0; i < console_buffer.size(); i++)
	{
		ImGui::Text(console_buffer[i].c_str());
	}

	ImGui::SetScrollY(ImGui::GetScrollMaxY());
	ImGui::End();

	ImGuiWindowFlags clear_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

	bool opened = true; 
	ImGui::Begin("Clear", NULL, clear_flags);

	if (ImGui::Button("Clear Console"))
	{
		ClearConsole(); 
	}

	ImGui::End(); 
	
}