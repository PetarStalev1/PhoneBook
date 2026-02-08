#include "pch.h"
#include "CSessionManager.h"
#include <atldbcli.h>
#include <atltrace.h> 

/////////////////////////////////////////////////////////////////////////////
// CSessionManager

// Constants
// ----------------


// Constructor / Destructor
// ----------------

CSessionManager::CSessionManager(): m_bInitialized(false)
{
}

CSessionManager::~CSessionManager()
{
    Close();
}

CSessionManager& CSessionManager::GetInstance()
{
    static CSessionManager instance;
    return instance;
}

CSession* CSessionManager::GetSession()
{
    if (!m_bInitialized)
    {
        if (!Initialize())
        {
            return nullptr;
        }
    }

    return &m_oSession;
}

CDataSource* CSessionManager::GetDatasource()
{
    return &m_oDataSource;
}

void CSessionManager::Close()
{
    if (m_oSession.m_spOpenRowset != nullptr)
        m_oSession.Close();

    if (m_oDataSource.m_spInit != nullptr)
        m_oDataSource.Close();

    m_bInitialized = false;
}

bool CSessionManager::Initialize()
{
    CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
    oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, SERVER_NAME);
    oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, SECURITY_PROVIDER);
    oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, DATABASE_NAME);
    oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
    oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
    oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

    HRESULT hResult = m_oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
    if (FAILED(hResult))
    {
        ATLTRACE(_T("Failed to open data source. HRESULT: 0x%08X\n"), hResult);
        return false;
    }

    hResult = m_oSession.Open(m_oDataSource);
    if (FAILED(hResult))
    {
        ATLTRACE(_T("Failed to open session. HRESULT: 0x%08X\n"), hResult);
        m_oDataSource.Close();
        return false;
    }

    m_bInitialized = true;
    return true;
}
// Overrides
// ----------------
