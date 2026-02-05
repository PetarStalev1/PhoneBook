#include "pch.h"
#include "framework.h"

#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

#include "CPhoneTypesDocument.h"
#include "CPhoneTypesData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPhoneTypesDocument::CPhoneTypesDocument() noexcept
{
}

BOOL CPhoneTypesDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    /*if (!m_oPhoneTypesData.SelectAll())
    {
        AfxMessageBox(_T("Неуспешно зареждане на типове телефони."));
        return FALSE;
    }*/

    return TRUE;
}

void CPhoneTypesDocument::ReloadPhoneTypes()
{
    //m_oPhoneTypesData.SelectAll();
    UpdateAllViews(nullptr);
}

void CPhoneTypesDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

#ifdef _DEBUG
void CPhoneTypesDocument::AssertValid() const
{
    CDocument::AssertValid();
}

void CPhoneTypesDocument::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

// Избор по ID
bool CPhoneTypesDocument::SelectPhoneTypeById(PHONE_TYPES& oPhoneType)
{
    return false;
}
bool CPhoneTypesDocument::InsertPhoneType(PHONE_TYPES& oPhoneType)
{
    if (!m_oPhoneTypesData.InsertPhoneType(oPhoneType))
    {
        AfxMessageBox(_T("Грешка при добавяне на тип телефон."));
        return false;
    }

    UpdateAllViews(nullptr, static_cast<LPARAM>(UpdateHints::Insert), (CObject*)&oPhoneType);
    return true;
}

bool CPhoneTypesDocument::DeletePhoneType(PHONE_TYPES& oPhoneType)
{
    if (!m_oPhoneTypesData.DeletePhoneType(oPhoneType.lID))
    {
        AfxMessageBox(_T("Грешка при изтриване на тип телефон."));
        return false;
    }

    UpdateAllViews(nullptr, static_cast<LPARAM>(UpdateHints::Delete), (CObject*)&oPhoneType);
    return true;
}

bool CPhoneTypesDocument::UpdatePhoneType(PHONE_TYPES& oPhoneType)
{
    if (!m_oPhoneTypesData.UpdatePhoneType(oPhoneType))
    {
        AfxMessageBox(_T("Грешка при обновяване на тип телефон."));
        return false;
    }

    UpdateAllViews(nullptr, static_cast<LPARAM>(UpdateHints::Update), (CObject*)&oPhoneType);
    return true;
}
