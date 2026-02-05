#pragma once
#include "CChangeContext.h"
class CDialogChangeContext
{
public:
    CDialogChangeContext()
        : m_lNextTempID(-1)
    {
    }
    ~CDialogChangeContext();
    long GenerateTempID()
    {
        return m_lNextTempID--;
    }

    // Key = TempID или RealID
    CMap<long, long, ChangeEntry, ChangeEntry&> m_mapChanges;

private:
    long m_lNextTempID;
};