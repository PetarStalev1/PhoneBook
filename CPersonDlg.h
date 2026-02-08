#pragma once
#include "afxdialogex.h"
#include "Structures.h"
#include "PhoneNumberDialogType.h"
#include "CPhoneNumbersView.h"
#include "CPersonDialogType.h"
#include "InputValidator.h"
#include "CPtrAutoArray.h"


/// <summary> Дефиниция на тип за карта с ЕГН-та (Key: ЕГН, Value: ID) </summary>
typedef CMap<CString, LPCTSTR, long, long> CMapUCN;

/////////////////////////////////////////////////////////////////////////////
// CPersonDlg

/// <summary> 
/// Диалогов прозорец за управление на данни за физически лица (PERSONS).
/// Поддържа операции за добавяне, редактиране и преглед.
/// </summary>
class CPersonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonDlg)
	// Constants
	// ----------------
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PERSON };
#endif

	// Constructor / Destructor
	// ----------------
public:
	/// <summary> Конструктор </summary>
	/// <param name="pPerson"> Указател към структурата с данни на човека (NULL за нов запис) </param>
	/// <param name="pCities"> Указател към масива с градове за попълване на ComboBox-а </param>
	/// <param name="pUsedUCNs"> Указател към карта със заети ЕГН-та за валидация на уникалност </param>
	/// <param name="pCtx"> Указател към контекста за промени (Insert/Update/Delete) </param>
	/// <param name="eType"> Тип на диалога (Add/Edit/View) </param>
	/// <param name="pParent"> Родителски прозорец </param>
	CPersonDlg(PERSONS* pPerson, 
		CPtrAutoArray<CITIES>* pCities, 
		const CMapUCN* pUsedUCNs, 
		CDialogChangeContext* pCtx,
		PersonDialogType eType, 
		CWnd* pParent = nullptr);

	/// <summary> Деструктор </summary>
	virtual ~CPersonDlg();

	/// <summary> Достъп до редактирания или новосъздаден запис </summary>
	/// <returns> Указател към член-променливата m_recPerson </returns>
	PERSONS* GetPerson() { return &m_recPerson; }



	// Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	BOOL PreTranslateMessage(MSG* pMsg);

	/// <summary> Помощен клас за валидация на входните данни </summary>
	CInputValidator m_validator;

	/// <summary> Локално копие на данните за човека </summary>
	PERSONS m_recPerson;       

	/// <summary> Тип на отворения диалог (Add, Edit, View) </summary>
	PersonDialogType m_eType;

	/// <summary> Контекст, съхраняващ временните промени </summary>
	CDialogChangeContext* m_pChangeCtx;

	/// <summary> Указател към масива с всички градове (Reference Data) </summary>
	const CPtrAutoArray<CITIES>* m_pCitiesArray;

	/// <summary> Указател към картата с използвани ЕГН-та </summary>
	const CMapUCN* m_pUsedUCNs;
	DECLARE_MESSAGE_MAP()

public:
	//Полетата на диалога:
	  CEdit m_edFirstName;
	  CEdit m_edMiddleName;
	  CEdit m_edLastName;
	  CEdit m_edUCN;
	  CEdit m_edAddress;
	  CComboBox m_cmbCity;
};
