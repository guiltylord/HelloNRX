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
	for (int i = 0; i < myObj.vecId.size() - 1; i++)
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

	addToModelSpace(myObj.vecId[myObj.countPoint], arc); //в данный момент countPoint=7

	arc->addReactor(myReactor);
	myObj.countPoint++;
	myObj.countPoint++;
	arc->close();
	myObj.currId = nullptr;
}

void LineReactor::modified(const NcDbObject* object)
{
	//один из указателей = nullptr, но необходимо создать два, 
	//птшмт проблемы с областями видимости
	NcDbArc* pArc = NcDbArc::cast(object);
	NcDbLine* mainObj = NcDbLine::cast(object);


	if (myObj.countPoint <= 8)
		return;

	AcDbObjectId currId = object->id();
	//нужно ли нам дальше редактировать обьекты?
	//ведь modified вызывается для каждого обьекта, который изменяется
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

	//главный обьект - линия перед дугой
	if (currId == myObj.vecId[6])
	{
		NcDbObject* object4;
		AcDbObjectId frthId = myObj.vecId[0]; //next
		Nano::ErrorStatus errorr2 = acdbOpenObject(object4, frthId, kForWrite);

		NcDbLine* prevObj = NcDbLine::cast(object2);
		NcDbArc* nextObj = NcDbArc::cast(object3);
		NcDbLine* extraObj = NcDbLine::cast(object4);

		prevObj->setEndPoint(mainObj->startPoint());

		remakeArc(nextObj, extraObj->startPoint(), mainObj->endPoint());
		return;
	}

	//главный обьект - линия после дуги
	if (currId == myObj.vecId[0])
	{
		NcDbObject* object4;
		AcDbObjectId frthId = myObj.vecId[6]; //next
		Nano::ErrorStatus errorr2 = acdbOpenObject(object4, frthId, kForWrite);

		NcDbLine* nextObj = NcDbLine::cast(object3);
		NcDbArc* prevObj = NcDbArc::cast(object2);
		NcDbLine* extraObj = NcDbLine::cast(object4);

		nextObj->setStartPoint(mainObj->endPoint());

		remakeArc(prevObj, mainObj->startPoint(), extraObj->endPoint());
		return;
	}

	//главный обьект - дуга
	if (currId == myObj.vecId[7])
	{
		NcDbLine* prevObj = NcDbLine::cast(object2);
		NcDbLine* nextObj = NcDbLine::cast(object3);

		prevObj->setEndPoint(calculatePointOnArc(pArc->center(), pArc->radius(), pArc->startAngle()));
		nextObj->setStartPoint(calculatePointOnArc(pArc->center(), pArc->radius(), pArc->endAngle()));
		return;
	}


	//главный обьект - линия не рядом с дугой
	else
	{
		NcDbLine* prevObj = NcDbLine::cast(object2);
		NcDbLine* nextObj = NcDbLine::cast(object3);

		NcGePoint3d test1 = mainObj->startPoint();
		NcGePoint3d test2 = mainObj->endPoint();
		prevObj->setEndPoint(test1);
		nextObj->setStartPoint(test2);
	}
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