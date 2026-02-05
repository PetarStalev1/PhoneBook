//#include "CPhonebookDocument.h"
//#include <afxwin.h>
//#include "Structures.h"
//#include "CCitiesData.h"
//#include "CPhoneNumberDisplay.h"
//
//class CitiesDoc : public CPhonebookDocument<CITIES> {
//public:
//	CitiesDoc() noexcept;
//	DECLARE_DYNCREATE(CitiesDoc);
//	virtual BOOL OnNewDocument() override;
//	virtual void Serialize(CArchive& ar);
//private:
//	CCitiesData oCitiesData;
//	CPhoneNumberDisplay* GetDisplayInformation();
//	BOOL InsertCity(const CITIES& recCity);
//
//#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
//#endif
//protected:
//	DECLARE_MESSAGE_MAP()
//};