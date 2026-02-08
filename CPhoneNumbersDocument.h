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

/// <summary>
/// Типове операции за обновяване на View-тата.
/// Използват се като "Hint" в UpdateAllViews.
/// </summary>
enum DocumentDataOperation
{
    DocumentDataOperationLoad = 0,
    DocumentDataOperationInsert,
    DocumentDataOperationUpdate,
    DocumentDataOperationDelete,
    DocumentDataOperationUpdatePerson
};

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersDocument

/// <summary>
/// Основен клас Документ (Data Model).
/// Съхранява всички списъци с данни (Хора, Градове, Телефони) в паметта,
/// управлява връзката с базата данни и поддържа индекси за бързо търсене.
/// </summary>
class CPhoneNumbersDocument : public CDocument
{
    DECLARE_DYNCREATE(CPhoneNumbersDocument)

    // Constructor / Destructor
    // ----------------
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
    // Operations (Business Logic)
    // ----------------

    /// <summary> Зарежда всички данни от базата данни в масивите и построява индексите </summary>
    BOOL LoadAll();
    BOOL SelectByNameUCNAddress(CPtrAutoArray<PHONE_NUMBERS>& oAutoArray,
        const CString& strName,
        const CString& strUCN,
        const CString& strAddress);
    // --- CRUD Operations: Phone Numbers ---
    BOOL InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber);
    BOOL UpdatePhoneNumber(PHONE_NUMBERS& reCPhoneNumber);
    BOOL DeletePhoneNumber(long lID);

    // --- CRUD Operations: Persons ---
    BOOL AddOrUpdatePerson(PERSONS& rec);
    BOOL DeletePerson(long lID);

    // --- CRUD Operations: Generic / Helpers ---
    BOOL AddOrUpdatePhone(PHONE_NUMBERS& rec); // Wrapper
    BOOL DeletePhone(long lID);                // Wrapper

    // --- CRUD Operations: Nomenclatures ---
    BOOL AddOrUpdateCity(CITIES& rec);
    BOOL DeleteCity(long lID);

    BOOL AddOrUpdatePhoneType(PHONE_TYPES& rec);
    BOOL DeletePhoneType(long lID);

    /// <summary> 
    /// Прилага пакетни промени, направени в диалог (чрез Context), към базата и кеша.
    /// </summary>
    /// <param name="ctx"> Контекстът с натрупаните промени </param>
    bool ApplyChanges(CDialogChangeContext& ctx);


    // --- Persons Accessors ---
    INT_PTR GetPersonCount() const;
    const PERSONS* GetPersonAt(INT_PTR nIndex) const;
    const PERSONS* GetPersonByID(long lID) const;

    // --- Phones Accessors ---
    INT_PTR GetPhoneCount() const;
    const PHONE_NUMBERS* GetPhoneAt(INT_PTR nIndex) const;
    const PHONE_NUMBERS* GetPhoneByID(long lID) const;

    // --- Cities Accessors ---
    INT_PTR GetCityCount() const;
    const CITIES* GetCityAt(INT_PTR nIndex) const;
    const CITIES* GetCityByID(long lID) const;

    // --- Phone Types Accessors ---
    INT_PTR GetPhoneTypeCount() const;
    const PHONE_TYPES* GetPhoneTypeAt(INT_PTR nIndex) const;
    const PHONE_TYPES* GetPhoneTypeByID(long lID) const;
public:
    // --- Direct Container Access (Dependency Injection) ---
     // Използват се за подаване на масивите към диалозите
    CPtrAutoArray<PERSONS>* GetAllPersons() { return &m_arrPersons; }
    CPtrAutoArray<CITIES>* GetAllCities() { return &m_arrCities; }
    CPtrAutoArray<PHONE_TYPES>* GetAllPhoneTypes() { return &m_arrPhoneTypes; }
    CMapUCN* GetUCNMap() { return &m_mapUCNs; }

protected:
    /// <summary> Отразява промяна в Град от Context-а към локалния масив </summary>
    void ReflectChangeCity(ChangeEntry& entry);
    /// <summary> Отразява промяна в Човек от Context-а към локалния масив </summary>
    void ReflectChangePerson(ChangeEntry& entry);
    /// <summary> Отразява промяна в Телефон от Context-а към локалния масив </summary>
    void ReflectChangePhone(ChangeEntry& entry);

    /// <summary> Преизчислява hash-map индексите след зареждане на данни </summary>
    void RebuildIndexes();

    // Members (Data Storage)
    // ----------------
protected:

    // Основни масиви с данни (Master Data)
    CPtrAutoArray<PERSONS>        m_arrPersons;
    CPtrAutoArray<PHONE_NUMBERS> m_arrPhoneNumbers;
    CPtrAutoArray<CITIES>        m_arrCities;
    CPtrAutoArray<PHONE_TYPES>   m_arrPhoneTypes;

    /// <summary> Карта за проверка на уникалност на ЕГН (UCN -> PersonID) </summary>
    CMapUCN m_mapUCNs;

    // Индекси за бързо търсене по ID (O(1) достъп)
    std::unordered_map<long, PERSONS*>        m_idxPersons;
    std::unordered_map<long, PHONE_NUMBERS*> m_idxPhones;
    std::unordered_map<long, CITIES*>         m_idxCities;
    std::unordered_map<long, PHONE_TYPES*>   m_idxPhoneTypes;

    // Data Access Objects (Table Accessors)
protected:
    CPhoneNumbersData      m_oPhoneNumbersData;    
    CPersonData        m_oPersonsData;
    CCitiesData        m_oCitiesData;
    CPhoneTypesData    m_oPhoneTypesData;
protected:

    DECLARE_MESSAGE_MAP()
};
