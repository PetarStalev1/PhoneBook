#pragma once
#include "Structures.h"
#include "CPhoneNumberTable.h"
#include "CPersonsTable.h"
#include "CPhoneTypesTable.h"
#include "CCitiesTable.h"
#include "CPhoneNumbersData.h"
#include "CDialogChangeContext.h"


/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersData

/// <summary>
/// Клас за управление на данните за телефонни номера (Data Access Object).
/// Осъществява връзка с базата данни, управлява сесиите и предоставя
/// методи за извличане, добавяне, редакция и изтриване на записи.
/// </summary>
class CPhoneNumbersData
{
	// Constructor / Destructor
	// ----------------
public:
	CPhoneNumbersData();
	~CPhoneNumbersData();

public:
	/// <summary> Извлича всички телефонни номера от базата данни </summary>
	/// <param name="oPhoneNumbersAutoArray"> Масив, в който ще бъдат записани резултатите </param>
	/// <returns> true при успех, false при грешка </returns>
	bool SelectAll(CPtrAutoArray<PHONE_NUMBERS>& oPhoneNumbersAutoArray);

	/// <summary> Извлича конкретен запис по ID </summary>
	/// <param name="lID"> Идентификатор на записа </param>
	/// <param name="recPhoneNumber"> Структура, в която ще се върнат данните </param>
	/// <returns> true ако записът е намерен, false ако не е </returns>
	bool SelectWhereID(const long lID, PHONE_NUMBERS& recPhoneNumber);

	/// <summary> Обновява съществуващ запис в базата данни </summary>
	/// <param name="lID"> Идентификатор на записа за редакция </param>
	/// <param name="recPhoneNumbers"> Новите данни </param>
	/// <returns> true при успешен Update, false при грешка </returns>
	bool UpdateWhereID(long lID,  PHONE_NUMBERS& recPhoneNumbers);

	/// <summary> Добавя нов телефонен номер в базата данни </summary>
	/// <param name="recPhoneNumber"> Данните за новия запис </param>
	/// <returns> true при успешен Insert, false при грешка </returns>
	bool InsertPhoneNumber(PHONE_NUMBERS& recPhoneNumber);

	/// <summary> Изтрива запис по ID </summary>
	/// <param name="lID"> Идентификатор на записа за триене </param>
	/// <returns> true при успешно изтриване, false при грешка </returns>
	bool DeleteWhereID(const long lID);

	// Advanced Operations
	// ----------------
public:
	/// <summary> 
	/// Прилага пакет от промени (транзакция), натрупани в диалогов контекст.
	/// Използва се за запазване на промените от CPhoneNumberDlg/CPersonSelectDlg.
	/// </summary>
	/// <param name="ctx"> Контекстът, съдържащ списък с Insert/Update/Delete операции </param>
	/// <returns> true при успешно прилагане на всички промени </returns>
	bool ApplyChanges(CDialogChangeContext& ctx);

	/// <summary> Търсене на телефони по разширени критерии (Име, ЕГН, Адрес) </summary>
	/// <param name="oAutoArray"> Изходен масив за резултатите </param>
	/// <param name="strName"> Търсене по име/презиме/фамилия </param>
	/// <param name="strUCN"> Търсене по ЕГН </param>
	/// <param name="strAddress"> Търсене по адрес </param>
	/// <returns> TRUE при успешно изпълнение на заявката </returns>
	BOOL SelectByNameUCNAddress(CPtrAutoArray<PHONE_NUMBERS>& oAutoArray,
		const CString& strName,
		const CString& strUCN,
		const CString& strAddress);
	
	// Data Accessors (Tables)
		// ----------------
public:
	/// <summary> Аксесор за таблица PHONE_NUMBERS </summary>
	CPhoneNumberTable m_oPhoneNumbersTable;
	/// <summary> Аксесор за таблица PERSONS </summary>
	CPersonsTable m_oPersonsTable;
	/// <summary> Аксесор за таблица PHONE_TYPES </summary>
	CPhoneTypesTable m_oPhoneTypesTable;
	/// <summary> Аксесор за таблица CITIES </summary>
	CCitiesTable m_oCitiesTable;


	// Database Session
	// ----------------
private:
	/// <summary> Сесия към базата данни (OLE DB) </summary>
	CSession* m_pSession = nullptr;
};

