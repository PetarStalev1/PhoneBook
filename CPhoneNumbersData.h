#pragma once
#include "Structures.h"
#include "CPhoneNumberTable.h"
#include "CPersonsTable.h"
#include "CPhoneTypesTable.h"
#include "CCitiesTable.h"
#include "CPhoneNumbersData.h"
#include "CDialogChangeContext.h"

class CPhoneNumbersData
{
public:
	CPhoneNumbersData();
	~CPhoneNumbersData();
private:
	CSession* m_pSession = nullptr;
public:
	bool SelectAll(CPtrAutoArray<PHONE_NUMBERS>& oPhoneNumbersAutoArray);
	bool SelectWhereID(const long lID, PHONE_NUMBERS& recPhoneNumber);
	bool UpdateWhereID(long lID,  PHONE_NUMBERS& recPhoneNumbers);
	bool InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber);
	bool DeleteWhereID(const long lID);
	bool ApplyChanges(CDialogChangeContext& ctx);
	BOOL SelectByNameUCNAddress(CPtrAutoArray<PHONE_NUMBERS>& oAutoArray,
		const CString& strName,
		const CString& strUCN,
		const CString& strAddress);
	
public:
	CPhoneNumberTable m_oPhoneNumbersTable;
	CPersonsTable m_oPersonsTable;
	CPhoneTypesTable m_oPhoneTypesTable;
	CCitiesTable m_oCitiesTable;
};

