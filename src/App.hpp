#pragma once

#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

class Window;
class App {
	int argumentCount;
	char** argumentValues;
	std::string appName;
	std::string libName;
	std::string engineName;
	std::string applicationVersion;
	std::string engineVersion;
	std::string apiVersion;
	Window* window;
	json graphicObjects;
public:	
	App(int argc, char** argv);
	~App();
	int run();
	Window* getWindow();
	std::string& getAppName();
	std::string& getEngineName();
	std::string& getLibName();
	std::string& getApplicationVersion();
	std::string& getEngineVersion();
	std::string& getApiVersion();
};