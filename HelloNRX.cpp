//
// Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
// 
// Данное программное обеспечение, все исключительные права на него, его
// документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
// Данное программное обеспечение может использоваться при разработке и входить
// в состав разработанных программных продуктов при соблюдении условий
// использования, оговоренных в «Лицензионном договоре присоединения
// на использование программы для ЭВМ «Платформа nanoCAD»».
// 
// Данное программное обеспечение защищено в соответствии с законодательством
// Российской Федерации об интеллектуальной собственности и международными
// правовыми актами.
// 
// Используя данное программное обеспечение,  его документацию и
// сопроводительные материалы вы соглашаетесь с условиями использования,
// указанными выше. 
//



#include <math.h>
#include <vector>


#include "stdafx.h"

void addToModelSpace(AcDbObjectId& objId, AcDbEntity* pEntity);


struct Point3
{
    double x;
    double y;
    double z;
};

vector<Point3> mkVector(double func(double))
{
    vector<Point3> Points;
    for (double phi = 0; phi < 2 * PI; phi += PI / 20)
    {   
        double r = func(phi);
        double x = r * cos(phi);
        double y = r * sin(phi);
        double z = phi / 2 / PI;
        Points.push_back(Point3{ x,y,z });
    }
    return Points;
}

double func(double phi)
{
    return sin(phi) / cos(phi);
}

void helloNrxCmd()
{
    AcDbObjectId* myIDs = new AcDbObjectId; 
    //vector <AcDbObjectId> myIDs;
    

    while (int i = 0 < 40)
    {
        myIDs[i] = AcDbObjectId();
        //myIDs.push_back(AcDbObjectId());
        i++;
    }

    vector<Point3> Points = mkVector(&func);

    for(int i=0; i < 39; i++)
    {
        Point3 currPoint = Points[i];
        Point3 nextPoint = Points[i++];
        AcDbLine* pLine = new AcDbLine(AcGePoint3d(currPoint.x, currPoint.y, currPoint.z), AcGePoint3d(nextPoint.x, nextPoint.y, nextPoint.z));
        addToModelSpace(myIDs[i], pLine);

        pLine->close();
    }

    delete[] myIDs;

   /* AcDbObjectId line9Id;
    AcDbObjectId line10Id;
    AcDbObjectId line11Id;
    AcDbObjectId lel1[3] = { line9Id, line10Id,line11Id };
    double lol1[10] = { -40, -40,-100,-40,-70,10,-40,-40,-100,-40};
    for (int i = 0; i < 6; i += 2)
    {

        AcDbLine* pLine = new AcDbLine(AcGePoint3d(lol1[i], lol1[i + 1], 0), AcGePoint3d(lol1[i + 2], lol1[i + 3], 0));
        addToModelSpace(lel1[i / 2], pLine);

        pLine->close();
    }*/





    /*AcDbObjectId circle1Id;

    AcDbCircle* circle = new AcDbCircle(AcGePoint3d(-130,80,0), AcGeVector3d::kZAxis, 25);

    addToModelSpace(circle1Id, circle);
    circle->close();*/


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

    /*AcGePoint3d center(110, 0.0, 0.0);
    double radius = 110;
    double startAngle = PI / 6 * 4;
    double endAngle = PI / 6 / 2 * 14;
    AcDbObjectId arcId;
    AcDbArc* arc = new AcDbArc(center, radius, startAngle, endAngle);

    addToModelSpace(arcId, arc);
    arc->close();*/
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
