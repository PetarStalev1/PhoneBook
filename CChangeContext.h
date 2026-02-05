#pragma once
#include <afxtempl.h>
#include "Structures.h"

enum ChangeType
{
    ChangeInsert,
    ChangeUpdate,
    ChangeDelete
};

enum EntityType
{
    EntityCity,
    EntityPerson,
    EntityPhone
};

struct ChangeEntry
{
    ChangeType eChange;
    EntityType eEntity;
    void* pRecord;   // CITIES*, PERSONS*, PHONE_NUMBERS*
};