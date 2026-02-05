#pragma once
#include <atldbcli.h>

#define DATABASE_NAME _T("PhoneBook")
#define SERVER_NAME _T("DESKTOP-DVRNDC4")
#define SECURITY_PROVIDER _T("SSPI")

class CSessionManager
{
	// Constants
	// ----------------
public:

	// Constructor / Destructor
	// ----------------
private:
	CSessionManager();
	virtual ~CSessionManager();

	// Methods
	// ----------------
public:
	static CSessionManager& GetInstance();
	CSession* GetSession();
	CDataSource* GetDatasource();
	void Close();

private:
	bool Initialize();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	CDataSource m_oDataSource;
	CSession m_oSession;
	bool m_bInitialized;

	CSessionManager(const CSessionManager&) = delete;
	CSessionManager& operator=(const CSessionManager&) = delete;
};

