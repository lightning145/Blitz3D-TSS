
#include "MD_Math.h"

namespace MD_Math{

    MATRIX IdentityMatrix()
    {
		MATRIX temp = {
            1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };  
        return temp;
    }

    float Abs(float number)
    {
        if(number > 0.0f)
        {
            return number;
        }
        else if (number == 0.0f)
        {
            return 0.0f;
        }
        else if (number < 0.0f)
        {
            return -1.0f * number;
        }
        else
        {
            return 0.0f;
        }
    
    }

    float Mod(float number1, float number2)
    {
        if(number2 == 0.0f)
            return 0.0f;
    
        int y = (int)(number1 / number2);
        return number1 - (float)y * number2;
    }

    float Max(float number1, float number2)
    {
        if(number1 > number2)
        {
            return number1;
        }
        else
        {
            return number2;
        }
    
    }

    float Min(float number1, float number2)
    {
        if(number1 < number2)
        {
            return number1;
        }
        else
        {
            return number2;
        }
    
    }

    float Sqrt(float number)
    {
        long i;
	    float y;

	    y  = number;
	    i  = * ( long * ) &y;						// evil floating point bit level hacking
	    i  = ( i >> 1 ) + 0x1fc00000;               // new magic number
	    y  = * ( float * ) &i;
        y = 0.5f * ( y + number / y);// 1st iteration
        y = 0.5f * ( y + number / y);// 2nd iteration
        //y = 0.5f * ( y + number / y);// 3rd iteration

	    return y;
    }

    float Cbrt(float number)
    {
        long i;
	    float y;

	    y  = number;
	    i  = * ( long * ) &y;					 // evil floating point bit level hacking
	    i  = ( i / 3 ) + 0x2a9f84fe;               // new magic number
	    y  = * ( float * ) &i;
        y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 1st iteration
        y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 2nd iteration
        y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 3rd iteration
        y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 4th iteration

	    return y;
    }

    //From https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_math.c#L552
    float Rsqrt(float number)
    {
        long i;
	    float x2, y;
	    const float threehalfs = 1.5F;

	    x2 = number * 0.5F;
	    y  = number;
    	i  = * ( long * ) &y;						// evil floating point bit level hacking
    	i  = 0x5f375a86 - ( i >> 1 );               // what the fuck?
    	y  = * ( float * ) &i;
    	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    
	    return y;
    }

    float Hypot(float x,float y)
    {
        return Sqrt((x*x + y*y));
    }

    float Factorial(int number)
    {
        float r = 1.0f;
    
        float f= 1.0f;
        for (int i = 1; i < number; i++)
        {
            f = f + 1.0f;
            r = r * f;

        }

        return r;
    
    }

    float Pow(float x, int y)
    {
        float r = 1.0f ;

        for (int i = 0; i < y; i++)
        {
            r = r * x;
        }
    
        return r;
    }

    float Sin(float x)
    {
	   int i;
	   int j;
       float r = 0.0f;

       int k = (int)(x / MD_MATH_2PI);

       if(k < 0)
       {     
            for (j = 0; j < (-1) * k; j++)
            {
                x = x + MD_MATH_2PI;
            }
        }
    
        for (j = 0; j < k; j++)
        {
            x = x - MD_MATH_2PI;
        }

       for (i = 0; i < 13; i++)
       {

        r = r + ((Pow(-1.0f,i)/ Factorial(2 * i + 1)) *
                 Pow(x,(2 * i + 1)));
       }
   
       return r;
    }

    float Cos(float x) 
    {
       int i;
	   int j;

       float r = 0.0f;

       int k = (int)(x / MD_MATH_2PI);
    
        if(k < 0)
        {
            for (j = 0; j < (-1) * k; j++)
            {
                x = x + MD_MATH_2PI;
            }
        }

        for (j = 0; j < k; j++)
        {
             x = x - MD_MATH_2PI;
        }

       for (i = 0; i < 13; i++)
       {
        r = r +((Pow(-1.0f,i)/ Factorial(2 * i)) *
                 Pow(x,(2 * i)));
       }
   
       return r;   
    }

    float Tan(float x )
    {
        return Sin(x) / Cos(x);
    }

    float Cot(float x)
    {
        return Cos(x)/Sin(x);
    }

    float ArcSin(float x)
    {
		int i;

        float r = 0.0f;

        for (i = 0; i < 8; i++)
        {
            r = r + (
                (Factorial(2 * i))/(Pow(4.0f, i) * (2 * i + 1) * (Pow(Factorial(i),2)))
            ) * 
            (Pow(x, 2 * i + 1));
        }
    
        return r;
    }

    float ArcCos(float x)
    {
        return MD_MATH_PI * 0.5f - ArcSin(x);
    }

    float ArcTan(float x)
    {
        float r = 0.0f;

        for (int i = 0; i < 24; i++)
        {
            r = r + (Pow(-1.0f, i) / (float)(2 * i + 1) ) * 
                    (Pow(x, 2 * i + 1));
        }
    
        return r;
    }

    float Sec(float x)
    {
        return 1.0f/ Cos(x);
    }

    float Csc(float x)
    {
        return 1.0f / Sin(x);
    }

    bool Equal(float a,float b, float epsilon)
    {
        if(Abs(a - b) < epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    float EtoXPower(float x)
    {
        return 1 + x + (x*x)/Factorial(2)+
        (x*x*x)/Factorial(3)+
        (x*x*x*x)/Factorial(4)+
        (x*x*x*x*x)/Factorial(5)+
        (x*x*x*x*x*x)/Factorial(6)+
        (x*x*x*x*x*x*x)/Factorial(7)+
        (x*x*x*x*x*x*x*x)/Factorial(8)+
        (x*x*x*x*x*x*x*x*x)/Factorial(9)+
        (x*x*x*x*x*x*x*x*x*x)/Factorial(10)+
        (x*x*x*x*x*x*x*x*x*x*x)/Factorial(11)+
        (x*x*x*x*x*x*x*x*x*x*x*x)/Factorial(12);
    }

    float lnx(float x)
    {
        if (x <= 0.0) {
            return 0.0; 
        }

        int k = 0;
        while (x > 2.0) {
            x /= 2.0;
            k++;
        }

        float t = x - 1.0f;  
        float result = 0.0;
        float term = t;     
        float sign = 1.0;  
        int n = 1;           

        while (n <= 13) {
            double current = sign * term / n;
            result += (float)current;

            term *= t;      
            sign = -sign; 
            n++;           
        }

        result += k * MD_MATH_LN2;

        return result;
    
    }

    float lgx(float x)
    {
        return lnx(x) * 0.43429448190325176f;
    }

    float log2(float x)
    {
        return lnx(x) * 1.4426950408889634f;
    }

    float AngularToRadian(float angle)
    {
        return angle * MD_MATH_PI * MD_MATH_OneHundredAndEighty;
    }

    float RadianToangle(float Radian)
    {
        return Radian * 180.0f * MD_MATH_PIReciprocal;
    }

    float ComputeRectanglePerimeter(float Length, float Width)
    {
        return (Length + Width) * 2;
    }

    float ComputeSquarePerimeter(float Length)
    {
        return Length * 4;
    }

    float ComputeRectangleArea(float Length, float Width)
    {
        return Length * Width;
    }

    float ComputeSquareArea(float Length)
    {
        return Length * Length;
    }

    float ComputeTriangleArea(float Bottom, float Height)
    {
        return Bottom * Height * 0.5f; 
    }

    float ComputeParallelogramArea(float Bottom, float Height)
    {
        return Bottom * Height;
    }

    float ComputeTrapezoidArea(float Top, float Bottom,float Height)
    {
        return (Top + Bottom) * Height * 0.5f;
    }

    float ComputeCirclePerimeter(float radius)
    {
        return 2.0f * MD_MATH_PI * radius;
    }

    float ComputeCircleArea(float radius)
    {
        return MD_MATH_PI * radius * radius;
    }

    float ComputeCuboidVolume(float Length, float Width, float Height)
    {
        return Length * Width * Height;
    }

    float ComputeCubeVolume(float Length)
    {
        return Length * Length;
    }

    float ComputeCylinderSideArea(float radius, float height)
    {
        return 2.0f * MD_MATH_PI * radius * height;
    }

    float ComputeConeSideArea(float radius, float busbar)
    {
        return 2.0f * MD_MATH_PI * radius * busbar;
    }

    float ComputeRoundTableSideArea(float radius1,float radius2,float busbar)
    {
        return MD_MATH_PI * (radius1 + radius2) * busbar;
    }

    float ComputeCylinderSurfaceArea(float radius, float height)
    {
        return ComputeCylinderSideArea(radius,height) + ComputeCircleArea(radius);
    }

    float ComputeConeSurfaceArea(float radius, float busbar)
    {
        return ComputeCircleArea(radius) + MD_MATH_PI * radius * busbar;
    }

    float ComputeRoundTableSurfaceArea(float radius1,float radius2, float busbar)
    {
        return ComputeCircleArea(radius1) + ComputeCircleArea(radius2) + MD_MATH_PI * radius1 * busbar + MD_MATH_PI * radius2 * busbar;
    }

    float ComputeBallSurfaceArea(float radius)
    {
        return 4.0f * MD_MATH_PI * radius;
    }

    float ComputeCylinderVolume(float radius, float height)
    {
        return ComputeCircleArea(radius) * height;
    }

    float ComputeConeVolume(float radius, float height)
    {
        return MD_MATH_THIRD * ComputeCylinderVolume(radius,height);
    }

    float ComputeRoundTableVolume(float radius1, float radius2, float height)
    {
        return MD_MATH_THIRD * height * (ComputeCircleArea(radius1) + ComputeCircleArea(radius2) + MD_MATH_PI * (radius1 + radius2));
    }

    float ComputeBallVolume(float radius)
    {
        return 4.0f * MD_MATH_THIRD * MD_MATH_PI * radius * radius * radius;
    }

    // About Vector--------------------------------------------------------------------------------
    VECTOR2::VECTOR2(float _x, float _y)
    {
        x = _x; y = _y;
    }

    VECTOR2 VECTOR2::operator + (const VECTOR2& v2)
    {
		VECTOR2 temp(x + v2.x, y + v2.y);
        return temp;
    }

    void VECTOR2::operator += (const VECTOR2& v2)
    {
        x = x + v2.x;
        y = y + v2.y;
    }

    VECTOR2 VECTOR2::operator - (const VECTOR2& v2)
    {
		VECTOR2 temp(x - v2.x, y - v2.y);
        return temp;
    }

    void VECTOR2::operator -= (const VECTOR2& v2)
    {
        x = x - v2.x;
        y = y - v2.y;
    }

    float VECTOR2::operator * (const VECTOR2& v2)
    {
        return 
           x * v2.x +
           y * v2.y
        ;
    }

    VECTOR2 VECTOR2::operator * (const float& f)
    {
		VECTOR2 temp(f * x,
            f * y);
        return temp;
    }

    void VECTOR2::operator *= (const float& f)
    {
        x = f * x;
        y = f * y;
    }

    bool VECTOR2::operator == (const VECTOR2& v2)
    {
        if(
            Equal(x,v2.x,MD_MATH_EPSILON) &&
            Equal(y,v2.y,MD_MATH_EPSILON)
        )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool VECTOR2::operator != (const VECTOR2& v2)
    {
        if(!(
            Equal(x,v2.x,MD_MATH_EPSILON) &&
            Equal(y,v2.y,MD_MATH_EPSILON)
            )
        )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    VECTOR3::VECTOR3(float _x, float _y, float _z)
    {
        x = _x; y = _y; z = _z;
    }

    VECTOR3 VECTOR3::operator + (const VECTOR3& v2)
    {
		VECTOR3 temp(x + v2.x,
            y + v2.y,
            z + v2.z);
        return temp;
    }

    void VECTOR3::operator += (const VECTOR3& v2)
    {
        x = x + v2.x;
        y = y + v2.y;
        z = z + v2.z;
    }

    VECTOR3 VECTOR3::operator - (const VECTOR3& v2)
    {
		VECTOR3 temp(
            x - v2.x,
            y - v2.y,
            z - v2.z
			);
        return temp;
    }

    void VECTOR3::operator -= (const VECTOR3& v2)
    {
        x = x - v2.x;
        y = y - v2.y;
        z = z - v2.z;
    }

    float VECTOR3::operator * (const VECTOR3& v2)
    {
        return 
            x * v2.x + y * v2.y + z * v2.z 
        ;
    }

    VECTOR3 VECTOR3::operator * (const float& f)
    {
		VECTOR3 temp(
			f * x,
            f * y,
            f * z
			);
        return temp;
    }

    void VECTOR3::operator *= (const float& f)
    {
        x = f * x;
        y = f * y;
        z = f * z;
    }

    bool VECTOR3::operator == (const VECTOR3& v2)
    {
        if(
            Equal(x,v2.x,MD_MATH_EPSILON) &&
            Equal(y,v2.y,MD_MATH_EPSILON) &&
            Equal(z,v2.z,MD_MATH_EPSILON) 
        )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool VECTOR3::operator != (const VECTOR3& v2)
    {
        if(!(
            Equal(x,v2.x,MD_MATH_EPSILON) &&
            Equal(y,v2.y,MD_MATH_EPSILON) &&
            Equal(z,v2.z,MD_MATH_EPSILON) 
        )
        )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //---------------------------------------------------------------------------------------------------------
    //About Matrix --------------------------------------------------------------------------------------------

    MATRIX MATRIX::operator + (const MATRIX& m2)
    {
        MATRIX temp = {
			_11 + m2._11,
            _21 + m2._21,
            _31 + m2._31,
            _41 + m2._41,

            _12 + m2._12,
            _22 + m2._22,
            _32 + m2._32,
            _42 + m2._42,
    
            _13 + m2._13,
            _23 + m2._23,
            _33 + m2._33,
            _43 + m2._43,

            _14 + m2._14,
            _24 + m2._24,
            _34 + m2._34,
            _44 + m2._44
		};

        return temp;
    }  

    void MATRIX::operator += (const MATRIX& m2)
    {
        _11 = _11 + m2._11;
        _21 = _21 + m2._21;
        _31 = _31 + m2._31;
        _41 = _41 + m2._41;

        _12 = _12 + m2._12;
        _22 = _22 + m2._22;
        _32 = _32 + m2._32;
        _42 = _42 + m2._42;
    
       _13 = _13 + m2._13;
       _23 = _23 + m2._23;
       _33 = _33 + m2._33;
       _43 = _43 + m2._43;

       _14 = _14 + m2._14;
       _24 = _24 + m2._24;
       _34 = _34 + m2._34;
       _44 = _44 + m2._44;
    }  

    MATRIX MATRIX::operator - (const MATRIX& m2)
    {
		MATRIX temp = {
			_11 - m2._11,
            _21 - m2._21,
            _31 - m2._31,
            _41 - m2._41,

            _12 - m2._12,
            _22 - m2._22,
            _32 - m2._32,
            _42 - m2._42,
    
            _13 - m2._13,
            _23 - m2._23,
            _33 - m2._33,
            _43 - m2._43,

            _14 - m2._14,
            _24 - m2._24,
            _34 - m2._34,
            _44 - m2._44
		};
        return temp;
    }   

    void MATRIX::operator -= (const MATRIX& m2)
    {
        _11 = _11 - m2._11;
        _21 = _21 - m2._21;
        _31 = _31 - m2._31;
        _41 = _41 - m2._41;

        _12 = _12 - m2._12;
        _22 = _22 - m2._22;
        _32 = _32 - m2._32;
        _42 = _42 - m2._42;
    
        _13 = _13 - m2._13;
        _23 = _23 - m2._23;
        _33 = _33 - m2._33;
        _43 = _43 - m2._43;

        _14 = _14 - m2._14;
        _24 = _24 - m2._24;
        _34 = _34 - m2._34;
        _44 = _44 - m2._44;
    } 

    MATRIX MATRIX::operator * (const MATRIX& m2)
    {
		MATRIX temp = {
			m2._11 * _11 + m2._21 * _12 + m2._31 * _13 + m2._41 * _14,
            m2._12 * _11 + m2._22 * _12 + m2._32 * _13 + m2._42 * _14,
            m2._13 * _11 + m2._23 * _12 + m2._33 * _13 + m2._43 * _14,
            m2._14 * _11 + m2._24 * _12 + m2._34 * _13 + m2._44 * _14,

            m2._11 * _21 + m2._21 * _22 + m2._31 * _23 + m2._41 * _24,
            m2._12 * _21 + m2._22 * _22 + m2._32 * _23 + m2._42 * _24,
            m2._13 * _21 + m2._23 * _22 + m2._33 * _23 + m2._43 * _24,
            m2._14 * _21 + m2._24 * _22 + m2._34 * _23 + m2._44 * _24,

            m2._11 * _31 + m2._21 * _32 + m2._31 * _33 + m2._41 * _34,
            m2._12 * _31 + m2._22 * _32 + m2._32 * _33 + m2._42 * _34,
            m2._13 * _31 + m2._23 * _32 + m2._33 * _33 + m2._43 * _34,
            m2._14 * _31 + m2._24 * _32 + m2._34 * _33 + m2._44 * _34,

            m2._11 * _41 + m2._21 * _42 + m2._31 * _43 + m2._41 * _44,
            m2._12 * _41 + m2._22 * _42 + m2._32 * _43 + m2._42 * _44,
            m2._13 * _41 + m2._23 * _42 + m2._33 * _43 + m2._43 * _44,
            m2._14 * _41 + m2._24 * _42 + m2._34 * _43 + m2._44 * _44,
		};
        return temp;
    }

    void MATRIX::operator *= (const MATRIX& m2)
    {
        _11 = m2._11 * _11 + m2._21 * _12 + m2._31 * _13 + m2._41 * _14;
        _12 = m2._12 * _11 + m2._22 * _12 + m2._32 * _13 + m2._42 * _14;
        _13 = m2._13 * _11 + m2._23 * _12 + m2._33 * _13 + m2._43 * _14;
        _14 = m2._14 * _11 + m2._24 * _12 + m2._34 * _13 + m2._44 * _14;

        _21 = m2._11 * _21 + m2._21 * _22 + m2._31 * _23 + m2._41 * _24;
        _22 = m2._12 * _21 + m2._22 * _22 + m2._32 * _23 + m2._42 * _24;
        _23 = m2._13 * _21 + m2._23 * _22 + m2._33 * _23 + m2._43 * _24;
        _24 = m2._14 * _21 + m2._24 * _22 + m2._34 * _23 + m2._44 * _24;

        _31 = m2._11 * _31 + m2._21 * _32 + m2._31 * _33 + m2._41 * _34;
        _32 = m2._12 * _31 + m2._22 * _32 + m2._32 * _33 + m2._42 * _34;
        _33 = m2._13 * _31 + m2._23 * _32 + m2._33 * _33 + m2._43 * _34;
        _34 = m2._14 * _31 + m2._24 * _32 + m2._34 * _33 + m2._44 * _34;

        _41 = m2._11 * _41 + m2._21 * _42 + m2._31 * _43 + m2._41 * _44;
        _42 = m2._12 * _41 + m2._22 * _42 + m2._32 * _43 + m2._42 * _44;
        _43 = m2._13 * _41 + m2._23 * _42 + m2._33 * _43 + m2._43 * _44;
        _44 = m2._14 * _41 + m2._24 * _42 + m2._34 * _43 + m2._44 * _44;
    }

    MATRIX MATRIX::operator * (const float& f)
    {
		MATRIX temp = {
			f * _11,
            f * _21, 
            f * _31, 
            f * _41, 

            f * _12, 
            f * _22, 
            f * _32, 
            f * _42, 

            f * _13, 
            f * _23, 
            f * _33, 
            f * _43, 

            f * _14, 
            f * _24, 
            f * _34, 
            f * _44 
		};
        return temp;
    }

    void MATRIX::operator *= (const float& f)
    {
        _11 = f * _11; 
        _21 = f * _21; 
        _31 = f * _31; 
        _41 = f * _41; 

        _12 = f * _12; 
        _22 = f * _22; 
        _32 = f * _32; 
        _42 = f * _42; 

        _13 = f * _13; 
        _23 = f * _23; 
        _33 = f * _33; 
        _43 = f * _43; 

        _14 = f * _14; 
        _24 = f * _24; 
        _34 = f * _34; 
        _44 = f * _44; 
    }

    bool MATRIX::operator == (const MATRIX& m)
    {
        if(
            Equal(_11,m._11,MD_MATH_EPSILON) &&
            Equal(_21,m._21,MD_MATH_EPSILON) &&
            Equal(_31,m._31,MD_MATH_EPSILON) &&
            Equal(_41,m._41,MD_MATH_EPSILON) &&

            Equal(_12,m._12,MD_MATH_EPSILON) &&
            Equal(_22,m._22,MD_MATH_EPSILON) &&
            Equal(_32,m._32,MD_MATH_EPSILON) &&
            Equal(_42,m._42,MD_MATH_EPSILON) &&

            Equal(_13,m._13,MD_MATH_EPSILON) &&
            Equal(_23,m._23,MD_MATH_EPSILON) &&
            Equal(_33,m._33,MD_MATH_EPSILON) &&
            Equal(_43,m._43,MD_MATH_EPSILON) &&

            Equal(_14,m._14,MD_MATH_EPSILON) &&
            Equal(_24,m._24,MD_MATH_EPSILON) &&
            Equal(_34,m._34,MD_MATH_EPSILON) &&
            Equal(_44,m._44,MD_MATH_EPSILON) 
        )
        {
            return true;
        }
        else{
            return false;
        }
    }

    bool MATRIX::operator != (const MATRIX& m)
    {
        if(!(
            Equal(_11,m._11,MD_MATH_EPSILON) &&
            Equal(_21,m._21,MD_MATH_EPSILON) &&
            Equal(_31,m._31,MD_MATH_EPSILON) &&
            Equal(_41,m._41,MD_MATH_EPSILON) &&

            Equal(_12,m._12,MD_MATH_EPSILON) &&
            Equal(_22,m._22,MD_MATH_EPSILON) &&
            Equal(_32,m._32,MD_MATH_EPSILON) &&
            Equal(_42,m._42,MD_MATH_EPSILON) &&

            Equal(_13,m._13,MD_MATH_EPSILON) &&
            Equal(_23,m._23,MD_MATH_EPSILON) &&
            Equal(_33,m._33,MD_MATH_EPSILON) &&
            Equal(_43,m._43,MD_MATH_EPSILON) &&

            Equal(_14,m._14,MD_MATH_EPSILON) &&
            Equal(_24,m._24,MD_MATH_EPSILON) &&
            Equal(_34,m._34,MD_MATH_EPSILON) &&
            Equal(_44,m._44,MD_MATH_EPSILON) 
        )
        )
        {
            return true;
        }
        else{
            return false;
        }
    }

    VECTOR4::VECTOR4(float _x, float _y, float _z, float _w)
    {
        x = _x; y = _y; z = _z; w = _w;
    }

    VECTOR4 VECTOR4::operator + (const VECTOR4& v2)
    {
		VECTOR4 temp(
			x + v2.x,
            y + v2.y,
            z + v2.z,
            w + v2.w
			);
        return temp;
    }

    void VECTOR4::operator += (const VECTOR4& v2)
    {
        x = x + v2.x;
        y = y + v2.y;
        z = z + v2.z;
        w = w + v2.w;
    }

    VECTOR4 VECTOR4::operator - (const VECTOR4& v2)
    {
		VECTOR4 temp(
			x - v2.x,
            y - v2.y,
            z - v2.z,
            w - v2.w
			);
        return temp;
    }
 
    void VECTOR4::operator -= (const VECTOR4& v2)
    {
        x = x - v2.x;
       y = y - v2.y;
        z = z - v2.z;
        w = w - v2.w;
    }

    float VECTOR4::operator * (const VECTOR4& v2)
    {
        return x * v2.x + y * v2.y + z * v2.z + w * v2.w;
    }

    VECTOR4 VECTOR4::operator * (const float& f)
    {
		VECTOR4 temp(
			f * x,
            f * y,
            f * z,
            f * w
			);
        return temp;
    }

    void VECTOR4::operator *= (const float& f)
    {
        x = f * x;
        y = f * y;
        z = f * z;
        w = f * w;
    }

    bool VECTOR4::operator == (const VECTOR4& v2)
    {
        if(
            Equal(x,v2.x,MD_MATH_EPSILON) &&
            Equal(y,v2.y,MD_MATH_EPSILON) &&
            Equal(z,v2.z,MD_MATH_EPSILON) &&
            Equal(w,v2.w,MD_MATH_EPSILON)
        )
        {
            return true;
        }
        else
        {
            return false;
        }    
    }

    bool VECTOR4::operator != (const VECTOR4& v2)
    {
        if(!(
            Equal(x,v2.x,MD_MATH_EPSILON) &&
            Equal(y,v2.y,MD_MATH_EPSILON) &&
            Equal(z,v2.z,MD_MATH_EPSILON) &&
            Equal(w,v2.w,MD_MATH_EPSILON)
        )
        )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    VECTOR4 VECTOR4::operator * (const MATRIX m)
    {
		VECTOR4 temp(
			x * m._11 + y * m._12 + z * m._13 + w * m._14,
            x * m._21 + y * m._22 + z * m._23 + w * m._24,
            x * m._31 + y * m._32 + z * m._33 + w * m._34,
            x * m._41 + y * m._42 + z * m._43 + w * m._44
			);
        return temp;
    }

    void VECTOR4::operator *= (const MATRIX m)
    {
        x = x * m._11 + y * m._12 + z * m._13 + w * m._14;
        y = x * m._21 + y * m._22 + z * m._23 + w * m._24;
        z = x * m._31 + y * m._32 + z * m._33 + w * m._34;
        w = x * m._41 + y * m._42 + z * m._43 + w * m._44;
    }

    float ComputeVector2Length(VECTOR2 v)
    {
        return Hypot(v.x,v.y);
    }

    float ComputeVector3Length(VECTOR3 v)
    {
        return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float ComputeVector4Length(VECTOR4 v)
    {
        return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }

    VECTOR2 Vector2Normalized(VECTOR2 v)
    {
        VECTOR2 r(0.0f,0.0f);
        r.x = v.x * Rsqrt(v.x*v.x + v.y * v.y); 
        r.y = v.y * Rsqrt(v.x*v.x + v.y * v.y); 
        return r;
    }

    VECTOR3 Vector3Normalized(VECTOR3 v)
    {
        VECTOR3 r(0.0f, 0.0f, 0.0f);
        r.x = v.x * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z); 
        r.y = v.y * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z); 
        r.z = v.z * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z); 
        return r;
    }

    VECTOR4 Vector4Normalized(VECTOR4 v)
    {
        VECTOR4 r(0.0f, 0.0f, 0.0f, 0.0f);
        r.x = v.x * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w); 
        r.y = v.y * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w);
        r.z = v.z * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w); 
        r.w = v.w * Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w);  
        return r;
    }

    VECTOR3 VectorCross(VECTOR3 v1, VECTOR3 v2)
    {
        VECTOR3 r(0.0f, 0.0f ,0.0f);
        r.x = v1.y * v2.z - v2.y * v1.z;
        r.y = v1.z * v2.x - v2.z * v1.x;
        r.z = v1.x * v2.y - v2.x * v1.y;
        return r;
    }

    float ProjectionOfVector2(VECTOR2 v1, VECTOR2 v2)
    {
        return 
            ComputeVector2Length(v1) * 
            Cos(ArcCos((v1*v2) * Rsqrt(v1.x * v1.x + v1.y * v1.y)*
            Rsqrt(v2.x* v2.x + v2.y*v2.y )))
        ;
    }

    float ProjectionOfVector3(VECTOR3 v1, VECTOR3 v2)
    {
        return 
            ComputeVector3Length(v1) * 
            Cos(ArcCos((v1*v2) * Rsqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z)*
                Rsqrt(v2.x* v2.x + v2.y*v2.y + v2.z * v2.z)))
        ;
    }

    float ProjectionOfVector4(VECTOR4 v1, VECTOR4 v2)
    {
        return 
            ComputeVector4Length(v1) * 
            Cos(ArcCos((v1*v2) * Rsqrt(v1.x * v1.x + v1.y * v1.y+ v1.z * v1.z+ v1.w * v1.w)*
                Rsqrt(v2.x* v2.x + v2.y*v2.y + v1.z * v1.z+ v1.w * v1.w)))
        ;
    }

    VECTOR2 ProjectionVector2(VECTOR2 v1, VECTOR2 v2)
    {
        VECTOR2 r = Vector2Normalized(v2);
        return r * ProjectionOfVector2(v1,v2);
    }

    VECTOR3 ProjectionVector3(VECTOR3 v1, VECTOR3 v2)
    {
        VECTOR3 r = Vector3Normalized(v2);
        return r * ProjectionOfVector3(v1,v2);
    }

    VECTOR4 ProjectionVector4(VECTOR4 v1, VECTOR4 v2)
    {
        VECTOR4 r = Vector4Normalized(v2);
        return r * ProjectionOfVector4(v1,v2);
    }

    MATRIX MatrixTranspose(MATRIX m)
    {
        MATRIX r;

        r._11 = m._11;
        r._21 = m._12;
        r._31 = m._13;
        r._41 = m._14;

        r._12 = m._21;
        r._22 = m._22;
        r._32 = m._23;
        r._42 = m._24;

        r._13 = m._31;
        r._23 = m._32;
        r._33 = m._33;
        r._43 = m._34;

        r._14 = m._41;
        r._24 = m._42;
        r._34 = m._43;
        r._44 = m._44;
    
        return r;
    }

    float DetMatrix2x2(MATRIX2x2 m)
    {
        return m._11 * m._22 - m._12 * m._21;
    }

    float DetMatrix3x3(MATRIX3x3 m)
    {
        MATRIX2x2 m1 = {m._22,m._23,
                        m._32,m._33};

        MATRIX2x2 m2 = {m._21,m._23,
                        m._31,m._33};

        MATRIX2x2 m3 = {m._21,m._22,
                        m._31,m._32};

        return m._11 * DetMatrix2x2(m1) - m._12 * DetMatrix2x2(m2) + m._13 * DetMatrix2x2(m3);  
    }

    float DetMatrix4x4(MATRIX m)
    {
        MATRIX3x3 m1 = {m._22,m._23,m._24,
                        m._32,m._33,m._34,
                        m._42,m._43,m._44};       

        MATRIX3x3 m2 = {m._21,m._23,m._24,
                        m._31,m._33,m._34,
                        m._41,m._43,m._44};

        MATRIX3x3 m3 = {m._21,m._22,m._24,
                        m._31,m._32,m._34,
                        m._41,m._42,m._44};      

        MATRIX3x3 m4 = {m._21,m._22,m._23,
                        m._31,m._32,m._33,
                        m._41,m._42,m._43};

        return m._11 * DetMatrix3x3(m1) - m._12 * DetMatrix3x3(m2) + m._13 * DetMatrix3x3(m3) - m._14 *DetMatrix3x3(m4);                                                                                                                                                
    }

    MATRIX AdjointMatrix(MATRIX m)
    {
        MATRIX rm;
        MATRIX adj;      
 
        MATRIX3x3 m1 = {m._22, m._23,m._24,
                        m._32,m._33,m._34,
                        m._42,m._43,m._44};
        MATRIX3x3 m2 = {m._21,m._23,m._24,
                        m._31,m._33,m._34,
                        m._41,m._43,m._44};
        MATRIX3x3 m3 = {m._21,m._22,m._24,
                        m._31,m._32,m._34,
                        m._41,m._42,m._44};
        MATRIX3x3 m4 = {m._21,m._22,m._23,
                        m._31,m._32,m._33,
                        m._41,m._42,m._43};

        MATRIX3x3 m5 = {m._12,m._13,m._14,
                        m._32,m._33,m._34,
                        m._42,m._43,m._44};
        MATRIX3x3 m6 = {m._11,m._13,m._14,
                        m._31,m._33,m._34,
                        m._41,m._43,m._44};
        MATRIX3x3 m7 = {m._11,m._12,m._14,
                        m._31,m._32,m._34,
                        m._41,m._42,m._44};
        MATRIX3x3 m8 = {m._11,m._12,m._13,
                        m._31,m._32,m._33,
                        m._41,m._42,m._43};

        MATRIX3x3 m9 = {m._12,m._13,m._14,
                        m._22,m._23,m._24,
                        m._42,m._43,m._44};
        MATRIX3x3 m10 = {m._12,m._13,m._14,
                        m._21,m._23,m._24,
                        m._41,m._43,m._44};
        MATRIX3x3 m11 = {m._11,m._12,m._14,
                        m._21,m._22,m._24,
                        m._41,m._42,m._44};
        MATRIX3x3 m12 = {m._11,m._12,m._13,
                        m._21,m._22,m._23,
                        m._41,m._42,m._43};

        MATRIX3x3 m13 = {m._12,m._13,m._14,
                        m._22,m._23,m._24,
                        m._32,m._33,m._34};
        MATRIX3x3 m14 = {m._11,m._13,m._14,
                        m._21,m._23,m._24,
                        m._31,m._33,m._34};
        MATRIX3x3 m15 = {m._11,m._12,m._14,
                        m._21,m._22,m._24,
                        m._31,m._32,m._34};
        MATRIX3x3 m16 = {m._11,m._12,m._13,
                        m._21,m._22,m._23,
                        m._31,m._32,m._33};

        rm._11 = DetMatrix3x3(m1);
        rm._12 = (-1) * DetMatrix3x3(m2);
        rm._13 = DetMatrix3x3(m3);
        rm._14 = (-1) * DetMatrix3x3(m4);

        rm._21 = (-1) * DetMatrix3x3(m5);
        rm._22 = DetMatrix3x3(m6);
        rm._23 = (-1) * DetMatrix3x3(m7);
        rm._24 = DetMatrix3x3(m8);

        rm._31 = DetMatrix3x3(m9);
        rm._32 = (-1) * DetMatrix3x3(m10);
        rm._33 = DetMatrix3x3(m11);
        rm._34 = (-1) * DetMatrix3x3(m12);

        rm._41 = (-1) * DetMatrix3x3(m13);
        rm._42 = DetMatrix3x3(m14);
        rm._43 = (-1) * DetMatrix3x3(m15);
        rm._44 = DetMatrix3x3(m16);

        adj._11 = rm._11;
        adj._12 = rm._21;
        adj._13 = rm._31;
        adj._14 = rm._41;

        adj._21 = rm._12;
        adj._22 = rm._22;
        adj._23 = rm._32;
        adj._24 = rm._42;

        adj._31 = rm._13;
        adj._32 = rm._23;
        adj._33 = rm._33;
        adj._34 = rm._43;

        adj._41 = rm._14;
        adj._42 = rm._24;
        adj._43 = rm._34;
        adj._44 = rm._44;

        return adj;
    }

    MATRIX InvMatrix(MATRIX m)
    {
       MATRIX Inv,adj;
   
       float det = DetMatrix4x4(m);

       if(Abs(det) < MD_MATH_EPSILON)
       {
        return IdentityMatrix();
       }

       float _1det = 1.0f / det;
       adj = AdjointMatrix(m);

       Inv = adj * _1det;
    
        return Inv;
    }

    void MatrixToValue(MATRIX m, float* out) {
	    out[0] = m._11; out[1] = m._21; out[2] = m._31; out[3] = m._41;
    	out[4] = m._12; out[5] = m._22; out[6] = m._32; out[7] = m._42;
    	out[8] = m._13; out[9] = m._23; out[10] = m._33; out[11] = m._43;
    	out[12] = m._14; out[13] = m._24; out[14] = m._34; out[15] = m._44;
    }

    void Vector2ToValue(VECTOR2 v, float* out)
    {
        out[0] = v.x; out[1] = v.y;
    }

    void Vector3ToValue(VECTOR3 v, float* out)
    {
        out[0] = v.x; out[1] = v.y; out[2] = v.z;
    }

    void Vector4ToValue(VECTOR4 v, float* out)
    {
        out[0] = v.x; out[1] = v.y; out[2] = v.z; out[3] = v.w;
    }

    //Plane-------------------------------------------------------------------------------

    PLANE PlaneFromPointAndNormal(VECTOR3 Point,VECTOR3 Normal)
    {
        PLANE plane;
        plane.a = Point.x;
        plane.b = Point.y;
        plane.c = Point.z;
        plane.d = (-1.0f) * (Normal * Point);

        return plane;
    }

    PLANE PlaneFromPoints(VECTOR3 p0,
                            VECTOR3 p1,
                                VECTOR3 p2)
    {
        PLANE plane;
        VECTOR3 u = p1 - p0;
        VECTOR3 v = p2 - p0;
        VECTOR3 n = VectorCross(u,v);

        plane.a = n.x;
        plane.b = n.y;
        plane.c = n.z;
        plane.d = (-1.0f) * (n * p0);

        return plane;
    }

    float PlaneDot(PLANE p, VECTOR3 v)
    {
        VECTOR3 n(0.0f, 0.0f, 0.0f);
        float r;
        n.x = p.a;
        n.y = p.b;
        n.y = p.c;

        r = (n * v) + p.d;
        return r;
    }

    PLANE PlaneNormalize(PLANE p)
    {
        PLANE r;
        float nn;

        nn = Rsqrt(p.a * p.a + p.b * p.b + p.c * p.c);

        r.a = nn * p.a;
        r.b = nn * p.b;
        r.c = nn * p.c;
        r.d = nn * p.d;

        return r;
    
    }

    PLANE PlaneTransform(PLANE p, MATRIX m)
    {
        PLANE r;
        VECTOR4 pp(0.0f, 0.0f, 0.0f ,0.0f);
        p = PlaneNormalize(p);

        pp.x = p.a;
        pp.y = p.b;
        pp.z = p.c;
        pp.w = p.d;

        m = InvMatrix(m);
        m = MatrixTranspose(m);

        pp *= m;

        r.a = pp.x;
        r.b = pp.y;
        r.c = pp.z;
        r.d = pp.w;

        return r;
    }

    //Ray-----------------------------------------------------------------
    VECTOR3 CreateRay(RAY ray,float t)
    {
        VECTOR3 r(0.0f, 0.0f, 0.0f);

        if(t < 0.0f)
        {
            r.x = 0.0f;
            r.y = 0.0f;
            r.z = 0.0f;
            return r;
        }

        r = ray.p0 + ray.u * t;

        return r;

    }

    VECTOR3 Intersection(RAY ray, PLANE p)
    {
        VECTOR3 r(0.0f, 0.0f, 0.0f);
        VECTOR3 n(0.0f, 0.0f, 0.0f);
        n.x = p.a;
        n.y = p.b;
        n.z = p.c;
        float t;

        t = ((-1) * p.d - (n * ray.p0))/(n * ray.u);

        r = ray.p0 + ray.u * t;

        return r;
    }

    MATRIX ScaleMatrix(float x, float y, float z)
    {
        MATRIX r = {
            x,0.0f,0.0f,0.0f,
            0.0f,y,0.0f,0.0f,
            0.0f,0.0f,z,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };

        return r;
    }

    MATRIX TranslationMatrix(float x, float y, float z)
    {
        MATRIX r = {
            1.0f, 0.0f ,0.0f ,x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f ,z,
            0.0f, 0.0f, 0.0f ,1.0f
        };

        return r;
    }

    MATRIX RotationMatrix(float Angle, char Axis)
    {
	
	    if(Axis == 'X')
	    {
		    MATRIX xr = {
			    1.0f, 0.0f, 0.0f, 0.0f,
			    0.0f, Cos(Angle),(-1) * Sin(Angle), 0.0f,
			    0.0f, Sin(Angle),Cos(Angle), 0.0f,
			    0.0f, 0.0f, 0.0f, 1.0f
		    };
		    return xr;
	    }
	    else if('Y'){
		    MATRIX yr = {
			    Cos(Angle), 0.0f, Sin(Angle), 0.0f,
			    0.0f , 1.0f , 0.0f, 0.0f ,
			    (-1) * Sin(Angle), 0.0f, Cos(Angle), 0.0f,
			    0.0f, 0.0f, 0.0f, 1.0f
		    };
		    return yr;
	    }
	    else if('Z'){
		    MATRIX zr = {
			    Cos(Angle), (-1) * Sin(Angle) , 0.0f, 0.0f,
			    Sin(Angle), Cos(Angle), 0.0f, 0.0f,
			    0.0f, 0.0f, 1.0f, 0.0f,
			    0.0f, 0.0f, 0.0f ,1.0f
		    };
		    return zr;
	    }
	    else
	    {
		    return IdentityMatrix();
	    }
    }
	
    MATRIX OrthoMatrixRH(float l, float r, float b, float t, float n, float f)
    {
	    MATRIX result = IdentityMatrix();
		
    	result._11 = 2.0f/(r-l);
    	result._21 = 0.0f;
    	result._31 = 0.0f;
    	result._41 = 0.0f;
	
    	result._12 = 0.0f;
    	result._22 = 2.0f/(t-b);
    	result._32 = 0.0f;
    	result._42 = 0.0f;
	
    	result._13 = 0.0f;
    	result._23 = 0.0f;
    	result._33 = 2.0f/(n-f);
    	result._43 = 0.0f;
	
    	result._14 = -(r+l)/(r-l);
    	result._24 = -(t+b)/(t-b);
	    result._34 = (n+f)/(n-f);
	    result._44 = 1.0f;
	
    	return result;
    }
	
    MATRIX PerspectiveMatrixRH(float fovy, float aspect, float zNear, float zFar)
    {
	    MATRIX result = {0.0f,0.0f,0.0f,0.0f,
	                    0.0f,0.0f,0.0f,0.0f,
	                    0.0f,0.0f,0.0f,0.0f,
	                    0.0f,0.0f,0.0f,0.0f}; 

    	float f = 1.0f / Tan(fovy / 2.0f);

	    result._11  = f / aspect;  
	    result._22  = f;           
	    result._33 = (zFar + zNear) / (zNear - zFar);  
    	result._34 = -1.0f;                         
    	result._43 = (2.0f * zFar * zNear) / (zNear - zFar); 
	
	    return MatrixTranspose(result);
    }
	
    MATRIX ViewMatrixRH(VECTOR3 eye, VECTOR3 target, VECTOR3 up)
    {
	    MATRIX view;

        VECTOR3 cameraPos = eye * -1.0f;

        VECTOR3 cameraDirection = Vector3Normalized(
            eye - target
        );
        VECTOR3 cameraRight = Vector3Normalized(
            VectorCross(
                up, cameraDirection
            )
        );
        VECTOR3 cameraUp = VectorCross(
            cameraDirection, cameraRight
        );

        view._11 = cameraRight.x;
        view._21 = cameraUp.x;
        view._31 = cameraDirection.x;
        view._41 = 0.0f;

        view._12 = cameraRight.y;
        view._22 = cameraUp.y;
        view._32 = cameraDirection.y;
        view._42 = 0.0f;

        view._13 = cameraRight.z;
        view._23 = cameraUp.z;
        view._33 = cameraDirection.z;
        view._43 = 0.0f;

        view._14 = cameraPos.x * cameraRight.x + cameraPos.y * cameraRight.y + cameraPos.z * cameraRight.z;
        view._24 = cameraPos.x * cameraUp.x + cameraPos.y * cameraUp.y + cameraPos.z * cameraUp.z;
        view._34 = cameraPos.x * cameraDirection.x + cameraPos.y * cameraDirection.y + cameraPos.z * cameraDirection.z;
        view._44 = 1.0f;

	    return view;
    }

    MATRIX OrthoMatrixLH(float l, float r, float b, float t, float n, float f)
    {
        MATRIX result = IdentityMatrix();

        result._11 = 2.0f / (r - l);
        result._12 = 0.0f;
        result._13 = 0.0f;
        result._14 = 0.0f;

        result._21 = 0.0f;
        result._22 = 2.0f / (t - b);
        result._23 = 0.0f;
        result._24 = 0.0f;

        result._31 = 0.0f;
        result._32 = 0.0f;
        result._33 = 1.0f / (f - n);      
        result._34 = 0.0f;

        result._41 = -(r + l) / (r - l);  
        result._42 = -(t + b) / (t - b);  
        result._43 = -n / (f - n);       
        result._44 = 1.0f;

        return result;
    }

    MATRIX PerspectiveMatrixLH(float fovy, float aspect, float zNear, float zFar)
    {
        MATRIX result = { 0 };
        float f = 1.0f / Tan(fovy / 2.0f);

        result._11 = f / aspect;
        result._22 = f;
        result._33 = zFar / (zFar - zNear);      
        result._34 = 1.0f;                      
        result._43 = -zNear * zFar / (zFar - zNear); 

        return MatrixTranspose(result); 
    }

    MATRIX ViewMatrixLH(VECTOR3 eye, VECTOR3 target, VECTOR3 up)
    {
        MATRIX view;

        VECTOR3 cameraPos = eye * -1.0f;
    
        VECTOR3 cameraDirection = Vector3Normalized(
                 eye - target
        );
        VECTOR3 cameraRight = Vector3Normalized(
            VectorCross(
                up, cameraDirection
            )
        );
        VECTOR3 cameraUp = VectorCross(
            cameraDirection, cameraRight
        );

        view._11 = cameraRight.x;
        view._21 = cameraUp.x;
        view._31 = cameraDirection.x;
        view._41 = 0.0f;

        view._12 = cameraRight.y;
        view._22 = cameraUp.y;
        view._32 = cameraDirection.y;
        view._42 = 0.0f;

        view._13 = cameraRight.z;
        view._23 = cameraUp.z;
        view._33 = cameraDirection.z;
        view._43 = 0.0f;

        view._14 = -1.0f * (cameraPos.x * cameraRight.x + cameraPos.y * cameraRight.y + cameraPos.z * cameraRight.z);
        view._24 = -1.0f * (cameraPos.x * cameraUp.x + cameraPos.y * cameraUp.y + cameraPos.z * cameraUp.z);
        view._34 = -1.0f * (cameraPos.x * cameraDirection.x + cameraPos.y * cameraDirection.y + cameraPos.z * cameraDirection.z);
        view._44 = 1.0f;

	    return view;
    }
	
    MATRIX ReflectMatrix(PLANE p)
    {
	    MATRIX reflect = IdentityMatrix();
	
	    PLANE plane = PlaneNormalize(p);
	
	    const float a = plane.a;
	    const float b = plane.b;
	    const float c = plane.c;
	    const float d = plane.d;
	
	    reflect._11 = 1 - 2*a*a;
	    reflect._12 = -2*a*b;
	    reflect._13 = -2*a*c;
	    reflect._14 = -2*a*d;
	
	    reflect._21 = -2*a*b;
	    reflect._22 = 1 - 2*b*b;
	    reflect._23 = -2*b*c;
	    reflect._24 = -2*b*d;
	
	    reflect._31 = -2*a*c;
	    reflect._32 = -2*b*c;
	    reflect._33 = 1 - 2*c*c;
    	reflect._34 = -2*c*d;
	
	    return reflect;
    }

	//If OpenGL Need Transpose
    MATRIX ShadowMatrix(VECTOR4 Light, PLANE p)
    {
	    MATRIX shadow = {0.0f,0.0f,0.0f,0.0f,
	                    0.0f,0.0f,0.0f,0.0f,
	                    0.0f,0.0f,0.0f,0.0f,
	                    0.0f,0.0f,0.0f,0.0f};
	
	    PLANE plane = PlaneNormalize(p);
	    const float a = plane.a;
	    const float b = plane.b;
	    const float c = plane.c;
	    const float d = plane.d;
	
	    const float lx = Light.x;
	    const float ly = Light.y;
	    const float lz = Light.z;
	    const float lw = Light.w;
	
	    const float dot = a * lx + b * ly + c * lz + d * lw;
	
	    shadow._11 = dot - a * lx;
	    shadow._21 = - b * lx;
	    shadow._31 = - c * lx;
	    shadow._41 = - d * lx;
	
	    shadow._12 = - a * ly;
	    shadow._22 = dot - b * ly;
	    shadow._32 = - c * ly;
	    shadow._42 = - d * ly;
	
	    shadow._13 = - a * lz;
	    shadow._23 = - b * lz;
	    shadow._33 = dot - c * lz;
	    shadow._43 = - d * lz;
	
	    shadow._14 = - a * lw;
	    shadow._24 = - b * lw;
	    shadow._34 = - c * lw;
	    shadow._44 = dot - d * lw;
	
	    return shadow;
    }

    MATRIX NormalMatrix(MATRIX model)
    {
        return MatrixTranspose(InvMatrix(model));
    }

    MATRIX BillboardMatrix(const VECTOR3& position, const MATRIX& view) {
    
    MATRIX translation = {
        1.0f, 0.0f, 0.0f, position.x,
        0.0f, 1.0f, 0.0f, position.y,
        0.0f, 0.0f, 1.0f, position.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    MATRIX rotation = {
        view._11, view._12, view._13, 0.0f,
        view._21, view._22, view._23, 0.0f,
        view._31, view._32, view._33, 0.0f,
        0.0f,    0.0f,    0.0f,    1.0f
    };
    
    MATRIX invRotation = {
        rotation._11, rotation._21, rotation._31, 0.0f,
        rotation._12, rotation._22, rotation._32, 0.0f,
        rotation._13, rotation._23, rotation._33, 0.0f,
        0.0f,        0.0f,        0.0f,        1.0f
    };
    
    return translation * invRotation;
}

    VECTOR2 VECTOR2::operator - ()
    {
		VECTOR2 temp(
			-1.0f * x,
            -1.0f * y
			);
        return temp;
    }

    VECTOR3 VECTOR3::operator - ()
    {
		VECTOR3 temp(
			-1.0f * x,
            -1.0f * y,
            -1.0f * z
			);
        return temp;
    }

    VECTOR4 VECTOR4::operator - ()
    {
		VECTOR4 temp(
			-1.0f * x,
            -1.0f * y,
            -1.0f * z,
            -1.0f * w
			);
        return temp;
    }

    MATRIX MATRIX::operator - ()
    {
		MATRIX temp = {
			-1.0f * _11, -1.0f * _12, -1.0f * _13, -1.0f * _14,
            -1.0f * _21, -1.0f * _22, -1.0f * _23, -1.0f * _24,
            -1.0f * _31, -1.0f * _32, -1.0f * _33, -1.0f * _34,
            -1.0f * _41, -1.0f * _42, -1.0f * _43, -1.0f * _44
		};
        return temp;
    }

    float ArcTan2(float y, float x) {
   
    if (Abs(x) < MD_MATH_EPSILON && Abs(y) < MD_MATH_EPSILON) {
        return 0.0f; 
    }
    
    if (Abs(x) < MD_MATH_EPSILON) {
        if (y > 0.0f) {
            return MD_MATH_PI_2; // π/2
        } else {
            return -MD_MATH_PI_2; // -π/2
        }
    }
    
    float base = ArcTan(y / x);
    
    if (x > 0.0f) {

        return base;
    } else {
     
        if (y >= 0.0f) {
            return base + MD_MATH_PI;
        } else {
            return base - MD_MATH_PI; 
        }
    }
    }

    QUATERNION::QUATERNION(float _x, float _y, float _z, float _w) 
        : x(_x), y(_y), z(_z), w(_w) 
    {}

    QUATERNION QUATERNION::operator + (const QUATERNION& q) const {
        return QUATERNION(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    void QUATERNION::operator += (const QUATERNION& q) {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;
    }

    QUATERNION QUATERNION::operator - (const QUATERNION& q) const {
        return QUATERNION(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    void QUATERNION::operator -= (const QUATERNION& q) {
        x -= q.x;
        y -= q.y;
        z -= q.z;
        w -= q.w;
    }

    //Hamilton Dot
    QUATERNION QUATERNION::operator * (const QUATERNION& q) const {
        return QUATERNION(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }

    void QUATERNION::operator *= (const QUATERNION& q) {
        *this = *this * q;
    }

    QUATERNION QUATERNION::operator * (float scalar) const {
        return QUATERNION(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    void QUATERNION::operator *= (float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
    }

    QUATERNION QUATERNION::operator / (float scalar) const {
        float invScalar = 1.0f / scalar;
        return QUATERNION(x * invScalar, y * invScalar, z * invScalar, w * invScalar);
    }

    void QUATERNION::operator /= (float scalar) {
        float invScalar = 1.0f / scalar;
        x *= invScalar;
        y *= invScalar;
        z *= invScalar;
        w *= invScalar;
    }

    bool QUATERNION::operator == (const QUATERNION& q) const {
        return Equal(x, q.x, MD_MATH_EPSILON) &&
               Equal(y, q.y, MD_MATH_EPSILON) &&
               Equal(z, q.z, MD_MATH_EPSILON) &&
               Equal(w, q.w, MD_MATH_EPSILON);
    }

    bool QUATERNION::operator != (const QUATERNION& q) const {
        return !(*this == q);
    }

    QUATERNION QUATERNION::operator - () const {
        return QUATERNION(-x, -y, -z, -w);
    }

    float QuaternionLength(const QUATERNION& q) {
        return Sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    }

    QUATERNION QuaternionNormalize(const QUATERNION& q) {
        float len = QuaternionLength(q);
        if (len < MD_MATH_EPSILON) {
            return QUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
        }
        return q / len;
    }

    QUATERNION QuaternionConjugate(const QUATERNION& q) {
        return QUATERNION(-q.x, -q.y, -q.z, q.w);
    }

    QUATERNION QuaternionInverse(const QUATERNION& q) {
        float lenSq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
        if (lenSq < MD_MATH_EPSILON) {
            return QUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
        }
        return QuaternionConjugate(q) / lenSq;
    }

    float QuaternionDot(const QUATERNION& q1, const QUATERNION& q2) {
        return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
    }

    QUATERNION QuaternionFromAxisAngle(VECTOR3 axis, float angle) {
        float halfAngle = angle * 0.5f;
        float sinHalf = Sin(halfAngle);
        
        float length = ComputeVector3Length(axis);
        if (length > MD_MATH_EPSILON) {
            axis.x /= length;
            axis.y /= length;
            axis.z /= length;
        }
        
        return QUATERNION(
            axis.x * sinHalf,
            axis.y * sinHalf,
            axis.z * sinHalf,
            Cos(halfAngle)
        );
    }

    //pitch-x, yaw-y, roll-z
    QUATERNION QuaternionFromEulerAngles(float pitch, float yaw, float roll) {
        float halfPitch = pitch * 0.5f;
        float halfYaw = yaw * 0.5f;
        float halfRoll = roll * 0.5f;
        
        float cosPitch = Cos(halfPitch);
        float sinPitch = Sin(halfPitch);
        float cosYaw = Cos(halfYaw);
        float sinYaw = Sin(halfYaw);
        float cosRoll = Cos(halfRoll);
        float sinRoll = Sin(halfRoll);
        
        return QUATERNION(
            sinPitch * cosYaw * cosRoll - cosPitch * sinYaw * sinRoll,
            cosPitch * sinYaw * cosRoll + sinPitch * cosYaw * sinRoll,
            cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll,
            cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll
        );
    }

    QUATERNION QuaternionFromMatrix(const MATRIX& m) {
        float trace = m._11 + m._22 + m._33;
        QUATERNION q;
        
        if (trace > 0.0f) {
            float s = 0.5f / Sqrt(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (m._32 - m._23) * s;
            q.y = (m._13 - m._31) * s;
            q.z = (m._21 - m._12) * s;
        } else {
            if (m._11 > m._22 && m._11 > m._33) {
                float s = 2.0f * Sqrt(1.0f + m._11 - m._22 - m._33);
                q.w = (m._32 - m._23) / s;
                q.x = 0.25f * s;
                q.y = (m._12 + m._21) / s;
                q.z = (m._13 + m._31) / s;
            } else if (m._22 > m._33) {
                float s = 2.0f * Sqrt(1.0f + m._22 - m._11 - m._33);
                q.w = (m._13 - m._31) / s;
                q.x = (m._12 + m._21) / s;
                q.y = 0.25f * s;
                q.z = (m._23 + m._32) / s;
            } else {
                float s = 2.0f * Sqrt(1.0f + m._33 - m._11 - m._22);
                q.w = (m._21 - m._12) / s;
                q.x = (m._13 + m._31) / s;
                q.y = (m._23 + m._32) / s;
                q.z = 0.25f * s;
            }
        }
        
        return QuaternionNormalize(q);
    }

    MATRIX MatrixFromQuaternion(const QUATERNION& q) {
        QUATERNION nq = QuaternionNormalize(q);
        float xx = nq.x * nq.x;
        float yy = nq.y * nq.y;
        float zz = nq.z * nq.z;
        float xy = nq.x * nq.y;
        float xz = nq.x * nq.z;
        float yz = nq.y * nq.z;
        float wx = nq.w * nq.x;
        float wy = nq.w * nq.y;
        float wz = nq.w * nq.z;
        
        MATRIX m;
        m._11 = 1.0f - 2.0f * (yy + zz);
        m._12 = 2.0f * (xy - wz);
        m._13 = 2.0f * (xz + wy);
        m._14 = 0.0f;
        
        m._21 = 2.0f * (xy + wz);
        m._22 = 1.0f - 2.0f * (xx + zz);
        m._23 = 2.0f * (yz - wx);
        m._24 = 0.0f;
        
        m._31 = 2.0f * (xz - wy);
        m._32 = 2.0f * (yz + wx);
        m._33 = 1.0f - 2.0f * (xx + yy);
        m._34 = 0.0f;
        
        m._41 = 0.0f;
        m._42 = 0.0f;
        m._43 = 0.0f;
        m._44 = 1.0f;
        
        return m;
    }

    //pitch-x, yaw-y, roll-z
    VECTOR3 EulerAnglesFromQuaternion(const QUATERNION& q) {
        QUATERNION nq = QuaternionNormalize(q);
        
        float xx = nq.x * nq.x;
        float yy = nq.y * nq.y;
        float zz = nq.z * nq.z;
        float xy = nq.x * nq.y;
        float xz = nq.x * nq.z;
        float yz = nq.y * nq.z;
        float wx = nq.w * nq.x;
        float wy = nq.w * nq.y;
        float wz = nq.w * nq.z;
        
        float m11 = 1.0f - 2.0f * (yy + zz);
        float m12 = 2.0f * (xy - wz);
        float m13 = 2.0f * (xz + wy);
        
        float m21 = 2.0f * (xy + wz);
        float m22 = 1.0f - 2.0f * (xx + zz);
        float m23 = 2.0f * (yz - wx);
        
        float m31 = 2.0f * (xz - wy);
        float m32 = 2.0f * (yz + wx);
        float m33 = 1.0f - 2.0f * (xx + yy);
    
        VECTOR3 angles(0.0f, 0.0f, 0.0f);
        
        // Yaw (y-axis rotation)
        angles.y = ArcTan2(m13, m33);
        
        // Pitch (x-axis rotation)
        angles.x = ArcTan2(-m23, Sqrt(m21*m21 + m22*m22));
        
        // Roll (z-axis rotation)
        angles.z = ArcTan2(m21, m22);
        
        return angles;
    }

    VECTOR3 RotateVector(const QUATERNION& q, const VECTOR3& v) {

        QUATERNION p(v.x, v.y, v.z, 0.0f);
        
        //v' = q * v * q^-1
        QUATERNION result = q * p * QuaternionInverse(q);
        
        return VECTOR3(result.x, result.y, result.z);
    }

    // SLERP
    QUATERNION QuaternionSlerp(const QUATERNION& q1, const QUATERNION& q2, float t) {
        QUATERNION from = QuaternionNormalize(q1);
        QUATERNION to = QuaternionNormalize(q2);
        
        float dot = QuaternionDot(from, to);
        
        if (dot < 0.0f) {
            to = -to;
            dot = -dot;
        }
        
        const float DOT_THRESHOLD = 0.9995f;
        if (dot > DOT_THRESHOLD) {
            return QuaternionLerp(from, to, t);
        }
        
        dot = Min(Max(dot, -1.0f), 1.0f);
        
        float theta_0 = ArcCos(dot);      
        float theta = theta_0 * t;       
        
        QUATERNION q3 = to - from * dot;
        q3 = QuaternionNormalize(q3);
        
        return from * Cos(theta) + q3 * Sin(theta);
    }

    //NLERP
    QUATERNION QuaternionLerp(const QUATERNION& q1, const QUATERNION& q2, float t) {
        QUATERNION result = q1 * (1.0f - t) + q2 * t;
        return QuaternionNormalize(result);
    }

    float Mix(float x, float y, float a) {
        return x * (1.0f - a) + y * a;
    }

    VECTOR2 Mix(const VECTOR2& x, const VECTOR2& y, float a) {
        return VECTOR2(
            Mix(x.x, y.x, a),
            Mix(x.y, y.y, a)
        );
    }
    
    VECTOR3 Mix(const VECTOR3& x, const VECTOR3& y, float a) {
        return VECTOR3(
            Mix(x.x, y.x, a),
            Mix(x.y, y.y, a),
            Mix(x.z, y.z, a)
        );
    }
    
    VECTOR4 Mix(const VECTOR4& x, const VECTOR4& y, float a) {
        return VECTOR4(
            Mix(x.x, y.x, a),
            Mix(x.y, y.y, a),
            Mix(x.z, y.z, a),
            Mix(x.w, y.w, a)
        );
    }
    
    QUATERNION Mix(const QUATERNION& q1, const QUATERNION& q2, float a) {
        QUATERNION result(
            Mix(q1.x, q2.x, a),
            Mix(q1.y, q2.y, a),
            Mix(q1.z, q2.z, a),
            Mix(q1.w, q2.w, a)
        );
        
        return QuaternionNormalize(result);
    }

}