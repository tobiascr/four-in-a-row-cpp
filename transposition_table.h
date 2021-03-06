#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

namespace Engine
{

class TranspositionTable
{
public:
    TranspositionTable();

    ~TranspositionTable();

    void clear();
    // Set every value in the table to zero.

    uint64_t* values;

    const int size = 50000000;
};
}

#endif
