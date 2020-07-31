#include "Camera.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include <iostream>

Camera::Camera()
	: up(Vector3::UnitY)
	, fov(Math::ToRadians(50.f))
	, nearDist(0.1f)
	, farDist(100.f)
	, target(Vector3::Zero)
{
//	UpdateFrustum();
}

Camera::Camera(Vector3 inPos) : cameraPos(inPos) {
	g_inputManager = InputManager::Get();
	Init();
	
}

// Sets up View Matrix and Project Matrix(Perspective for now)
void Camera::Init() {
	aspectRatio = DisplayManager::SCREEN_WIDTH / DisplayManager::SCREEN_HEIGHT;
	viewMatrix = Matrix4::CreateLookAt(cameraPos, forward, up);
	projectionMatrix = Matrix4::CreatePerspectiveFOV(fov, DisplayManager::SCREEN_WIDTH,
		DisplayManager::SCREEN_HEIGHT, nearDist, farDist);
}

void Camera::Update(float deltaTime) {
	right = Normalize(Cross(forward, up));
	if (g_inputManager->KeyDown(SDL_SCANCODE_W)) {
		cameraPos += cameraSpeed * deltaTime * forward;
	}
	if (g_inputManager->KeyDown(SDL_SCANCODE_S)) {
		cameraPos -= cameraSpeed * deltaTime * forward;
	}
	if (g_inputManager->KeyDown(SDL_SCANCODE_A)) {
		cameraPos -= cameraSpeed * deltaTime * right;
	}
	if (g_inputManager->KeyDown(SDL_SCANCODE_D)) {
		cameraPos += cameraSpeed * deltaTime * right;
	}
	//MouseRotation();
	viewMatrix = Matrix4::CreateLookAt(cameraPos, cameraPos + forward, up);
	UpdateFrustum();
}

void Camera::MouseRotation() {
	Vector2 mousePos = g_inputManager->MousePos();
	Vector2 offset(mousePos.x - prevMousePos.x, prevMousePos.y - mousePos.y);
	prevMousePos = mousePos;
	offset *= mouseSens;

	yaw += offset.x;
	pitch += offset.y;
	Math::Clamp(pitch, -89.f, 89.f);

	float x = cosf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
	float y = sinf(Math::ToRadians(pitch));
	float z = sinf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
	forward = Normalize(Vector3(x, y, z));
}

// recalculate view frustum planes
void Camera::UpdateFrustum() {
	Vector3 Z = viewMatrix.GetZAxis();
	Vector3 X = viewMatrix.GetXAxis();
	Vector3 Y = viewMatrix.GetYAxis();


	float tan = 2.f * tanf(fov / 2.f);
	frustum.hNear = tan * nearDist;
	frustum.wNear = frustum.hNear * aspectRatio;

	frustum.hFar = tan * farDist;
	frustum.wFar = frustum.hFar * aspectRatio;

	Vector3 fc = cameraPos - Z * nearDist;
	Vector3 nc = cameraPos - Z * farDist;

	frustum.planes[NEARP].UpdatePlane(-1*Z, nc);
	frustum.planes[FARP].UpdatePlane(Z, fc);


	Vector3 aux, normal, point;

	point = (nc + Y * frustum.hNear);
	aux = Normalize(point - cameraPos);
	normal = Cross(aux, X);
	frustum.planes[TOP].UpdatePlane(normal, point);

	point = (nc - Y * frustum.hNear);
	aux = Normalize(point - cameraPos);
	normal = Cross(X, aux);
	frustum.planes[BOTTOM].UpdatePlane(normal, point);

	point = (nc - X * frustum.wNear);
	aux = Normalize(point - cameraPos);
	normal = Cross(aux, Y);
	frustum.planes[LEFT].UpdatePlane(normal, point);

	point = (nc + Y * frustum.wNear);
	aux = Normalize(point - cameraPos);
	normal = Cross(Y, aux);
	frustum.planes[RIGHT].UpdatePlane(normal, point);
}

// helper function for view frustum updates
void Camera::ConstructPlane(Vector3 point, float dimen, PlaneDir planeIdx, Vector3 axis1, Vector3 axis2) {
	Vector3 p = (point + axis1 * dimen);
	Vector3 normal = Normalize(p - cameraPos) * axis2;
	frustum.planes[planeIdx].UpdatePlane(normal, p);
}