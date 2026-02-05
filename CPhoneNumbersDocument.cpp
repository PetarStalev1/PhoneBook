#include "pch.h"
#include "framework.h"

#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

#include "CPersonData.h"
#include "CPhoneNumbersDocument.h"
#include "CPhoneNumbersData.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ROW_INDEX_NOT_FOUND -1

IMPLEMENT_DYNCREATE(CPhoneNumbersDocument, CDocument)

BEGIN_MESSAGE_MAP(CPhoneNumbersDocument, CDocument)
END_MESSAGE_MAP()


CPhoneNumbersDocument::CPhoneNumbersDocument() noexcept 
{
}

CPhoneNumbersDocument::~CPhoneNumbersDocument()
{
}


void CPhoneNumbersDocument::RebuildIndexes()
{
    m_idxPersons.clear();
    m_idxPhones.clear();
    m_idxCities.clear();
    m_idxPhoneTypes.clear();

    for (int i = 0; i < m_arrPersons.GetCount(); i++)
    {
        PERSONS* p = m_arrPersons.GetAt(i);
        m_idxPersons[p->lID] = p;
    }

    for (int i = 0; i < m_arrPhoneNumbers.GetCount(); i++)
    {
        PHONE_NUMBERS* p = m_arrPhoneNumbers.GetAt(i);
        m_idxPhones[p->lID] = p;
    }

    for (int i = 0; i < m_arrCities.GetCount(); i++)
    {
        CITIES* p = m_arrCities.GetAt(i);
        m_idxCities[p->lID] = p;
    }

    for (int i = 0; i < m_arrPhoneTypes.GetCount(); i++)
    {
        PHONE_TYPES* p = m_arrPhoneTypes.GetAt(i);
        m_idxPhoneTypes[p->lID] = p;
    }
}

BOOL CPhoneNumbersDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;
    
    
    return LoadAll();
}

void CPhoneNumbersDocument::Serialize(CArchive& ar)
{
    //if (ar.IsStoring())
    //{
    //    // TODO: add storing code here
    //}
    //else
    //{
    //    // TODO: add loading code here
    //}
    UNREFERENCED_PARAMETER(ar);
}

void CPhoneNumbersDocument::DeleteContents()
{
    m_arrPersons.RemoveAll();
    m_arrPhoneNumbers.RemoveAll();
    m_arrCities.RemoveAll();
    m_arrPhoneTypes.RemoveAll();

    m_idxPersons.clear();
    m_idxPhones.clear();
    m_idxCities.clear();
    m_idxPhoneTypes.clear();

    

    CDocument::DeleteContents();

}
BOOL CPhoneNumbersDocument::LoadAll()
{
    DeleteContents();

    
    if (!m_oPersonsData.SelectAll (m_arrPersons)) return FALSE;
    if (!m_oPhoneNumbersData.SelectAll(m_arrPhoneNumbers)) return FALSE; 
    if (!m_oCitiesData.SelectAll(m_arrCities)) return FALSE;
    if (!m_oPhoneTypesData.SelectAll(m_arrPhoneTypes)) return FALSE;

    RebuildIndexes();

    UpdateAllViews(nullptr, DocumentDataOperationLoad);
    return TRUE;
}


#ifdef _DEBUG
void CPhoneNumbersDocument::AssertValid() const
{
    CDocument::AssertValid();
}

void CPhoneNumbersDocument::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG



INT_PTR CPhoneNumbersDocument::GetPersonCount() const
{
    return m_arrPersons.GetCount();
}
const PERSONS* CPhoneNumbersDocument::GetPersonAt(INT_PTR nIndex) const
{
    return m_arrPersons.GetAt((int)nIndex);
}

const PERSONS* CPhoneNumbersDocument::GetPersonByID(long lID) const
{
    auto it = m_idxPersons.find(lID);
    return (it != m_idxPersons.end()) ? it->second : nullptr;
}

// ----------------------------

INT_PTR CPhoneNumbersDocument::GetPhoneCount() const
{
    return m_arrPhoneNumbers.GetCount();
}

const PHONE_NUMBERS* CPhoneNumbersDocument::GetPhoneAt(INT_PTR nIndex) const
{
    return m_arrPhoneNumbers.GetAt((int)nIndex);
}

const PHONE_NUMBERS* CPhoneNumbersDocument::GetPhoneByID(long lID) const
{
    auto it = m_idxPhones.find(lID);
    return (it != m_idxPhones.end()) ? it->second : nullptr;
}

// ----------------------------

INT_PTR CPhoneNumbersDocument::GetCityCount() const
{
    return m_arrCities.GetCount();
}

const CITIES* CPhoneNumbersDocument::GetCityAt(INT_PTR nIndex) const
{
    return m_arrCities.GetAt((int)nIndex);
}

const CITIES* CPhoneNumbersDocument::GetCityByID(long lID) const
{
    auto it = m_idxCities.find(lID);
    return (it != m_idxCities.end()) ? it->second : nullptr;
}

// ----------------------------

INT_PTR CPhoneNumbersDocument::GetPhoneTypeCount() const
{
    return m_arrPhoneTypes.GetCount();
}

const PHONE_TYPES* CPhoneNumbersDocument::GetPhoneTypeAt(INT_PTR nIndex) const
{
    return m_arrPhoneTypes.GetAt((int)nIndex);
}

const PHONE_TYPES* CPhoneNumbersDocument::GetPhoneTypeByID(long lID) const
{
    auto it = m_idxPhoneTypes.find(lID);
    return (it != m_idxPhoneTypes.end()) ? it->second : nullptr;
}

BOOL CPhoneNumbersDocument::AddOrUpdatePerson(PERSONS& rec)
{
    auto it = m_idxPersons.find(rec.lID);

    if (it != m_idxPersons.end())
    {
        if (!m_oPersonsData.UpdateWhereID(rec.lID,rec))
        {
            return FALSE;
        }
        *(it->second) = rec;
        UpdateAllViews(nullptr, DocumentDataOperationUpdate, (CObject*)it->second);
    }
    else
    {
        if (!m_oPersonsData.InsertPerson(rec))
        {
            return FALSE;
        }
        PERSONS* p = new PERSONS(rec);
        m_arrPersons.Add(p);
        m_idxPersons[p->lID] = p;
        UpdateAllViews(nullptr, DocumentDataOperationInsert, (CObject*)p);
    }
    return TRUE;
}
BOOL CPhoneNumbersDocument::InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber)
{
    if (!m_oPhoneNumbersData.InsertPhoneNumber(recPhoneNumber))
    {
        return FALSE;
    }
    PHONE_NUMBERS* pNewRecord = new PHONE_NUMBERS(recPhoneNumber);
    m_arrPhoneNumbers.Add(pNewRecord);
    m_idxPhones[pNewRecord->lID] = pNewRecord;
    UpdateAllViews(nullptr, DocumentDataOperationInsert, (CObject*)pNewRecord);
    return TRUE;
}

BOOL CPhoneNumbersDocument::UpdatePhoneNumber(PHONE_NUMBERS& reCPhoneNumber)
{
    if (!m_oPhoneNumbersData.UpdateWhereID(reCPhoneNumber.lID, reCPhoneNumber))
    {
        return FALSE;
    }  
    return TRUE;
}

BOOL CPhoneNumbersDocument::DeletePhoneNumber(long lID)
{
    if (!m_oPhoneNumbersData.DeleteWhereID(lID))
    {
        return FALSE;
    }
    UpdateAllViews(nullptr, DocumentDataOperationDelete, (CObject*)lID);

    return TRUE;
}

BOOL CPhoneNumbersDocument::AddOrUpdatePhone(PHONE_NUMBERS& rec)
{
    auto it = m_idxPhones.find(rec.lID);

    if (it != m_idxPhones.end())
    {
        if (!m_oPhoneNumbersData.UpdateWhereID(rec.lID, rec))
        {
            return FALSE;
        }
        *(it->second) = rec;

        UpdateAllViews(nullptr, DocumentDataOperationUpdate, (CObject*)it->second);

    }
    else
    {
        if (!m_oPhoneNumbersData.InsertPhoneNumber(rec))
        {
            return FALSE;
        }

        PHONE_NUMBERS* p = new PHONE_NUMBERS(rec);
        m_arrPhoneNumbers.Add(p);
        m_idxPhones[p->lID] = p;
        UpdateAllViews(nullptr, DocumentDataOperationInsert, (CObject*)p);

    }

    return TRUE;
}

BOOL CPhoneNumbersDocument::AddOrUpdateCity(CITIES& rec)
{

    if (rec.lID <= 0)
    {
        return m_oCitiesData.Insert(rec);
    }
    return m_oCitiesData.UpdateWhereID(rec.lID,rec);
    auto it = m_idxCities.find(rec.lID);

    if (it != m_idxCities.end())
    {
        if (!m_oCitiesData.UpdateWhereID(rec.lID, rec))
            return FALSE;
        *(it->second) = rec;

        UpdateAllViews(nullptr, DocumentDataOperationUpdate, (CObject*)it->second);
    }
    else
    {
        CITIES recNew = rec;

        // INSERT в БД
        if (!m_oCitiesData.Insert(recNew))
            return FALSE;

        // INSERT в кеша
        CITIES* p = new CITIES(recNew);
        m_arrCities.Add(p);
        m_idxCities[p->lID] = p;

        UpdateAllViews(nullptr, DocumentDataOperationInsert, (CObject*)p);
    }
    return TRUE;
}

BOOL CPhoneNumbersDocument::AddOrUpdatePhoneType(PHONE_TYPES& rec)
{
    auto it = m_idxPhoneTypes.find(rec.lID);

    if (it != m_idxPhoneTypes.end())
    {
        if (!m_oPhoneTypesData.UpdatePhoneType(rec))
            return FALSE;

        *(it->second) = rec;

        UpdateAllViews(nullptr, DocumentDataOperationUpdate, (CObject*)it->second);
    }
    else
    {
        PHONE_TYPES recNew = rec;

        if (!m_oPhoneTypesData.InsertPhoneType(recNew))
            return FALSE;

        PHONE_TYPES* p = new PHONE_TYPES(recNew);
        m_arrPhoneTypes.Add(p);
        m_idxPhoneTypes[p->lID] = p;

        UpdateAllViews(nullptr, DocumentDataOperationInsert, (CObject*)p);
    }
    return TRUE;
}

bool CPhoneNumbersDocument::ApplyChanges(CDialogChangeContext& ctx)
{
    if (!m_oPhoneNumbersData.ApplyChanges(ctx))
    {
        return false;
    }
    POSITION pos = ctx.m_mapChanges.GetStartPosition();
    long lKey;
    ChangeEntry entry;

    while (pos != NULL)
    {
        ctx.m_mapChanges.GetNextAssoc(pos, lKey, entry);

        switch (entry.eEntity)
        {
        case EntityCity:
            ReflectChangeCity(entry);
            break;
        case EntityPerson:
            ReflectChangePerson(entry);
            break;
        case EntityPhone:
            ReflectChangePhone(entry);
            break;
        }
    }

    ctx.m_mapChanges.RemoveAll();

    return true;
}

BOOL CPhoneNumbersDocument::DeletePerson(long lID)
{
    auto it = m_idxPersons.find(lID);
    if (it == m_idxPersons.end()) return FALSE;

    PERSONS* p = it->second;

    if (!m_oPersonsData.DeleteWhereID(lID)) return FALSE;

    m_idxPersons.erase(it);

    UpdateAllViews(nullptr, DocumentDataOperationDelete, (CObject*)p);

    INT_PTR nIndex = m_arrPersons.GetIndex(p);
    if (nIndex != -1)
    {
        m_arrPersons.RemoveAt(nIndex); 
    }

    return TRUE;
}

BOOL CPhoneNumbersDocument::DeletePhone(long lID)
{
    auto it = m_idxPhones.find(lID);
    if (it == m_idxPhones.end()) return FALSE;

    PHONE_NUMBERS* p = it->second;

    if (!m_oPhoneNumbersData.DeleteWhereID(lID)) return FALSE;

    m_idxPhones.erase(it);

    UpdateAllViews(nullptr, DocumentDataOperationDelete, (CObject*)p);

    INT_PTR nIndex = m_arrPhoneNumbers.GetIndex(p);
    if (nIndex != -1)
    {
        m_arrPhoneNumbers.RemoveAt(nIndex);
    }

    return TRUE;
}

BOOL CPhoneNumbersDocument::DeleteCity(long lID)
{
    auto it = m_idxCities.find(lID);
    if (it == m_idxCities.end())
        return FALSE;

    CITIES* pCity = it->second;

    for (int i = 0; i < m_arrPersons.GetCount(); i++)
    {
        if (m_arrPersons.GetAt(i)->lCityID == lID)
        {
            AfxMessageBox(_T("Не може да изтриете този град, защото се използва от потребители!"), MB_ICONSTOP);
            return FALSE;
        }
    }

    if (!m_oCitiesData.Delete(lID)) 
        return FALSE;

    m_idxCities.erase(it);

    UpdateAllViews(nullptr, DocumentDataOperationDelete, (CObject*)pCity);

    for (int i = 0; i < m_arrCities.GetCount(); i++)
    {
        if (m_arrCities.GetAt(i) == pCity)
        {
            m_arrCities.RemoveAt(i);
            break;
        }
    }

    return TRUE;
}

BOOL CPhoneNumbersDocument::DeletePhoneType(long lID)
{
    auto it = m_idxPhoneTypes.find(lID);
    if (it == m_idxPhoneTypes.end())
        return FALSE;

    PHONE_TYPES* pType = it->second;

    for (int i = 0; i < m_arrPhoneNumbers.GetCount(); i++)
    {
        if (m_arrPhoneNumbers.GetAt(i)->lPhoneTypeID == lID)
        {
            AfxMessageBox(_T("Не може да изтриете този тип, защото има телефони от този вид!"), MB_ICONSTOP);
            return FALSE;
        }
    }

    if (!m_oPhoneTypesData.DeletePhoneType(lID))
        return FALSE;

    m_idxPhoneTypes.erase(it);

    UpdateAllViews(nullptr, DocumentDataOperationDelete, (CObject*)pType);

    for (int i = 0; i < m_arrPhoneTypes.GetCount(); i++)
    {
        if (m_arrPhoneTypes.GetAt(i) == pType)
        {
            m_arrPhoneTypes.RemoveAt(i);
            break;
        }
    }

    return TRUE;
}
void CPhoneNumbersDocument::ReflectChangeCity(ChangeEntry& entry)
{
    CITIES* pCtxRecord = (CITIES*)entry.pRecord;

    switch (entry.eChange)
    {
    case ChangeInsert:
    {
        CITIES* pNewCache = new CITIES(*pCtxRecord);
        m_arrCities.Add(pNewCache);
        m_idxCities[pNewCache->lID] = pNewCache;
        break;
    }
    case ChangeUpdate:
    {
        auto it = m_idxCities.find(pCtxRecord->lID);
        if (it != m_idxCities.end())
        {
            CITIES* pCache = it->second;
            *pCache = *pCtxRecord;
        }
        break;
    }
    case ChangeDelete:
    {
        long lID = pCtxRecord->lID;
        auto it = m_idxCities.find(lID);
        if (it != m_idxCities.end())
        {
            CITIES* pCache = it->second;
            INT_PTR nIndex = m_arrCities.GetIndex(pCache);
            m_idxCities.erase(it);
            if (nIndex != -1) m_arrCities.RemoveAt(nIndex);

        }
        break;
    }
    }

    delete pCtxRecord;
}

void CPhoneNumbersDocument::ReflectChangePerson(ChangeEntry& entry)
{
    PERSONS* pCtxRecord = (PERSONS*)entry.pRecord;

    switch (entry.eChange)
    {
    case ChangeInsert:
    {
        PERSONS* pNewCache = new PERSONS(*pCtxRecord);
        m_arrPersons.Add(pNewCache);
        m_idxPersons[pNewCache->lID] = pNewCache;
        break;
    }
    case ChangeUpdate:
    {
        auto it = m_idxPersons.find(pCtxRecord->lID);
        if (it != m_idxPersons.end())
        {
            PERSONS* pCache = it->second;
            *pCache = *pCtxRecord;
            for (int i = 0; i < m_arrPhoneNumbers.GetCount(); i++)
            {
                PHONE_NUMBERS* pPhone = m_arrPhoneNumbers.GetAt(i);

                if (pPhone->lPersonID == pCache->lID)
                {
                    UpdateAllViews(nullptr, DocumentDataOperationUpdate, (CObject*)pPhone);
                }
            }
        }
        break;
    }
    case ChangeDelete:
    {
        long lID = pCtxRecord->lID;
        auto it = m_idxPersons.find(lID);
        if (it != m_idxPersons.end())
        {
            PERSONS* pCache = it->second;
            m_idxPersons.erase(it);
            INT_PTR nIndex = m_arrPersons.GetIndex(pCache);
            if (nIndex != -1) m_arrPersons.RemoveAt(nIndex);

        }
        break;
    }
    }

    delete pCtxRecord;
}

void CPhoneNumbersDocument::ReflectChangePhone(ChangeEntry& entry)
{
    PHONE_NUMBERS* pCtxRecord = (PHONE_NUMBERS*)entry.pRecord;

    switch (entry.eChange)
    {
    case ChangeInsert:
    {
        PHONE_NUMBERS* pNewCache = new PHONE_NUMBERS(*pCtxRecord);
        m_arrPhoneNumbers.Add(pNewCache);
        m_idxPhones[pNewCache->lID] = pNewCache;
        UpdateAllViews(nullptr, DocumentDataOperationInsert, (CObject*)pNewCache);
        break;
    }
    case ChangeUpdate:
    {
        auto it = m_idxPhones.find(pCtxRecord->lID);
        if (it != m_idxPhones.end())
        {
            PHONE_NUMBERS* pCache = it->second;
            *pCache = *pCtxRecord;
            UpdateAllViews(nullptr, DocumentDataOperationUpdate, (CObject*)pCache);
        }
        break;
    }
    case ChangeDelete:
    {
        long lID = pCtxRecord->lID;
        auto it = m_idxPhones.find(lID);
        if (it != m_idxPhones.end())
        {
            PHONE_NUMBERS* pCache = it->second;
            m_idxPhones.erase(it);
            INT_PTR nIndex = m_arrPhoneNumbers.GetIndex(pCache);
            if (nIndex != -1) m_arrPhoneNumbers.RemoveAt(nIndex);

            UpdateAllViews(nullptr, DocumentDataOperationDelete, (CObject*)lID);
        }
        break;
    }
    }

    delete pCtxRecord;
}