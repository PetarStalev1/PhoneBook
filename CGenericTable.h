#pragma once
#include <atldbcli.h>
#include "CErrorLogger.h"
#include "CPtrAutoArray.h"
#include "CSessionManager.h"
#include "Structures.h"

// Макроси за грешки
#define DB_TABLE_OPEN_CONNECTION_ERROR_MESSAGE _T("Unable to get session from Manager")
#define DB_TABLE_QUERY_ERROR_MESSAGE _T("Error when executing query.\n Error: %#lx.\nQuery: \"%s\"")
#define DB_TABLE_ERROR_CODE_MESSAGE _T("\nError: %#lx")

// SQL Шаблони
#define DB_TABLE_SELECT_ALL_QUERY  _T("SELECT * FROM %s")
#define DB_TABLE_SELECT_BY_ID_QUERY  _T("SELECT * FROM %s WHERE [ID] = %d")
#define DB_TABLE_SELECT_BY_ID_WITH_LOCK_QUERY _T("SELECT * FROM %s WITH(UPDLOCK) WHERE ID = %d")
#define DB_TABLE_SELECT_NOTHING_QUERY _T("SELECT TOP 0 * FROM %s")

// Съобщения
#define DB_TABLE_NO_RESULTS_ERROR_MESSAGE _T("Error: No results found.")
#define DB_TABLE_UPDATE_ERROR_MESSAGE _T("Error when updating.")
#define DB_TABLE_UPDATE_COUNTER_ERROR_MESSAGE _T("Error: Update counter mismatch (Data changed by another user).")
#define DB_TABLE_INSERT_ERROR_MESSAGE _T("Error when inserting.")
#define DB_TABLE_SELECT_ROW_TO_DELETE_ERROR_MESSAGE _T("Error when selecting the row to delete")
#define DB_TABLE_DELETE_ERROR_MESSAGE  _T("Error when deleting.")


/////////////////////////////////////////////////////////////////////////////
// CGenericTable
//
// TAccessor: Клас Аксесор (напр. CPhoneNumberAccessor)
// TRecord:   Структура с данни (напр. PHONE_NUMBERS)
template<class TAccessor, class TRecord>
class CGenericTable : protected CCommand<CAccessor<TAccessor>>
{
    // Constructor / Destructor
    // ----------------
public:
    // Конструктор 1: Използва Singleton сесията (Стандартен)
    CGenericTable(TRecord& recRecord, const TCHAR* szTableName);

    // Конструктор 2: Използва външна сесия (за Транзакции)
    CGenericTable(TRecord& recRecord, const TCHAR* szTableName, CSession& oSession);

    virtual ~CGenericTable();

    // Methods
    // ----------------
private:
    void AddPropSetProperties();

protected:
    // Управление на връзката
    BOOL OpenConnection();
    void CloseConnection();

    // Помощни функции
    void ShowErrorMessage(const HRESULT hResult, const CString& strError) const;
    void ShowErrorMessage(const CString& strError) const;
    void ShowErrorMessageQuery(const HRESULT hResult, const CString& strQuery) const;

    // Изпълнение на заявки
    BOOL ExecuteQuery(const CString& strQuery);
    BOOL SelectWhereIDInSession(const long lID, const BOOL bWithLock);

    // Транзакции (Само ако не е външна връзка или ако искаме да управляваме текущата)
    BOOL StartTransaction();
    BOOL Commit();
    BOOL Abort();

public:
    // CRUD Операции
    BOOL SelectAll(CPtrAutoArray<TRecord>& oAutoArray);
    BOOL SelectWhereID(const long lID, TRecord& recRecord);
    BOOL UpdateWhereID(const long lID, TRecord& recRecord);
    BOOL InsertRecord(TRecord& recRecord);
    BOOL DeleteWhereID(const long lID);

    // Members
    // ----------------
private:
    TRecord* m_pRecord;         // Указател към структурата с данни

protected:
    BOOL m_bExternalConnection; // Дали ползваме наша сесия или подадена отвън
    CString m_szTableName;      // Име на таблицата
    CSession* m_pSession;       // Текущата сесия
    CDBPropSet m_oDBPropSet;    // Настройки
};


// Constructor 1: Default (Singleton Session)
template<class TAccessor, class TRecord>
CGenericTable<TAccessor, TRecord>::CGenericTable(TRecord& recRecord, const TCHAR* szTableName)
    : m_oDBPropSet(DBPROPSET_ROWSET)
{
    m_bExternalConnection = FALSE;
    m_pSession = nullptr; // Ще го вземем при OpenConnection
    m_szTableName = szTableName;
    m_pRecord = &recRecord;

    AddPropSetProperties();
}

// Constructor 2: External Session
template<class TAccessor, class TRecord>
CGenericTable<TAccessor, TRecord>::CGenericTable(TRecord& recRecord, const TCHAR* szTableName, CSession& oSession)
    : m_oDBPropSet(DBPROPSET_ROWSET)
{
    m_bExternalConnection = TRUE;
    m_pSession = &oSession; // Ползваме подадената сесия
    m_szTableName = szTableName;
    m_pRecord = &recRecord;

    AddPropSetProperties();
}

template<class TAccessor, class TRecord>
CGenericTable<TAccessor, TRecord>::~CGenericTable()
{
    
}

template<class TAccessor, class TRecord>
void CGenericTable<TAccessor, TRecord>::AddPropSetProperties()
{
    m_oDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
    m_oDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
    m_oDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
    m_oDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

// Connection Logic
template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::OpenConnection()
{
    if (m_bExternalConnection) return TRUE; 

    m_pSession = CSessionManager::GetInstance().GetSession();
    if (m_pSession == nullptr)
    {
        ShowErrorMessage(E_FAIL, DB_TABLE_OPEN_CONNECTION_ERROR_MESSAGE);
        return FALSE;
    }
    return TRUE;
}

template<class TAccessor, class TRecord>
void CGenericTable<TAccessor, TRecord>::CloseConnection()
{
    if (!m_bExternalConnection)
        m_pSession = nullptr;
}

// Transactions
template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::StartTransaction()
{
    if (m_bExternalConnection) return TRUE;
    if (!m_pSession) return FALSE;
    return !FAILED(m_pSession->StartTransaction());
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::Abort()
{
    if (m_bExternalConnection) return TRUE;
    if (!m_pSession) return FALSE;
    return !FAILED(m_pSession->Abort());
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::Commit()
{
    if (m_bExternalConnection) return TRUE;
    if (!m_pSession) return FALSE;
    return !FAILED(m_pSession->Commit());
}

// Error Handling
template<class TAccessor, class TRecord>
void CGenericTable<TAccessor, TRecord>::ShowErrorMessageQuery(const HRESULT hResult, const CString& strQuery) const
{
    CString strError;
    strError.Format(DB_TABLE_QUERY_ERROR_MESSAGE, hResult, strQuery);
    CErrorLogger::LogMessage(strError, TRUE, FALSE);
}

template<class TAccessor, class TRecord>
void CGenericTable<TAccessor, TRecord>::ShowErrorMessage(const HRESULT hResult, const CString& strError) const
{
    CString strMessage;
    strMessage.Format(DB_TABLE_ERROR_CODE_MESSAGE, hResult);
    strMessage = strError + strMessage;
    CErrorLogger::LogMessage(strMessage, TRUE, FALSE);
}

template<class TAccessor, class TRecord>
void CGenericTable<TAccessor, TRecord>::ShowErrorMessage(const CString& strError) const
{
    CErrorLogger::LogMessage(strError, TRUE, FALSE);
}

// Execution
template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::ExecuteQuery(const CString& strQuery)
{
    const HRESULT hResult = Open(*m_pSession, strQuery, &m_oDBPropSet);
    if (FAILED(hResult))
    {
        ShowErrorMessageQuery(hResult, strQuery);
        return FALSE;
    }
    return TRUE;
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::SelectWhereIDInSession(const long lID, const BOOL bWithLock)
{
    CString strQuery;
    if (bWithLock)
        strQuery.Format(DB_TABLE_SELECT_BY_ID_WITH_LOCK_QUERY, m_szTableName, lID);
    else
        strQuery.Format(DB_TABLE_SELECT_BY_ID_QUERY, m_szTableName, lID);

    return ExecuteQuery(strQuery);
}

// CRUD Implementations
template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::SelectAll(CPtrAutoArray<TRecord>& oAutoArray)
{
    if (!OpenConnection()) return FALSE;

    CString strQuery;
    strQuery.Format(DB_TABLE_SELECT_ALL_QUERY, m_szTableName);

    if (!ExecuteQuery(strQuery))
    {
        CloseConnection();
        return FALSE;
    }

    while (MoveNext() == S_OK)
    {
        TRecord* pRecordToAdd = new TRecord((*m_pRecord));

        oAutoArray.Add(pRecordToAdd);
    }

    Close();
    CloseConnection();
    return TRUE;
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::SelectWhereID(const long lID, TRecord& recRecord)
{
    if (!OpenConnection()) return FALSE;

    if (!SelectWhereIDInSession(lID, FALSE))
    {
        CloseConnection();
        return FALSE;
    }

    if (MoveNext() != S_OK)
    {
        ShowErrorMessage(DB_TABLE_NO_RESULTS_ERROR_MESSAGE); 
        Close();
        CloseConnection();
        return FALSE;
    }

    recRecord = (*m_pRecord);

    Close();
    CloseConnection();
    return TRUE;
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::UpdateWhereID(const long lID, TRecord& recRecord)
{
    if (!OpenConnection()) return FALSE;
    StartTransaction();

    if (!SelectWhereIDInSession(lID, TRUE))
    {
        Abort();
        CloseConnection();
        return FALSE;
    }

    if (MoveFirst() != S_OK)
    {
        ShowErrorMessage(DB_TABLE_NO_RESULTS_ERROR_MESSAGE);
        Abort();
        Close();
        CloseConnection();
        return FALSE;
    }

    if (recRecord.lUpdateCounter != (*m_pRecord).lUpdateCounter)
    {
        ShowErrorMessage(DB_TABLE_UPDATE_COUNTER_ERROR_MESSAGE);
        Abort();
        Close();
        CloseConnection();
        return FALSE;
    }

    (*m_pRecord) = recRecord;
    (*m_pRecord).lUpdateCounter++;
    recRecord.lUpdateCounter++;
    HRESULT hResult = SetData(1);
    if (FAILED(hResult))
    {
        Abort();
        ShowErrorMessage(hResult, DB_TABLE_UPDATE_ERROR_MESSAGE);
        Close();
        CloseConnection();
        return FALSE;
    }

    Commit();
    Close();
    CloseConnection();
    return TRUE;
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::InsertRecord(TRecord& recRecord)
{
    if (!OpenConnection()) return FALSE;

    CString strQuery;
    strQuery.Format(DB_TABLE_SELECT_NOTHING_QUERY, m_szTableName);

    if (!ExecuteQuery(strQuery))
    {
        CloseConnection();
        return FALSE;
    }

    (*m_pRecord) = recRecord;
    (*m_pRecord).lUpdateCounter = 0;

    HRESULT hResult = Insert(1, false);
    if (FAILED(hResult))
    {
        Close();
        ShowErrorMessage(hResult, DB_TABLE_INSERT_ERROR_MESSAGE);
        CloseConnection();
        return FALSE;
    }

    MoveFirst();
    recRecord = (*m_pRecord);

    Close();
    CloseConnection();
    return TRUE;
}

template<class TAccessor, class TRecord>
BOOL CGenericTable<TAccessor, TRecord>::DeleteWhereID(const long lID)
{
    if (!OpenConnection()) return FALSE;

    CString strQuery;
    strQuery.Format(DB_TABLE_SELECT_BY_ID_WITH_LOCK_QUERY, m_szTableName, lID);

    if (!ExecuteQuery(strQuery))
    {
        CloseConnection();
        return FALSE;
    }

    if (MoveFirst() != S_OK)
    {
        ShowErrorMessage(DB_TABLE_SELECT_ROW_TO_DELETE_ERROR_MESSAGE);
        Close();
        CloseConnection();
        return FALSE;
    }

    HRESULT hResult = Delete();
    if (FAILED(hResult))
    {
        ShowErrorMessage(hResult, DB_TABLE_DELETE_ERROR_MESSAGE);
        Close();
        CloseConnection();
        return FALSE;
    }

    Close();
    CloseConnection();
    return TRUE;
}