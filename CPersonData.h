#pragma once

#include "Structures.h"        
#include "CPersonsTable.h"        


class CPersonData
{
public:
	CPersonData();
	~CPersonData();

public:
	bool SelectAll(CPtrAutoArray<PERSONS>& oPersonsAutoArray);
	bool SelectWhereID(const long lID, PERSONS& recPerson);
	bool UpdateWhereID(long lID, PERSONS& recPerson);
	bool InsertPerson(PERSONS& recPerson);
	bool DeleteWhereID(const long lID);
	//BOOL SelectByNameUCNAddress(CPtrAutoArray<CPhoneNumber>& oAutoArray, const CString& strName, const CString& strUCN, const CString& strAddress);

private:
	CPersonsTable m_oPersonsTable;
};

