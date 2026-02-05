#include "pch.h"
#include "CPersonView.h"
#include "pch.h"
#include "PhoneBook.h"
#include "CPersonDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPersonView, CListView)

BEGIN_MESSAGE_MAP(CPersonView, CListView)
    // Add handlers here as needed
END_MESSAGE_MAP()

CPersonView::CPersonView() noexcept
{
    // Constructor logic, if needed
}

CPersonDocument* CPersonView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPersonDocument)));
    return (CPersonDocument*)m_pDocument;
}

BOOL CPersonView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CListView::PreCreateWindow(cs);
}

void CPersonView::OnInitialUpdate()
{
    //CListView::OnInitialUpdate();

    //CListCtrl& listCtrl = GetListCtrl();
    //listCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
    //listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //listCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
    //listCtrl.InsertColumn(1, _T("Име"), LVCFMT_LEFT, 100);           
    //listCtrl.InsertColumn(2, _T("Презиме"), LVCFMT_LEFT, 100);       
    //listCtrl.InsertColumn(3, _T("Фамилия"), LVCFMT_LEFT, 100);      
    //listCtrl.InsertColumn(4, _T("ЕГН"), LVCFMT_LEFT, 100);           
    //listCtrl.InsertColumn(5, _T("Град"), LVCFMT_LEFT, 120);          
    //listCtrl.InsertColumn(6, _T("Адрес"), LVCFMT_LEFT, 150);        
    //listCtrl.InsertColumn(7, _T("Телефон"), LVCFMT_LEFT, 150);         
    //listCtrl.InsertColumn(8, _T("Тип на телефон"), LVCFMT_LEFT, 150);   

    //CPersonDocument* pDoc = GetDocument();
    //const CPersonData& oData = pDoc->GetPersonData();
    //const CPersonsArray& oPersons = oData.GetPersons();


    //for (int nIndex = 0; nIndex < oPersons.GetCount(); ++nIndex)
    //{
    //    const PERSONS* pPerson = oPersons[nIndex];

    //    CString strID;
    //    strID.Format(_T("%ld"), pPerson->m_nId);

    //    int index = listCtrl.InsertItem(nIndex, strID);
    //    listCtrl.SetItemText(index, 1, pPerson->m_szFirstName);
    //    listCtrl.SetItemText(index, 2, pPerson->m_szMiddleName);
    //    listCtrl.SetItemText(index, 3, pPerson->m_szLastName);
    //    listCtrl.SetItemText(index, 4, pPerson->m_szCitizenId);

    //    CString strCityName;
    //    
    //    CITIES oCity;
    //    oCity.m_nID = pPerson->m_nCityID;
    //   
    //    /*CITIES* pCity = oData.GetCityById(pPerson->m_nCityID, oCity);
    //    if (pCity != nullptr)
    //    {
    //        strCityName = pCity->m_szCityName;  
    //    }*/

    //    listCtrl.SetItemText(index, 5, strCityName);
    //    listCtrl.SetItemText(index, 6, pPerson->m_szAdress);

    //    CString strUpdate;  
    //    
    //    strUpdate.Format(_T("%ld"), pPerson->m_nUpdateCounter);
    //    listCtrl.SetItemText(index, 7, strUpdate);
    //}
}

#ifdef _DEBUG
void CPersonView::AssertValid() const
{
    CListView::AssertValid();
}

void CPersonView::Dump(CDumpContext& dc) const
{
    CListView::Dump(dc);
}
#endif
