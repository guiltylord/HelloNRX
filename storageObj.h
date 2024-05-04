#pragma once
class storageObj
{
public:
	storageObj();

	vector<AcGePoint3d> vecPoint;
	vector<AcDbObjectId> vecId;
	int countPoint;
	bool isModifying;
	AcDbObjectId currId;

	AcDbObjectId GetPrevID(AcDbObjectId currID);
	AcDbObjectId GetNextID(AcDbObjectId currID);
private:
	vector<AcGePoint3d> mkPoints();
	vector<AcDbObjectId> mkAcIDs(int neededSize);
};
