// CPersonDlg.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "afxdialogex.h"
#include "CPersonDlg.h"
#include "CDialogChangeContext.h"


IMPLEMENT_DYNAMIC(CPersonDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CPersonDlg, CDialogEx)
END_MESSAGE_MAP()

// Constants
// ----------------

// Constructor / Destructor
// ----------------



CPersonDlg::CPersonDlg(PERSONS* pPerson,
    CPtrAutoArray<CITIES>* pCities,
    const CMapUCN* pUsedUCNs,
    CDialogChangeContext* pCtx,
    PersonDialogType eType,
    CWnd* pParent)
    : CDialogEx(IDD_DIALOG_PERSON, pParent),
    m_pCitiesArray(pCities),
    m_pUsedUCNs(pUsedUCNs),
    m_pChangeCtx(pCtx),
    m_eType(eType)
{
    if (pPerson)
    {
        m_recPerson = *pPerson;
    }
    else
    {
        m_recPerson.lID = 0;
        m_recPerson.szFirstName[0] = '\0';
        m_recPerson.szMiddleName[0] = '\0';
        m_recPerson.szLastName[0] = '\0';
        m_recPerson.szAddress[0] = '\0';
        m_recPerson.szUCN[0] = '\0';
        m_recPerson.lCityID = 0;
    }
}

CPersonDlg::~CPersonDlg()
{
}

// Overrides
// ----------------
void CPersonDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDB_PERSONS_DIALOG_FIRSTNAME, m_edFirstName);
    DDX_Control(pDX, IDC_EDB_PERSONS_DIALOG_MIDDLE_NAME, m_edMiddleName);
    DDX_Control(pDX, IDC_EDB_PERSONS_DIALOG_LAST_NAME, m_edLastName);
    DDX_Control(pDX, IDC_EDB_PERSONS_DIALOG_UCN, m_edUCN);
    DDX_Control(pDX, IDC_EDB_PERSONS_DIALOG_ADDRESS, m_edAddress);
    DDX_Control(pDX, IDC_CMB_CITIES, m_cmbCity);
}


BOOL CPersonDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_cmbCity.ResetContent();

    if (m_pChangeCtx)
    {
        POSITION pos = m_pChangeCtx->m_mapChanges.GetStartPosition();
        long lKey;
        ChangeEntry entry;

        while (pos)
        {
            m_pChangeCtx->m_mapChanges.GetNextAssoc(pos, lKey, entry);

            if (entry.eEntity == EntityCity && entry.eChange == ChangeInsert)
            {
                CITIES* pCity = (CITIES*)entry.pRecord;
                int nIndex = m_cmbCity.AddString(pCity->szCityName);
                m_cmbCity.SetItemData(nIndex, (DWORD_PTR)pCity->lID);
            }
        }
    }

    if (m_pCitiesArray)
    {
        for (INT_PTR i = 0; i < m_pCitiesArray->GetCount(); i++)
        {
            const CITIES* pArrayCity = m_pCitiesArray->GetAt(i);
            const CITIES* pCityToShow = pArrayCity; 

            if (m_pChangeCtx)
            {
                ChangeEntry entry;
                if (m_pChangeCtx->m_mapChanges.Lookup(pArrayCity->lID, entry))
                {
                    if (entry.eEntity == EntityCity)
                    {
                        if (entry.eChange == ChangeDelete)
                        {
                            continue;
                        }
                        if (entry.eChange == ChangeUpdate)
                        {
                            pCityToShow = (CITIES*)entry.pRecord;
                        }
                    }
                }
            }

            int nIndex = m_cmbCity.AddString(pCityToShow->szCityName);
            m_cmbCity.SetItemData(nIndex, (DWORD_PTR)pCityToShow->lID);
        }
    }

    bool bSelected = false;
    int nCmbCount = m_cmbCity.GetCount();

    for (int i = 0; i < nCmbCount; i++)
    {
        if ((long)m_cmbCity.GetItemData(i) == m_recPerson.lCityID)
        {
            m_cmbCity.SetCurSel(i);
            bSelected = true;
            break;
        }
    }

    if (!bSelected && nCmbCount > 0)
    {
        m_cmbCity.SetCurSel(0);
        if (m_eType == PersonDialogTypeAdd)
        {
            m_recPerson.lCityID = (long)m_cmbCity.GetItemData(0);
        }
    }

    if (m_eType == PersonDialogTypeEdit)
    {
        m_edFirstName.SetWindowText(m_recPerson.szFirstName);
        m_edMiddleName.SetWindowText(m_recPerson.szMiddleName);
        m_edLastName.SetWindowText(m_recPerson.szLastName);
        m_edUCN.SetWindowText(m_recPerson.szUCN);
        m_edAddress.SetWindowText(m_recPerson.szAddress);
        SetWindowText(_T("Редакция на човек"));
    }
    else
    {
        m_edFirstName.SetWindowText(_T(""));
        m_edMiddleName.SetWindowText(_T(""));
        m_edLastName.SetWindowText(_T(""));
        m_edUCN.SetWindowText(_T(""));
        m_edAddress.SetWindowText(_T(""));
        SetWindowText(_T("Нов човек"));
        m_edFirstName.SetFocus();
    }

    return TRUE;
}
void CPersonDlg::OnOK()
{
    if (!m_validator.ValidateTextField(m_edFirstName, 30, 2))
    {
        MessageBox(_T("Моля въведете валидно име!"),
            _T("Грешка"), MB_OK | MB_ICONERROR);
        m_edFirstName.SetFocus();
        return;
    }

    if (!m_validator.ValidateTextField(m_edLastName, 30, 2))
    {
        MessageBox(_T("Моля въведете валидна фамилия!"),
            _T("Грешка"), MB_OK | MB_ICONERROR);
        m_edLastName.SetFocus();
        return;
    }

    if (!m_validator.ValidateNumber(m_edUCN, 10, 10))
    {
        MessageBox(_T("ЕГН трябва да съдържа точно 10 цифри!"),
            _T("Грешка"), MB_OK | MB_ICONERROR);
        m_edUCN.SetFocus();
        m_edUCN.SetSel(0, -1);
        return;
    }

    if (!m_validator.ValidateComboBox(m_cmbCity))
    {
        MessageBox(_T("Моля изберете град!"),
            _T("Грешка"), MB_OK | MB_ICONERROR);
        return;
    }

    CString strCurrentUCN;
    m_edUCN.GetWindowText(strCurrentUCN);

    if (m_pUsedUCNs != nullptr)
    {
        long lFoundID;
        if (m_pUsedUCNs->Lookup(strCurrentUCN, lFoundID))
        {
            if (lFoundID != m_recPerson.lID)
            {
                MessageBox(_T("Това ЕГН вече съществува в системата!"), _T("Грешка"), MB_OK | MB_ICONSTOP);
                m_edUCN.SetFocus();
                m_edUCN.SetSel(0, -1);
                return;
            }
        }
    }

    m_edFirstName.GetWindowText(m_recPerson.szFirstName, PERSONS_NAME_LENGTH);
    m_edMiddleName.GetWindowText(m_recPerson.szMiddleName, PERSONS_NAME_LENGTH);
    m_edLastName.GetWindowText(m_recPerson.szLastName, PERSONS_NAME_LENGTH);

    _tcscpy_s(m_recPerson.szUCN, strCurrentUCN);

    m_edAddress.GetWindowText(m_recPerson.szAddress, PERSONS_ADDRESS_LENGTH);

    int nSel = m_cmbCity.GetCurSel();
    if (nSel != CB_ERR)
    {
        m_recPerson.lCityID = (long)m_cmbCity.GetItemData(nSel);
    }
    else
    {
        MessageBox(_T("Моля изберете град!"), _T("Грешка"), MB_OK | MB_ICONERROR);
        return;
    }

    CDialogEx::OnOK();
}
BOOL CPersonDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}

// Methods
// ----------------