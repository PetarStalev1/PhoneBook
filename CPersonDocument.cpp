#include "pch.h"
#include "CPersonDocument.h"

CPersonDocument::CPersonDocument() noexcept
{

}
CPersonDocument::~CPersonDocument()
{
	
}

void CPersonDocument::AssertValid() const
{
}

void CPersonDocument::Dump(CDumpContext& dc) const
{
}

void CPersonDocument::ReloadPersons()
{
}

bool CPersonDocument::InsertPerson(PERSONS& oPerson)
{
	return false;
}

bool CPersonDocument::DeletePerson(PERSONS& oPerson)
{
	return false;
}



bool CPersonDocument::SelectPersonByEGN(PERSONS& oPerson)
{
	return false;
}

BOOL CPersonDocument::OnNewDocument()
{
	return 0;
}

void CPersonDocument::Serialize(CArchive& ar)
{
}
