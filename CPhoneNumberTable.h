#pragma once
#include "CPhoneNumberAccessor.h"
#include "Structures.h"
#include "CSessionManager.h"
#include "CGenericTable.h"
typedef CTypedPtrArray<CPtrArray, PHONE_NUMBERS*>CPhoneNumbersArray;


class CPhoneNumberTable : public CGenericTable<CPhoneNumbersAccessor,PHONE_NUMBERS>
{
public:
	CPhoneNumberTable();
	CPhoneNumberTable(CSession& oSession);
	virtual ~CPhoneNumberTable();
	
	bool SelectFromPersonsWhereId(const long lPhoneNumberID);
	BOOL SelectWherePersonID(const long lPersonID, CPtrAutoArray<PHONE_NUMBERS>& oResultArray);

};

