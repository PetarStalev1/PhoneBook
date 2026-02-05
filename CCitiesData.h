#pragma once
#include "pch.h"
#include "Structures.h"
#include "CPtrAutoArray.h"
#include "CCitiesTable.h"
class CCitiesData
{
public:
	CCitiesData();
	~CCitiesData();
public:
	bool SelectAll(CPtrAutoArray<CITIES>& oCitiesAutoArray);
	BOOL Insert(CITIES& recCity);
	BOOL UpdateWhereID(const long lID, CITIES& recCity);
	BOOL Delete(long lID);
	CCitiesTable oTable;

};