#include "pch.h"
#include "CPhoneNumberTable.h"
#include <afxdb.h>
#define PHONE_NUMBERS_TABLE_NAME _T("PHONE_NUMBERS")
#define SELECT_WHERE_PERSON_ID_QUERY _T("SELECT * FROM PHONE_NUMBERS WHERE [PERSON_ID] = %d")



CPhoneNumberTable::CPhoneNumberTable() :
	CGenericTable(m_recPhoneNumber, PHONE_NUMBERS_TABLE_NAME)
{
}


CPhoneNumberTable::CPhoneNumberTable(CSession& oSession)
	: CGenericTable(m_recPhoneNumber,PHONE_NUMBERS_TABLE_NAME, oSession)
{
}
CPhoneNumberTable::~CPhoneNumberTable()
{
}

// CPhoneNumbersTable.cpp

BOOL CPhoneNumberTable::SelectWherePersonID(const long lPersonID, CPtrAutoArray<PHONE_NUMBERS>& oResultArray)
{
    if (!OpenConnection()) return FALSE;

    CString strQuery;
    strQuery.Format(_T("SELECT * FROM PHONE_NUMBERS WHERE PERSON_ID = %d"), lPersonID);

    if (!ExecuteQuery(strQuery))
    {
        CloseConnection();
        return FALSE;
    }

    while (MoveNext() == S_OK)
    {
        PHONE_NUMBERS* pPhone = new PHONE_NUMBERS(m_recPhoneNumber);
        oResultArray.Add(pPhone);
    }

    Close();
    CloseConnection();
    return TRUE;
}


bool CPhoneNumberTable::SelectFromPersonsWhereId(const long lPersonID)
{

	CString strQuery;
	strQuery.Format(SELECT_WHERE_PERSON_ID_QUERY, lPersonID);

	return ExecuteQuery(strQuery.GetString());
}

