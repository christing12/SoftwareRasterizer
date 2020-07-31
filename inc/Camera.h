#pragma once
#include "Geometry.h"

class InputManager;

class Camera {
public:
	enum PlaneDir {
		TOP, BOTTOM, LEFT, RIGHT, NEARP, FARP
	};
	struct Frustum {
		float hNear, wNear, hFar, wFar;
		Plane planes[6];
		
	};

	struct MouseInfo {

	};

	Camera();
	Camera(Vector3 inPos);
	~Camera() {}

	void Init();
	void Update(float deltaTime);

	void UpdateFrustum();
	
	Matrix4 viewMatrix = Matrix4::Identity;
	Matrix4 projectionMatrix = Matrix4::Identity;
	Vector3 cameraPos;

	Vector3 up = Vector3::UnitY;
	Vector3 forward = Vector3::UnitZ * -1.f;
	Vector3 target = Vector3::Zero;
	Vector3 right = Vector3::UnitX;

	Frustum frustum;
private:
	InputManager* g_inputManager;
	void ConstructPlane(Vector3 point, float dimen, PlaneDir planeIdx, Vector3 axis1, Vector3 axis2);
	float fov = Math::ToRadians(50.f), aspectRatio, nearDist = 0.1f, farDist = 100.f;

	const float cameraSpeed = 0.5f;
	const float mouseSens = 0.1f;
	float yaw, pitch;

	Vector2 prevMousePos;
	void MouseRotation();
};