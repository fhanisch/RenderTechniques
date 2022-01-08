#include "App.hpp"
#include "Window.hpp"
#include "VulkanSetup.hpp"
#include "MyFile.hpp"
#include "RenderScene.hpp"
#include "Matrix.hpp"
#include "Test.hpp"
#include "Noise.hpp"

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

	Noise noise(66);
	float X;
	float noiseValue[21];

	for (int i = 0; i < 21; i++) {
		X = 2.0f * (float)i / 20.0f - 1.0f;
		std::cout << X << "   ";
	}
	std::cout << '\n';
	for (int i = 0; i < 21; i++) {
		X = 2.0f * (float)i / 20.0f - 1.0f;
		std::cout << floor(X) << "   ";
	}
	std::cout << '\n';
	for (int i = 0; i < 21; i++) {
		X = 2.0f * (float)i / 20.0f - 1.0f;
		std::cout << X - floor(X) << "   ";
	}
	std::cout << '\n';
	for (int i = 0; i < 21; i++) {
		X = 2.0f * (float)i / 20.0f - 1.0f;
		X = abs(X);
		std::cout << X - floor(X) << "   ";
	}
	std::cout << '\n';
	for (int i = 0; i < 21; i++) {
		X = 2.0f * (float)i / 20.0f - 1.0f;
		noiseValue[i] = noise.perlinNoise1D(X);
	}
	std::cout << '\n';
	for (int i = 0; i < 21; i++) {
		std::cout << noiseValue[i] << "   ";
	}
	std::cout << '\n';

	Vector3 v = { 1.0f, 2.0f, 3.0f };	
	v.print();
	std::cout << v.length() << std::endl;
	Vector3 u = normalize(v);
	u.print();
	std::cout << u.length() << std::endl;
	Vector3 s = 10.0f + v + 1.0f + u;
	Vector r = Vector(1.0f) + 10.0f;
	s.print();

	Test test;
	float x = 0.0f;
	test.cd(10, 10.0, x);

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