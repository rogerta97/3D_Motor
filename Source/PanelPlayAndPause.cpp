#include "PanelPlayAndPause.h"
#include "Application.h"

PanelPlayAndPause::PanelPlayAndPause()
{
}

void PanelPlayAndPause::Draw()
{
	bool state_modified = false; 

	ImGui::Begin("Play And Pause");

	if (ImGui::Button("PLAY") && App->GetState() != APP_STATE_PLAY)
	{
		play_presed = !play_presed; 
		state_modified = true; 
		App->SetState(APP_STATE_PLAY);
		LOG("App State: PLAY");
	}

	ImGui::SameLine(); 

	if (ImGui::Button("PAUSE") && App->GetState() != APP_STATE_PAUSE)
	{
		pause_presed = !pause_presed; 
		state_modified = true;
		App->SetState(APP_STATE_PAUSE);
		LOG("App State: PAUSE");
	}

	ImGui::SameLine(); 

	if (ImGui::Button("STOP") && App->GetState() != APP_STATE_STOP)
	{
		stop_presed = !stop_presed; 
		state_modified = true;
		App->SetState(APP_STATE_STOP); 
		LOG("App State: STOP"); 
	}

	if (state_modified)
		App->SendMessageToModules();

	ImGui::End();
}

PanelPlayAndPause::~PanelPlayAndPause()
{
}
