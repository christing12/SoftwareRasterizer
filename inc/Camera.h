#pragma once
#include "EngineMath.h"



class Camera {
public:
	enum PlaneDir {
		TOP, BOTTOM, LEFT, RIGHT, NEARP, FARP
	};
	struct Frustum {
		float hNear, wNear, hFar, wFar;
		Plane planes[6];
		void Print();
		
	};

	Camera();
	Camera(Vector3 inPos);
	~Camera() {}

	void Init();

	void UpdateCamInfo(float fov, float aspectRatio, float nearDIst, float farDist);
	void UpdateFrustum();
	
	Matrix4 viewMatrix = Matrix4::Identity;
	Matrix4 projectionMatrix = Matrix4::Identity;
	Vector3 cameraPos;

	Vector3 up;
	Vector3 forward;
	Vector3 left;
	Vector3 target = Vector3::Zero;
	
	void Reset();

	Frustum frustum;
private:
	void ConstructPlane(Vector3 point, float dimen, PlaneDir planeIdx, Vector3 axis1, Vector3 axis2);
	float fov = Math::ToRadians(50.f), aspectRatio, nearDist = 0.1f, farDist = 100.f;
};