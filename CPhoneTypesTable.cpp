#include "pch.h"
#include "CPhoneTypesTable.h"

#define PHONE_TYPES_TABLE_NAME _T("PHONE_TYPES")
#define SELECT_ALL_PHONE_TYPES_QUERY      _T("SELECT * FROM PHONE_TYPES")
#define SELECT_PHONE_TYPES_BY_ID_QUERY    _T("SELECT * FROM PHONE_TYPES WHERE ID = %d")

CPhoneTypesTable::CPhoneTypesTable() : CGenericTable(m_recPhoneType, PHONE_TYPES_TABLE_NAME)
{
}

CPhoneTypesTable::CPhoneTypesTable(CSession& oSession) 
	: CGenericTable(m_recPhoneType, PHONE_TYPES_TABLE_NAME, oSession)
{
}

CPhoneTypesTable::~CPhoneTypesTable()
{
}
