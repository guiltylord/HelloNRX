﻿#include <math.h>
#include <vector>

#include "stdafx.h"
#include <iostream>
#include "calcArc.h"
#include "storageObj.h"
using namespace calcArc;


using namespace std;

void addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity);
vector<AcDbObjectId> IDs;
storageObj myObj = storageObj();

class LineReactor : public NcDbObjectReactor
{
public:


    /*double x1, y1, z1;
    double x2, y2, z2;*/

    NcGePoint3d startPoint1 = NcGePoint3d(0, 0, 0);
    NcGePoint3d endPoint1 = NcGePoint3d(100, 100, 0);
    NcGePoint3d startPoint2 = endPoint1;
    NcGePoint3d endPoint2 = NcGePoint3d(150, 50, 0);

    void modified(const NcDbObject* object)
    {
        NcDbLine* pLine = NcDbLine::cast(object);
        NcDbObject* object2;
        acdbOpenObject(object2, IDs[1], kForWrite);

        NcDbLine* pLine2 = NcDbLine::cast(object2);

        if (pLine == NULL)
            return;

        if (IDs.size() < 2)
            return;

        int sizeList = IDs.size();

        for (int i = 0; i < sizeList; i++)
        {
            if (object->objectId() == IDs[i])
            {
                /*switch (i)
                {
                case 0:
                    if (pLine != nullptr)
                    {
                        pLine->setStartPoint(NcGePoint3d(0, 0, 0));
                        pLine->setEndPoint(NcGePoint3d(100, 100, 0));
                    }
                    break;
                case 1:
                    if (pLine != nullptr)
                    {
                        pLine->setStartPoint(NcGePoint3d(100, 100, 0));
                        pLine->setEndPoint(NcGePoint3d(150, 50, 0));
                    }
                    break;
                default:
                    break;
                }*/

                switch (i)
                {
                case 0:

                    if (pLine != nullptr)
                    {
                        /*AcDbObjectId ID = IDs[1];
                          NcDbLine = ID->objectId();
                          NcDbLine* pLine2 = NcDbLine::cast();*/
                        pLine->setStartPoint(NcGePoint3d(0, 0, 0));

                        //есть getStartPoint()
                        //
                        // 
                        // 
                        // 
                        // 

                        //pLine->setEndPoint(NcGePoint3d(100, 100, 0)); 
                    }
                    break;
                case 1:
                    if (pLine != nullptr)
                    {
                        pLine->setStartPoint(NcGePoint3d(100, 100, 0));
                        pLine->setEndPoint(NcGePoint3d(150, 50, 0));
                    }
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
};

LineReactor* myReactor;


void helloNrxCmd()
{
	//нужны методы получить пред и след, вектор для этого есть. и желательно запихнуть это в класс
	
	
	//делаю контур
	for (int i = 0; i < myObj.countPoint-1; i++)
	{
		AcDbLine* pLine = new AcDbLine(myObj.vecPoint[i], myObj.vecPoint[i + 1]);

		addToModelSpace(myObj.vecId[i], pLine);

		pLine->close();
	}

	
	//right arc
	AcGePoint3d pointStartAngleArc = myObj.vecPoint[myObj.countPoint - 1]; 
	AcGePoint3d pointEndAngleArc = myObj.vecPoint[0];
	double radiusArc = 110;

	AcGePoint3d center = findCircleCenter(pointStartAngleArc, pointEndAngleArc, radiusArc);

	double startAngle = calculateFullAngle(center, pointStartAngleArc);
	double endAngle = calculateFullAngle(center, pointEndAngleArc);
	
	AcDbArc* arc = new AcDbArc(center, radiusArc, startAngle, endAngle);

	addToModelSpace(myObj.vecId[myObj.countPoint - 1], arc);//тут возможно будет проблема с айдишниками #теперь не должно
	arc->close();

	//triangle
	/*AcDbObjectId line9Id;
	AcDbObjectId line10Id;
	AcDbObjectId line11Id;
	AcDbObjectId lel1[3] = { line9Id, line10Id,line11Id };
	double lol1[10] = { -40, -40,-100,-40,-70,10,-40,-40,-100,-40 };
	for (int i = 0; i < 6; i += 2)
	{

		AcDbLine* pLine = new AcDbLine(AcGePoint3d(lol1[i], lol1[i + 1], 0), AcGePoint3d(lol1[i + 2], lol1[i + 3], 0));
		addToModelSpace(lel1[i / 2], pLine);

		pLine->close();
	}*/


	//circle
   /* AcDbObjectId circle1Id;

	AcDbCircle* circle = new AcDbCircle(AcGePoint3d(-130, 80, 0), AcGeVector3d::kZAxis, 25);

	addToModelSpace(circle1Id, circle);
	circle->close();*/

	//dont need
	////int i = 0;
	////AcDbLine* pLine = new AcDbLine(AcGePoint3d(lol[i], lol[i + 1], 0), AcGePoint3d(lol[i + 2], lol[i + 3], 0));
	//////AcDbLine* pLine = new AcDbLine(AcGePoint3d(0, 0, 0), AcGePoint3d(-72, 0, 0));
	////addToModelSpace(lineId, pLine);
	//
	//
	//AcDbObjectId arc1Id;

	//AcDbArc* pArc = new AcDbArc(AcGePoint3d(0, 0, 0), 110, 120, 0);
	////AcDbCurve()
	//addToModelSpace(arc1Id, pArc);

	//pArc->close();
	//
	//

	///*
	//AcDbObjectId arc1Id;
	//AcDbArc* arc = new AcDbArc();

	//arc->setCenter(AcGePoint3d(0, 0, 0));
	//arc->setRadius(110);
	//arc->setStartAngle(0);
	//arc->setEndAngle(120);

	//addToModelSpace(arc1Id, arc);

	//arc->close();*/

	//AcDbObjectId circle11Id;

	//AcDbArc* arc = new AcDbArc();


	//AcDbCircle* circle = new AcDbCircle();
	//// Устанавливаем параметры окружности
	//circle->setCenter(AcGePoint3d(0, 0, 0));
	//circle->setRadius(AcGePoint3d(0, 0, 0).distanceTo(AcGePoint3d(110, 0, 0)));
	// Создаем дугу из трех точек

	//AcDbObjectId arc1Id;
	////AcDbArc* arc = new AcDbArc(AcGePoint3d(110, 0, 0), AcGePoint3d(110, 0, 0), 5);
	//AcDbArc* arc = new AcDbArc;
	 
	
}


void addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity)
{
	AcDbBlockTable* pBlockTable;
	AcDbBlockTableRecord* pBlock;

	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForRead);

	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite);
	pBlockTable->close();

	pBlock->appendAcDbEntity(objId, pEntity);
	pBlock->close();
}

extern "C" __declspec(dllexport) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);

		acedRegCmds->addCommand(L"HELLONRX_GROUP",
			L"_HELLONRX",
			L"HELLONRX",
			ACRX_CMD_TRANSPARENT,
			helloNrxCmd);
		break;

	case AcRx::kUnloadAppMsg:
		acedRegCmds->removeGroup(L"HELLONRX_GROUP");
		break;
	}

	return AcRx::kRetOK;
}
