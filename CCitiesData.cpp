#include "pch.h"
#include "CCitiesData.h"

CCitiesData::CCitiesData()
{
}

CCitiesData::~CCitiesData()
{
}
bool CCitiesData::SelectAll(CPtrAutoArray<CITIES>& oCitiesAutoArray)
{
	return oTable.SelectAll(oCitiesAutoArray);
}

BOOL CCitiesData::Insert(CITIES& recCity)
{
	return oTable.InsertRecord(recCity);
}

BOOL CCitiesData::UpdateWhereID(const long lID, CITIES& recCity)
{
	return oTable.UpdateWhereID(lID,recCity);
}

BOOL CCitiesData::Delete(long lID)
{
	return oTable.DeleteWhereID(lID);
}

