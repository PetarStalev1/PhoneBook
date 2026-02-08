#pragma once
#include "CPhoneNumberAccessor.h"
#include "Structures.h"
#include "CSessionManager.h"
#include "CGenericTable.h"

/// <summary> Дефиниция на масив от указатели към PHONE_NUMBERS </summary>
typedef CTypedPtrArray<CPtrArray, PHONE_NUMBERS*>CPhoneNumbersArray;

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumberTable

/// <summary>
/// Клас-аксесор за таблица PHONE_NUMBERS в базата данни.
/// Наследява CGenericTable, чрез който получава базови CRUD функционалности,
/// и добавя специфични заявки за филтриране на телефони.
/// </summary>
class CPhoneNumberTable : public CGenericTable<CPhoneNumbersAccessor,PHONE_NUMBERS>
{
	// Constructor / Destructor
	// ----------------
public:
	/// <summary> Стандартен конструктор </summary>
	CPhoneNumberTable();

	/// <summary> Конструктор с подаване на активна сесия </summary>
	/// <param name="oSession"> Референция към отворена OLE DB сесия </param>
	CPhoneNumberTable(CSession& oSession);

	/// <summary> Деструктор </summary>
	virtual ~CPhoneNumberTable();

	/// <summary> 
	/// Извлича конкретен телефонен номер по неговото ID.
	/// </summary>
	/// <param name="lPhoneNumberID"> Първичен ключ на записа (PHONE_ID) </param>
	/// <returns> true при успех, false при грешка или липса на запис </returns>
	bool SelectFromPersonsWhereId(const long lPhoneNumberID);

	/// Извлича всички телефонни номера, свързани с конкретен човек.
	/// (Foreign Key търсене: PERSON_ID)
	/// </summary>
	/// <param name="lPersonID"> ID на човека, чиито телефони търсим </param>
	/// <param name="oResultArray"> Изходен масив, в който се добавят намерените записи </param>
	/// <returns> TRUE при успешно изпълнение на заявката </returns>
	BOOL SelectWherePersonID(const long lPersonID, CPtrAutoArray<PHONE_NUMBERS>& oResultArray);

};

