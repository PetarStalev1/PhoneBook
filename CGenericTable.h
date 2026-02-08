#pragma once
#include <atldbcli.h>
#include "CErrorLogger.h"
#include "CPtrAutoArray.h"
#include "CSessionManager.h"
#include "Structures.h"

#define DB_TABLE_OPEN_CONNECTION_ERROR_MESSAGE _T("Unable to get session from Manager")
#define DB_TABLE_QUERY_ERROR_MESSAGE _T("Error when executing query.\n Error: %#lx.\nQuery: \"%s\"")
#define DB_TABLE_ERROR_CODE_MESSAGE _T("\nError: %#lx")

#define DB_TABLE_SELECT_ALL_QUERY  _T("SELECT * FROM %s")
#define DB_TABLE_SELECT_BY_ID_QUERY  _T("SELECT * FROM %s WHERE [ID] = %d")
#define DB_TABLE_SELECT_BY_ID_WITH_LOCK_QUERY _T("SELECT * FROM %s WITH(UPDLOCK) WHERE ID = %d")
#define DB_TABLE_SELECT_NOTHING_QUERY _T("SELECT TOP 0 * FROM %s")

#define DB_TABLE_NO_RESULTS_ERROR_MESSAGE _T("Error: No results found.")
#define DB_TABLE_UPDATE_ERROR_MESSAGE _T("Error when updating.")
#define DB_TABLE_UPDATE_COUNTER_ERROR_MESSAGE _T("Error: Update counter mismatch (Data changed by another user).")
#define DB_TABLE_INSERT_ERROR_MESSAGE _T("Error when inserting.")
#define DB_TABLE_SELECT_ROW_TO_DELETE_ERROR_MESSAGE _T("Error when selecting the row to delete")
#define DB_TABLE_DELETE_ERROR_MESSAGE  _T("Error when deleting.")


/////////////////////////////////////////////////////////////////////////////
// CGenericTable

/// <summary>
/// Базов клас за работа с таблица от БД.
/// </summary>
/// <typeparam name="C"> Аксесор съответстващ на таблицата </typeparam>
template<class TAccessor, class TRecord>
class CGenericTable : protected CCommand<CAccessor<TAccessor>>
{
    // Constructor / Destructor
    // ----------------
public:
    /// <summary> 
    /// Стандартен конструктор. 
    /// Инициализира таблицата, като ще използва споделената сесия от CSessionManager (Singleton).
    /// </summary>
    /// <param name="recRecord"> Референция към структурата (буфера), в която се записват/читат данните (TRecord). </param>
    /// <param name="szTableName"> Името на таблицата в базата данни (напр. _T("PHONE_NUMBERS")). </param>
    CGenericTable(TRecord& recRecord, const TCHAR* szTableName);

    /// <summary> 
    /// Конструктор с външна сесия.
    /// Използва се, когато операциите трябва да бъдат част от по-голяма транзакция, 
    /// управлявана от подадената сесия.
    /// </summary>
    /// <param name="recRecord"> Референция към структурата (буфера) за данните. </param>
    /// <param name="szTableName"> Името на таблицата в базата данни. </param>
    /// <param name="oSession"> Референция към вече отворена OLE DB сесия. </param>
    CGenericTable(TRecord& recRecord, const TCHAR* szTableName, CSession& oSession);

    /// <summary> Виртуален деструктор </summary>
    virtual ~CGenericTable();

    // Methods
    // ----------------
private:
    /// <summary> Задава стойностите на m_oDBPropSet </summary>
    void AddPropSetProperties();

protected:
    /// <summary> Отваря нова сесия </summary>
    BOOL OpenConnection();

    /// <summary> Започва транзакция </summary>
    void CloseConnection();

    /// <summary> Показва съобщение за грешка </summary>
    void ShowErrorMessage(const HRESULT hResult, const CString& strError) const;

    /// <summary> Показва съобщение за грешка </summary>
    void ShowErrorMessage(const CString& strError) const;

    /// <summary> Показва съобщение за грешка </summary>
    void ShowErrorMessageQuery(const HRESULT hResult, const CString& strQuery) const;

    /// <summary> Изпълнява заявка </summary>
    BOOL ExecuteQuery(const CString& strQuery);

    /// <summary> Намира запис по зададено ID, изпълнява се в текущата сесия </summary>
    BOOL SelectWhereIDInSession(const long lID, const BOOL bWithLock);


    /// <summary> Започва транзакция </summary>
    BOOL StartTransaction();

    /// <summary> Завършва транзакция </summary>
    BOOL Commit();

    /// <summary> Спира транзакция </summary>
    BOOL Abort();

public:
    /// <summary> Получава всички записи и пълни масива </summary>
    BOOL SelectAll(CPtrAutoArray<TRecord>& oAutoArray);

    /// <summary> Намира запис по зададено ID </summary>
    BOOL SelectWhereID(const long lID, TRecord& recRecord);

    /// <summary> Променя запис по ID </summary>
    BOOL UpdateWhereID(const long lID, TRecord& recRecord);

    /// <summary> Създава нов запис </summary>
    BOOL InsertRecord(TRecord& recRecord);

    /// <summary> Изтрива запис по ID </summary>
    BOOL DeleteWhereID(const long lID);

    // Members
    // ----------------
private:
    /// <summary> Указател към записа </summary>
    TRecord* m_pRecord;         

protected:
    BOOL m_bExternalConnection; 

    /// <summary> Указател към връзката за БД </summary>
    CString m_szTableName; 
    /// <summary> Текущата сесия </summary>
    CSession* m_pSession;       
    /// <summary> DBPropSet за заявките </summary>
    CDBPropSet m_oDBPropSet;    
};


// Constructor / Destructor
// ----------------

template<class TAccessor, class TRecord>
CGenericTable<TAccessor, TRecord>::CGenericTable(TRecord& recRecord, const TCHAR* szTableName)
    : m_oDBPropSet(DBPROPSET_ROWSET)
{
    m_bExternalConnection = FALSE;
    m_pSession = nullptr;
    m_szTableName = szTableName;
    m_pRecord = &recRecord;

    AddPropSetProperties();
}

template<class TAccessor, class TRecord>
CGenericTable<TAccessor, TRecord>::CGenericTable(TRecord& recRecord, const TCHAR* szTableName, CSession& oSession)
    : m_oDBPropSet(DBPROPSET_ROWSET)
{
    m_bExternalConnection = TRUE;
    m_pSession = &oSession;
    m_szTableName = szTableName;
    m_pRecord = &recRecord;

    AddPropSetProperties();
}

template<class TAccessor, class TRecord>
CGenericTable<TAccessor, TRecord>::~CGenericTable()
{
    
}

// Methods
// ----------------

template<class TAccessor, class TRecord>
void CGenericTable<TAccessor, TRecord>::AddPropSetProperties()
{
    m_oDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
    m_oDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
    m_oDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
    m_oDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

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