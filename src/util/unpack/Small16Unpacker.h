//
// Created by harper on 3/15/18.
//

#ifndef SIMDSCAN_SMALL16UNPACKER_H
#define SIMDSCAN_SMALL16UNPACKER_H


#include <assert.h>
#include "Unpacker.h"

class Small16Unpacker : public Unpacker {
private:
    int entrySize;
    __m256i* shuffleInst;
    __m256i* shiftInst;
    __m256i mask;
public:
    Small16Unpacker(int es);

    virtual ~Small16Unpacker();

    __m256i unpack(uint8_t *data, uint8_t offset);
};


#endif //SIMDSCAN_SMALL16UNPACKER_H
