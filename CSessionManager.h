#pragma once
#include <atldbcli.h>

#define DATABASE_NAME _T("PhoneBook")
#define SERVER_NAME _T("DESKTOP-DVRNDC4")
#define SECURITY_PROVIDER _T("SSPI")

/////////////////////////////////////////////////////////////////////////////
// CSessionManager

/// <summary>
/// Singleton клас за връзка с базата данни
/// </summary>

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
private:
	CSessionManager(const CSessionManager&) = delete;
	CSessionManager& operator=(const CSessionManager&) = delete;
public:
	/// <summary> Метод за достъп до инстанцията на класа </summary>
	static CSessionManager& GetInstance();
	CSession* GetSession();

	/// <summary> Достъп до CDataSource </summary>
	CDataSource* GetDatasource();

	/// <summary> Затваря връзката с базата данни </summary>
	void Close();

private:
	bool Initialize();

	// Overrides
	// ----------------

	// Members
	// ----------------
private:
	/// <summary> Връзката с БД </summary>
	CDataSource m_oDataSource;
	CSession m_oSession;

	bool m_bInitialized;

};

