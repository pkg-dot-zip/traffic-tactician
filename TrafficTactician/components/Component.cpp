#include "Component.h"
#include <iostream>

Component::Component()
{
	std::cout << "Component added" << std::endl;

}

Component::~Component()
{
	std::cout << "Component deleted" << std::endl;
}
