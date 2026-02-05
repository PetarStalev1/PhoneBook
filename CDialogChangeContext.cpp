#include "pch.h"
#include "CDialogChangeContext.h"

CDialogChangeContext::~CDialogChangeContext()
{
    POSITION pos = m_mapChanges.GetStartPosition();
    long lKey;
    ChangeEntry entry;

    while (pos != NULL)
    {
        m_mapChanges.GetNextAssoc(pos, lKey, entry);

        if (entry.pRecord != nullptr)
        {
            if (entry.eEntity == EntityPerson)
            {
                delete (PERSONS*)entry.pRecord;
            }
            else if (entry.eEntity == EntityCity)
            {
                delete (CITIES*)entry.pRecord;
            }
            else if (entry.eEntity == EntityPhone)
            {
                delete (PHONE_NUMBERS*)entry.pRecord;
            }
        }
    }

    m_mapChanges.RemoveAll();
}
