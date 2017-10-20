#include "maCursorKeys.h"

CursorKeys * CursorKeys::m_Focus = NULL;

CursorKeys::CursorKeys():
    m_EntryMode(em_normal)
{
    //ctor
}

CursorKeys::~CursorKeys()
{
    //dtor
}

