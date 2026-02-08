#pragma once
#include <afxcview.h>
#include "Structures.h"           
#include "CPhoneNumbersDocument.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersView

/// <summary>
/// Основен View клас за визуализация на телефонния указател.
/// Наследява CListView за таблично представяне на данните.
/// Работи в тясно сътрудничество с CPhoneNumbersDocument (MVC pattern).
/// </summary>

/// <summary>
/// enum отговарящ на колона от PhoneNumbers ListView
/// </summary>
enum PhoneBookViewColumn {
    PhoneBookViewColumnNumber = 0,
    PhoneBookViewColumnFirstAndLastName,
    PhoneBookViewColumnPhoneType
};

class CPhoneNumbersView : public CListView
{
protected:
    /// <summary> Защитен конструктор (създава се само чрез Dynamic Create) </summary>
    CPhoneNumbersView() noexcept;
    DECLARE_DYNCREATE(CPhoneNumbersView)

    // --- Attributes ---
public:
    /// <summary> Връща указател към документа, свързан с този изглед </summary>
    CPhoneNumbersDocument* GetDocument() const;

private:
    /// <summary> Помощен масив (кеш) за резултатите от търсенето </summary>
    CPtrAutoArray<PHONE_NUMBERS> m_oAutoArray;

    /// <summary> Флаг, указващ дали в момента е активен филтър (търсене) </summary>
    BOOL m_bIsSearch = FALSE;

    /// <summary> Критерий за търсене по име </summary>
    CString m_strSearchName;
    /// <summary> Критерий за търсене по ЕГН </summary>
    CString m_strSearchUCN;
    /// <summary> Критерий за търсене по адрес </summary>
    CString m_strSearchAddress;


    // --- Operations (Internal Helpers) ---
private:
    /// <summary> Настройва стиловете на List Control-а (Grid lines, Full row select) </summary>
    void InitializeListCtrl();

    /// <summary> Дефинира колоните на таблицата (Име, Телефон, Тип) </summary>
    void SetColumns();

    /// <summary> Зарежда всички данни от документа в списъка при стартиране </summary>
    void SetInitialData();

    /// <summary> Намира индекса на ред в списъка по ID на записа </summary>
    /// <param name="lID"> ID на търсения телефонен номер </param>
    /// <returns> Индексът в CListCtrl или -1, ако не е намерен </returns>
    int GetIndexByID(const long lID) const;

    /// <summary> Задава текста на клетките за конкретен ред </summary>
    /// <param name="nIndex"> Индекс на реда </param>
    /// <param name="oPhoneNumber"> Данните за визуализация </param>
    void SetListViewItem(int nIndex, const PHONE_NUMBERS& oPhoneNumber);

    // --- CRUD Visualization Helpers ---

    /// <summary> Добавя нов ред в списъка (визуално отражение на Insert) </summary>
    void OperationInsert(const PHONE_NUMBERS* pPhone);

    /// <summary> Обновява съществуващ ред (визуално отражение на Update) </summary>
    void OperationUpdate(const PHONE_NUMBERS* pPhone);

    /// <summary> Премахва ред от списъка (визуално отражение на Delete) </summary>
    void OperationDelete(const long lID);

    /// <summary> Връща данни за текущо избрания ред </summary>
    const PHONE_NUMBERS* GetSelectedPhoneNumber() const;

    /// <summary> Проверява дали има селектиран ред </summary>
    BOOL IsSelectedRow() const;

    /// <summary> Филтрира списъка спрямо критериите за търсене </summary>
    void ShowSearch();


    // --- Overrides ---
public:
    /// <summary> Стандартно чертаене (рядко се ползва при CListView) </summary>
    virtual void OnDraw(CDC* pDC);

    /// <summary> Настройки преди създаване на прозореца </summary>
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    /// <summary> Първоначална инициализация (вика InitializeListCtrl) </summary>
    virtual void OnInitialUpdate();

    /// <summary> Основен метод за обновяване при промяна в Документа </summary>
    /// <param name="lHint"> Тип на операцията (Insert/Update/Delete/Load) </param>
    /// <param name="pHint"> Указател към променения обект </param>
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);


    // --- Implementation ---
public:
    virtual ~CPhoneNumbersView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif


    // --- Message Map Handlers ---
protected:
    /// <summary> Обработка на десен клик (подготовка за контекстно меню) </summary>
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

    /// <summary> Събитие при пускане на десен бутон на мишката </summary>
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

    /// <summary> Обработка на двоен ляв клик (Отваря режим View) </summary>
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

    /// <summary> Обработка на клавиатура (напр. Delete бутон) </summary>
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

    /// <summary> Обновява всички редове, свързани с даден човек (при промяна на името му) </summary>
    void UpdateRowsForPerson(const PERSONS* pPerson);

    // Команди от менюто
    /// <summary> Отваря диалог за добавяне на нов запис </summary>
    afx_msg void OnContextAdd();

    /// <summary> Отваря диалог за редакция на избрания запис </summary>
    afx_msg void OnContextEdit();

    /// <summary> Изтрива избрания запис (с потвърждение) </summary>
    afx_msg void OnContextDelete();

    /// <summary> Отваря диалог за преглед (Read-only) </summary>
    afx_msg void OnContextView();

    /// <summary> Отваря диалога за търсене </summary>
    afx_msg void OnContextSearch();

    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  
inline CPhoneNumbersDocument* CPhoneNumbersView::GetDocument() const
{
    return reinterpret_cast<CPhoneNumbersDocument*>(m_pDocument);
}
#endif