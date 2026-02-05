#pragma once
#include "CCitiesAccessor.h"
#include "CSessionManager.h"
#include "CGenericTable.h"
#include "Structures.h"


class CCitiesTable : public CGenericTable<CCitiesAccessor,CITIES>
{

// Constructor / Destructor
// ----------------
public:
	CCitiesTable();
	CCitiesTable(CSession& oSession);
	virtual ~CCitiesTable();
	

};
