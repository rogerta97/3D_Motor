#ifndef _PANELCONSOLE_H_
#define _PANELCONSOLE_H_

#include "imgui.h"
#include <vector>

class PanelConsole
{
public:

	void PrintConsole();
	void AddToConsole(std::string new_str); 
	void ClearConsole(); 

	void SetBoolean(bool _new); 

private:

	std::vector<std::string> console_buffer;

	bool show_console = false; 

};

#endif 
