#include <iostream>
#include <memory>

#include "easylogging++.h"
#include <imgui.h>
#include <glm/gtx/string_cast.hpp>

namespace mainMenu
{
	bool menu_should_run = true;

	bool shouldRunMenu()
	{
		return menu_should_run;
	}

	void runMenu() {
		// TODO: Implement menu skip.

		LOG(INFO) << "Running main menu" << std::endl;

		while (shouldRunMenu)
		{
			
		}
	}
}