#pragma once

#include <afxwin.h>
#include "CPhoneTypesData.h"

class CPhoneTypesDocument : public CDocument
{
protected:
    CPhoneTypesDocument() noexcept;

public:
    enum class UpdateHints
    {
        Insert = 0,
        Delete = 1,
        Update = 2
    };

public:
    CPhoneTypesData m_oPhoneTypesData;

public:
    const CPhoneTypesData& GetPhoneTypesData() const { return m_oPhoneTypesData; }
    bool SelectPhoneTypeById(PHONE_TYPES& oPhoneType);
    bool InsertPhoneType(PHONE_TYPES& oPhoneType);
    bool DeletePhoneType(PHONE_TYPES& oPhoneType);
    bool UpdatePhoneType(PHONE_TYPES& oPhoneType);
    void ReloadPhoneTypes();

public:
    virtual BOOL OnNewDocument() override;
    virtual void Serialize(CArchive& ar) override;

#ifdef _DEBUG
    virtual void AssertValid() const override;
    virtual void Dump(CDumpContext& dc) const override;
#endif
};
