#include "stdafx.h"
#include "storageObj.h"

storageObj::storageObj()
{
	vecPoint = mkPoints();
    countPoint = 0;
	vecId = mkAcIDs(vecPoint.size());
    flag = false;
}


vector<AcGePoint3d> storageObj::mkPoints()
{
	double lol[] = { 14.7372,-55,-127,-55,-167,-15,-167,112,-119,152,-69,65,-19,152,55, 95.2628, };
	//double lol[] = { 14.7372,-55,-127,-55,-167,-15,-167,112 };
	vector<AcGePoint3d> Points;
	for (int i = 0; i < size(lol) - 1; i += 2)
	{
		Points.push_back(AcGePoint3d(lol[i], lol[i + 1], 0));
	}
	return Points;
}

vector<AcDbObjectId> storageObj::mkAcIDs(int neededSize)
{
	vector<AcDbObjectId> vec;
	for (int i = 0; i < neededSize; i++)
	{
		AcDbObjectId* lineId = new AcDbObjectId();
		vec.push_back(*lineId);
	}
	return vec;
}

AcDbObjectId storageObj::GetPrevID(AcDbObjectId currID)
{
    //if (vecId.size() < 2) { // Нет смысла искать предыдущий, если в векторе меньше двух элементов
    //    return AcDbObjectId(); // Возвращаем пустой ID или обрабатываем этот случай соответствующим образом
    //}

    for (int i = 0; i < vecId.size(); i++) // Использование vecId.size() вместо countPoint-1 для простоты
    {
        if (vecId[i] == currID)
        {
            if (i == 0) // Если текущий элемент первый, возвращаем последний элемент
            {
                return vecId[vecId.size() - 1];
            }
            else // Иначе возвращаем предыдущий элемент
            {
                return vecId[i - 1];
            }
        }
    }
    //return AcDbObjectId(); // Возвращаем пустой ID, если currID не найден
}

AcDbObjectId storageObj::GetNextID(AcDbObjectId currID)
{
    if (vecId.size() < 2) { // Нет смысла искать следующий, если в векторе меньше двух элементов
        return AcDbObjectId(); // Возвращаем пустой ID или обрабатываем этот случай соответствующим образом
    }

    for (int i = 0; i < vecId.size(); i++) // Использование vecId.size() вместо countPoint-1 для простоты
    {
        if (vecId[i] == currID)
        {
            if (i == vecId.size() - 1) // Если текущий элемент последний, возвращаем первый элемент
            {
                return vecId[0];
            }
            else // Иначе возвращаем следующий элемент
            {
                return vecId[i + 1];
            }
        }
    }
    return AcDbObjectId(); // Возвращаем пустой ID, если currID не найден
}