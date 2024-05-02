#include <math.h>
#include <vector>

#include "stdafx.h"
#include <iostream>
#include "calcArc.h"
#include "storageObj.h"
using namespace calcArc;


using namespace std;

void addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity);
vector<AcDbObjectId> IDs;
storageObj myObj= storageObj();

class LineReactor : public NcDbObjectReactor
{
public:
	void modified(const NcDbObject* object);
};

LineReactor* myReactor;


void helloNrxCmd()
{
	//делаю контур

    //storageObj myObj = storageObj();
	for (int i = 0; i < myObj.vecId.size(); i++)
	{

		AcDbLine* pLine = new AcDbLine(myObj.vecPoint[i], myObj.vecPoint[i + 1]);
		addToModelSpace(myObj.vecId[i], pLine);
        pLine->addReactor(myReactor);
		pLine->close();
        myObj.countPoint++;
	}

	//right arc
	//AcGePoint3d pointStartAngleArc = myObj.vecPoint[myObj.countPoint - 1]; 
	//AcGePoint3d pointEndAngleArc = myObj.vecPoint[0];
	//double radiusArc = 110;

	//AcGePoint3d center = findCircleCenter(pointStartAngleArc, pointEndAngleArc, radiusArc);

	//double startAngle = calculateFullAngle(center, pointStartAngleArc);
	//double endAngle = calculateFullAngle(center, pointEndAngleArc);
	//
	//AcDbArc* arc = new AcDbArc(center, radiusArc, startAngle, endAngle);

	//addToModelSpace(myObj.vecId[myObj.countPoint - 1], arc);//тут возможно будет проблема с айдишниками #теперь не должно
	//arc->close();	
}

void LineReactor::modified(const NcDbObject* object)
{
    NcDbLine* pLine = NcDbLine::cast(object);

    if (pLine == NULL)
        return;

    if (myObj.countPoint <= 7)
        return;

    NcDbObject* object2;
    AcDbObjectId currId = object->id();
    AcDbObjectId secId = myObj.GetPrevID(currId);
    acdbOpenObject(object2, secId, kForWrite);
    NcDbLine* pLine2 = NcDbLine::cast(object2);

    NcDbObject* object3;
    AcDbObjectId thrdId = myObj.GetNextID(object->id());
    acdbOpenObject(object3, thrdId, kForWrite);
    NcDbLine* pLine3 = NcDbLine::cast(object3);

    for (int i = 0; i < myObj.countPoint-1; i++)
    {
        if (object->objectId() == myObj.vecId[i])
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
            myObj.flag = true;
            //switch (i)
            //{
            //case 0:

            //    if (pLine != nullptr)
            //    {
            //        /*AcDbObjectId ID = IDs[1];
            //          NcDbLine = ID->objectId();
            //          NcDbLine* pLine2 = NcDbLine::cast();*/
            //        pLine->setStartPoint(NcGePoint3d(0, 0, 0));

            //        //есть getStartPoint()
            //        //
            //        // 
            //        // 
            //        // 
            //        // 

            //        //pLine->setEndPoint(NcGePoint3d(100, 100, 0)); 
            //    }
            //    break;
            //case 1:
            //    if (pLine != nullptr)
            //    {
            //        pLine->setStartPoint(NcGePoint3d(100, 100, 0));
            //        pLine->setEndPoint(NcGePoint3d(150, 50, 0));
            //    }
            //    break;
            //default:
            //    break;
            //}
            //break;
        }
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
