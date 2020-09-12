#include "Camera.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "RenderObj.h"
#include <iostream>
#include <ctime>

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
	UpdateFrustum();
}

// Sets up View Matrix and Project Matrix(Perspective for now)
void Camera::Init() {
	aspectRatio = DisplayManager::SCREEN_WIDTH / DisplayManager::SCREEN_HEIGHT;
	viewMatrix = Matrix4::CreateLookAt(cameraPos, forward, up);
	projectionMatrix = Matrix4::CreatePerspectiveFOV(fov, DisplayManager::SCREEN_WIDTH,
		DisplayManager::SCREEN_HEIGHT, nearDist, farDist);
}

void Camera::Update(float deltaTime) {
	Pan(deltaTime);
	Look(deltaTime);
	Vector3 zoomPos = Zoom(deltaTime);

	viewMatrix = Matrix4::CreateLookAt(zoomPos, cameraPos + forward, up);
	UpdateFrustum();
}

// simple panning using WASD 
void Camera::Pan(float deltaTime) {
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
}

// Updates camera forward vector based on mouse click
void Camera::Look(float deltaTime) {
	if (g_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::LEFT)) {
		prevMousePos = g_inputManager->MousePos();
		return;
	}
	if (!g_inputManager->MouseButtonDown(InputManager::MOUSE_BUTTON::LEFT)) { return; }

	// --- Updating mouse positions --- //
	Vector2 currMousePos = g_inputManager->MousePos();
	Vector2 offset = Vector2(currMousePos.x - prevMousePos.x, prevMousePos.y - currMousePos.y);
	prevMousePos = currMousePos;

	// --- Updating yaw and pitch --- //
	const float sensitivity = 0.1f;
	offset *= sensitivity;
	yaw += offset.x;
	pitch += offset.y;
	Math::Clamp(pitch, -89.f, 89.f);

	// --- Calcaulting forward vector --- //
	float x = cosf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
	float y = sinf(Math::ToRadians(pitch));
	float z = sinf(Math::ToRadians(yaw)) * cosf(Math::ToRadians(pitch));
	forward = Normalize(Vector3(x, y, z));
}

Vector3 Camera::Zoom(float deltaTime) {
	// --- Checking zoom parameters --- //
	int wheelPos = g_inputManager->WheelPos();
	if (wheelPos > 0) zoomLevel++;
	else if (wheelPos < 0) zoomLevel--;
	return cameraPos + zoomLevel * zoomIncrement * forward;
}

void Camera::MouseRotation() {
	if (!g_inputManager->MouseButtonDown(InputManager::MOUSE_BUTTON::LEFT)) { return; }
	if (g_inputManager->MouseButtonPressed(InputManager::MOUSE_BUTTON::LEFT)) { return; }
	Vector2 currMousePos = g_inputManager->MousePos();

	Vector2 offset = Vector2(currMousePos.x - prevMousePos.x, prevMousePos.y - currMousePos.y);
	prevMousePos = currMousePos;

	const float sensitivity = 0.1f;
	offset *= sensitivity;

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


	float tan = tanf(fov / 2.f);
	frustum.hNear = tan * nearDist;
	frustum.wNear = frustum.hNear * aspectRatio;

	frustum.hFar = tan * farDist;
	frustum.wFar = frustum.hFar * aspectRatio;

	Vector3 nc = cameraPos - Z * nearDist;
	Vector3 fc = cameraPos - Z * farDist;

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

// frustum clipping
bool Camera::Frustum::Clipped(RenderObj* obj) {
	Vector3 verts[8];
	obj->bounds.ConstructVerts(verts);

	// convert to world space
	for (int i = 0; i < 8; i++) {
		verts[i] = Transform(verts[i], obj->transform);
	}

	int out, in;
	for (int i = 0; i < 6; i++) {
		out = 0, in = 0;
		for (int j = 0; j < 8 && (in == 0 || out == 0); j++) {
			if (planes[i].Distance(verts[j]) < 0.f) {
				out++;
			}
			else {
				in++;
			}
		}
		if (!in) return true;
	}
	return false;
}