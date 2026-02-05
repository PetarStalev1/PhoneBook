#include "pch.h"
#include "PhoneBook.h"
#include "PersonsSearchDialog.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CPersonsSearchDialog, CDialogEx)


CPersonsSearchDialog::CPersonsSearchDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PERSONS_SEARCH, pParent)
{

}

CPersonsSearchDialog::~CPersonsSearchDialog()
{
}

void CPersonsSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_NAME_SEARCH, m_edbName);
	DDX_Control(pDX, IDC_EDB_SEARCH_UCN, m_edbUCN);
	DDX_Control(pDX, IDC_EDB_ADDRESS_SEARCH, m_edbAddress);
}


BEGIN_MESSAGE_MAP(CPersonsSearchDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPersonsSearchDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CPersonsSearchDialog message handlers


void CPersonsSearchDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_edbName.GetWindowTextW(m_strName);
	m_edbUCN.GetWindowTextW(m_strUCN);
	m_edbAddress.GetWindowTextW(m_strAddress);


	CDialogEx::OnOK();
}