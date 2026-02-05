#pragma once
#include "afxdialogex.h"
#include "Structures.h"
#include "PhoneNumberDialogType.h"
#include "CPhoneNumbersView.h"
#include "CPersonDialogType.h"
#include "InputValidator.h"

// CPersonDlg dialog
typedef CMap<CString, LPCTSTR, long, long> CMapUCN;
class CPersonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonDlg)

public:
	CPersonDlg(PERSONS* pPerson, CPtrArray* pCities, const CMapUCN* pUsedUCNs, PersonDialogType eType, CWnd* pParent = nullptr);
	virtual ~CPersonDlg();
	PERSONS* GetPerson() { return &m_recPerson; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PERSON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	BOOL PreTranslateMessage(MSG* pMsg);
	CInputValidator m_validator;
	PERSONS m_recPerson;       
	PersonDialogType m_eType;
	const CPtrArray* m_oCitiesArray;
	const CMapUCN* m_pUsedUCNs;
	DECLARE_MESSAGE_MAP()

public:
	  CEdit m_edFirstName;
	  CEdit m_edMiddleName;
	  CEdit m_edLastName;
	  CEdit m_edUCN;
	  CEdit m_edAddress;
	  CComboBox m_cmbCity;
};
