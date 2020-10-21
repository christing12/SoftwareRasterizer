#pragma once
#include "EngineMath.h"


class Mesh;


struct Triangle {
    Vector3 verts[3];
    Triangle(Vector3 a, Vector3 b, Vector3 c) {
        verts[0] = a;
        verts[1] = b;
        verts[2] = c;
    }
    Triangle() {

    }

    Vector3& operator[](int idx) {
        return verts[idx];
    }

    Vector3 GetNormal() {
        Vector3 cross = Cross(verts[1] - verts[0], verts[2] - verts[0]);
        return Normalize(cross);
    }
};

struct BBox {
    int minX, minY, maxX, maxY;
    BBox() {}
    BBox(Vector2 min, Vector2 max)
        : minX(min.x)
        , minY(min.y)
        , maxX(max.x)
        , maxY(max.y)
    {

    }
};

struct Plane {
    Plane() {}
    Plane(Vector3 inNorm, float inD) : norm(inNorm), D(inD) {}
    Vector3 norm;
    float D;

    float Distance(const Vector3& point) {
        return Dot(norm, point) + D;
    }

    void UpdatePlane(const Vector3& p0, const Vector3& p1, const Vector3& p2) {
        norm = Normalize(Cross(p1 - p0, p2 - p0));
        D = -Dot(norm, p0);
    }

    void UpdatePlane(const Vector3& n, const Vector3& p) {
        norm = n;
        D = -Dot(norm, p);
    }
};

struct AABB {
    Vector3 min, max;

    AABB() {}

    AABB(Mesh* mesh);

    void ConstructVerts(Vector3 verts[8]);
    void Update();
   
};
