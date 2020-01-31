#include <vecmath.h>
const float pi = 3.14159265 ; // For portability across platforms


class Transform  
{
public:
	Transform();
	virtual ~Transform();
	static void left(float degrees, Vector3f& eye, Vector3f& up);
	static void up(float degrees, Vector3f& eye, Vector3f& up);
	static Matrix4f lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
	static Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
	static Matrix3f rotate(const float degrees, const Vector3f& axis) ;
	static Matrix4f scale(const float sx, const float sy, const float sz) ; 
	static Matrix4f translate(const float tx, const float ty, const float tz);
	static Vector3f upvector(const Vector3f& up, const Vector3f& zvec) ; 
};

