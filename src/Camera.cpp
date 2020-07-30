#include "Camera.h"
#include "DisplayManager.h"
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
	Init();
}

void Camera::Init() {
	aspectRatio = DisplayManager::SCREEN_WIDTH / DisplayManager::SCREEN_HEIGHT;
	viewMatrix = Matrix4::CreateLookAt(cameraPos, target, Vector3::UnitY);
	projectionMatrix = Matrix4::CreatePerspectiveFOV(fov, DisplayManager::SCREEN_WIDTH,
		DisplayManager::SCREEN_HEIGHT, nearDist, farDist);
}

void Camera::UpdateCamInfo(float fov, float aspectRatio, float nearDist, float farDist) {
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearDist = nearDist;
	this->farDist = farDist;

	UpdateFrustum();
}

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

void Camera::ConstructPlane(Vector3 point, float dimen, PlaneDir planeIdx, Vector3 axis1, Vector3 axis2) {
	Vector3 p = (point + axis1 * dimen);
	Vector3 normal = Normalize(p - cameraPos) * axis2;
	frustum.planes[planeIdx].UpdatePlane(normal, p);
}