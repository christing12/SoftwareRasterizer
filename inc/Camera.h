#pragma once
#include "Geometry.h"

class InputManager;
class RenderObj;

class Camera {
public:
	enum PlaneDir {
		TOP, BOTTOM, LEFT, RIGHT, NEARP, FARP
	};
	struct Frustum {
		float hNear, wNear, hFar, wFar;
		Plane planes[6];
		
		bool Clipped(RenderObj* obj);
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

	bool isVisible(RenderObj* obj) {
		return !frustum.Clipped(obj);
	}
private:
	InputManager* g_inputManager;
	float fov = Math::ToRadians(50.f), aspectRatio, nearDist = 0.1f, farDist = 100.f;

	const float cameraSpeed = 0.8f;
	const float mouseSens = 0.1f;
	float yaw = -90.f, pitch = 0.f;

	int zoomLevel = 0;
	float zoomIncrement = 0.1;

	Vector2 prevMousePos;
	void MouseRotation();

	void Pan(float deltaTime);
	void Look(float deltaTime);
	Vector3 Zoom(float deltaTime);
};