#include "pch.h"
#include "PhoneBook.h"
#include "CPersonSelectDlg.h"
#include "CPhoneNumbersView.h"     
#include "CPhoneNumbersDocument.h" 
#include "CPersonDlg.h"
#include "CDialogChangeContext.h"

IMPLEMENT_DYNAMIC(CPersonSelectDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CPersonSelectDlg, CDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_PERSONS, &CPersonSelectDlg::OnNMDblClkPersons)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_PERSONS, &CPersonSelectDlg::OnNMRClickPersons)
    ON_COMMAND(ID_PERSON_ADD, &CPersonSelectDlg::OnContextAdd)
    ON_COMMAND(ID_PERSON_EDIT, &CPersonSelectDlg::OnContextEdit)
    ON_COMMAND(ID_PERSON_DELETE, &CPersonSelectDlg::OnContextDelete)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PERSONS, &CPersonSelectDlg::OnNMCustomdrawPersons)
END_MESSAGE_MAP()

CPersonSelectDlg::CPersonSelectDlg(CPhoneNumbersView* pView, 
    CDialogChangeContext* pCtx, 
    CPtrArray* pCities, 
    CMapUCN* pUCNs, 
    long lCurrentPersonID, 
    CWnd* pParent)
    : CDialogEx(IDD_PERSON_SELECT_DLG, pParent),
    m_pView(pView),
    m_pChangeCtx(pCtx),
    m_pCities(pCities),
    m_pUCNs(pUCNs),
    m_lInitialID(lCurrentPersonID)
{
    m_lSelectedPersonID = lCurrentPersonID;
}

CPersonSelectDlg::~CPersonSelectDlg() {}

void CPersonSelectDlg::FillList()
{
    m_lscPersons.DeleteAllItems();
    int nIndex = 0;

    if (m_pChangeCtx)
    {
        POSITION pos = m_pChangeCtx->m_mapChanges.GetStartPosition();
        long lKey;
        ChangeEntry entry;

        while (pos)
        {
            m_pChangeCtx->m_mapChanges.GetNextAssoc(pos, lKey, entry);

            if (entry.eEntity == EntityPerson && entry.eChange == ChangeInsert)
            {
                PERSONS* pPerson = (PERSONS*)entry.pRecord;
                InsertPersonItem(nIndex, pPerson);
                nIndex++;
            }
        }
    }

    if (m_pView)
    {
        auto* pDoc = m_pView->GetDocument();

        for (INT_PTR i = 0; i < pDoc->GetPersonCount(); i++)
        {
            const PERSONS* pDocPerson = pDoc->GetPersonAt(i);
            const PERSONS* pPersonToShow = pDocPerson; 

            ChangeEntry entry;
            if (m_pChangeCtx && m_pChangeCtx->m_mapChanges.Lookup(pDocPerson->lID, entry))
            {
                if (entry.eEntity == EntityPerson)
                {
                    if (entry.eChange == ChangeDelete)
                    {
                        continue; 
                    }
                    if (entry.eChange == ChangeUpdate)
                    {
                        pPersonToShow = (PERSONS*)entry.pRecord;
                    }
                }
            }

            InsertPersonItem(nIndex, pPersonToShow);
            nIndex++;
        }
    }
}

void CPersonSelectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PERSONS, m_lscPersons);
}


BOOL CPersonSelectDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    m_lscPersons.SetExtendedStyle(
        m_lscPersons.GetExtendedStyle() |
        LVS_EX_FULLROWSELECT |
        LVS_EX_GRIDLINES |
        LVS_EX_DOUBLEBUFFER
    );
    m_lscPersons.InsertColumn(0, _T("Име"), LVCFMT_LEFT, 100);
    m_lscPersons.InsertColumn(1, _T("Фамилия"), LVCFMT_LEFT, 100);
    m_lscPersons.InsertColumn(2, _T("ЕГН"), LVCFMT_LEFT, 100);
    m_lscPersons.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    FillList();


    if (m_lInitialID > 0)
    {
        int nCount = m_lscPersons.GetItemCount();
        for (int i = 0; i < nCount; i++)
        {
            PERSONS* p = (PERSONS*)m_lscPersons.GetItemData(i);
            if (p && p->lID == m_lInitialID)
            {
                m_lscPersons.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                m_lscPersons.EnsureVisible(i, FALSE);
                break;
            }
        }
    }

    return TRUE;
}

void CPersonSelectDlg::OnNMRClickPersons(NMHDR* pNMHDR, LRESULT* pResult)
{
    CMenu menu;
    if (menu.LoadMenu(IDR_MENU_PERSON_SELECT))
    {
        CMenu* pSubMenu = menu.GetSubMenu(0);
        if (pSubMenu)
        {
            CPoint point;
            GetCursorPos(&point);

            long lSelectedID = GetSelectedPersonID();

            if (lSelectedID == 0)
            {
                pSubMenu->EnableMenuItem(ID_PERSON_EDIT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                pSubMenu->EnableMenuItem(ID_PERSON_DELETE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            }

            pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
        }
    }
    *pResult = 0;
}

long CPersonSelectDlg::GetSelectedPersonID()
{
    POSITION pos = m_lscPersons.GetFirstSelectedItemPosition();
    if (pos)
    {
        int nIndex = m_lscPersons.GetNextSelectedItem(pos);
        PERSONS* p = (PERSONS*)m_lscPersons.GetItemData(nIndex);
        return p ? p->lID : 0;
    }
    return 0;
}
void CPersonSelectDlg::OnContextAdd()
{
    PERSONS newPerson;

    SecureZeroMemory(&newPerson, sizeof(PERSONS));
    newPerson.lID = m_pChangeCtx->GenerateTempID();
    CPersonDlg dlg(&newPerson, m_pCities, m_pUCNs, PersonDialogTypeAdd);

    if (dlg.DoModal() == IDOK)
    {
        PERSONS* pResult = dlg.GetPerson();
        if (pResult)
        {
            newPerson = *pResult;
            RegisterPersonChange(newPerson, ChangeInsert);
            FillList();
        }
        
    }
}

void CPersonSelectDlg::OnContextEdit()
{
    POSITION pos = m_lscPersons.GetFirstSelectedItemPosition();
    if (!pos) return;
    int nIndex = m_lscPersons.GetNextSelectedItem(pos);

    PERSONS* pCurrentPerson = (PERSONS*)m_lscPersons.GetItemData(nIndex);
    if (!pCurrentPerson) return;

    PERSONS editPerson = *pCurrentPerson;
    PERSONS oldPerson = editPerson; 

    CPersonDlg dlg(&editPerson, m_pCities, m_pUCNs, PersonDialogTypeEdit);

    if (dlg.DoModal() == IDOK)
    {
        PERSONS* pResult = dlg.GetPerson();

        if (pResult && *pResult != oldPerson)
        {
            editPerson = *pResult;

            RegisterPersonChange(editPerson, ChangeUpdate);

            FillList();
        }
    }
}

void CPersonSelectDlg::OnContextDelete()
{
    POSITION pos = m_lscPersons.GetFirstSelectedItemPosition();
    if (!pos) return;
    int nIndex = m_lscPersons.GetNextSelectedItem(pos);

    PERSONS* pPerson = (PERSONS*)m_lscPersons.GetItemData(nIndex);
    long lID = pPerson->lID;

    if (MessageBox(_T("Сигурни ли сте, че искате да изтриете този човек?"), _T("Потвърждение"), MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        ChangeEntry entry;

        if (m_pChangeCtx->m_mapChanges.Lookup(lID, entry))
        {
            if (entry.eChange == ChangeInsert)
            {
                delete (PERSONS*)entry.pRecord; 
                m_pChangeCtx->m_mapChanges.RemoveKey(lID); 
            }
            else
            {
                delete (PERSONS*)entry.pRecord;

                PERSONS* pDummy = new PERSONS;
                pDummy->lID = lID;

                entry.eChange = ChangeDelete;
                entry.pRecord = pDummy;
                m_pChangeCtx->m_mapChanges.SetAt(lID, entry);
            }
        }
        else
        {

            PERSONS* pDummy = new PERSONS;
            pDummy->lID = lID;

            ChangeEntry newEntry;
            newEntry.eEntity = EntityPerson;
            newEntry.eChange = ChangeDelete;
            newEntry.pRecord = pDummy;

            m_pChangeCtx->m_mapChanges.SetAt(lID, newEntry);
        }

        FillList();
    }
}
void CPersonSelectDlg::OnOK()
{
    POSITION pos = m_lscPersons.GetFirstSelectedItemPosition();
    if (pos)
    {
        int nIndex = m_lscPersons.GetNextSelectedItem(pos);
        PERSONS* pPerson = (PERSONS*)m_lscPersons.GetItemData(nIndex);

        if (pPerson != nullptr)
        {
            m_lSelectedPersonID = pPerson->lID;
        }
        else
        {
            m_lSelectedPersonID = 0;
        }
        CDialogEx::OnOK();
    }
    else
    {
        m_lSelectedPersonID = m_lInitialID;
        //MessageBox(_T("Моля изберете човек!"), _T("Инфо"), MB_OK);
    }
}



void CPersonSelectDlg::OnNMDblClkPersons(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnOK();
    *pResult = 0;
}

void CPersonSelectDlg::RefreshPersonList()
{
}

void CPersonSelectDlg::RegisterPersonChange(const PERSONS& personData, ChangeType changeType)
{
    if (!m_pChangeCtx) return;

    ChangeEntry entry;

    if (m_pChangeCtx->m_mapChanges.Lookup(personData.lID, entry))
    {
        if (entry.eEntity == EntityPerson)
        {
            PERSONS* pBufferedPerson = (PERSONS*)entry.pRecord;
            *pBufferedPerson = personData; 
        } 
    }
    else
    {
        PERSONS* pNewRecord = new PERSONS;
        *pNewRecord = personData; 

        ChangeEntry newEntry;
        newEntry.eEntity = EntityPerson;
        newEntry.eChange = changeType; 
        newEntry.pRecord = pNewRecord;

        m_pChangeCtx->m_mapChanges.SetAt(personData.lID, newEntry);
    }
}
void CPersonSelectDlg::InsertPersonItem(int nIndex, const PERSONS* pPerson)
{
    int iItem = m_lscPersons.InsertItem(nIndex, pPerson->szFirstName);
    m_lscPersons.SetItemText(iItem, 1, pPerson->szLastName);
    m_lscPersons.SetItemText(iItem, 2, pPerson->szUCN);
    m_lscPersons.SetItemData(iItem, (DWORD_PTR)pPerson);
}

void CPersonSelectDlg::OnNMCustomdrawPersons(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
    {
        if (pLVCD->nmcd.uItemState & CDIS_SELECTED)
        {
            *pResult = CDRF_DODEFAULT;
            return;
        }

        int nRow = (int)pLVCD->nmcd.dwItemSpec;
        PERSONS* pPerson = (PERSONS*)m_lscPersons.GetItemData(nRow);

        if (!pPerson) break;

        COLORREF clrBack = CLR_DEFAULT;

        if (pPerson->lID == m_lInitialID)
        {
            clrBack = RGB(200, 230, 255);
        }
        else
        {
            if (m_pChangeCtx)
            {
                ChangeEntry entry;
                if (m_pChangeCtx->m_mapChanges.Lookup(pPerson->lID, entry) &&
                    entry.eEntity == EntityPerson)
                {
                    if (entry.eChange == ChangeInsert)
                        clrBack = RGB(200, 255, 200);
                    else if (entry.eChange == ChangeUpdate)
                        clrBack = RGB(255, 230, 180);
                }
            }
        }

        if (clrBack != CLR_DEFAULT)
        {
            pLVCD->clrTextBk = clrBack;
            *pResult = CDRF_NEWFONT;
            return;
        }

        *pResult = CDRF_DODEFAULT;
        break;
    }
    }
}
