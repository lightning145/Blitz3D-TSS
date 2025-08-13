#ifndef MD_MATH_H
#define MD_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MD_MATH_PI 3.14159265358979323846f//26433832795028841971693993751058209749445923078164062862089986280348253421170679f;
//Take 100 decimal places for pi
#define MD_MATH_2PI 6.283185307179586f 
#define MD_MATH_PI_2   1.57079632679489661923f
#define MD_MATH_PI_4 0.78539816339744830962f
#define MD_MATH_3_2_PI 4.71238898038468985769f

#define MD_MATH_E 2.71828182845904523536f//02874713526624977572470936999595749669676277240766303535475945713821785251664274f;

#define MD_MATH_PIReciprocal 0.31830988618379067153f//7767526745028724068919291480912897495334688117793595268453070180227605532506171912f;

#define MD_MATH_THIRD 0.33333333333333333333f//33333333333333333333333333333333333333333333333333333333333333333333333333333333;

//The value of root number 2 must be 100 digits
#define MD_MATH_SQRT2 1.41421356237309504880f//1688724209698078569671875376948073176679737990732478462107038850387534327641573f;
//The value of root number 3 must be 100 digits
#define MD_MATH_SQRT3 1.73205080756887729352f//7446341505872366942805253810380628055806979451933016908800037081146186757248576f;

#define MD_MATH_OneHundredAndEighty 0.005555555555555556f

#define MD_MATH_1Degree 0.017453f
#define MD_MATH_1Radian 57.30f

#define MD_MATH_EPSILON 1.192092896e-07f

#define MD_MATH_LN2 0.69314718055994530942f

//Special trigonometric values
#define MD_MATH_SIN0  0.0f
#define MD_MATH_SIN30  0.5f
#define MD_MATH_SIN45  MD_MATH_SQRT2 * 0.5f
#define MD_MATH_SIN60  MD_MATH_SQRT3 * 0.5f
#define MD_MATH_SIN90  1.0f
#define MD_MATH_SIN120  MD_MATH_SQRT3 * 0.5f
#define MD_MATH_SIN150  0.5f
#define MD_MATH_SIN180  0.0f
#define MD_MATH_SIN270  -1.0f

#define MD_MATH_COS0  1.0f
#define MD_MATH_COS30  MD_MATH_SQRT3 * 0.5f
#define MD_MATH_COS45  MD_MATH_SQRT2 * 0.5f
#define MD_MATH_COS60  0.5f
#define MD_MATH_COS90  0.0f
#define MD_MATH_COS120  -1.0f * 0.5f
#define MD_MATH_COS150  -1.0f * MD_MATH_SQRT3 *0.5f
#define MD_MATH_COS180  -1.0f
#define MD_MATH_COS270  0.0f  

#define MD_MATH_TAN0  0.0f
#define MD_MATH_TAN30 MD_MATH_SQRT3 * MD_MATH_THIRD
#define MD_MATH_TAN45  1.0f
#define MD_MATH_TAN60  MD_MATH_SQRT3
#define MD_MATH_TAN120  -1.0f * MD_MATH_SQRT3
#define MD_MATH_TAN150  -1.0f * MD_MATH_SQRT3 * MD_MATH_THIRD
#define MD_MATH_TAN180  0.0f                      

namespace MD_Math{

    float Abs(float number);
    float Mod(float number1, float number2);
    float Pow(float x, int y);
    float Max(float number1, float number2);
    float Min(float number1, float number2);
    float Sqrt(float number);
    float Cbrt(float number);
    float Rsqrt(float number);
    float Hypot(float x,float y);
    float Factorial(int number);
    float Sin(float x);
    float Cos(float x);
    float Tan(float x );
    float Cot(float x);
    float ArcSin(float x);
    float ArcCos(float x);
    float ArcTan(float x);
    float ArcTan2(float y, float x);
    float Sec(float x);
    float Csc(float x);
    bool Equal(float a,float b, float epsilon);
    float EtoXPower(float x);
    float lnx(float x);
    float lgx(float x);
    float log2(float x);
    float AngularToRadian(float angle);
    float RadianToangle(float Radian);
    float ComputeRectanglePerimeter(float Length, float Width);
    float ComputeSquarePerimeter(float Length);
    float ComputeRectangleArea(float Length, float Width);
    float ComputeSquareArea(float Length);
    float ComputeTriangleArea(float Bottom, float Height);
    float ComputeParallelogramArea(float Bottom, float Height);
    float ComputeTrapezoidArea(float Top, float Bottom,float Height);
    float ComputeCirclePerimeter(float radius);
    float ComputeCircleArea(float radius);
    float ComputeCuboidVolume(float Length, float Width, float Height);
    float ComputeCubeVolume(float Length);
    float ComputeCylinderSideArea(float radius, float height);
    float ComputeConeSideArea(float radius, float busbar);
    float ComputeRoundTableSideArea(float radius1,float radius2,float busbar);
    float ComputeCylinderSurfaceArea(float radius, float height);
    float ComputeConeSurfaceArea(float radius, float busbar);
    float ComputeRoundTableSurfaceArea(float radius1,float radius2, float busbar);
    float ComputeBallSurfaceArea(float radius);
    float ComputeCylinderVolume(float radius, float height);
    float ComputeConeVolume(float radius, float height);
    float ComputeRoundTableVolume(float radius1, float radius2, float height);
    float ComputeBallVolume(float radius);

    typedef struct VECTOR2
    {
        VECTOR2(float _x, float _y);
        float x,y;

        VECTOR2 operator + (const VECTOR2& v2);
        void operator += (const VECTOR2& v2);

        VECTOR2 operator - (const VECTOR2& v2);
        void operator -= (const VECTOR2& v2);

        float operator * (const VECTOR2& v2);
        VECTOR2 operator * (const float& f);
        void operator *= (const float& f);

        bool operator == (const VECTOR2& v2);
        bool operator != (const VECTOR2& v2);

        VECTOR2 operator - ();

    } VECTOR2;

    typedef struct VECTOR3
    {
        VECTOR3(float _x, float _y, float _z);
        float x,y,z;

        VECTOR3 operator + (const VECTOR3& v2);
        void operator += (const VECTOR3& v2);

        VECTOR3 operator - (const VECTOR3& v2);
        void operator -= (const VECTOR3& v2);

        float operator * (const VECTOR3& v2);
        VECTOR3 operator * (const float& f);
        void operator *= (const float& f);

        bool operator == (const VECTOR3& v2);
        bool operator != (const VECTOR3& v2);

        VECTOR3 operator - ();

    } VECTOR3;

    typedef struct MATRIX
    {
        float _11, _12, _13, _14,
              _21, _22, _23, _24,
              _31, _32, _33, _34,
              _41, _42, _43, _44;

        MATRIX operator + (const MATRIX& m2);      
        void operator += (const MATRIX& m2);    

        MATRIX operator - (const MATRIX& m2);      
        void operator -= (const MATRIX& m2);

        MATRIX operator * (const MATRIX& m2);
        void operator *= (const MATRIX& m2);

        MATRIX operator * (const float& f);
        void operator *= (const float& f);

        bool operator == (const MATRIX& m);
        bool operator != (const MATRIX& m);

        MATRIX operator - ();

    } MATRIX;

    typedef struct VECTOR4
    {
        VECTOR4(float _x, float _y, float _z, float _w);
        float x,y,z,w;

        VECTOR4 operator + (const VECTOR4& v2);
        void operator += (const VECTOR4& v2);

        VECTOR4 operator - (const VECTOR4& v2);
        void operator -= (const VECTOR4& v2);

        float operator * (const VECTOR4& v2);
        VECTOR4 operator * (const float& f);
        void operator *= (const float& f);

        VECTOR4 operator * (const MATRIX m);
        void operator *= (const MATRIX m);

        bool operator == (const VECTOR4& v2);
        bool operator != (const VECTOR4& v2);

        VECTOR4 operator - ();
    }VECTOR4;

    typedef struct MATRIX2x2
    {
        float _11, _12, 
              _21, _22;
    } MATRIX2x2;

    typedef struct MATRIX3x3
    {
        float _11, _12, _13,
              _21, _22, _23,
              _31, _32, _33;
    } MATRIX3x3;

    MATRIX IdentityMatrix();

    typedef struct QUATERNION
    {
        QUATERNION(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f);
        float x, y, z, w;

        QUATERNION operator + (const QUATERNION& q) const;
        void operator += (const QUATERNION& q);
        
        QUATERNION operator - (const QUATERNION& q) const;
        void operator -= (const QUATERNION& q);
        
        QUATERNION operator * (const QUATERNION& q) const;
        void operator *= (const QUATERNION& q);
        
        QUATERNION operator * (float scalar) const;
        void operator *= (float scalar);
        
        QUATERNION operator / (float scalar) const;
        void operator /= (float scalar);
        
        bool operator == (const QUATERNION& q) const;
        bool operator != (const QUATERNION& q) const;
        
        QUATERNION operator - () const;

    } QUATERNION;

    float QuaternionLength(const QUATERNION& q);
    QUATERNION QuaternionNormalize(const QUATERNION& q);
    QUATERNION QuaternionConjugate(const QUATERNION& q);
    QUATERNION QuaternionInverse(const QUATERNION& q);
    float QuaternionDot(const QUATERNION& q1, const QUATERNION& q2);

    QUATERNION QuaternionFromAxisAngle(VECTOR3 axis, float angle);
    QUATERNION QuaternionFromEulerAngles(float pitch, float yaw, float roll);
    QUATERNION QuaternionFromMatrix(const MATRIX& m);

    MATRIX MatrixFromQuaternion(const QUATERNION& q);
    VECTOR3 EulerAnglesFromQuaternion(const QUATERNION& q);

    VECTOR3 RotateVector(const QUATERNION& q, const VECTOR3& v);
    QUATERNION QuaternionSlerp(const QUATERNION& q1, const QUATERNION& q2, float t);
    QUATERNION QuaternionLerp(const QUATERNION& q1, const QUATERNION& q2, float t);

    template<typename T>
    T Mix(const T& x, const T& y, float a) {
        return x * (1.0f - a) + y * a;
    }
    
    float Mix(float x, float y, float a);
    
    VECTOR2 Mix(const VECTOR2& x, const VECTOR2& y, float a);
    VECTOR3 Mix(const VECTOR3& x, const VECTOR3& y, float a);
    VECTOR4 Mix(const VECTOR4& x, const VECTOR4& y, float a);
    
    QUATERNION Mix(const QUATERNION& q1, const QUATERNION& q2, float a);

    typedef struct IVECTOR4 { // Just For Assimp
    int x, y, z, w;
    IVECTOR4() : x(0), y(0), z(0), w(0) {}
    IVECTOR4(int a, int b, int c, int d) : x(a), y(b), z(c), w(d) {}
    } IVECTOR4;

    typedef struct IVECTOR2 { // Just For FreeType
    int x, y;
    IVECTOR2() : x(0), y(0){}
    IVECTOR2(int a, int b) : x(a), y(b){}
    } IVECTOR2;


// About Vector--------------------------------------------------------------------------------

    float ComputeVector2Length(VECTOR2 v);
    float ComputeVector3Length(VECTOR3 v);
    float ComputeVector4Length(VECTOR4 v);

    VECTOR2 Vector2Normalized(VECTOR2 v);
    VECTOR3 Vector3Normalized(VECTOR3 v);
    VECTOR4 Vector4Normalized(VECTOR4 v);

    VECTOR3 VectorCross(VECTOR3 v1, VECTOR3 v2);

    float ProjectionOfVector2(VECTOR2 v1, VECTOR2 v2);
    float ProjectionOfVector3(VECTOR3 v1, VECTOR3 v2);
    float ProjectionOfVector4(VECTOR4 v1, VECTOR4 v2);

    VECTOR2 ProjectionVector2(VECTOR2 v1, VECTOR2 v2);
    VECTOR3 ProjectionVector3(VECTOR3 v1, VECTOR3 v2);
    VECTOR4 ProjectionVector4(VECTOR4 v1, VECTOR4 v2);
//---------------------------------------------------------------------------------------------------------
//About Matrix --------------------------------------------------------------------------------------------

    MATRIX MatrixTranspose(MATRIX m);    
    MATRIX AdjointMatrix(MATRIX m);
    MATRIX InvMatrix(MATRIX m);
    void MatrixToValue(MATRIX m, float* out);
    void Vector2ToValue(VECTOR2 v, float* out);
    void Vector3ToValue(VECTOR3 v, float* out);
    void Vector4ToValue(VECTOR4 v, float* out);

//Plane-------------------------------------------------------------------------------

    typedef struct PLANE  
    {
        float a,b,c,d;
    } PLANE;

    PLANE PlaneFromPointAndNormal(VECTOR3 Point,VECTOR3 Normal);

    PLANE PlaneFromPoints(VECTOR3 p0,
                          VECTOR3 p1,
                          VECTOR3 p2);
									  
    float PlaneDot(PLANE p, VECTOR3 v);
    PLANE PlaneNormalize(PLANE p);
    PLANE PlaneTransform(PLANE p, MATRIX m);

//Ray-----------------------------------------------------------------

    typedef struct RAY
    {
        VECTOR3 p0,u;
    } RAY;


    VECTOR3 CreateRay(RAY ray,float t);
    VECTOR3 Intersection(RAY ray, PLANE p);
	
    MATRIX ScaleMatrix(float x, float y, float z);
    MATRIX TranslationMatrix(float x, float y, float z);
    MATRIX RotationMatrix(float Angle, char Axis);
	
    MATRIX PerspectiveMatrixRH(float fovy, float aspect, float zNear, float zFar);	
    MATRIX OrthoMatrixRH(float l, float r, float b, float t, float n, float f);	
    MATRIX ViewMatrixRH(VECTOR3 eye, VECTOR3 target, VECTOR3 up);	
	
    MATRIX PerspectiveMatrixLH(float fovy, float aspect, float zNear, float zFar);
    MATRIX OrthoMatrixLH(float l, float r, float b, float t, float n, float f);
    MATRIX ViewMatrixLH(VECTOR3 eye, VECTOR3 target, VECTOR3 up);

    MATRIX ReflectMatrix(PLANE p);	
    MATRIX ShadowMatrix(VECTOR4 Light, PLANE p);	

    MATRIX NormalMatrix(MATRIX model);
    MATRIX BillboardMatrix(const VECTOR3& position, const MATRIX& view);
}

#endif