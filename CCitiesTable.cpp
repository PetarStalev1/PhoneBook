#include "pch.h"
#include "CCitiesTable.h"
#define CITIES_TABLE_NAME				_T("CITIES")
#define SELECT_ALL_CITIES_QUERY           _T("SELECT * FROM CITIES")
#define INSERT_CITY_QUERY                 _T("SELECT TOP 0 * FROM CITIES")
#define SELECT_CITY_BY_ID_QUERY          _T("SELECT * FROM CITIES WHERE ID = %d")
#define SELECT_CITY_BY_NAME_QUERY          _T("SELECT * FROM CITIES WHERE NAME = %s AND REGION = %s" )
#define SELECT_BY_NAME _T("SELECT * FROM CITIES WHERE UPPER([CITY_NAME]) LIKE UPPER(N'%%%s%%') OR UPPER([DISTRICT]) LIKE UPPER(N'%%%s%%')")


/////////////////////////////////////////////////////////////////////////////
// CCitiesTable
// 
// Constructor / Destructor
// ----------------

CCitiesTable::CCitiesTable() :
	CGenericTable(m_recCity, CITIES_TABLE_NAME)
{

}

CCitiesTable::CCitiesTable(CSession& oSession)
	: CGenericTable(m_recCity, CITIES_TABLE_NAME, oSession)
{
}

CCitiesTable::~CCitiesTable()
{

}

