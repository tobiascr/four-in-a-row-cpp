#include <iostream>
#include "transposition_table.h"

namespace Engine
{
TranspositionTable::TranspositionTable()
{
    values = new uint64_t[size];
    clear();
}

TranspositionTable::~TranspositionTable()
{
    delete[] values;
}

void TranspositionTable::clear()
{
    for(int i=0; i < size; i++)
    {
        values[i] = 0;
    }
}
}
