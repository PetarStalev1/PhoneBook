#pragma once
#include <afxtempl.h>   
#include "Structures.h"
#include "CPhoneTypesTable.h"
typedef CTypedPtrArray<CPtrArray, PHONE_TYPES*> CPhoneTypesArray;

class CPhoneTypesData
{
public:
	CPhoneTypesData();
	~CPhoneTypesData();
public:
	BOOL SelectAll(CPtrAutoArray<PHONE_TYPES>& oPhoneTypesAutoArray);
	BOOL InsertPhoneType(PHONE_TYPES& oPhoneType);
	BOOL DeletePhoneType(long lID);
	BOOL UpdatePhoneType(PHONE_TYPES& oPhoneType);
	bool GetPhoneTypeById(long nPhoneTypeId, PHONE_TYPES& oPhoneType) const;
	
private:
	CPhoneTypesArray m_oPhoneTypeArray;
	CPhoneTypesTable m_oTable;
};

