#pragma once
#include <afxcview.h>
#include "CPersonDocument.h"

class CPersonView : public CListView
{
protected:
    CPersonView() noexcept;
    DECLARE_DYNCREATE(CPersonView)

public:
    virtual void OnInitialUpdate();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
    virtual void AssertValid() const;
    CPersonDocument* GetDocument() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
};

