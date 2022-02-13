#pragma once
#include "Matrix.hpp"

enum MotionType { TRANSLATIONAL, ROTATORY };

class Camera {
public:
	/// <summary>
	/// Orientation and Position of the Camera in the World Space
	/// </summary>
	Matrix OP; /// Matrix of Orientation and Position of the Camera in the World Space
	float elevation = 0.0f;
	float Xi = 0.0f;
	MotionType motionType = TRANSLATIONAL;
	Camera();
};