#include "MyFile.hpp"
#include <iostream>

MyFile::MyFile(const char* fileName, const char* options) {
	file = fopen(fileName, options);
	if (!file) {
		std::cout << "Could not open '" << fileName << "'!\n";
		return;
	}
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	rewind(file);
	data = new char[fileSize + 1];
}

MyFile::~MyFile() {
	delete[] data;
	fclose(file);
}

int MyFile::read() {
	fread(data, fileSize, 1, file);
	data[fileSize] = 0;
	return 0;
}

char* MyFile::getData() {
	return data;
}