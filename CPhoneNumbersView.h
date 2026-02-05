#pragma once
#include <afxcview.h>
#include "Structures.h"          
#include "CPhoneNumbersDocument.h"
#include "Resource.h"



/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersView

/// <summary>
/// View клас за таблицата PHONE_NUMBERS
/// </summary>
class CPhoneNumbersView : public CListView // Наследяваме директно CListView
{
protected:
    CPhoneNumbersView() noexcept;
    DECLARE_DYNCREATE(CPhoneNumbersView)

    // --- Attributes ---
public:
    CPhoneNumbersDocument* GetDocument() const;

private:
    // Кеш за търсене
    CPtrAutoArray<PHONE_NUMBERS> m_oAutoArray;
    BOOL m_bIsSearch = FALSE;
    CString m_strSearchName;
    CString m_strSearchUCN;
    CString m_strSearchAddress;

    // --- Operations ---
public:


private:
    // Помощни методи (преместени от CPhoneBookListView)
    void InitializeListCtrl();
    void SetColumns();
    void SetInitialData();

    // Helper за намиране на индекс по ID
    int GetIndexByID(const long lID) const;

    // Helper за визуализация
    void SetListViewItem(int nIndex, const PHONE_NUMBERS& oPhoneNumber);
    // CRUD Helpers
    void OperationInsert(const PHONE_NUMBERS* pPhone);
    void OperationUpdate(const PHONE_NUMBERS* pPhone);
    void OperationDelete(const long lID);

   const  PHONE_NUMBERS* GetSelectedPhoneNumber() const;
    BOOL IsSelectedRow() const;
    void ShowSearch();

    // --- Overrides ---
public:
    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate(); // Тук викаме InitializeListCtrl
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    // --- Implementation ---
public:
    virtual ~CPhoneNumbersView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // --- Message Map ---
protected:
    // Handler-и преместени от CPhoneBookListView
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    void UpdateRowsForPerson(const PERSONS* pPerson);

    // Команди от менюто
    afx_msg void OnContextAdd();
    afx_msg void OnContextEdit();
    afx_msg void OnContextDelete();
    afx_msg void OnContextView();
    afx_msg void OnContextSearch();

    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  
inline CPhoneNumbersDocument* CPhoneNumbersView::GetDocument() const
{
    return reinterpret_cast<CPhoneNumbersDocument*>(m_pDocument);
}
#endif