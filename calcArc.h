#pragma once

namespace calcArc
{
	//вычисляет угол относительно горизонтали, проведенной через центр будущей дуги
	double calculateFullAngle(AcGePoint3d center, AcGePoint3d start);

	//вычисляет центр окружности через две точки на дуге и радиус
	AcGePoint3d findCircleCenter(AcGePoint3d A, AcGePoint3d B, double radius, int side);

	//дает координаты угла на дуге эщкере
	AcGePoint3d calculatePointOnArc(AcGePoint3d center, double radius, double angleRadians);

	//подбирает свойства дуги, чтобы она совпадала с точками
	void remakeArc(AcDbArc* pArc, AcGePoint3d endAnglePoint, AcGePoint3d startAnglePoint);
};