#pragma once
#include "afxdialogex.h"


// AddPersonDialog dialog

class AddPersonDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddPersonDialog)

public:
	AddPersonDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddPersonDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
