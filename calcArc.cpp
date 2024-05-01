#include "stdafx.h"
//#include "calcArc.h"
//
//double calcArc::calculateFullAngle(AcGePoint3d center, AcGePoint3d start)
//{
//
//	AcGePoint3d helper = AcGePoint3d(center.x + 1, center.y, 0);
//
//	AcGeVector2d A(center.x - helper.x, center.y - helper.y);
//	AcGeVector2d C(center.x - start.x, center.y - start.y);
//
//	// Normalize vectors A and C
//	A.normalize();
//	C.normalize();
//
//	// Calculate the arc cosine of the dot product
//	double angle = acos(A.dotProduct(C));
//
//	// Determine the orientation of the angle using a 2D cross product equivalent ("perp dot product")
//	double orientation = A.x * C.y - A.y * C.x;
//	// If the orientation is negative, subtract the angle from 2π
//	if (orientation < 0.0) {
//		angle = 2 * PI - angle;
//	}
//
//	// Convert from radians to degrees
//	return angle;
//}
//
//AcGePoint3d calcArc:: findCircleCenter(AcGePoint3d A, AcGePoint3d B, double radius)
//{	
//	// Midpoint of chord AB
//	AcGePoint3d M = AcGePoint3d((B.x + A.x) / 2.0, (B.y + A.y) / 2.0, 0);
//
//	// Length of the chord AB
//	double chordLength = AcGeVector3d(A - B).length();
//
//	// Distance from midpoint M to the circle center O
//	double OM = sqrt(radius * radius - (chordLength / 2.0) * (chordLength / 2.0));
//	//определяет, где будет центр относительно хорды
//	int side = -1;
//
//	// Direction of the perpendicular to chord AB
//	AcGeVector3d AB = AcGeVector3d(side * (A - B));
//	AcGeVector3d direction = AcGeVector3d(-AB.y, AB.x, 0);
//
//	// Normalize the direction vector
//	direction.normalize();
//	// Calculate the center of the circle using direction and distance OM from M
//	AcGePoint3d center = M + direction * OM;
//	return center;
//}
//

#pragma once
#include "stdafx.h"

namespace calcArc
{
	double calculateFullAngle(AcGePoint3d center, AcGePoint3d start)
	{

		AcGePoint3d helper = AcGePoint3d(center.x + 1, center.y, 0);

		AcGeVector2d A(center.x - helper.x, center.y - helper.y);
		AcGeVector2d C(center.x - start.x, center.y - start.y);

		// Normalize vectors A and C
		A.normalize();
		C.normalize();

		// Calculate the arc cosine of the dot product
		double angle = acos(A.dotProduct(C));

		// Determine the orientation of the angle using a 2D cross product equivalent ("perp dot product")
		double orientation = A.x * C.y - A.y * C.x;

		// If the orientation is negative, subtract the angle from 2π
		if (orientation < 0.0) {
			angle = 2 * PI - angle;
		}

		// Convert from radians to degrees

		return angle;
	}


	AcGePoint3d findCircleCenter(AcGePoint3d A, AcGePoint3d B, double radius)
	{
		// Midpoint of chord AB
		AcGePoint3d M = AcGePoint3d((B.x + A.x) / 2.0, (B.y + A.y) / 2.0, 0);

		// Length of the chord AB
		double chordLength = AcGeVector3d(A - B).length();

		// Distance from midpoint M to the circle center O
		double OM = sqrt(radius * radius - (chordLength / 2.0) * (chordLength / 2.0));

		//определяет, где будет центр относительно хорды
		int side = -1;

		// Direction of the perpendicular to chord AB
		AcGeVector3d AB = AcGeVector3d(side * (A - B));
		AcGeVector3d direction = AcGeVector3d(-AB.y, AB.x, 0);

		// Normalize the direction vector
		direction.normalize();

		// Calculate the center of the circle using direction and distance OM from M
		AcGePoint3d center = M + direction * OM;

		return center;
	}	
};
