#pragma once
#include <afxwin.h>
#include <unordered_map>

#include "Structures.h"
#include "CPtrAutoArray.h"

#include "CPhoneNumbersData.h"
#include "CPersonData.h"
#include "CCitiesData.h"
#include "CPhoneTypesData.h"
#include "CDialogChangeContext.h"

enum DocumentDataOperation
{
    DocumentDataOperationLoad = 0,
    DocumentDataOperationInsert,
    DocumentDataOperationUpdate,
    DocumentDataOperationDelete,
    DocumentDataOperationUpdatePerson
};

class CPhoneNumbersDocument : public CDocument
{
    DECLARE_DYNCREATE(CPhoneNumbersDocument)

public:
    CPhoneNumbersDocument() noexcept;
    virtual ~CPhoneNumbersDocument();

    virtual BOOL OnNewDocument() override;
    virtual void Serialize(CArchive& ar) override;
    virtual void DeleteContents() override;

#ifdef _DEBUG
    virtual void AssertValid() const override;
    virtual void Dump(CDumpContext& dc) const override;
#endif

    BOOL LoadAll();
    BOOL InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber);
    BOOL UpdatePhoneNumber(PHONE_NUMBERS& reCPhoneNumber);
    BOOL DeletePhoneNumber(long lID);
    BOOL AddOrUpdatePerson(PERSONS& rec);
    BOOL AddOrUpdatePhone(PHONE_NUMBERS& rec);
    BOOL AddOrUpdateCity(CITIES& rec);
    BOOL AddOrUpdatePhoneType(PHONE_TYPES& rec);
    bool ApplyChanges(CDialogChangeContext& ctx);
    BOOL DeletePerson(long lID);
    BOOL DeletePhone(long lID);
    BOOL DeleteCity(long lID);
    BOOL DeletePhoneType(long lID);

    INT_PTR GetPersonCount() const;
    const PERSONS* GetPersonAt(INT_PTR nIndex) const;
    const PERSONS* GetPersonByID(long lID) const;

    INT_PTR GetPhoneCount() const;
    const PHONE_NUMBERS* GetPhoneAt(INT_PTR nIndex) const;
    const PHONE_NUMBERS* GetPhoneByID(long lID) const;

    INT_PTR GetCityCount() const;
    const CITIES* GetCityAt(INT_PTR nIndex) const;
    const CITIES* GetCityByID(long lID) const;

    INT_PTR GetPhoneTypeCount() const;
    const PHONE_TYPES* GetPhoneTypeAt(INT_PTR nIndex) const;
    const PHONE_TYPES* GetPhoneTypeByID(long lID) const;

protected:
    void ReflectChangeCity(ChangeEntry& entry);
    void ReflectChangePerson(ChangeEntry& entry);
    void ReflectChangePhone(ChangeEntry& entry);
protected:
    CPtrAutoArray<PERSONS>        m_arrPersons;
    CPtrAutoArray<PHONE_NUMBERS> m_arrPhoneNumbers;
    CPtrAutoArray<CITIES>        m_arrCities;
    CPtrAutoArray<PHONE_TYPES>   m_arrPhoneTypes;

    std::unordered_map<long, PERSONS*>        m_idxPersons;
    std::unordered_map<long, PHONE_NUMBERS*> m_idxPhones;
    std::unordered_map<long, CITIES*>         m_idxCities;
    std::unordered_map<long, PHONE_TYPES*>   m_idxPhoneTypes;
protected:
    CPhoneNumbersData      m_oPhoneNumbersData;    
    CPersonData        m_oPersonsData;
    CCitiesData        m_oCitiesData;
    CPhoneTypesData    m_oPhoneTypesData;
protected:
    void RebuildIndexes();

    DECLARE_MESSAGE_MAP()
};
