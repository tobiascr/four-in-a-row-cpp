#include <iostream>
#include "transposition_table.h"

namespace Engine
{
    TranspositionTable::TranspositionTable()
    {
        values = new uint64_t[size];
        for(int i=0; i < size; i++)
        {
            values[i] = 0;
        }
    }

    TranspositionTable::~TranspositionTable()
    {
        delete[] values;
    }
}
