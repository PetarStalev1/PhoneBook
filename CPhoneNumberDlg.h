#pragma once
#include <afxdialogex.h>
#include "Structures.h"
#include "resource.h"
#include "PhoneNumberDialogType.h"
#include "CitiesListViewActions.h"
#include "CPhoneNumbersView.h"
#include "CDialogChangeContext.h"

typedef CMap<CString, LPCTSTR, long, long> CMapUCN;


class CPhoneNumberDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPhoneNumberDlg)
private:
    const PhoneNumberDialogType m_ePhoneNumberDialogType;
    CArray<long, long> m_deletedIDs;
    CDialogChangeContext* m_pChangeCtx;
    PERSONS m_recOriginalPerson;
    PHONE_NUMBERS m_recOriginalPhone;
public:
    CitiesListViewActions m_eCitiesListViewAction;
    CPhoneNumberDlg(PHONE_NUMBERS* pPhoneNumber, 
                    const PhoneNumberDialogType ePhoneNumberDialogType, 
                    CDialogChangeContext* pCtx,
                    CWnd* pParent = nullptr);
    PHONE_NUMBERS m_recPhoneNumber;
    PERSONS       m_recPerson;

    virtual ~CPhoneNumberDlg();
    virtual void OnOK();    
    BOOL SetDialogName();

    PHONE_NUMBERS m_oPhoneNumber;
    PHONE_NUMBERS& GetPhoneNumber() { return m_recPhoneNumber; }    
    const PHONE_NUMBERS* GetPhoneNumberPtr() const { return &m_recPhoneNumber; }
    static int CALLBACK CompareCities_MainDlg(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    CDialogChangeContext* GetChangeContext() const { return m_pChangeCtx; }
    PERSONS& GetPerson() { return m_recPerson; }
    BOOL SetDialogFields();
    BOOL ValidateUserInput() const;
    void SelectCityInListByID(long lID);
    void MakeEditLookLikeLabel(CEdit& ed);
    void SetPhoneTypesComboBox();
    void SetInitialData();
    afx_msg void OnPopupView();
    afx_msg LRESULT OnSelectCity(WPARAM, LPARAM);
    afx_msg void OnNMRClickCities(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedViewPersons();
    afx_msg void OnNMCustomdrawCities(NMHDR* pNMHDR, LRESULT* pResult);
    void OnContextAddCity();
    void OnContextEditCity();
    void OnContextDeleteCity();
    CPhoneNumbersView* getPNView();
    BOOL SetItemText(const CITIES& recCities, const int nIndexList);

    CString CityToString(const CITIES& recCity) const;
    void SetColumns();
    long GetActiveCityID() const;
    void ResortCities();
private:
    void SetReadOnly();
    void GetMergedUCNs(CMapUCN& mapUCNs);
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PHONE_NUMBERS_ONEDIT };
#endif    

protected:
    int m_nPhoneNumberId;
    CString m_sPhoneNumber;
    void GetMergedCities(CPtrArray& arrCities);
    virtual BOOL OnInitDialog();
    bool HasCityChanged(const CITIES& oldCity, const CITIES& newCity) const;
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()
private:
    long m_lTemporaryID;
    int m_nCityToSelect = -1;
public:
    CEdit m_edbPhoneNumber;
    CEdit m_edbFirstName;
    CEdit m_edbMiddleName;
    CEdit m_edbLastName;
    CEdit m_edbUCN;
    CListCtrl m_lscCity;
    CEdit m_edbAddress;
    CComboBox m_cmbPhoneNumbersTypes;
    CButton m_btnViewPersons;
};

