#include "PanelModalWindow.h"

PanelModalWindow::PanelModalWindow()
{
	text = ""; 
	yes_button_text = ""; 
	no_button_text = ""; 
	curr_state = MODAL_NULL;
}

PanelModalWindow::~PanelModalWindow()
{
}

void PanelModalWindow::SetData(const char* text, const char* yes_text, const char* no_text, const char* window_title)
{
	this->text = text; 
	yes_button_text = yes_text; 
	no_button_text = no_text; 
	this->window_title = window_title;
}

modal_state PanelModalWindow::GetState()
{
	return curr_state;
}

void PanelModalWindow::SetState(modal_state new_st)
{
	curr_state = new_st;
}

void PanelModalWindow::Update()
{

	ImGui::OpenPopup(window_title);

	if (ImGui::BeginPopupModal(window_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(text);
		ImGui::Separator();

		if (ImGui::Button(yes_button_text, ImVec2(120, 0)))
		{ 
			curr_state = MODAL_YES;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		if (ImGui::Button(no_button_text, ImVec2(120, 0)))
		{ 
			curr_state = MODAL_NO;	
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::EndPopup();
	}


}
