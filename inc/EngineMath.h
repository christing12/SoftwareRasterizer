#pragma once
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <smmintrin.h>
#include <cstring>



// SHUFFLER is like shuffle, but has easier to understand indices
#define _MM_SHUFFLER( xi, yi, zi, wi ) _MM_SHUFFLE( wi, zi, yi, xi )

/*
3D MATH INFO
RIGHT HANDED COORDINATE SYSTEM ( POSITIVE Z GOING INTO THE SCREEN)
COUNTER-CLOCKWISE ROTATIONS
COLUMN MAJOR MATRICES
COUNTER CLOCKWISE WINDING ORDER
*/

namespace Math
{
    const float Pi = 3.1415926535f;
    const float TwoPi = Pi * 2.0f;
    const float PiOver2 = Pi / 2.0f;

    inline float ToRadians(float degrees)
    {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(float radians)
    {
        return radians * 180.0f / Pi;
    }

    template <typename T>
    T Max(const T& a, const T& b)
    {
        return (a < b ? b : a);
    }

    template <typename T>
    T Min(const T& a, const T& b)
    {
        return (a < b ? a : b);
    }

    template <typename T>
    T Clamp(const T& value, const T& lower, const T& upper)
    {
        return Min(upper, Max(lower, value));
    }

    inline float Lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    //----------------------------------------------------------------------------------------
    // returns true if val is "close enough" to be considered the zero
    //----------------------------------------------------------------------------------------
    inline bool IsZero(float val, float epsilon = 0.001f)
    {
        if (fabs(val) <= epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //----------------------------------------------------------------------------------------
    // returns true if a and b are "close enough" to be considered the same value
    //----------------------------------------------------------------------------------------
    inline bool IsCloseEnuf(float a, float b, float epsilon = 0.001f)
    {
        return fabsf(a - b) <= epsilon * Max(Max(1.0f, fabsf(a)), fabsf(b));
    }
}   //namespace Math

//------------------------------------------------------------------------------------
// 2D Vector
//------------------------------------------------------------------------------------




class Vector2
{
public:
    float x;
    float y;

    Vector2()
        : x(0.0f)
        , y(0.0f)
    {}

    Vector2(float inX, float inY)
        : x(inX)
        , y(inY)
    {}

    // Set both components in one line
    void Set(float inX, float inY)
    {
        x = inX;
        y = inY;
    }

    // Vector addition (a + b)
    friend Vector2 operator+(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x + b.x, a.y + b.y);
    }

    // Vector +=
    Vector2& operator+=(const Vector2& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }

    // Vector subtraction (a - b)
    friend Vector2 operator-(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x - b.x, a.y - b.y);
    }

    // Vector -=
    Vector2& operator-=(const Vector2& right)
    {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    // Component-wise multiplication
    // (a.x * b.x, ...)
    friend Vector2 operator*(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x * b.x, a.y * b.y);
    }

    // Scalar multiplication
    friend Vector2 operator*(const Vector2& vec, float scalar)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar multiplication
    friend Vector2 operator*(float scalar, const Vector2& vec)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar *=
    Vector2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Scalar division
    friend Vector2 operator/(const Vector2& vec, float scalar)
    {
        return Vector2(vec.x / scalar, vec.y / scalar);
    }

    // Scalar /=
    Vector2& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Length squared of vector
    float LengthSq() const
    {
        return (x * x + y * y);
    }

    // Length of vector
    float Length() const
    {
        return (sqrtf(LengthSq()));
    }

    // Normalize this vector
    void Normalize()
    {
        float length = Length();
        x /= length;
        y /= length;
    }

    // Normalize the provided vector
    static Vector2 Normalize(const Vector2& vec)
    {
        Vector2 temp = vec;
        temp.Normalize();
        return temp;
    }

    // Dot product between two vectors (a dot b)
    static float Dot(const Vector2& a, const Vector2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // Lerp from A to B by f
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
    {
        return a + (b - a) * f;
    }

    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 UnitX;
    static const Vector2 UnitY;

};

// 3D Vector
class Vector3
{
public:
    float x;
    float y;
    float z;
    float w;
    Vector3()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    {}

    explicit Vector3(float inX, float inY, float inZ)
        : x(inX)
        , y(inY)
        , z(inZ)
    {}

    explicit Vector3(__m128 value)
    {
        memcpy(this, &value, sizeof(float) * 3);
    }

    // Set all three components in one line
    void Set(float inX, float inY, float inZ)
    {
        x = inX;
        y = inY;
        z = inZ;
    }

    // Vector addition (a + b)
    friend Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    // Vector subtraction (a - b)
    friend Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    // Component-wise multiplication
    friend Vector3 operator*(const Vector3& left, const Vector3& right)
    {
        return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
    }

    // Scalar multiplication
    friend Vector3 operator*(const Vector3& vec, float scalar)
    {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    // Scalar multiplication
    friend Vector3 operator*(float scalar, const Vector3& vec)
    {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    // Scalar *=
    Vector3& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Scalar division
    friend Vector3 operator/(const Vector3& vec, float scalar)
    {
        return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }

    // Scalar /=
    Vector3& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Vector +=
    Vector3& operator+=(const Vector3& right)
    {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

    // Vector -=
    Vector3& operator-=(const Vector3& right)
    {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        return *this;
    }

    // Length squared of vector
    float LengthSq() const
    {
        return (x * x + y * y + z * z);
    }

    // Length of vector
    float Length() const
    {
        return (sqrtf(LengthSq()));
    }

    // Normalize this vector
    void Normalize()
    {
        float length = this->Length();
        x = x / length;
        y = y / length;
        z = z / length;
    }

    // Normalize the provided vector
    friend Vector3 Normalize(const Vector3& vec)
    {
        float length = vec.Length();
        return Vector3(vec.x / length, vec.y / length, vec.z / length);
    }

    // Dot product between two vectors (a dot b)
    friend float Dot(const Vector3& a, const Vector3& b)
    {
        return (a.x * b.x + a.y * b.y + a.z * b.z);
    }

    // Cross product between two vectors (a cross b)
    friend Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        float newX = a.y * b.z - a.z * b.y;
        float newY = a.z * b.x - a.x * b.z;
        float newZ = a.x * b.y - a.y * b.x;
        return Vector3(newX, newY, newZ);
    }

    // Lerp from A to B by f
    friend Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
    {
        return (a + (b - a) * f);
    }

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;

    static bool UnitTest();
};

// 4x4 Matrix
class Matrix4
{
public:
    union
    {
        __m128 rows[4];
        float mat[4][4];
    };

    Matrix4()
    {
        rows[0] = _mm_setzero_ps();
        rows[1] = _mm_setzero_ps();
        rows[2] = _mm_setzero_ps();
        rows[3] = _mm_setzero_ps();
    }

    explicit Matrix4(const float inMat[4][4])
    {
        memcpy(mat, inMat, 16 * sizeof(float));
    }

    explicit Matrix4(const __m128 inRows[4])
    {
        rows[0] = inRows[0];
        rows[1] = inRows[1];
        rows[2] = inRows[2];
        rows[3] = inRows[3];
    }

    // Cast to a const float pointer
    const float* GetAsFloatPtr() const
    {
        return &mat[0][0];
    }

    // Matrix multiplication (a * b)
    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
    {
        // transpose b
        __m128 bT[4];
        __m128 tmp0 = _mm_shuffle_ps(b.rows[0], b.rows[1], 0x44);
        __m128 tmp2 = _mm_shuffle_ps(b.rows[0], b.rows[1], 0xee);
        __m128 tmp1 = _mm_shuffle_ps(b.rows[2], b.rows[3], 0x44);
        __m128 tmp3 = _mm_shuffle_ps(b.rows[2], b.rows[3], 0xee);
        bT[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        bT[1] = _mm_shuffle_ps(tmp0, tmp1, 0xdd);
        bT[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        bT[3] = _mm_shuffle_ps(tmp2, tmp3, 0xdd);

        __m128 rows[4];
        for (int i = 0; i < 4; i++)
        {
            rows[i] = _mm_add_ps(
                _mm_add_ps(_mm_dp_ps(a.rows[i], bT[0], 0xF1),
                    _mm_dp_ps(a.rows[i], bT[1], 0xF2)
                ),
                _mm_add_ps(_mm_dp_ps(a.rows[i], bT[2], 0xF4),
                    _mm_dp_ps(a.rows[i], bT[3], 0xF8)
                )
            );
        }

        return Matrix4(rows);
    }

    Matrix4& operator*=(const Matrix4& right)
    {
        *this = *this * right;
        return *this;
    }

    // Invert the matrix - super slow
    void Invert();

    // Get the translation component of the matrix
    Vector3 GetTranslation() const
    {
        return Vector3(mat[0][3], mat[1][3], mat[2][3]);
    }

    // Get the X axis of the matrix (forward)
    Vector3 GetXAxis() const
    {
        return Normalize(Vector3(mat[0][0], mat[1][0], mat[2][0]));
    }

    // Get the Y axis of the matrix (left)
    Vector3 GetYAxis() const
    {
        return Normalize(Vector3(mat[0][1], mat[1][1], mat[2][1]));
    }

    // Get the Z axis of the matrix (up)
    Vector3 GetZAxis() const
    {
        return Normalize(Vector3(mat[0][2], mat[1][2], mat[2][2]));
    }

    // Extract the scale component from the matrix
    Vector3 GetScale() const
    {
        __m128 x = _mm_dp_ps(rows[0], rows[0], 0x71);
        __m128 y = _mm_dp_ps(rows[1], rows[1], 0x72);
        __m128 z = _mm_dp_ps(rows[2], rows[2], 0x74);
        return Vector3(_mm_sqrt_ps(_mm_add_ps(_mm_add_ps(x, y), z)));
    }

    // Transpose this matrix
    void Transpose()
    {
        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);
    }

    // Transpose the provided matrix
    friend Matrix4 Transpose(const Matrix4& inMat)
    {
        Matrix4 retVal = inMat;
        retVal.Transpose();
        return retVal;
    }

    // Create a scale matrix with x, y, and z scales
    static Matrix4 CreateScale(float xScale, float yScale, float zScale)
    {
        float temp[4][4] =
        {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, zScale, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateScale(const Vector3& scaleVector)
    {
        return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
    }

    // Create a scale matrix with a uniform factor
    static Matrix4 CreateScale(float scale)
    {
        return CreateScale(scale, scale, scale);
    }

    // Rotation about x-axis
    static Matrix4 CreateRotationX(float theta)
    {

        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f , 0.0f },
            { 0.0f, cosf(theta), -sinf(theta), 0.0f },
            { 0.0f, sinf(theta), cosf(theta), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Rotation about y-axis
    static Matrix4 CreateRotationY(float theta)
    {
        float temp[4][4] =
        {
            { cosf(theta), 0.0f, sinf(theta), 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { -sinf(theta), 0.0f, cosf(theta), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Rotation about z-axis
    static Matrix4 CreateRotationZ(float theta)
    {
        float temp[4][4] =
        {
            { cosf(theta), -sinf(theta), 0.0f, 0.0f },
            { sinf(theta), cosf(theta), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateYawPitchRoll(float yaw, float pitch, float roll)
    {
        return Matrix4::CreateRotationZ(roll)
            * Matrix4::CreateRotationX(pitch)
            * Matrix4::CreateRotationY(yaw);
    }

    static Matrix4 CreateTranslation(const Vector3& trans)
    {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f, trans.x },
            { 0.0f, 1.0f, 0.0f, trans.y },
            { 0.0f, 0.0f, 1.0f, trans.z },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& at, const Vector3& inUp)
    {
        Vector3 forward = Normalize(eye - at);
        Vector3 right = Normalize(Cross(inUp, forward));
        Vector3 up = Cross(forward, right);

        float temp[4][4] =
        {
            { right.x,      right.y,    right.z,    -Dot(right, eye) },
            { up.x,         up.y,       up.z,       -Dot(up, eye) },
            { forward.x,    forward.y,  forward.z,  -Dot(forward, eye) },
            {0.f, 0.f, 0.f, 1.f}
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateOrtho(float width, float height, float nearClip, float farClip)
    {
        float temp[4][4] =
        {
            { 2.0f / width, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f / height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f },
            { 0.0f, 0.0f, nearClip / (nearClip - farClip), 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreatePerspectiveFOV(float fov, float width, float height, float znear, float zfar)
    {
        float angle = fov / 2.f;
        float AR = width / height;

        float xScale = 1.0f / tanf(angle);
        float yScale = xScale * AR;
        float c = znear / (zfar - znear);
        float d = zfar * c;


        float test[4][4] =
        {
            {xScale, 0, 0, 0},
            {0, yScale, 0, 0},
            {0, 0, c, d},
            {0, 0,     -1, 0}
        };

        return Matrix4(test);
    }

    static Matrix4 TBNMatrix(const Vector3& tangent, const Vector3& biTangent, const Vector3& normal) {
        float temp[4][4] = {
            {tangent.x, tangent.y, tangent.z, 0},
            {biTangent.x, biTangent.y, biTangent.z, 0},
            {normal.x, normal.y, normal.z, 0},
            {0, 0, 0, 1}
        };
        return Matrix4(temp);
    }

    static const Matrix4 Identity;
};

inline Vector3 Transform(const Vector3& vec, const Matrix4& mat, float w = 1.f)
{
    Vector3 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[0][1] +
        vec.z * mat.mat[0][2] + w * mat.mat[0][3];
    retVal.y = vec.x * mat.mat[1][0] + vec.y * mat.mat[1][1] +
        vec.z * mat.mat[1][2] + w * mat.mat[1][3];
    retVal.z = vec.x * mat.mat[2][0] + vec.y * mat.mat[2][1] +
        vec.z * mat.mat[2][2] + w * mat.mat[2][3];
    retVal.w = vec.x * mat.mat[3][0] + vec.y * mat.mat[3][1] +
        vec.z * mat.mat[3][2] + w * mat.mat[3][3];
    return retVal;
}

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

    void UpdatePlane(const Vector3& p0, const Vector3& p1, const Vector3& p2) {
        norm = Normalize(Cross(p1 - p0, p2 - p0));
        D = -Dot(norm, p0);
    }

    void UpdatePlane(const Vector3& n, const Vector3& p) {
        norm = n;
        D = -Dot(norm, p);
    }
};


struct Vertex {
    Vector3 pos;
    Vector3 normal;
    Vector3 uv;
    Vector3 tangent;
};