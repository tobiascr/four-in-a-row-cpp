#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

namespace Engine
{

class TranspositionTable
{
public:
    TranspositionTable();

    ~TranspositionTable();

    uint64_t* values;

    const int size = 1000000;
};
}

#endif
