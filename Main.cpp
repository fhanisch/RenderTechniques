#include "App.hpp"
#include "Window.hpp"
#include "VulkanSetup.hpp"
#include "MyFile.hpp"
#include "RenderScene.hpp"
#include "Matrix.hpp"

App::App(int argc, char** argv) {
	int width, height;
	argumentCount = argc;
	argumentValues = argv;
	MyFile appFile("C:/Home/Entwicklung/MyC++Apps/Vulkan/app.json", "rb");
	appFile.read();
	json appData = json::parse(appFile.getData());
	appName = appData["appName"];
	engineName = appData["engineName"];
	libName = appData["libName"];
	applicationVersion = appData["applicationVersion"];
	engineVersion = appData["engineVersion"];
	apiVersion = appData["apiVersion"];
	std::cout << std::setw(4) << appData << '\n';
	auto& windowSettings = appData["WindowSettings"];
	bool fullScreen = windowSettings["fullScreen"];
	if (fullScreen) {
		width = windowSettings["fullScreenWidth"];
		height = windowSettings["fullScreenHeight"];
	}
	else {
		width = windowSettings["width"];
		height = windowSettings["height"];
	}
	window = new Window(width, height, fullScreen);
	graphicObjects = appData["GraphicObjects"];
}

App::~App() {
	delete window;
	std::cout << "App closed." << std::endl;	
}

int App::run() {
	bool bStartApp = false;
	std::cout << "App started.\n";
	window->createWindow();
	VulkanSetup vkSetup(this);
	for (int i = 0; i < argumentCount; i++) {
		if (std::string(argumentValues[i]) == "-instanceVersion") {
			vkSetup.printInstanceVersion();
		}
		else if (std::string(argumentValues[i]) == "-printLayers") {
			vkSetup.printLayers();
		}
		else if (std::string(argumentValues[i]) == "-printExtensions") {
			vkSetup.printExtensions();
		}
		else if (std::string(argumentValues[i]) == "-printDevices") {
			vkSetup.printPhysicalDevices();
		}
		else if (std::string(argumentValues[i]) == "-start") {
			bStartApp = true;
		}
	}
	
	if (bStartApp) {
		vkSetup.init();
		RenderScene scene(&vkSetup, graphicObjects);
		scene.assembleScene();
		window->showWindow();
		scene.updateUniformBuffers();
		while (!window->checkMessage()) {			
			scene.camMotion();
			scene.updateUniformBuffers();
			scene.drawFrame();
		}
	}

	return 0;
}

Window* App::getWindow() { return window; }

std::string& App::getAppName() {
	return appName;
}

std::string& App::getEngineName() {
	return engineName;
}

std::string& App::getLibName() {
	return libName;
}

std::string& App::getApplicationVersion() {
	return applicationVersion;
}

std::string& App::getEngineVersion() {
	return engineVersion;
}

std::string& App::getApiVersion() {
	return apiVersion;
}

void checkArguments(int argc, char** argv) {
	if (argc <= 1) {
		std::cerr << "No input arguments!\n";
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		std::cout << "Argument #" << i << ": " << argv[i] << std::endl;
		if (std::string(argv[i]) == "-instanceVersion") {}
		else if (std::string(argv[i]) == "-printLayers") {}
		else if (std::string(argv[i]) == "-printExtensions") {}
		else if (std::string(argv[i]) == "-printDevices") {}
		else if (std::string(argv[i]) == "-start") {}
		else {
			std::cerr << "Unknown argument!\n";
			exit(1);
		}
	}
}

int main(int argc, char** argv) {

	checkArguments(argc, argv);

	Matrix m, a;
	m[3][3] = 0.12345f;
	m.print();
	m * a;

	std::vector<float> vf = createMesh1D(11);
	for (int i = 0; i < vf.size(); i++) {
		std::cout << vf[i] << "  " << std::endl;
	}
	std::vector<unsigned short> vi = createLineIndices(11);
	for (int i = 0; i < vi.size(); i++) {
		std::cout << vi[i] << "  " << std::endl;
	}

	App app(argc, argv);
		
	return app.run();
}