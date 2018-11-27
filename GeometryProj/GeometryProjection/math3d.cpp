#include "math3d.h"
#include <math.h>

VECTOR3D Add(VECTOR3D a, VECTOR3D b)
{
	VECTOR3D ret;
	ret.x = a.x + b.x;
	ret.y = a.y + b.y;
	ret.z = a.z + b.z;
	return ret;
}
VECTOR3D Substract(VECTOR3D a, VECTOR3D b) 
{
	VECTOR3D ret;
	ret.x = a.x - b.x;
	ret.y = a.y - b.y;
	ret.z = a.z - b.z;
	return ret;
}
VECTOR3D Scale(VECTOR3D a, VECTOR3D b) 
{
	VECTOR3D res;
	res.x = a.x * b.x;
	res.y = a.y * b.y;
	res.z = a.z * b.z;
	return res;
}
VECTOR3D MultiplyWithScalar(float scalar, VECTOR3D a)
{
	VECTOR3D ret;
	ret.x = a.x * scalar;
	ret.y = a.y * scalar;
	ret.z = a.z * scalar;
	return ret;
}
VECTOR3D DivideWithScalar(float scalar, VECTOR3D a)
{
	VECTOR3D ret;
	ret.x = a.x / scalar;
	ret.y = a.y / scalar;
	ret.z = a.z / scalar;
	return ret;
}
double Magnitude(VECTOR3D a)
{
	return sqrt((pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2)));
}
VECTOR3D Normalized(VECTOR3D a) 
{
	double modVector = Magnitude(a);
	return  DivideWithScalar(modVector, a);
}
VECTOR3D CrossProduct(VECTOR3D a, VECTOR3D b) 
{
	VECTOR3D res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}
double DotProduct(VECTOR3D a, VECTOR3D b)
{
	VECTOR3D cross = CrossProduct(a, b);
	double mod = Magnitude(cross);
	double magA = Magnitude(a);
	double magB = Magnitude(b);
	double sinAng = mod / (magA * magB);
	double angle = asin(sinAng);

	return magA * magB * cos(angle);
}
MATRIX3 Transpose(MATRIX3 m) {
	MATRIX3 res;
	res.column0.x = m.column0.x;
	res.column0.y = m.column1.x;
	res.column0.z = m.column2.x;

	res.column1.x = m.column0.y;
	res.column1.y = m.column1.y;
	res.column1.z = m.column2.y;

	res.column2.x = m.column0.z;
	res.column2.y = m.column1.z;
	res.column2.z = m.column2.z;

	return res;
}
VECTOR3D Transform(MATRIX3 m, VECTOR3D a) {
	VECTOR3D res;
	res.x = ((m.column0.x * a.x) + (m.column1.x * a.y) + (m.column2.x * a.z));
	res.y = ((m.column0.y * a.x) + (m.column1.y * a.y) + (m.column2.y * a.z));
	res.z = ((m.column0.z * a.x) + (m.column1.z * a.y) + (m.column2.z * a.z));
	return res;
}
QUATERNION QuaternionFromAngleAxis(float angle, VECTOR3D axis){
    VECTOR3D norm = Normalized(axis);
    QUATERNION qua;
    qua.vec = norm;
    qua.angle = angle;
    return qua;
}
QUATERNION Multiply(QUATERNION a, QUATERNION b) {
    float angle = a.angle*b.angle - a.vec.x*b.vec.x - a.vec.y*b.vec.y - a.vec.z*b.vec.z;
    
    float xCoorQua = a.angle*b.vec.x + a.vec.x*b.angle + a.vec.y*b.vec.z - a.vec.z*b.vec.y;
    float yCoorQua = a.angle*b.vec.y + a.vec.y*b.angle + a.vec.z*b.vec.x - a.vec.x*b.vec.z;
    float zCoorQua = a.angle*b.vec.z + a.vec.z*b.angle + a.vec.x*b.vec.y - a.vec.y*b.vec.x;
    
    VECTOR3D vec;
    vec.x = xCoorQua;
    vec.y = yCoorQua;
    vec.z = zCoorQua;
    
    QUATERNION qua;
    qua.angle = angle;
    qua.vec = vec;
    
    return qua;
}
QUATERNION Conjugate(QUATERNION a) {
    
    QUATERNION qua;
    qua.angle = a.angle;
    
    VECTOR3D vec;
    vec.x = -1*a.vec.x;
    vec.y = -1*a.vec.y;
    vec.z = -1*a.vec.z;
    
    qua.vec = vec;
    
    return qua;
}

float Norm(QUATERNION a) {
    return (a.angle*a.angle + a.vec.x*a.vec.x + a.vec.y*a.vec.y + a.vec.z*a.vec.z);
}

QUATERNION Scale(QUATERNION a, float esc) {
    QUATERNION qua;
    qua.angle = a.angle * esc;
    qua.vec.x = a.vec.x * esc;
    qua.vec.y = a.vec.y * esc;
    qua.vec.z = a.vec.z * esc;
    return qua;
}

QUATERNION Inverse(QUATERNION a) {
    QUATERNION qua = Conjugate(a);
    float normQua = Norm(qua);
    return Scale(qua, 1/normQua);
}
VECTOR3D RotateWithQuaternion(VECTOR3D a, QUATERNION q) {
    QUATERNION quaFromVec;
    quaFromVec.angle = 0;
    quaFromVec.vec.x = a.x;
    quaFromVec.vec.y = a.y;
    quaFromVec.vec.z = a.z;
    
    QUATERNION inverted = Inverse(q);
    QUATERNION aux = Multiply(q, quaFromVec);
    QUATERNION auxFromInv = Multiply(aux, inverted);
    
    VECTOR3D vec;
    vec.x = auxFromInv.vec.x;
    vec.y = auxFromInv.vec.y;
    vec.z = auxFromInv.vec.z;
    return vec;
}
MATRIX4 InverseOrthogonalMatrix(MATRIX3 A, VECTOR3D t)
{
	MATRIX4 sol;
	VECTOR3D tr = MultiplyWithScalar(-1, Transform(A, t));
	MATRIX3 At = Transpose(A);

	sol.m[0] = At.column0.x;
	sol.m[1] = At.column0.y;
	sol.m[2] = At.column0.z;
	sol.m[3] = 0;

	sol.m[4] = At.column1.x;
	sol.m[5] = At.column1.y;
	sol.m[6] = At.column1.z;
	sol.m[7] = 0;

	sol.m[8] = At.column2.x;
	sol.m[9] = At.column2.y;
	sol.m[10] = At.column2.z;
	sol.m[11] = 0;

	sol.m[12] = tr.x;
	sol.m[13] = tr.y;
	sol.m[14] = tr.z;
	sol.m[15] = 1;

	return sol;
}

QUATERNION QuaternionFromToVectors(VECTOR3D from, VECTOR3D to) {
    QUATERNION quaFrom = ToQuaternion(from.z, from.y, from.x);
    QUATERNION quaTo = ToQuaternion(to.z, to.y, to.x);
    return Multiply(quaFrom, quaTo);
}

QUATERNION ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    
    QUATERNION q;
    q.angle = cy * cp * cr + sy * sp * sr;
    q.vec.x = cy * cp * sr - sy * sp * cr;
    q.vec.y = sy * cp * sr + cy * sp * cr;
    q.vec.z = sy * cp * cr - cy * sp * sr;
    return q;
}
