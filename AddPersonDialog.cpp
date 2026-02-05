// AddPersonDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "afxdialogex.h"
#include "AddPersonDialog.h"


// AddPersonDialog dialog

IMPLEMENT_DYNAMIC(AddPersonDialog, CDialogEx)

AddPersonDialog::AddPersonDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

AddPersonDialog::~AddPersonDialog()
{
}

void AddPersonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddPersonDialog, CDialogEx)
END_MESSAGE_MAP()


// AddPersonDialog message handlers
