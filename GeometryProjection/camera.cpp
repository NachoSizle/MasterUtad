#include "camera.h"

// TODO
// devuelve los valores de distancia de los planos a partir del fov horizontal
FRUSTUM makeFrustum(double fovX, double aspectRatio, double nearValue, double farValue)
{
	const double DEG2RAD = 3.14159265 / 180;

	double tangent = 0; // tangent of half fovX
	double width = 0; // half width of near plane
	double height = 0; // half height of near plane

	FRUSTUM ret;
	// TODO : rellenar valores de ret
	
	return ret;
}

MATRIX4 lookAt(VECTOR3D eyePosition, VECTOR3D target, VECTOR3D upVector)
{
	VECTOR3D forward = Substract(target, eyePosition);

	VECTOR3D side = CrossProduct(forward, upVector);
	VECTOR3D up = CrossProduct(side, forward);
	VECTOR3D invertForward = MultiplyWithScalar(-1.0, forward);

	side = Normalized(side);
	up = Normalized(up);
	invertForward = Normalized(invertForward);

	MATRIX3 rotationMatrix = { side, up, invertForward };
	return InverseOrthogonalMatrix(rotationMatrix, eyePosition);
} 