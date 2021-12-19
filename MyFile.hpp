#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

class MyFile {
	FILE* file;
	int fileSize;
	char* data;
public:
	MyFile(const char* fileName, const char* options);
	~MyFile();
	int read();
	int write();
	char* getData();
};