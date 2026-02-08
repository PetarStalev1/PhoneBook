#pragma once
#include "afxdialogex.h"
#include "CitiesDialogType.h"
#include "Structures.h"
#include <afxwin.h>

/////////////////////////////////////////////////////////////////////////////
// CCitiesDialog

/// <summary>
/// Диалогов прозорец за управление на номенклатура "Градове" (CITIES).
/// Използва се за добавяне на нови градове или редактиране на съществуващи.
/// </summary>
class CCitiesDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CCitiesDialog)

    // Constructor / Destructor
    // ----------------
public:

    /// <summary> Конструктор </summary>
    /// <param name="pCity"> Указател към структурата с данни за града (вход/изход) </param>
    /// <param name="eCitiesDialogType"> Режим на работа (Add или Edit) </param>
    /// <param name="pParent"> Родителски прозорец </param>
    CCitiesDialog(CITIES* pCity, const CitiesDialogType eCitiesDialogType, CWnd* pParent /*= nullptr*/);

    /// <summary> Деструктор </summary>
    virtual ~CCitiesDialog();

    // Members
     // ----------------
public:
    /// <summary> Режим на диалога (Add/Edit), зададен в конструктора </summary>
    const CitiesDialogType m_eCitiesDialogType;

    /// <summary> Указател към данните, които се визуализират или попълват </summary>
    CITIES* m_pCity;


    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CITY_DIALOG };
#endif
    // Overrides
    // ----------------
protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    /// <summary> Задава заглавието на прозореца според m_eCitiesDialogType </summary>
    BOOL SetDialogName();
    DECLARE_MESSAGE_MAP()
public:
    CEdit m_edbCityName;
    CEdit m_edbCityRegion;
};
