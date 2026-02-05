#include "pch.h"
#include "CPhoneTypesData.h"

CPhoneTypesData::CPhoneTypesData()
{

}

CPhoneTypesData::~CPhoneTypesData()
{

}
BOOL CPhoneTypesData::SelectAll(CPtrAutoArray<PHONE_TYPES>& oPhoneTypesAutoArray)
{
	
	return m_oTable.SelectAll(oPhoneTypesAutoArray);

}
BOOL CPhoneTypesData::InsertPhoneType(PHONE_TYPES& oPhoneType)
{

	return TRUE;

}
BOOL CPhoneTypesData::DeletePhoneType(long lID)
{

	
	return TRUE;
}
BOOL CPhoneTypesData::UpdatePhoneType(PHONE_TYPES& oPhoneType)
{

	return TRUE;

}
bool CPhoneTypesData::GetPhoneTypeById(long nPhoneTypeId, PHONE_TYPES& oPhoneType) const
{
	/*int nawdaw = m_oPhoneTypeArray.GetSize();
	for (int i = 0; i < m_oPhoneTypeArray.GetSize(); ++i)
	{
		if (m_oPhoneTypeArray[i]->m_nID == nPhoneTypeId)
		{
			oPhoneType = *m_oPhoneTypeArray[i];
			return true;
		}
	}*/

	return false;
}
