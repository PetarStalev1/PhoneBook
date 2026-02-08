#include "pch.h"
#include "PhoneBook.h"
#include "PersonsSearchDialog.h"
#include "afxdialogex.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsSearchDialog


IMPLEMENT_DYNAMIC(CPersonsSearchDialog, CDialogEx)

// Constructor / Destructor
// ----------------
CPersonsSearchDialog::CPersonsSearchDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PERSONS_SEARCH, pParent)
{

}

CPersonsSearchDialog::~CPersonsSearchDialog()
{
}

// Overrides
// ----------------
void CPersonsSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_NAME_SEARCH, m_edbName);
	DDX_Control(pDX, IDC_EDB_SEARCH_UCN, m_edbUCN);
	DDX_Control(pDX, IDC_EDB_ADDRESS_SEARCH, m_edbAddress);
}


BEGIN_MESSAGE_MAP(CPersonsSearchDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPersonsSearchDialog::OnOK)
END_MESSAGE_MAP()



void CPersonsSearchDialog::OnOK()
{
	m_edbName.GetWindowTextW(m_strName);
	m_edbUCN.GetWindowTextW(m_strUCN);
	m_edbAddress.GetWindowTextW(m_strAddress);


	CDialogEx::OnOK();
}