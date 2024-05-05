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
	void modified(const NcDbObject* object);
	int counter = 0;
};

LineReactor* myReactor;


void helloNrxCmd()
{
	//делаю контур

	//storageObj myObj = storageObj();
	for (int i = 0; i < myObj.vecId.size()-1; i++)
	{

		AcDbLine* pLine = new AcDbLine(myObj.vecPoint[i], myObj.vecPoint[i + 1]);
		addToModelSpace(myObj.vecId[i], pLine);
		pLine->addReactor(myReactor);
		pLine->close();
		myObj.countPoint++;
	}

	//right arc
	AcGePoint3d pointStartAngleArc = myObj.vecPoint[myObj.countPoint];
	AcGePoint3d pointEndAngleArc = myObj.vecPoint[0];
	double radiusArc = 110;

	AcGePoint3d center = findCircleCenter(pointStartAngleArc, pointEndAngleArc, radiusArc, -1);

	double startAngle = calculateFullAngle(center, pointStartAngleArc);
	double endAngle = calculateFullAngle(center, pointEndAngleArc);

	AcDbArc* arc = new AcDbArc(center, radiusArc, startAngle, endAngle);

	addToModelSpace(myObj.vecId[myObj.countPoint], arc);
	myObj.countPoint++;
	arc->addReactor(myReactor);
	arc->close();
}

void LineReactor::modified(const NcDbObject* object)
{
	NcDbArc* pArc = NcDbArc::cast(object);
	NcDbLine* pLine = NcDbLine::cast(object);


	if (myObj.countPoint <= 7)
		return;

	AcDbObjectId currId = object->id();


	if (myObj.currId == nullptr)
		myObj.currId = currId;
	else
	{
		myReactor->counter++;
		if (myReactor->counter == 2)
		{
			myReactor->counter = 0;
			myObj.currId = nullptr;
			return;
		}
		return;
	}

	NcDbObject* object2;
	AcDbObjectId secId = myObj.GetPrevID(currId); //previous
	Nano::ErrorStatus errorr1 = acdbOpenObject(object2, secId, kForWrite);

	NcDbObject* object3;
	AcDbObjectId thrdId = myObj.GetNextID(currId); //next
	Nano::ErrorStatus errorr2 = acdbOpenObject(object3, thrdId, kForWrite);

	if (currId == myObj.vecId[6])
	{
		NcDbObject* object4;
		AcDbObjectId frthId = myObj.vecId[0]; //next
		Nano::ErrorStatus errorr2 = acdbOpenObject(object4, frthId, kForWrite);

		NcDbLine* pLine2 = NcDbLine::cast(object2);
		NcDbArc* pLine3 = NcDbArc::cast(object3);
		NcDbLine* pLine4 = NcDbLine::cast(object4);

		NcGePoint3d test1 = pLine->startPoint();
		NcGePoint3d test2 = pLine->endPoint();
		NcGePoint3d test3 = pLine4->startPoint();
		pLine2->setEndPoint(test1);
		int radius = 110;
		while(true)
		{
			AcGePoint3d center = findCircleCenter(test2, test3, radius, -1);
			double startAngle = calculateFullAngle(center, test2);
			double endAngle = calculateFullAngle(center, test3);
			//AcDbArc* arc = new AcDbArc(center, 110, startAngle, endAngle);
			pLine3->setCenter(center);
			pLine3->setStartAngle(startAngle);
			pLine3->setEndAngle(endAngle);
			pLine3->setRadius(radius);
			if ((calculatePointOnArc(pLine3->center(), radius, pLine3->startAngle()) == pLine->endPoint()) && (calculatePointOnArc(pLine3->center(), radius, pLine3->endAngle()) == test3))
			{
				break;
			}
			radius++;
		}
		return;
	}

	if (currId == myObj.vecId[0])
	{
		NcDbObject* object4;
		AcDbObjectId frthId = myObj.vecId[6]; //next
		Nano::ErrorStatus errorr2 = acdbOpenObject(object4, frthId, kForWrite);

		NcDbLine* pLine3 = NcDbLine::cast(object3);
		NcDbArc* pLine2 = NcDbArc::cast(object2);
		NcDbLine* pLine4 = NcDbLine::cast(object4);

		NcGePoint3d test1 = pLine->startPoint();
		NcGePoint3d test2 = pLine->endPoint();
		NcGePoint3d test3 = pLine4->endPoint();
		pLine3->setStartPoint(test2);


		int radius = 110;
		while (true)
		{
			AcGePoint3d center = findCircleCenter(test1, test3, radius, 1);
			double startAngle = calculateFullAngle(center, test3);
			double endAngle = calculateFullAngle(center, test1);
			//AcDbArc* arc = new AcDbArc(center, 110, startAngle, endAngle);
			pLine2->setCenter(center);
			pLine2->setStartAngle(startAngle);
			pLine2->setEndAngle(endAngle);
			pLine2->setRadius(radius);
			if ((calculatePointOnArc(pLine2->center(), radius, pLine2->startAngle()) == test3) &&
				(calculatePointOnArc(pLine2->center(), radius, pLine2->endAngle()) == test1))
			{
				break;
			}
			radius++;
		}
		return;
	}

	if (currId == myObj.vecId[7])
	{
		NcDbLine* pLine2 = NcDbLine::cast(object2);
		NcDbLine* pLine3 = NcDbLine::cast(object3);
		
		pLine2->setEndPoint(calculatePointOnArc(pArc->center(), pArc->radius(), pArc->startAngle()));
		pLine3->setStartPoint(calculatePointOnArc(pArc->center(), pArc->radius(), pArc->endAngle()));
		return;
	}
	else
	{
		NcDbLine* pLine2 = NcDbLine::cast(object2);
		NcDbLine* pLine3 = NcDbLine::cast(object3);

		NcGePoint3d test1 = pLine->startPoint();
		NcGePoint3d test2 = pLine->endPoint();
		pLine2->setEndPoint(test1);
		pLine3->setStartPoint(test2);
	}

	

	//айди посл лин = 6, дуги = 7, перв лин = 0
	//AcDbObjectId* someId = new AcDbObjectId(); //нужный айди объекта, чтобы определить дугу
	//if (currId == *someId)
	//{
	//	if (true)//вариант,когда линия-линия-дуга
	//	{
	//		return;
	//	}
	//	if (true)//вариант,когда линия-дуга-линия
	//	{
	//		return;
	//	}
	//	if (true)//вариант,когда дуга-линия-линия
	//	{
	//		return;
	//	}
	//	if (true)//вариант,когда линия-линия-линия
	//	{
	//		
	//	}
	//}
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

		myReactor = new LineReactor;
		acedRegCmds->addCommand(L"HELLONRX_GROUP",
			L"_HELLONRX",
			L"HELLONRX",
			ACRX_CMD_TRANSPARENT,
			helloNrxCmd);
		break;

	case AcRx::kUnloadAppMsg:
		acedRegCmds->removeGroup(L"HELLONRX_GROUP");
		delete myReactor;
		break;

	}

	return AcRx::kRetOK;
}