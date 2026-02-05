#include "pch.h"
#include "CPersonData.h"
#include "CCitiesData.h"


CPersonData::CPersonData()
{
	
}
CPersonData::~CPersonData()
{

}
bool CPersonData::SelectAll(CPtrAutoArray<PERSONS>& oPersonsAutoArray)
{
	return m_oPersonsTable.SelectAll(oPersonsAutoArray);
}
bool CPersonData::SelectWhereID(const long lID, PERSONS& recPerson)
{
	return m_oPersonsTable.SelectWhereID(lID,recPerson);
}
bool CPersonData::UpdateWhereID(long lID, PERSONS& recPerson)
{
	return m_oPersonsTable.UpdateWhereID(lID,recPerson);
}
bool CPersonData::InsertPerson(PERSONS& recPerson)
{
	return m_oPersonsTable.InsertRecord(recPerson);
}
bool CPersonData::DeleteWhereID(const long lID)
{
	return m_oPersonsTable.DeleteWhereID(lID);
}
