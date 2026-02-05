#pragma once
#include "pch.h"
#include <atldbcli.h>
#include "CPersonAccessor.h"
#include "Structures.h" 
#include "CSessionManager.h"

#include "CGenericTable.h"
typedef CTypedPtrArray<CPtrArray, PERSONS*>CPersonsArray;

class CPersonsTable : public CGenericTable<CPersonsAccessor, PERSONS>
{

public:
	CPersonsTable();
	CPersonsTable(CSession& oSession);
	~CPersonsTable();


	// Methods
	// ----------------

	///// <summary> Получава всички записи по име </summary>
	BOOL SelectByNameUCNAddress(CPtrAutoArray<PERSONS>& oAutoArray, const CString& strName, const CString& strUCN, const CString& strAddress);


	///// <summary> Получава всички записи по азбучен ред и пълни масива </summary>
	BOOL SelectAllSorted(CPtrAutoArray<PERSONS>& oAutoArray);

	 
private:

};

