#pragma once
#include "stdafx.h"

namespace calcArc
{
	//вычисляет угол относительно горизонтали, проведенной через центр будущей дуги
	double calculateFullAngle(AcGePoint3d center, AcGePoint3d start);

	//вычисляет центр окружности через две точки на дуге и радиус
	AcGePoint3d findCircleCenter(AcGePoint3d A, AcGePoint3d B, double radius);
};

