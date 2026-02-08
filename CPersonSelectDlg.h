#pragma once
#include <afxdialogex.h>
#include "CPhoneNumbersView.h"
#include "CDialogChangeContext.h"
#include "CPtrAutoArray.h" 
#include "Structures.h"

/// <summary> Дефиниция на мап за бързо търсене на ЕГН </summary>
typedef CMap<CString, LPCTSTR, long, long> CMapUCN;

/////////////////////////////////////////////////////////////////////////////
// CPersonSelectDlg

/// <summary> 
/// Диалогов прозорец за избор на физическо лице от списък.
/// Класът поддържа и CRUD операции (Добавяне, Редакция, Изтриване) 
/// във временен буфер, които се прилагат само при потвърждение (OK).
/// </summary>
class CPersonSelectDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CPersonSelectDlg)

    // Constants
    // ----------------
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PERSON_SELECT_DLG };
#endif

    // Constructor / Destructor
    // ----------------
public:
    /// <summary> Конструктор </summary>
    /// <param name="pAllPersons"> Указател към масива с всички хора (Read-only източник) </param>
    /// <param name="pAllCities"> Указател към масива с градове (нужен за диалога за редакция) </param>
    /// <param name="pCtx"> Родителският контекст за промени (за четене и финално записване) </param>
    /// <param name="pUCNs"> Карта с ЕГН-та за валидация </param>
    /// <param name="lCurrentPersonID"> ID на текущо избрания човек (за предварителна селекция) </param>
    /// <param name="pParent"> Родителски прозорец </param>
    CPersonSelectDlg(
        CPtrAutoArray<PERSONS>* pAllPersons,
        CPtrAutoArray<CITIES>* pAllCities,
        CDialogChangeContext* pCtx,  
        CMapUCN* pUCNs,
        long lCurrentPersonID,
        CWnd* pParent = nullptr);

    /// <summary> Деструктор </summary>
    virtual ~CPersonSelectDlg();

    /// <summary> Запълва списъка (ListCtrl) с данни </summary>
    /// <remarks> Обединява данните от масива, родителския контекст и временния контекст </remarks>
    void FillList();

    // Members
    // ----------------

    /// <summary> ID на избрания човек (Резултат от диалога) </summary>
    long m_lSelectedPersonID = 0;

private:
    /// <summary> Указател към масива с хора </summary>
    CPtrAutoArray<PERSONS>* m_pAllPersons;
    /// <summary> Указател към масива с градове </summary>
    CPtrAutoArray<CITIES>* m_pAllCities;
    /// <summary> Указател към картата с ЕГН-та </summary>
    CMapUCN* m_pUCNs;

    /// <summary> Указател към контекста на родителя (CPhoneNumberDlg/View) </summary>
    CDialogChangeContext* m_pParentCtx;

    /// <summary> Временен локален контекст за промените в рамките на този диалог </summary>
    CDialogChangeContext m_tempCtx;

    /// <summary> Първоначално подаденото ID (за възстановяване при Cancel) </summary>
    long m_lInitialID;


    // Overrides
    // ----------------
protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

    /// <summary> Вмъква ред в списъка за даден човек </summary>
    /// <param name="nIndex"> Индекс на реда </param>
    /// <param name="pPerson"> Данни за човека </param>
    void InsertPersonItem(int nIndex, const PERSONS* pPerson);

    /// <summary> Връща ID-то на текущо селектирания ред в списъка </summary>
    long GetSelectedPersonID();

    /// <summary> Регистрира промяна (Insert/Update) във временния контекст </summary>
    /// <param name="personData"> Данните на човека </param>
    /// <param name="changeType"> Тип на промяната </param>
    void RegisterPersonChange(const PERSONS& personData, ChangeType changeType);


    // Handlers
    afx_msg void OnNMDblClkPersons(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMCustomdrawPersons(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMRClickPersons(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnContextAdd();
    afx_msg void OnContextEdit();
    afx_msg void OnContextDelete();

    DECLARE_MESSAGE_MAP()
protected:
    /// <summary> Списък с физически лица </summary>
    CListCtrl m_lscPersons;
};

