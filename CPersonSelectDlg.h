#pragma once
#include <afxdialogex.h>
#include "CPhoneNumbersView.h"
#include "CDialogChangeContext.h"
typedef CMap<CString, LPCTSTR, long, long> CMapUCN;
class CPersonSelectDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CPersonSelectDlg)
public:
    CPersonSelectDlg(CPhoneNumbersView* pView, 
        CDialogChangeContext* pCtx, 
        CPtrArray* pCities, 
        CMapUCN* pUCNs,
        long lCurrentPersonID,
        CWnd* pParent = nullptr);
    virtual ~CPersonSelectDlg();
    void FillList();
    long m_lSelectedPersonID = 0;
private:
    CPtrArray* m_pCities;
    long m_lInitialID;
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PERSON_SELECT_DLG };
#endif

protected:
    CPhoneNumbersView* m_pView;
    CListCtrl m_lscPersons;
    CDialogChangeContext* m_pChangeCtx;
    CMapUCN* m_pUCNs;    
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnNMDblClkPersons(NMHDR* pNMHDR, LRESULT* pResult);
    void RefreshPersonList();
    void RegisterPersonChange(const PERSONS& personData, ChangeType changeType);
    void InsertPersonItem(int nIndex, const PERSONS* pPerson);
    afx_msg void OnNMCustomdrawPersons(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMRClickPersons(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnContextAdd();
    afx_msg void OnContextEdit();
    afx_msg void OnContextDelete();

    long GetSelectedPersonID();
    DECLARE_MESSAGE_MAP()

};

