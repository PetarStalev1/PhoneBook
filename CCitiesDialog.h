#pragma once
#include "afxdialogex.h"
#include "CitiesDialogType.h"
#include "Structures.h"
#include <afxwin.h>

// CCitiesDialog dialog

class CCitiesDialog : public CDialogEx
{
public:
    CCitiesDialog(CITIES* pCity, const CitiesDialogType eCitiesDialogType, CWnd* pParent /*= nullptr*/);
    virtual ~CCitiesDialog();

    DECLARE_DYNAMIC(CCitiesDialog)
    const CitiesDialogType m_eCitiesDialogType;
    CITIES* m_pCity;


    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CITY_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    BOOL SetDialogName();
    DECLARE_MESSAGE_MAP()
public:
    CEdit m_edbCityName;
    CEdit m_edbCityRegion;
};
