#include "pch.h"
#include "CPhoneNumbersData.h"
#include "CPhoneNumberTable.h"


/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersData

// Constructor / Destructor
// ----------------


CPhoneNumbersData::CPhoneNumbersData()
{
}

CPhoneNumbersData::~CPhoneNumbersData()
{
}

// Methods
// ----------------
bool CPhoneNumbersData::SelectAll(CPtrAutoArray<PHONE_NUMBERS>& oPhoneNumbersAutoArray)
{
	return m_oPhoneNumbersTable.SelectAll(oPhoneNumbersAutoArray);
}


bool CPhoneNumbersData::UpdateWhereID(long lID,  PHONE_NUMBERS& recPhoneNumbers)
{
    
    return m_oPhoneNumbersTable.UpdateWhereID(lID, recPhoneNumbers);
}


bool CPhoneNumbersData::SelectWhereID(const long lID, PHONE_NUMBERS& recPhoneNumber)
{
	return m_oPhoneNumbersTable.SelectWhereID(lID, recPhoneNumber);
}

bool CPhoneNumbersData::InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber)
{
	return m_oPhoneNumbersTable.InsertRecord(recPhoneNumber);
}
bool CPhoneNumbersData::DeleteWhereID(const long lID)
{
	return m_oPhoneNumbersTable.DeleteWhereID(lID);
}


bool CPhoneNumbersData::ApplyChanges(CDialogChangeContext& ctx)
{
    CMap<long, long, long, long> mapTempToRealIDs;

    CArray<ChangeEntry, ChangeEntry&> arrCities;
    CArray<ChangeEntry, ChangeEntry&> arrPersons;
    CArray<ChangeEntry, ChangeEntry&> arrPhones;
    CArray<ChangeEntry, ChangeEntry&> arrDeletes;

    POSITION pos = ctx.m_mapChanges.GetStartPosition();
    long lKey;
    ChangeEntry entry;



    while (pos != NULL)
    {
        ctx.m_mapChanges.GetNextAssoc(pos, lKey, entry);

        if (entry.eChange == ChangeDelete)
        {
            arrDeletes.Add(entry);
            continue;
        }

        switch (entry.eEntity)
        {
        case EntityCity:      arrCities.Add(entry); break;
        case EntityPerson:    arrPersons.Add(entry); break;
        case EntityPhone:     arrPhones.Add(entry); break;
        }
    }
    CSession* pSession = CSessionManager::GetInstance().GetSession();
    if (!pSession)
        return FALSE;
    pSession->StartTransaction();

    for (INT_PTR i = 0; i < arrCities.GetCount(); i++)
    {
        ChangeEntry& item = arrCities.ElementAt(i);
        CITIES* pCity = (CITIES*)item.pRecord;
        long lOldID = pCity->lID;



        switch (item.eChange)
        {
        case ChangeInsert:
            if (!CCitiesTable(*pSession).InsertRecord(*pCity)) pSession->Abort();
            break;
        case ChangeUpdate:
            if (!CCitiesTable(*pSession).UpdateWhereID(pCity->lID, *pCity)) pSession->Abort();
            break;
        default:
            break;
        }

        if (lOldID != pCity->lID)
        {
            mapTempToRealIDs.SetAt(lOldID, pCity->lID);
        }
    }
    for (INT_PTR i = 0; i < arrPersons.GetCount(); i++)
    {
        ChangeEntry& item = arrPersons.ElementAt(i);
        PERSONS* pPerson = (PERSONS*)item.pRecord;
        long lOldID = pPerson->lID;

        long lRealCityID;
        if (mapTempToRealIDs.Lookup(pPerson->lCityID, lRealCityID))
        {
            pPerson->lCityID = lRealCityID;
        }
        
        switch (item.eChange)
        {
        case ChangeInsert:
            if(!CPersonsTable(*pSession).InsertRecord(*pPerson)) pSession->Abort();
            break;
        case ChangeUpdate:
            if (!CPersonsTable(*pSession).UpdateWhereID(pPerson->lID, *pPerson)) pSession->Abort();
            break;
        default:
            break;
        }

        if (lOldID != pPerson->lID)
        {
            mapTempToRealIDs.SetAt(lOldID, pPerson->lID);
        }
    }

    for (INT_PTR i = 0; i < arrPhones.GetCount(); i++)
    {
        ChangeEntry& item = arrPhones.ElementAt(i);
        PHONE_NUMBERS* pPhone = (PHONE_NUMBERS*)item.pRecord;

        long lRealPersonID;
        if (mapTempToRealIDs.Lookup(pPhone->lPersonID, lRealPersonID))
        {
            pPhone->lPersonID = lRealPersonID;
        }

        switch (item.eChange)
        {
        case ChangeInsert:
            if (!CPhoneNumberTable(*pSession).InsertRecord(*pPhone)) pSession->Abort();

            break;
        case ChangeUpdate:
            if (!CPhoneNumberTable(*pSession).UpdateWhereID(pPhone->lID, *pPhone)) pSession->Abort();
            break;
        default:
            break;
        }

    }
 

    for (INT_PTR i = 0; i < arrDeletes.GetCount(); i++)
    {
        ChangeEntry& item = arrDeletes.ElementAt(i);

        long lIDToDelete = 0;

        if (item.eEntity == EntityPerson) lIDToDelete = ((PERSONS*)item.pRecord)->lID;
        else if (item.eEntity == EntityCity) lIDToDelete = ((CITIES*)item.pRecord)->lID;
        else if (item.eEntity == EntityPhone) lIDToDelete = ((PHONE_NUMBERS*)item.pRecord)->lID;

        if (lIDToDelete <= 0) continue;

        switch (item.eEntity)
        {
        case EntityPhone:   CPhoneNumberTable(*pSession).DeleteWhereID(lIDToDelete); break;
        case EntityPerson: CPersonsTable(*pSession).DeleteWhereID(lIDToDelete); break;
        case EntityCity:   CCitiesTable(*pSession).DeleteWhereID(lIDToDelete); break;
        }
    }
    pSession->Commit();
    CSessionManager::GetInstance().Close();
    return true;
}

BOOL CPhoneNumbersData::SelectByNameUCNAddress(CPtrAutoArray<PHONE_NUMBERS>& oAutoArray,
    const CString& strName,
    const CString& strUCN,
    const CString& strAddress)
{
    CSession* pSession = CSessionManager::GetInstance().GetSession();
    if (!pSession) return FALSE;

    CPersonsTable oPersonsTable(*pSession);
    CPhoneNumberTable oPhoneNumbersTable(*pSession);

    CPtrAutoArray<PERSONS> arrPersons;
    if (!oPersonsTable.SelectByNameUCNAddress(arrPersons, strName, strUCN, strAddress))
    {
        return FALSE;
    }

    for (INT_PTR i = 0; i < arrPersons.GetCount(); i++)
    {
        PERSONS* pPerson = arrPersons.GetAt(i);
        long lCurrentPersonID = pPerson->lID;

        CPtrAutoArray<PHONE_NUMBERS> arrPhonesForThisPerson;
        oPhoneNumbersTable.SelectWherePersonID(lCurrentPersonID, arrPhonesForThisPerson);

        for (INT_PTR j = 0; j < arrPhonesForThisPerson.GetCount(); j++)
        {
            PHONE_NUMBERS* pSourcePhone = arrPhonesForThisPerson.GetAt(j);

            PHONE_NUMBERS* pNewPhoneRec = new PHONE_NUMBERS;

            *pNewPhoneRec = *pSourcePhone;

            oAutoArray.Add(pNewPhoneRec);
        }
    }

    return TRUE;
}
