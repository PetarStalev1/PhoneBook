#pragma once
#include "CPhoneTypesAccessor.h"
#include "Structures.h"
#include "CSessionManager.h"
#include "CGenericTable.h"



class CPhoneTypesTable :public CGenericTable<CPhoneTypesAccessor, PHONE_TYPES>
{
public:
	CPhoneTypesTable();
	CPhoneTypesTable(CSession& oSession);
	virtual ~CPhoneTypesTable();
	
private:

};

