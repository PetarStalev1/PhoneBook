#include "pch.h"
#include "PhoneBook.h"
#include "CPersonSelectDlg.h"
#include "CPhoneNumbersView.h"     
#include "CPhoneNumbersDocument.h" 
#include "CPersonDlg.h"
#include "CDialogChangeContext.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonSelectDlg

IMPLEMENT_DYNAMIC(CPersonSelectDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CPersonSelectDlg, CDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_PERSONS, &CPersonSelectDlg::OnNMDblClkPersons)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_PERSONS, &CPersonSelectDlg::OnNMRClickPersons)
    ON_COMMAND(ID_PERSON_ADD, &CPersonSelectDlg::OnContextAdd)
    ON_COMMAND(ID_PERSON_EDIT, &CPersonSelectDlg::OnContextEdit)
    ON_COMMAND(ID_PERSON_DELETE, &CPersonSelectDlg::OnContextDelete)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PERSONS, &CPersonSelectDlg::OnNMCustomdrawPersons)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
CPersonSelectDlg::CPersonSelectDlg(
    CPtrAutoArray<PERSONS>* pAllPersons,
    CPtrAutoArray<CITIES>* pAllCities,
    CDialogChangeContext* pCtx,
    CMapUCN* pUCNs,
    long lCurrentPersonID,
    CWnd* pParent)
    : CDialogEx(IDD_PERSON_SELECT_DLG, pParent),
    m_pAllPersons(pAllPersons),
    m_pAllCities(pAllCities),
    m_pParentCtx(pCtx),
    m_pUCNs(pUCNs),
    m_lInitialID(lCurrentPersonID)
{
    m_lSelectedPersonID = lCurrentPersonID;
}

CPersonSelectDlg::~CPersonSelectDlg() {}

// Overrides
// ----------------

BOOL CPersonSelectDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_pParentCtx)
    {
        POSITION pos = m_pParentCtx->m_mapChanges.GetStartPosition();
        long lKey;
        ChangeEntry entry;
        while (pos)
        {
            m_pParentCtx->m_mapChanges.GetNextAssoc(pos, lKey, entry);
            m_tempCtx.m_mapChanges.SetAt(lKey, entry);
        }
    }

    m_lscPersons.SetExtendedStyle(
        m_lscPersons.GetExtendedStyle() |
        LVS_EX_FULLROWSELECT |
        LVS_EX_GRIDLINES |
        LVS_EX_DOUBLEBUFFER
    );
    m_lscPersons.InsertColumn(0, _T("Име"), LVCFMT_LEFT, 100);
    m_lscPersons.InsertColumn(1, _T("Фамилия"), LVCFMT_LEFT, 100);
    m_lscPersons.InsertColumn(2, _T("ЕГН"), LVCFMT_LEFT, 100);

    FillList();


    if (m_lInitialID > 0)
    {
        ChangeEntry entry;
        bool bDeleted = false;

        if (m_tempCtx.m_mapChanges.Lookup(m_lInitialID, entry) && entry.eChange == ChangeDelete)
            bDeleted = true;
        else if (m_pParentCtx &&
            m_pParentCtx->m_mapChanges.Lookup(m_lInitialID, entry) &&
            entry.eChange == ChangeDelete)
            bDeleted = true;

        if (!bDeleted)
        {
            int nCount = m_lscPersons.GetItemCount();
            for (int i = 0; i < nCount; i++)
            {
                PERSONS* p = (PERSONS*)m_lscPersons.GetItemData(i);
                if (p && p->lID == m_lInitialID)
                {
                    m_lscPersons.SetItemState(
                        i,
                        LVIS_SELECTED | LVIS_FOCUSED,
                        LVIS_SELECTED | LVIS_FOCUSED
                    );
                    m_lscPersons.EnsureVisible(i, FALSE);
                    break;
                }
            }
        }
    }

    return TRUE;
}

void CPersonSelectDlg::OnOK()
{
    m_lSelectedPersonID = GetSelectedPersonID();

    if (m_pParentCtx)
    {
        POSITION pos = m_tempCtx.m_mapChanges.GetStartPosition();
        long lKey;
        ChangeEntry tempEntry;

        while (pos)
        {
            m_tempCtx.m_mapChanges.GetNextAssoc(pos, lKey, tempEntry);

            ChangeEntry parentEntry;
            if (m_pParentCtx->m_mapChanges.Lookup(lKey, parentEntry) &&
                parentEntry.pRecord != tempEntry.pRecord)
            {
                delete (PERSONS*)parentEntry.pRecord;
            }

            m_pParentCtx->m_mapChanges.SetAt(lKey, tempEntry);
        }

        m_tempCtx.m_mapChanges.RemoveAll();
    }

    CDialogEx::OnOK();
}

void CPersonSelectDlg::OnCancel()
{
    m_tempCtx.m_mapChanges.RemoveAll();

    CDialogEx::OnCancel();
}

void CPersonSelectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PERSONS, m_lscPersons);
}

// Methods
// ----------------
void CPersonSelectDlg::FillList()
{
    m_lscPersons.DeleteAllItems();
    int nIndex = 0;

    POSITION pos;
    long lKey;
    ChangeEntry entry;

    // Show from the temp map
    pos = m_tempCtx.m_mapChanges.GetStartPosition();
    while (pos)
    {
        m_tempCtx.m_mapChanges.GetNextAssoc(pos, lKey, entry);

        if (entry.eEntity == EntityPerson)
        {
            // Ако е Insert -> винаги показваме
            if (entry.eChange == ChangeInsert)
            {
                InsertPersonItem(nIndex++, (PERSONS*)entry.pRecord);
            }
            // Ако е Update, но ID-то е временно (<= 0), значи сме редактирали 
            // запис, добавен от родителя. Трябва да го покажем, иначе ще изчезне,
            // защото го няма в m_pAllPersons.
            else if (entry.eChange == ChangeUpdate && lKey <= 0)
            {
                InsertPersonItem(nIndex++, (PERSONS*)entry.pRecord);
            }
        }
    }

    // Show from the parent map
    if (m_pParentCtx)
    {
        pos = m_pParentCtx->m_mapChanges.GetStartPosition();
        while (pos)
        {
            m_pParentCtx->m_mapChanges.GetNextAssoc(pos, lKey, entry);

            // Ако вече имаме локална промяна за този запис в Temp, пропускаме го
            ChangeEntry tempEntry;
            if (m_tempCtx.m_mapChanges.Lookup(lKey, tempEntry))
                continue;

            if (entry.eEntity == EntityPerson && entry.eChange == ChangeInsert)
                InsertPersonItem(nIndex++, (PERSONS*)entry.pRecord);
        }
    }

    // Show from the array
    if (m_pAllPersons)
    {
        for (INT_PTR i = 0; i < m_pAllPersons->GetCount(); i++)
        {
            const PERSONS* pDocPerson = m_pAllPersons->GetAt(i);
            const PERSONS* pShow = pDocPerson;
            bool bSkip = false;

            if (m_tempCtx.m_mapChanges.Lookup(pDocPerson->lID, entry))
            {
                if (entry.eChange == ChangeDelete) bSkip = true;
                else if (entry.eChange == ChangeUpdate) pShow = (PERSONS*)entry.pRecord;
            }
            else if (m_pParentCtx && m_pParentCtx->m_mapChanges.Lookup(pDocPerson->lID, entry))
            {
                if (entry.eChange == ChangeDelete) bSkip = true;
                else if (entry.eChange == ChangeUpdate) pShow = (PERSONS*)entry.pRecord;
            }

            if (!bSkip)
            {
                InsertPersonItem(nIndex++, pShow);
            }
        }
    }
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
    newPerson.lID = m_tempCtx.GenerateTempID();
    CPersonDlg dlg(&newPerson, m_pAllCities, m_pUCNs, &m_tempCtx, PersonDialogTypeAdd);

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

    CPersonDlg dlg(&editPerson, m_pAllCities, m_pUCNs, &m_tempCtx, PersonDialogTypeEdit);

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

        if (m_tempCtx.m_mapChanges.Lookup(lID, entry))
        {
            if (entry.eChange == ChangeInsert)
            {
                delete (PERSONS*)entry.pRecord; 
                m_tempCtx.m_mapChanges.RemoveKey(lID);
            }
            else
            {
                delete (PERSONS*)entry.pRecord;

                PERSONS* pDummy = new PERSONS;
                pDummy->lID = lID;

                entry.eChange = ChangeDelete;
                entry.pRecord = pDummy;
                m_tempCtx.m_mapChanges.SetAt(lID, entry);
            }
        }
        else
        {

            PERSONS* pDummy = new PERSONS;
            pDummy->lID = lID;

            ChangeEntry newEntry{ ChangeDelete ,EntityPerson,pDummy};

            m_tempCtx.m_mapChanges.SetAt(lID, newEntry);
        }

        FillList();
    }
}

void CPersonSelectDlg::OnNMDblClkPersons(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnOK();
    *pResult = 0;
}

void CPersonSelectDlg::RegisterPersonChange(const PERSONS& personData, ChangeType changeType)
{
    
    ChangeEntry entry;

    if (m_tempCtx.m_mapChanges.Lookup(personData.lID, entry))
    {
        *(PERSONS*)entry.pRecord = personData;
    }
    else
    {
        PERSONS* pNewRecord = new PERSONS;
        *pNewRecord = personData; 

        ChangeEntry newEntry{ changeType ,EntityPerson,pNewRecord};
        m_tempCtx.m_mapChanges.SetAt(personData.lID, newEntry);
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
            ChangeEntry entry;
            if (m_tempCtx.m_mapChanges.Lookup(pPerson->lID, entry) &&
                entry.eEntity == EntityPerson)
            {
                if (entry.eChange == ChangeInsert)
                    clrBack = RGB(200, 255, 200);
                else if (entry.eChange == ChangeUpdate)
                    clrBack = RGB(255, 230, 180);
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
