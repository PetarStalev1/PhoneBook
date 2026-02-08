#pragma once
#include <afxdialogex.h>
#include "Structures.h"
#include "resource.h"
#include "PhoneNumberDialogType.h"
#include "CitiesListViewActions.h"
#include "CPhoneNumbersView.h"
#include "CDialogChangeContext.h"

/// <summary> Дефиниция на мап за бързо търсене на ЕГН </summary>
typedef CMap<CString, LPCTSTR, long, long> CMapUCN;

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumberDlg

/// <summary> 
/// Диалогов прозорец за управление на телефонен номер.
/// Позволява добавяне, редакция и преглед на запис, както и асоцииране с човек и град.
/// </summary>
class CPhoneNumberDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPhoneNumberDlg)

	// Constants
	// ----------------
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_NUMBERS_ONEDIT };
#endif

	// Constructor / Destructor
	// ----------------
public:
	/// <summary> Конструктор </summary>
	/// <param name="pPhoneNumber"> Указател към записа за телефон (NULL при добавяне) </param>
	/// <param name="eType"> Режим на работа (Add/Edit/View) </param>
	/// <param name="pCtx"> Контекст за следене на промените </param>
	/// <param name="pCities"> Указател към масива с градове </param>
	/// <param name="pPersons"> Указател към масива с хора </param>
	/// <param name="pPhoneTypes"> Указател към масива с типове телефони </param>
	/// <param name="pUCNs"> Карта с ЕГН-та за валидация </param>
	/// <param name="pParent"> Родителски прозорец </param>
	CPhoneNumberDlg(PHONE_NUMBERS* pPhoneNumber,
		const PhoneNumberDialogType eType,
		CDialogChangeContext* pCtx,
		CPtrAutoArray<CITIES>* pCities,
		CPtrAutoArray<PERSONS>* pPersons,
		CPtrAutoArray<PHONE_TYPES>* pPhoneTypes,
		CMapUCN* pUCNs,
		CWnd* pParent = nullptr);

	/// <summary> Деструктор </summary>
	virtual ~CPhoneNumberDlg();


	// Public Accessors
	// ----------------
public:
	/// <summary> Връща ID на текущо избрания град на човека </summary>
	long GetActiveCityID() const;

	/// <summary> Връща указател към контекста за промени </summary>
	CDialogChangeContext* GetChangeContext() const { return m_pChangeCtx; }

	/// <summary> Връща референция към телефонния запис </summary>
	PHONE_NUMBERS& GetPhoneNumber() { return m_recPhoneNumber; }

	/// <summary> Връща const указател към телефонния запис </summary>
	const PHONE_NUMBERS* GetPhoneNumberPtr() const { return &m_recPhoneNumber; }

	/// <summary> Връща референция към данните за асоциирания човек </summary>
	PERSONS& GetPerson() { return m_recPerson; }


	// Overrides
	// ----------------
protected:
	/// <summary> Инициализация на диалога (зареждане на контроли, масиви) </summary>
	virtual BOOL OnInitDialog();

	/// <summary> Обмен на данни между променливи и UI (DDX/DDV) </summary>
	virtual void DoDataExchange(CDataExchange* pDX);

	/// <summary> Обработка на бутон OK (Валидация и запис в контекста) </summary>
	virtual void OnOK();

	/// <summary> Обработка на системни съобщения (напр. клик върху ReadOnly полета) </summary>
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	// Internal Logic & UI Helpers
	// ----------------
private:
	/// <summary> Задава заглавието на прозореца според режима (Add/Edit) </summary>
	BOOL SetDialogName();

	/// <summary> Попълва UI контролите с данните от структурите </summary>
	BOOL SetDialogFields();

	/// <summary> Прави контролите Read-Only (за режим View) </summary>
	void SetReadOnly();

	/// <summary> Валидира въведените от потребителя данни </summary>
	BOOL ValidateUserInput() const;

	/// <summary> Визуално превръща Edit control в Label (без рамка, сив фон) </summary>
	void MakeEditLookLikeLabel(CEdit& ed);

	/// <summary> Зарежда типове телефони в ComboBox-а </summary>
	void SetPhoneTypesComboBox();


	// ListCtrl Logic (Cities)
	// ----------------
public:
	/// <summary> Callback функция за сортиране на градовете </summary>
	static int CALLBACK CompareCities_MainDlg(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	/// <summary> Пренарежда списъка с градове </summary>
	void ResortCities();

private:
	/// <summary> Настройва колоните на списъка с градове </summary>
	void SetColumns();

	/// <summary> Запълва списъка с първоначалните данни </summary>
	void SetInitialData();

	/// <summary> Задава текст на ред в списъка </summary>
	BOOL SetItemText(const CITIES& recCities, const int nIndexList);

	/// <summary> Селектира град в списъка по неговото ID </summary>
	void SelectCityInListByID(long lID);


	// Data Helpers (Merging)
	// ----------------
private:
	/// <summary> Обединява ЕГН-тата от масива и контекста (за проверка за дубликати) </summary>
	void GetMergedUCNs(CMapUCN& mapUCNs);

	/// <summary> Обединява градовете от масива и контекста (за диалога за избор на човек) </summary>
	void GetMergedCities(CPtrArray& arrCities);


	// Event Handlers
	// ----------------
protected:
	/// <summary> Отваря диалога за избор на човек (CPersonSelectDlg) </summary>
	afx_msg void OnBnClickedViewPersons();

	/// <summary> Custom Draw за оцветяване на редовете в списъка с градове </summary>
	afx_msg void OnNMCustomdrawCities(NMHDR* pNMHDR, LRESULT* pResult);

	/// <summary> Десен клик върху списъка с градове (Контекстно меню) </summary>
	afx_msg void OnNMRClickCities(NMHDR* pNMHDR, LRESULT* pResult);

	// Context Menu Commands (Cities)
	/// <summary> Добавяне на нов град (през контекстно меню) </summary>
	afx_msg void OnContextAddCity();
	/// <summary> Редакция на избран град (през контекстно меню) </summary>
	afx_msg void OnContextEditCity();
	/// <summary> Изтриване на избран град (през контекстно меню) </summary>
	afx_msg void OnContextDeleteCity();

	DECLARE_MESSAGE_MAP()


	// Members
	// ----------------
public:
	/// <summary> Структура, пазеща данните за телефонния номер </summary>
	PHONE_NUMBERS m_recPhoneNumber;

	/// <summary> Структура, пазеща данните за асоциирания човек </summary>
	PERSONS m_recPerson;

private:
	/// <summary> Режим на диалога (Add/Edit/View) </summary>
	const PhoneNumberDialogType m_ePhoneNumberDialogType;

	/// <summary> Указател към контекста за промени </summary>
	CDialogChangeContext* m_pChangeCtx;

	// Указатели към номенклатурите (инжектирани отвън)
	CPtrAutoArray<CITIES>* m_pAllCities;
	CPtrAutoArray<PERSONS>* m_pAllPersons;
	CPtrAutoArray<PHONE_TYPES>* m_pAllPhoneTypes;
	CMapUCN* m_pMapUCNs;


	// Controls
	// ----------------
public:
	CEdit m_edbPhoneNumber;
	CEdit m_edbFirstName;
	CEdit m_edbMiddleName;
	CEdit m_edbLastName;
	CEdit m_edbUCN;
	CListCtrl m_lscCity;
	CEdit m_edbAddress;
	CComboBox m_cmbPhoneNumbersTypes;
	CButton m_btnViewPersons;
};