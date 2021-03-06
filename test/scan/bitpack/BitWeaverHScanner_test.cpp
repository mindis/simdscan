//
// Created by Harper on 10/5/17.
//

#include <gtest/gtest.h>
#include <stdlib.h>
#include "../../../src/util/encode.h"
#include "../../../src/predicate/Predicate.h"
#include "../../../src/scan/bitpack/BitWeaverHScanner128.h"
#include "../../../src/scan/bitpack/BitWeaverHScanner256.h"
#include "../../../src/scan/bitpack/BitWeaverHScanner512.h"

TEST(BitWeaverHScanner128, TestScan) {
    int entrySize = 9;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                  18, 19, 20};
    int *encoded = (int *) aligned_alloc(16, 64 * sizeof(int));
    int *output = (int *) aligned_alloc(16, 64 * sizeof(int));
    bitweaverh_encode(data, encoded, 20, entrySize);

    BitWeaverHScanner128 *scanner = new BitWeaverHScanner128(entrySize);

    Predicate p(opr_eq, 18, 0);

    int numEntryInWord = 64 / (entrySize + 1);


    long *outputLong = (long *) output;

    scanner->scan(encoded, 20, output, &p);
    for (int i = 0; i < 20; i++) {
        int longIndex = i / numEntryInWord;
        int longOffset = (i % numEntryInWord) * (entrySize + 1) + entrySize;

        if (i == 17) {
            EXPECT_EQ(0, outputLong[longIndex] & (1L << longOffset)) << i;
        } else {
            EXPECT_EQ(1L << longOffset, outputLong[longIndex] & (1L << longOffset)) << i;
        }
    }
    free(encoded);
    free(output);
}

TEST(BitWeaverHScanner256, TestScan) {
    int entrySize = 9;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                  18, 19, 20};
    int *encoded = (int *) aligned_alloc(32, 64 * sizeof(int));
    int *output = (int *) aligned_alloc(32, 64 * sizeof(int));
    bitweaverh_encode(data, encoded, 20, entrySize);

    Scanner *scanner = new BitWeaverHScanner256(entrySize);

    Predicate p(opr_eq, 18, 0);

    int numEntryInWord = 64 / (entrySize + 1);


    long *outputLong = (long *) output;

    scanner->scan(encoded, 20, output, &p);
    for (int i = 0; i < 20; i++) {
        int longIndex = i / numEntryInWord;
        int longOffset = (i % numEntryInWord) * (entrySize + 1)+entrySize;

        if (i == 17) {
            EXPECT_EQ(0, outputLong[longIndex] & (1L << longOffset)) << i;
        } else {
            EXPECT_EQ(1L << longOffset, outputLong[longIndex] & (1L << longOffset)) << i;
        }
    }
    free(encoded);
    free(output);
}

TEST(BitWeaverHScanner512, TestScan) {
    int entrySize = 9;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                  18, 19, 20};
    int *encoded = (int *) aligned_alloc(64, 64 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 64 * sizeof(int));
    bitweaverh_encode(data, encoded, 20, entrySize);

    Scanner *scanner = new BitWeaverHScanner512(entrySize);

    Predicate p(opr_eq, 18, 0);

    int numEntryInWord = 64 / (entrySize + 1);


    long *outputLong = (long *) output;

    scanner->scan(encoded, 20, output, &p);
    for (int i = 0; i < 20; i++) {
        int longIndex = i / numEntryInWord;
        int longOffset = (i % numEntryInWord) * (entrySize + 1)+entrySize;

        if (i == 17) {
            EXPECT_EQ(0, outputLong[longIndex] & (1L << longOffset)) << i;
        } else {
            EXPECT_EQ(1L << longOffset, outputLong[longIndex] & (1L << longOffset)) << i;
        }
    }
    free(encoded);
    free(output);
}