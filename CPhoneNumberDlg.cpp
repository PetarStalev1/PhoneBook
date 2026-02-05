#include "pch.h"
#include "CPhoneNumberDlg.h"
#include "CPhoneNumbersView.h"
#include "CCitiesDialog.h"
#include "InputValidator.h"
#include "MainFrm.h"
#include "CPersonView.h"
#include "CPersonSelectDlg.h"

IMPLEMENT_DYNAMIC(CPhoneNumberDlg, CDialogEx)


BEGIN_MESSAGE_MAP(CPhoneNumberDlg, CDialogEx)
    ON_MESSAGE(WM_USER + 1, &CPhoneNumberDlg::OnSelectCity)
    ON_NOTIFY(NM_RCLICK, IDC_LSC_PN_CITIES, &CPhoneNumberDlg::OnNMRClickCities)
    ON_COMMAND(ID_CITY_EDIT, &CPhoneNumberDlg::OnContextEditCity)
    ON_COMMAND(ID_CITY_ADD, &CPhoneNumberDlg::OnContextAddCity)
    ON_COMMAND(ID_CITY_DELETE, &CPhoneNumberDlg::OnContextDeleteCity)
    ON_BN_CLICKED(IDC_VIEW_PERSONS, &CPhoneNumberDlg::OnBnClickedViewPersons)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LSC_PN_CITIES, &CPhoneNumberDlg::OnNMCustomdrawCities)
END_MESSAGE_MAP()
#define DIALOG_ADD_NAME _T("Добавяне")
#define DIALOG_EDIT_NAME _T("Редактиране")
#define DIALOG_VIEW_NAME _T("Преглед")
#define CITIES_NAME _T("Град")
#define CITIES_REGION _T("Регион")

#define NAME_MIN_FIELD_LENGTH 2
#define ADDRESS_MIN_FIELD_LENGTH 4
#define UCN_MIN_FIELD_LENGTH 10

#define SETTING_ITEM_TEXT_ERROR_MESSAGE _T("Грешка при въвеждането на текста във ListCtrl.")
#define CONFIRM_DELETE_MESSAGE _T("Сигурни ли сте, че искате да изтриете селектирания запис?")
#define VALIDATE_FIRST_NAME_MESSAGE _T("Моля въведете име.")
#define VALIDATE_MIDDLE_NAME_MESSAGE _T("Моля въведете презиме")
#define VALIDATE_LAST_NAME_MESSAGE _T("Моля въведете фамилия")
#define VALIDATE_UCN_MESSAGE _T("Моля въведете правилно ЕГН (10 цифри).")
#define VALIDATE_ADDRESS_MESSAGE _T("Моля въведете адрес")
#define VALIDATE_CITY_MESSAGE _T("Моля изберете град")
#define VALIDATE_PHONETYPE_MESSAGE _T("Моля изберете тип на телефона")
#define COLOR_ORIGINAL_CITY   RGB(200, 230, 255) // светло синьо
#define COLOR_INSERT_CITY     RGB(200, 255, 200) // светло зелено
#define COLOR_UPDATE_CITY     RGB(255, 230, 180) // светло оранжево

enum CitiesColumns {
    CitiesNameColumn = 0,
    CitiesRegionNameColumn
};






CPhoneNumberDlg::CPhoneNumberDlg(PHONE_NUMBERS* pPhoneNumber,
     const PhoneNumberDialogType ePhoneNumberDialogType, CDialogChangeContext* pCtx, CWnd* pParent)
    : CDialogEx(IDD_PHONE_NUMBERS_ONEDIT, pParent),
    m_ePhoneNumberDialogType(ePhoneNumberDialogType), m_pChangeCtx(pCtx)

{
    if (pPhoneNumber != NULL)
    {
        m_recPhoneNumber = *pPhoneNumber;
    }
    else
    {
        // Add Mode - нулиране
        m_recPhoneNumber.lID = 0;
        m_recPhoneNumber.lPersonID = 0;
        m_recPhoneNumber.szPhoneNumber[0] = '\0';

        m_recPerson.lID = 0;
        m_recPerson.lCityID = 0;
        m_recPerson.szFirstName[0] = '\0';
    }
   
}

CPhoneNumberDlg::~CPhoneNumberDlg()
{
}

void CPhoneNumberDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_EDB_PHONENUMBER, m_edbPhoneNumber);
    DDX_Control(pDX, IDC_EDB_PERSONS_FIRST_NAME, m_edbFirstName);
    DDX_Control(pDX, IDC_EDB_PERSONS_MIDDLE_NAME, m_edbMiddleName);
    DDX_Control(pDX, IDC_EDB_PERSONS_LAST_NAME, m_edbLastName);
    DDX_Control(pDX, IDC_EDB_PERSONS_UCN, m_edbUCN);
    DDX_Control(pDX, IDC_LSC_PN_CITIES, m_lscCity);
    DDX_Control(pDX, IDC_EDB_PERSONS_ADDRESS, m_edbAddress);
    DDX_Control(pDX, IDC_CMB_PHONE_TYPE, m_cmbPhoneNumbersTypes);
    DDX_Control(pDX, IDC_VIEW_PERSONS, m_btnViewPersons);
   
}

BOOL CPhoneNumberDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_LBUTTONDOWN)  
    {
        CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
        if (pWnd)
        {
            int nID = pWnd->GetDlgCtrlID();

            if (nID == IDC_EDB_PERSONS_FIRST_NAME ||
                nID == IDC_EDB_PERSONS_MIDDLE_NAME ||
                nID == IDC_EDB_PERSONS_LAST_NAME ||
                nID == IDC_EDB_PERSONS_UCN ||
                nID == IDC_EDB_PERSONS_ADDRESS)
            {
                OnBnClickedViewPersons();
                return TRUE;
            }
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CPhoneNumberDlg::GetMergedCities(CPtrArray& arrCities)
{
    arrCities.RemoveAll();

    CPhoneNumbersView* pView = getPNView();
    INT_PTR nCount = pView->GetDocument()->GetCityCount();

    for (INT_PTR i = 0; i < nCount; i++)
    {
        const CITIES* pCity = pView->GetDocument()->GetCityAt(i);

        ChangeEntry entry;
        if (m_pChangeCtx->m_mapChanges.Lookup(pCity->lID, entry) && entry.eEntity == EntityCity)
        {
            if (entry.eChange == ChangeDelete) continue; 
            if (entry.eChange == ChangeUpdate)
            {
                arrCities.Add((void*)entry.pRecord);
                continue;
            }
        }
        arrCities.Add((void*)pCity); 
    }

    POSITION pos = m_pChangeCtx->m_mapChanges.GetStartPosition();
    long lKey; ChangeEntry entry;
    while (pos != NULL)
    {
        m_pChangeCtx->m_mapChanges.GetNextAssoc(pos, lKey, entry);
        if (entry.eEntity == EntityCity && entry.eChange == ChangeInsert)
        {
            arrCities.Add((void*)entry.pRecord);
        }
    }
}

BOOL CPhoneNumberDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_recPhoneNumber.lPersonID > 0)
    {
        const PERSONS* pFoundPerson = getPNView()->GetDocument()->GetPersonByID(m_recPhoneNumber.lPersonID);
        if (pFoundPerson != NULL)
        {
            m_recPerson = *pFoundPerson;
        }
    }

    SetDialogName();
    SetPhoneTypesComboBox();
    SetColumns();
    SetInitialData();


    SetDialogFields();

    if (m_ePhoneNumberDialogType == PhoneNumberDialogTypeView)
    {
        SetReadOnly();
    }

    ResortCities(); 

    SelectCityInListByID(m_recPerson.lCityID);

    m_recOriginalPerson = m_recPerson;
    m_recOriginalPhone = m_recPhoneNumber;
    MakeEditLookLikeLabel(m_edbFirstName);
    MakeEditLookLikeLabel(m_edbMiddleName);
    MakeEditLookLikeLabel(m_edbLastName);
    MakeEditLookLikeLabel(m_edbUCN);
    MakeEditLookLikeLabel(m_edbAddress);
    return TRUE;
}

bool CPhoneNumberDlg::HasCityChanged(const CITIES& oldCity, const CITIES& newCity) const
{
    if (_tcscmp(oldCity.szCityName, newCity.szCityName) != 0)
        return true;

    if (_tcscmp(oldCity.szDistrict, newCity.szDistrict) != 0)
        return true;


    return false;
}

void CPhoneNumberDlg::OnOK()
{
 

    if (m_ePhoneNumberDialogType == PhoneNumberDialogTypeView)
    {
        CDialogEx::OnOK();
        return;
    }
    if (!ValidateUserInput())
        return;

    

    m_edbPhoneNumber.GetWindowText(m_recPhoneNumber.szPhoneNumber, PHONE_NUMBERS_NUMBER_LENGTH);

    m_edbFirstName.GetWindowText(m_recPerson.szFirstName, PERSONS_NAME_LENGTH);
    m_edbMiddleName.GetWindowText(m_recPerson.szMiddleName, PERSONS_NAME_LENGTH);
    m_edbLastName.GetWindowText(m_recPerson.szLastName, PERSONS_NAME_LENGTH);
    m_edbUCN.GetWindowText(m_recPerson.szUCN, PERSONS_UCN_LENGTH);
    m_edbAddress.GetWindowText(m_recPerson.szAddress, PERSONS_ADDRESS_LENGTH);

    POSITION pos = m_lscCity.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_lscCity.GetNextSelectedItem(pos);
        CITIES* pCity = (CITIES*)m_lscCity.GetItemData(nItem);
        if (pCity) m_recPerson.lCityID = pCity->lID;
    }
    else
    {
        MessageBox(_T("Моля изберете град!"), _T("Грешка"), MB_ICONEXCLAMATION);
        return;
    }

    int nIndexType = m_cmbPhoneNumbersTypes.GetCurSel();
    if (nIndexType != CB_ERR)
    {
        m_recPhoneNumber.lPhoneTypeID = (long)m_cmbPhoneNumbersTypes.GetItemData(nIndexType);
    }
    else
    {
        MessageBox(_T("Моля изберете тип на телефона!"), _T("Грешка"), MB_ICONEXCLAMATION);
        return;
    }

    m_recPhoneNumber.lPersonID = m_recPerson.lID;


    ChangeEntry personEntry;

    if (m_pChangeCtx->m_mapChanges.Lookup(m_recPerson.lID, personEntry))
    {
        if (personEntry.eEntity == EntityPerson)
        {
            PERSONS* pBufferedPerson = (PERSONS*)personEntry.pRecord;
            *pBufferedPerson = m_recPerson;
        }
    }
    else
    {
        PERSONS* pPersonRec = new PERSONS(m_recPerson);

        ChangeEntry newPersonEntry;
        newPersonEntry.eEntity = EntityPerson;
        newPersonEntry.pRecord = pPersonRec;
        newPersonEntry.eChange = ChangeUpdate;

        m_pChangeCtx->m_mapChanges.SetAt(pPersonRec->lID, newPersonEntry);
    }


    PHONE_NUMBERS* pPhoneRec = new PHONE_NUMBERS(m_recPhoneNumber);

    ChangeEntry phoneEntry;
    phoneEntry.eEntity = EntityPhone;
    phoneEntry.pRecord = pPhoneRec;

    if (m_ePhoneNumberDialogType == PhoneNumberDialogTypeAdd)
    {
        phoneEntry.eChange = ChangeInsert;
        pPhoneRec->lID = m_pChangeCtx->GenerateTempID();
        m_pChangeCtx->m_mapChanges.SetAt(pPhoneRec->lID, phoneEntry);
    }
    else if (m_ePhoneNumberDialogType == PhoneNumberDialogTypeEdit)
    {
        phoneEntry.eChange = ChangeUpdate;
        m_pChangeCtx->m_mapChanges.SetAt(pPhoneRec->lID, phoneEntry);
    }

    CDialogEx::OnOK();
}

BOOL CPhoneNumberDlg::SetDialogName()
{
    switch (m_ePhoneNumberDialogType)
    {
    case PhoneNumberDialogTypeAdd:
        SetWindowText(DIALOG_ADD_NAME);
        break;
    case PhoneNumberDialogTypeEdit:
        SetWindowText(DIALOG_EDIT_NAME);
        break;
    case PhoneNumberDialogTypeView:
        SetWindowText(DIALOG_VIEW_NAME);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


BOOL CPhoneNumberDlg::SetDialogFields()
{
    
    switch (m_ePhoneNumberDialogType)
    {

    case PhoneNumberDialogTypeAdd:
        if (m_recPerson.lID == 0)
        {
            break;
        }
    case PhoneNumberDialogTypeEdit:
    case PhoneNumberDialogTypeView:
       
        m_edbPhoneNumber.SetWindowTextW(m_recPhoneNumber.szPhoneNumber);
        m_edbFirstName.SetWindowTextW(m_recPerson.szFirstName);
        m_edbMiddleName.SetWindowTextW(m_recPerson.szMiddleName);
        m_edbLastName.SetWindowTextW(m_recPerson.szLastName);
        m_edbUCN.SetWindowTextW(m_recPerson.szUCN);
        m_edbAddress.SetWindowTextW(m_recPerson.szAddress);
        break;
    default:
        return FALSE;
        break;
    }
     
    return TRUE;
}

BOOL CPhoneNumberDlg::ValidateUserInput() const
{
    CInputValidator oInputValidator;

    if (!oInputValidator.ValidateTextField(m_edbFirstName, PERSONS_NAME_LENGTH, NAME_MIN_FIELD_LENGTH))
    {
        AfxMessageBox(VALIDATE_FIRST_NAME_MESSAGE, MB_OK, MB_ICONERROR);
        return FALSE;
    }
    if (!oInputValidator.ValidateTextField(m_edbMiddleName, PERSONS_NAME_LENGTH, NAME_MIN_FIELD_LENGTH))
    {
        AfxMessageBox(VALIDATE_MIDDLE_NAME_MESSAGE, MB_OK, MB_ICONERROR);
        return FALSE;
    }

    if (!oInputValidator.ValidateTextField(m_edbLastName, PERSONS_NAME_LENGTH, NAME_MIN_FIELD_LENGTH))
    {
        AfxMessageBox(VALIDATE_LAST_NAME_MESSAGE, MB_OK, MB_ICONERROR);
        return FALSE;
    }

    if (!oInputValidator.ValidateTextField(m_edbAddress, PERSONS_ADDRESS_LENGTH, ADDRESS_MIN_FIELD_LENGTH))
    {
        AfxMessageBox(VALIDATE_ADDRESS_MESSAGE, MB_OK, MB_ICONERROR);
        return FALSE;
    }

    if (!oInputValidator.ValidateNumber(m_edbUCN, UCN_MIN_FIELD_LENGTH, UCN_MIN_FIELD_LENGTH))
    {
        AfxMessageBox(VALIDATE_UCN_MESSAGE, MB_OK, MB_ICONERROR);
        return FALSE;
    }

    if (!oInputValidator.ValidateComboBox(m_cmbPhoneNumbersTypes))
    {
        AfxMessageBox(VALIDATE_PHONETYPE_MESSAGE, MB_OK, MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void CPhoneNumberDlg::MakeEditLookLikeLabel(CEdit& ed)
{
    ed.SetReadOnly(TRUE);

    ed.ModifyStyle(WS_BORDER, 0);
    ed.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

    ed.SetSel(0, 0);
    ed.EnableWindow(TRUE);
    ed.Invalidate();
}

void CPhoneNumberDlg::SetPhoneTypesComboBox()
{
    getPNView()->GetDocument()->GetPhoneTypeCount();

    for (INT_PTR nIndexLoop = 0; nIndexLoop < getPNView()->GetDocument()->GetPhoneTypeCount(); nIndexLoop++)
    {
        const PHONE_TYPES* pCurrentPhoneType = getPNView()->GetDocument()->GetPhoneTypeAt(nIndexLoop);

        CString format;
        format.Format(_T("%s"), pCurrentPhoneType->szType);
        int nIndex = m_cmbPhoneNumbersTypes.AddString(format.GetString());
        m_cmbPhoneNumbersTypes.SetItemData(nIndex, (DWORD_PTR)pCurrentPhoneType->lID);

        if (pCurrentPhoneType->lID == m_recPhoneNumber.lPhoneTypeID) {
            m_cmbPhoneNumbersTypes.SetCurSel(nIndex);
        }
    }
}
void CPhoneNumberDlg::SetInitialData()
{
    m_lscCity.DeleteAllItems();

    LONG style = GetWindowLong(m_lscCity.GetSafeHwnd(), GWL_STYLE);
    style |= LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
    SetWindowLong(m_lscCity.GetSafeHwnd(), GWL_STYLE, style);

    m_lscCity.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    long lSelectedCityID = -1;

    if (m_ePhoneNumberDialogType != PhoneNumberDialogTypeAdd)
    {
        lSelectedCityID = m_recPerson.lCityID;
    }


    

    for (INT_PTR nIndexLoop = 0; nIndexLoop < getPNView()->GetDocument()->GetCityCount(); nIndexLoop++)
    {
        const CITIES* pCity = getPNView()->GetDocument()->GetCityAt(nIndexLoop);
        if (m_ePhoneNumberDialogType == PhoneNumberDialogTypeView)
        {
            if (pCity->lID != lSelectedCityID)
                continue;
        }

      
        int nIndex = m_lscCity.InsertItem(LVIF_TEXT | LVIF_PARAM, nIndexLoop, pCity->szCityName, 0, 0, 0, (DWORD_PTR)pCity);
        m_lscCity.SetItemData(nIndex, (DWORD_PTR)pCity);
        SetItemText(*pCity, nIndex);

        
       /* if (pCity->lID == lSelectedCityID)
        {
            m_nCityToSelect = nIndex;
        }*/
    }

    //PostMessage(WM_USER + 1);
}

BOOL CPhoneNumberDlg::SetItemText(const CITIES& recCities, const int nIndexList)
{

    if (!m_lscCity.SetItemText(nIndexList, CitiesNameColumn, recCities.szCityName))
    {
        return FALSE;
    }
    if (!m_lscCity.SetItemText(nIndexList, CitiesRegionNameColumn, recCities.szDistrict))
    {
        return FALSE;
    }
    return 0;
}

CString CPhoneNumberDlg::CityToString(const CITIES& recCity) const
{
    CString strCity = recCity.szCityName;
    strCity += _T(" - ");
    strCity += recCity.szDistrict;
    return strCity;
}

void CPhoneNumberDlg::SetColumns()
{
    m_lscCity.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_lscCity.InsertColumn(CitiesNameColumn, CITIES_NAME, LVCFMT_LEFT, 90);
    m_lscCity.InsertColumn(CitiesRegionNameColumn, CITIES_REGION, LVCFMT_LEFT, 90);
}

void CPhoneNumberDlg::OnPopupView()
{
   
}

LRESULT CPhoneNumberDlg::OnSelectCity(WPARAM, LPARAM)
{
    if (m_nCityToSelect >= 0)
    {
        m_lscCity.SetItemState(
            m_nCityToSelect,
            LVIS_SELECTED | LVIS_FOCUSED,
            LVIS_SELECTED | LVIS_FOCUSED
        );

        m_lscCity.EnsureVisible(m_nCityToSelect, FALSE);
        m_lscCity.SetFocus();
    }
    return 0;
}

void CPhoneNumberDlg::OnNMRClickCities(NMHDR* pNMHDR, LRESULT* pResult)
{
    CMenu menu;
    if (menu.LoadMenu(IDR_MENU_CITIES))
    {
        CMenu* pSubMenu = menu.GetSubMenu(0);
        if (pSubMenu)
        {
            CPoint point;
            GetCursorPos(&point);

            POSITION pos = m_lscCity.GetFirstSelectedItemPosition();
            if (pos == NULL)
            {
                pSubMenu->EnableMenuItem(ID_CITY_EDIT, MF_BYCOMMAND | MF_GRAYED);
                pSubMenu->EnableMenuItem(ID_CITY_DELETE, MF_BYCOMMAND | MF_GRAYED);
            }

            pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
        }
    }

    *pResult = 0;
}

void CPhoneNumberDlg::OnBnClickedViewPersons()
{
    CPtrArray arrCities;
    GetMergedCities(arrCities);

    CMapUCN mapUCNs;
    GetMergedUCNs(mapUCNs);

    CPersonSelectDlg dlg(getPNView(), m_pChangeCtx, &arrCities, &mapUCNs, m_recPerson.lID, this);

    if (dlg.DoModal() == IDOK)
    {
        long lSelectedID = dlg.m_lSelectedPersonID;

        PERSONS personData;
        bool bFound = false;

        ChangeEntry entry;
        if (m_pChangeCtx->m_mapChanges.Lookup(lSelectedID, entry) && entry.eEntity == EntityPerson)
        {
            personData = *(PERSONS*)entry.pRecord;
            bFound = true;
        }
        else
        {
            const PERSONS* pPerson = getPNView()->GetDocument()->GetPersonByID(lSelectedID);
            if (pPerson != NULL)
            {
                personData = *pPerson;
                bFound = true;
            }
        }

        if (bFound)
        {
            m_recPerson = personData;
            m_recPhoneNumber.lPersonID = m_recPerson.lID;
            SetDialogFields();

            // 2. ВАЖНО: Пренареждаме градовете, за да може новият град да отиде най-горе
            ResortCities();

            // 3. Селектираме го (това ще го направи Тъмно син)
            SelectCityInListByID(m_recPerson.lCityID);

            // 4. Насилствено прерисуване, за да сме сигурни, че цветовете (светло синьо) ще се наложат
            m_lscCity.Invalidate();
            m_lscCity.UpdateWindow();
        }
    }
}



void CPhoneNumberDlg::OnContextAddCity()
{
    CITIES* pNewCity = new CITIES;

    pNewCity->lID = m_pChangeCtx->GenerateTempID();

    CCitiesDialog dlg(pNewCity, CitiesDialogTypeAdd, this);
    if (dlg.DoModal() != IDOK)
    {
        delete pNewCity;
        return;

    }
    int nIndex = m_lscCity.InsertItem(LVIF_TEXT | LVIF_PARAM, m_lscCity.GetItemCount(),
        pNewCity->szCityName, 0, 0, 0, (DWORD_PTR)pNewCity);
    m_lscCity.SetItemText(nIndex, 1, pNewCity->szDistrict);

    ChangeEntry entry;
    entry.eEntity = EntityCity;
    entry.eChange = ChangeInsert;
    entry.pRecord = pNewCity;

    m_pChangeCtx->m_mapChanges.SetAt(pNewCity->lID, entry);
    ResortCities();
  }

void CPhoneNumberDlg::OnContextEditCity()
{
    POSITION pos = m_lscCity.GetFirstSelectedItemPosition();
    if (pos == NULL) return;
    int nIndex = m_lscCity.GetNextSelectedItem(pos);

    CITIES* pCurrentCity = (CITIES*)m_lscCity.GetItemData(nIndex);
    if (!pCurrentCity) return;

    CITIES recEdit = *pCurrentCity;
    CITIES oldCity = recEdit;

    CCitiesDialog dlg(&recEdit, CitiesDialogTypeEdit, this);
    if (dlg.DoModal() != IDOK || recEdit == oldCity) return;

    ChangeEntry entry;
    if (m_pChangeCtx->m_mapChanges.Lookup(recEdit.lID, entry))
    {
        CITIES* pBufferedData = (CITIES*)entry.pRecord;
        *pBufferedData = recEdit;
    }
    else
    {
        CITIES* pNewRecord = new CITIES;
        *pNewRecord = recEdit;

        ChangeEntry newEntry;
        newEntry.eEntity = EntityCity;
        newEntry.eChange = ChangeUpdate; 
        newEntry.pRecord = pNewRecord;

        m_pChangeCtx->m_mapChanges.SetAt(recEdit.lID, newEntry);

        m_lscCity.SetItemData(nIndex, (DWORD_PTR)pNewRecord);
    }

    m_lscCity.SetItemText(nIndex, 0, recEdit.szCityName);
    m_lscCity.SetItemText(nIndex, 1, recEdit.szDistrict);

    ResortCities();
}

void CPhoneNumberDlg::OnContextDeleteCity()
{  
    POSITION pos = m_lscCity.GetFirstSelectedItemPosition();
    if (pos == NULL) return;
    int nIndex = m_lscCity.GetNextSelectedItem(pos);

    CITIES* pCity = (CITIES*)m_lscCity.GetItemData(nIndex);
    long lID = pCity->lID;

    ChangeEntry entry;
    if (m_pChangeCtx->m_mapChanges.Lookup(lID, entry))
    {
        if (entry.eChange == ChangeInsert)
        {
            delete (CITIES*)entry.pRecord;
            m_pChangeCtx->m_mapChanges.RemoveKey(lID);
        }
        else
        {
            delete (CITIES*)entry.pRecord;

            CITIES* pDummy = new CITIES; pDummy->lID = lID;

            entry.eChange = ChangeDelete;
            entry.pRecord = pDummy;
            m_pChangeCtx->m_mapChanges.SetAt(lID, entry);
        }
    }
    else
    {
        CITIES* pDummy = new CITIES; pDummy->lID = lID;

        ChangeEntry newEntry;
        newEntry.eEntity = EntityCity;
        newEntry.eChange = ChangeDelete;
        newEntry.pRecord = pDummy;

        m_pChangeCtx->m_mapChanges.SetAt(lID, newEntry);
    }

    m_lscCity.DeleteItem(nIndex);
}

CPhoneNumbersView* CPhoneNumberDlg::getPNView()
{
    CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
    if (pFrame == NULL) return NULL;

  
    CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
    if (pChild == NULL) return NULL;

    return (CPhoneNumbersView*)pChild->GetActiveView();
}

void CPhoneNumberDlg::SetReadOnly()
{
    m_edbPhoneNumber.SetReadOnly(TRUE);
    m_edbFirstName.SetReadOnly(TRUE);
    m_edbMiddleName.SetReadOnly(TRUE);
    m_edbLastName.SetReadOnly(TRUE);
    m_edbUCN.SetReadOnly(TRUE);
    m_edbAddress.SetReadOnly(TRUE);
    m_cmbPhoneNumbersTypes.EnableWindow(FALSE);
    m_lscCity.EnableWindow(FALSE);
    m_btnViewPersons.EnableWindow(FALSE);
}

void CPhoneNumberDlg::GetMergedUCNs(CMapUCN& mapUCNs)
{
    for (INT_PTR nIndex = 0; nIndex < getPNView()->GetDocument()->GetPersonCount(); nIndex++)
    {
        const PERSONS* pPerson = getPNView()->GetDocument()->GetPersonAt(nIndex);
        ChangeEntry entry;
        if (m_pChangeCtx->m_mapChanges.Lookup(pPerson->lID, entry) && entry.eEntity == EntityPerson)
        {
            if (entry.eChange == ChangeDelete) continue;
            if (entry.eChange == ChangeUpdate)
            {
                PERSONS* pUpd = (PERSONS*)entry.pRecord;
                mapUCNs.SetAt(pUpd->szUCN, pUpd->lID);
                continue;
            }
        }
        mapUCNs.SetAt(pPerson->szUCN, pPerson->lID);
    }
    POSITION pos = m_pChangeCtx->m_mapChanges.GetStartPosition();
    long lKey; ChangeEntry entry;
    while (pos) {
        m_pChangeCtx->m_mapChanges.GetNextAssoc(pos, lKey, entry);
        if (entry.eEntity == EntityPerson && entry.eChange == ChangeInsert) {
            PERSONS* pNew = (PERSONS*)entry.pRecord;
            mapUCNs.SetAt(pNew->szUCN, pNew->lID);
        }
    }
}

void CPhoneNumberDlg::SelectCityInListByID(long lID)
{
    POSITION pos = m_lscCity.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nOldItem = m_lscCity.GetNextSelectedItem(pos);
        m_lscCity.SetItemState(nOldItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
    }

    int nCount = m_lscCity.GetItemCount();
    for (int i = 0; i < nCount; i++)
    {
        CITIES* pCity = (CITIES*)m_lscCity.GetItemData(i);
        if (pCity != NULL && pCity->lID == lID)
        {
            m_lscCity.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
            m_lscCity.EnsureVisible(i, FALSE);
            return;
        }
    }
}
int CALLBACK CPhoneNumberDlg::CompareCities_MainDlg(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CPhoneNumberDlg* pDlg = reinterpret_cast<CPhoneNumberDlg*>(lParamSort);
    CITIES* pCity1 = reinterpret_cast<CITIES*>(lParam1);
    CITIES* pCity2 = reinterpret_cast<CITIES*>(lParam2);

    if (!pCity1 || !pCity2 || !pDlg) return 0;

    const long lActiveCityID = pDlg->GetActiveCityID();

    auto GetPriority = [&](CITIES* pCity) -> int
        {
           
            if (pCity->lID == lActiveCityID) return 0;

            CDialogChangeContext* pCtx = pDlg->GetChangeContext();

            if (pCtx != nullptr)
            {
                ChangeEntry entry;
                if (pCtx->m_mapChanges.Lookup(pCity->lID, entry))
                {
                    if (entry.eEntity == EntityCity &&
                        (entry.eChange == ChangeInsert || entry.eChange == ChangeUpdate))
                    {
                        return 1;
                    }
                }
            }
            return 2;
        };

    int p1 = GetPriority(pCity1);
    int p2 = GetPriority(pCity2);

    if (p1 != p2) return p1 - p2;

    return _tcscmp(pCity1->szCityName, pCity2->szCityName);
}
long CPhoneNumberDlg::GetActiveCityID() const
{
    return m_recPerson.lCityID;
}


void CPhoneNumberDlg::ResortCities()
{
    m_lscCity.SortItems(CompareCities_MainDlg, (LPARAM)this);
    m_lscCity.Invalidate();
    m_lscCity.UpdateWindow();
}
void CPhoneNumberDlg::OnNMCustomdrawCities(NMHDR* pNMHDR, LRESULT* pResult)
{ 
    LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

    if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
        return;
    }

    if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
    {
        int nRow = (int)pLVCD->nmcd.dwItemSpec;
        CITIES* pCity = (CITIES*)m_lscCity.GetItemData(nRow);

        if (!pCity)
        {
            *pResult = CDRF_DODEFAULT;
            return;
        }

        COLORREF clrBack = CLR_DEFAULT;
        if (pCity->lID == m_recPerson.lCityID)
        {
            clrBack = RGB(255, 255, 0); // Ярко Жълто
        }
        else if (m_pChangeCtx)
        {
            ChangeEntry entry;
            if (m_pChangeCtx->m_mapChanges.Lookup(pCity->lID, entry) && entry.eEntity == EntityCity)
            {
                if (entry.eChange == ChangeInsert)
                    clrBack = COLOR_INSERT_CITY; 
                else if (entry.eChange == ChangeUpdate)
                    clrBack = COLOR_UPDATE_CITY;
            }
        }

        if (clrBack != CLR_DEFAULT)
        {
            pLVCD->clrTextBk = clrBack;
            pLVCD->clrText = RGB(0, 0, 0); 

            pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;

            pLVCD->nmcd.uItemState &= ~CDIS_FOCUS;

            *pResult = CDRF_NEWFONT;
            return;
        }
    }

    *pResult = CDRF_DODEFAULT;
}