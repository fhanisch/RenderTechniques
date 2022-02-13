#include "Test.hpp"
#include <iostream>
#include <math.h>

Test::Test() {

}

float u = 0.5f;

void Test::cd(int resolution, float subdiv, float start) {
	for (int i = 0; i < resolution; i++) {
		float x = (float)i / subdiv + start;
		if (abs(u - x) * subdiv <= 1.0 && subdiv <= 100.0f) cd(resolution, float(resolution) * subdiv, x);
		else {
			std::cout << x << "\t";
		}
	}
}