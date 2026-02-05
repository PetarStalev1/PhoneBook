// CCitiesDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "afxdialogex.h"
#include "CCitiesDialog.h"
#include "CitiesDialogType.h"
#include "Structures.h"
#include <afxwin.h>

#define DIALOG_ADD_NAME _T("Добавяне")
#define DIALOG_EDIT_NAME _T("Редактиране")
// CCitiesDialog dialog

IMPLEMENT_DYNAMIC(CCitiesDialog, CDialogEx)

CCitiesDialog::CCitiesDialog(CITIES* pCity, const CitiesDialogType eCitiesDialogType, CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CITY_DIALOG, pParent),
    m_eCitiesDialogType(eCitiesDialogType), // Премахната излишна запетая тук
    m_pCity(pCity)
{
}

CCitiesDialog::~CCitiesDialog()
{
}
BOOL CCitiesDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    SetDialogName();

    if (m_eCitiesDialogType == CitiesDialogTypeEdit && m_pCity != NULL)
    {
        m_edbCityName.SetWindowText(m_pCity->szCityName);
        m_edbCityRegion.SetWindowText(m_pCity->szDistrict);
    }
    return TRUE;
}
void CCitiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_CITY_NAME, m_edbCityName);
	DDX_Control(pDX, IDC_EDB_CITY_REGION, m_edbCityRegion);
}

void CCitiesDialog::OnOK()
{
    if (m_pCity != NULL)
    {
        m_edbCityName.GetWindowText(m_pCity->szCityName, 64);
        m_edbCityRegion.GetWindowText(m_pCity->szDistrict, 64);
    }

    CDialogEx::OnOK();
}
BOOL CCitiesDialog::SetDialogName()
{
    switch (m_eCitiesDialogType)
    {
    case CitiesDialogTypeAdd:
        SetWindowText(DIALOG_ADD_NAME);
        break;
    case CitiesDialogTypeEdit:
        SetWindowText(DIALOG_EDIT_NAME);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BEGIN_MESSAGE_MAP(CCitiesDialog, CDialogEx)
END_MESSAGE_MAP()
