//
// Created by MINH NHUT on 6/8/2023.
//
#include <iostream>

#include "Template/Application.hpp"

int main()
{
	try {
		Application application;
		application.run();
	} catch (std::exception& e) {
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}