#include "pch.h"
#include "CPhoneNumbersView.h"
#include "CPhoneNumberDlg.h"
#include "PersonsSearchDialog.h"
#include "CErrorLogger.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersView

// Constants
// ----------------
#define INDEX_BY_ID_ERROR -1
#define PHONE_BOOK_NUMBER_COLUMN 100
#define PHONE_BOOK_NUMBER_TYPE 120
#define PHONE_BOOK_FIRST_AND_LAST_NAME 200
#define ALL_ITEMS_NUMBER 1000000

#define OPERATION_NOT_RECOGNISED_ERROR_MESSAGE _T("Не е разпозната операцията.")
#define STYLE_ERROR_MESSAGE _T("Грешка при настройката на стила.")
#define SETTING_ITEM_TEXT_ERROR_MESSAGE _T("Грешка при въвеждането на текста във View.")
#define DELETE_CONFIRM_MESSAGE _T("Потвърдете изтриването на данните.")
#define DELETE_ERROR_MESSAGE _T("Грешка при изтриване на данните.")
#define INSERT_ERROR_MESSAGE _T("Грешка при въвеждане на данните.")
#define UPDATE_ERROR_MESSAGE _T("Грешка при редакция на данните.")


IMPLEMENT_DYNCREATE(CPhoneNumbersView, CListView)

BEGIN_MESSAGE_MAP(CPhoneNumbersView, CListView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_KEYUP()
    ON_COMMAND(ID_POPUP_VIEW, &CPhoneNumbersView::OnContextView)
    ON_COMMAND(ID_POPUP_EDIT, &CPhoneNumbersView::OnContextEdit)
    ON_COMMAND(ID_POPUP_DELETE, &CPhoneNumbersView::OnContextDelete)
    ON_COMMAND(ID_POPUP_INSERT, &CPhoneNumbersView::OnContextAdd)
    ON_COMMAND(IDR_POPUP_SEARCH, &CPhoneNumbersView::OnContextSearch)
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------

CPhoneNumbersView::CPhoneNumbersView() noexcept {}
CPhoneNumbersView::~CPhoneNumbersView() {}

// Overrides
// ----------------
void CPhoneNumbersView::OnInitialUpdate()
{
    InitializeListCtrl();
    CListView::OnInitialUpdate();
}
CPhoneNumbersDocument* CPhoneNumbersView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneNumbersDocument)));
    return (CPhoneNumbersDocument*)m_pDocument;
}

BOOL CPhoneNumbersView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CListView::PreCreateWindow(cs);
}

void CPhoneNumbersView::OnDraw(CDC* /*pDC*/)
{
    CPhoneNumbersDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
}
void CPhoneNumbersView::InitializeListCtrl()
{
    CListCtrl& list = GetListCtrl();

    list.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
    list.SetExtendedStyle(
        list.GetExtendedStyle() |
        LVS_EX_FULLROWSELECT |
        LVS_EX_GRIDLINES
    );

    SetColumns();
    SetInitialData();
}


void CPhoneNumbersView::SetColumns()
{
    CListCtrl& list = GetListCtrl();
    list.InsertColumn(PhoneBookViewColumnNumber, _T("Телефон"), LVCFMT_RIGHT, PHONE_BOOK_NUMBER_COLUMN);
    list.InsertColumn(PhoneBookViewColumnFirstAndLastName, _T("Име"), LVCFMT_LEFT, PHONE_BOOK_FIRST_AND_LAST_NAME);
    list.InsertColumn(PhoneBookViewColumnPhoneType, _T("Тип"), LVCFMT_LEFT, PHONE_BOOK_NUMBER_TYPE);
}

void CPhoneNumbersView::SetInitialData()
{
    CListCtrl& list = GetListCtrl();
    list.DeleteAllItems();

    for (INT_PTR nIndex = 0; nIndex < GetDocument()->GetPhoneCount(); nIndex++)
    {
        const PHONE_NUMBERS* pPhoneNumber = GetDocument()->GetPhoneAt(nIndex);
        OperationInsert(pPhoneNumber);
    }
}


int CPhoneNumbersView::GetIndexByID(const long lID) const
{
    LVFINDINFO info;
    info.flags = LVFI_PARAM;
    info.lParam = lID;

    return GetListCtrl().FindItem(&info);
}

BOOL CPhoneNumbersView::IsSelectedRow() const
{
    return GetSelectedPhoneNumber() != NULL;
}

const PHONE_NUMBERS* CPhoneNumbersView::GetSelectedPhoneNumber() const
{
    CListCtrl& list = GetListCtrl();
    POSITION oPosition = list.GetFirstSelectedItemPosition();
    if (oPosition == NULL) return NULL;

    const int nItem = list.GetNextSelectedItem(oPosition);
    const long lID = (long)list.GetItemData(nItem);

    return GetDocument()->GetPhoneByID(lID);
}



void CPhoneNumbersView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CPhoneNumbersView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    CMenu oContextMenu;
    if (oContextMenu.LoadMenu(IDR_CONTEXT_PHONENUMBERS))
    {
        CMenu* oPopupMenu = oContextMenu.GetSubMenu(0);

        if (!IsSelectedRow())
        {
            oPopupMenu->EnableMenuItem(ID_POPUP_DELETE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            oPopupMenu->EnableMenuItem(ID_POPUP_EDIT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            oPopupMenu->EnableMenuItem(ID_POPUP_VIEW, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        }

        oPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
    }
#endif
}

void CPhoneNumbersView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    OnContextView();
    CListView::OnLButtonDblClk(nFlags, point);
}

void CPhoneNumbersView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_DELETE)
    {
        OnContextDelete();
    }
    CListView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CPhoneNumbersView::UpdateRowsForPerson(const PERSONS* pPerson)
{
    if (pPerson == NULL) return;

    CListCtrl& list = GetListCtrl();
    int nCount = list.GetItemCount();

    list.SetRedraw(FALSE);

    for (int i = 0; i < nCount; i++)
    {
        long lPhoneID = (long)list.GetItemData(i);
        const PHONE_NUMBERS* pPhone = GetDocument()->GetPhoneByID(lPhoneID);

        if (pPhone != NULL && pPhone->lPersonID == pPerson->lID)
        {
            SetListViewItem(i, *pPhone);
        }
    }

    list.SetRedraw(TRUE);
    list.Invalidate();
}



void CPhoneNumbersView::OperationInsert(const PHONE_NUMBERS* pPhoneNumber)
{
    CListCtrl& list = GetListCtrl();
    const int nIndex = list.InsertItem(LVIF_PARAM, 0,
        pPhoneNumber->szPhoneNumber, 0, 0, 0,
        pPhoneNumber->lID);

    SetListViewItem(nIndex, *pPhoneNumber);
}

void CPhoneNumbersView::OperationUpdate(const PHONE_NUMBERS* pPhoneNumber)
{
    CListCtrl& list = GetListCtrl();
    const int nIndex = GetIndexByID(pPhoneNumber->lID);
    if (nIndex != INDEX_BY_ID_ERROR)
    {
        SetListViewItem(nIndex, *pPhoneNumber);
    }
}

void CPhoneNumbersView::OperationDelete(const long lID)
{
    CListCtrl& list = GetListCtrl();
    const int nIndex = GetIndexByID(lID);
    if (nIndex != INDEX_BY_ID_ERROR)
    {
        list.DeleteItem(nIndex);
    }
}

void CPhoneNumbersView::SetListViewItem(int nIndex, const PHONE_NUMBERS& oPhoneNumber)
{

    CListCtrl& list = GetListCtrl();

    const PERSONS* pPerson = GetDocument()->GetPersonByID(oPhoneNumber.lPersonID);
    const PHONE_TYPES* pPhoneType = GetDocument()->GetPhoneTypeByID(oPhoneNumber.lPhoneTypeID);

    CString strName = _T("Unknown Person");
    CString strType = _T("Unknown Type");

    if (pPerson != NULL)
        strName.Format(_T("%s %s"), pPerson->szFirstName, pPerson->szLastName);

    if (pPhoneType != NULL)
        strType = pPhoneType->szType;

    list.SetItemText(nIndex, PhoneBookViewColumnNumber, oPhoneNumber.szPhoneNumber);
    list.SetItemText(nIndex, PhoneBookViewColumnFirstAndLastName, strName);
    list.SetItemText(nIndex, PhoneBookViewColumnPhoneType, strType);

    list.SetItemData(nIndex, (DWORD_PTR)oPhoneNumber.lID);
}

void CPhoneNumbersView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (pHint == NULL && lHint != DocumentDataOperationLoad)
        return;

    if (m_bIsSearch)
    {
        ShowSearch();
        return;
    }

    switch ((DocumentDataOperation)lHint)
    {
    case DocumentDataOperationLoad:
        SetInitialData();
        break;

    case DocumentDataOperationInsert:
        OperationInsert((PHONE_NUMBERS*)pHint);
        break;

    case DocumentDataOperationUpdate:
        OperationUpdate((PHONE_NUMBERS*)pHint);
        break;

    case DocumentDataOperationDelete:
        OperationDelete((long)(INT_PTR)pHint);
        break;

    case DocumentDataOperationUpdatePerson:
        UpdateRowsForPerson((PERSONS*)pHint);
        break;
    default:
        break;
    }
}

void CPhoneNumbersView::OnContextAdd()
{
   CDialogChangeContext changeCtx;
   CPhoneNumberDlg oDlg(NULL, 
       PhoneNumberDialogTypeEdit, &changeCtx,
       GetDocument()->GetAllCities(),
       GetDocument()->GetAllPersons(),
       GetDocument()->GetAllPhoneTypes(),
       GetDocument()->GetUCNMap(),
       this);

    if (oDlg.DoModal() == IDOK)
    {
        if (!GetDocument()->ApplyChanges(changeCtx))
        {
            CErrorLogger::LogMessage(INSERT_ERROR_MESSAGE, TRUE, TRUE);
            return;
        }
    }
}

void CPhoneNumbersView::OnContextEdit()
{
    const PHONE_NUMBERS* pConstSelected = GetSelectedPhoneNumber();
    if (pConstSelected == NULL) return;

    PHONE_NUMBERS recEdit = *pConstSelected;
    CDialogChangeContext changeCtx;
    CPhoneNumberDlg oDlg(&recEdit, PhoneNumberDialogTypeEdit, &changeCtx, 
        GetDocument()->GetAllCities(),
        GetDocument()->GetAllPersons(),
        GetDocument()->GetAllPhoneTypes(),
        GetDocument()->GetUCNMap(),
        this);
    if (oDlg.DoModal() == IDOK)
    {
        if (!GetDocument()->ApplyChanges(changeCtx))
        {
            CErrorLogger::LogMessage(UPDATE_ERROR_MESSAGE, TRUE, TRUE);
            return;
        }
    }
}

void CPhoneNumbersView::OnContextView()
{
    const PHONE_NUMBERS* pConstSelected = GetSelectedPhoneNumber();
    if (pConstSelected == NULL) return;

    PHONE_NUMBERS recView = *pConstSelected;

    CPhoneNumberDlg oDlg(&recView, PhoneNumberDialogTypeView, NULL, NULL, NULL, NULL, NULL, this);
    oDlg.DoModal();
}

void CPhoneNumbersView::OnContextDelete()
{
    const PHONE_NUMBERS* pPN = GetSelectedPhoneNumber();
    if (pPN == NULL) return;

    if (AfxMessageBox(DELETE_CONFIRM_MESSAGE, MB_YESNO | MB_ICONINFORMATION) == IDYES)
    {
        
        if (!GetDocument()->DeletePhoneNumber(pPN->lID))
        {
            CErrorLogger::LogMessage(DELETE_ERROR_MESSAGE, FALSE, TRUE);
        }
    }
}

void CPhoneNumbersView::OnContextSearch()
{
    CListCtrl& list = GetListCtrl();
    CPersonsSearchDialog oDialog;

    if (oDialog.DoModal() != IDOK)
    {
        // Cancel Search -> Reset
        m_bIsSearch = FALSE;
        SetInitialData();
        return;
    }

    if (oDialog.m_strName.IsEmpty() &&
        oDialog.m_strUCN.IsEmpty() &&
        oDialog.m_strAddress.IsEmpty())
    {
        m_bIsSearch = FALSE;
        SetInitialData();
        return;
    }
    m_bIsSearch = TRUE;
    m_strSearchName = oDialog.m_strName;
    m_strSearchUCN = oDialog.m_strUCN;
    m_strSearchAddress = oDialog.m_strAddress;

    ShowSearch();
}

void CPhoneNumbersView::ShowSearch()
{
    CListCtrl& list = GetListCtrl();
    list.DeleteAllItems();
    m_oAutoArray.RemoveAll();

    GetDocument()->SelectByNameUCNAddress(m_oAutoArray, m_strSearchName, m_strSearchUCN, m_strSearchAddress);

    for (INT_PTR i = 0; i < m_oAutoArray.GetCount(); i++)
    {
        PHONE_NUMBERS* pPhoneNumber = m_oAutoArray.GetAt(i);
        OperationInsert(pPhoneNumber);
    }
}



#ifdef _DEBUG
void CPhoneNumbersView::AssertValid() const
{
    CListView::AssertValid();
}

void CPhoneNumbersView::Dump(CDumpContext& dc) const
{
    CListView::Dump(dc);
}
#endif