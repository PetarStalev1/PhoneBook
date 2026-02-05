#pragma once
#include <afxwin.h>
#include "Structures.h"
#include "CPersonData.h" 

class CPersonDocument : public CDocument
{
public:
	CPersonDocument() noexcept;


	enum class UpdateHints
	{
		Insert = 0,
		Delete = 1,
		Update = 2
	};

public:
	CPersonData m_oPersonData;
	
	
	const CPersonData& GetPersonData() const { return m_oPersonData; }

	void ReloadPersons();
	bool InsertPerson(PERSONS& oPerson);
	bool DeletePerson(PERSONS& oPerson);
	bool UpdatePerson(PERSONS& oPerson);
	bool SelectPersonByEGN(PERSONS& oPerson);

	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	// Implementation
public:
	virtual ~CPersonDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif




};