//
// Created by harper on 3/13/18.
//
#include <gtest/gtest.h>
#include "../../../src/util/encode.h"
#include "../../../src/predicate/Predicate.h"
#include "../../../src/scan/rle/TrivialRLEScanner.h"
#include "../../../src/scan/rle/SimdRLEScanner.h"

TEST(SimdRLEScanner, TestLessAligned) {
    int entrySize = 9;
    int rlSize = 4;
    int data[] = {2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5};

    int *encoded = (int *) aligned_alloc(64, 400 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 400 * sizeof(int));
    int numPair = encode_rle(data, encoded, 168, entrySize, rlSize);

    SimdRLEScanner *scanner = new SimdRLEScanner(entrySize, rlSize, true);

    Predicate p(opr_less, 5, 0);

    scanner->scan(encoded, numPair, output, &p);

    int expected[] = {2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3};

    for (int i = 0; i < numPair; i++) {
        int bitoff = i * (entrySize + rlSize) + entrySize - 1;
        int intidx = bitoff / 32;
        int intoff = bitoff % 32;
        if (expected[i * 2] < 5) {
            EXPECT_FALSE(output[intidx] & (1 << intoff)) << "Compare " << i;
        } else {
            EXPECT_TRUE(output[intidx] & (1 << intoff)) << "Compare " << i;
        }
        int extractedRl = extract_entry(output, (bitoff + 1) / 32, (bitoff + 1) % 32, rlSize);
        EXPECT_EQ(expected[i * 2 + 1], extractedRl) << "Rl " << i;
    }

    free(encoded);
    free(output);
}

TEST(SimdRLEScanner, TestLessUnalignedNormal) {
    int entrySize = 9;
    int rlSize = 4;
    int data[] = {2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5};

    int *encoded = (int *) aligned_alloc(64, 400 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 400 * sizeof(int));
    int numPair = encode_rle(data, encoded, 168, entrySize, rlSize);

    SimdRLEScanner *scanner = new SimdRLEScanner(entrySize, rlSize, false);

    Predicate p(opr_less, 5, 0);

    scanner->scan(encoded, numPair, output, &p);

    int expected[] = {2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3};

    for (int i = 0; i < numPair; i++) {
        int bitoff = i * (entrySize + rlSize) + entrySize - 1;
        int intidx = bitoff / 32;
        int intoff = bitoff % 32;
        if (expected[i * 2] < 5) {
            EXPECT_FALSE(output[intidx] & (1 << intoff)) << "Compare " << i;
        } else {
            EXPECT_TRUE(output[intidx] & (1 << intoff)) << "Compare " << i;
        }
        int extractedRl = extract_entry(output, (bitoff + 1) / 32, (bitoff + 1) % 32, rlSize);
        EXPECT_EQ(expected[i * 2 + 1], extractedRl) << "Rl " << i;
    }

    free(encoded);
    free(output);
}

TEST(SimdRLEScanner, TestLessUnalignedFast) {
    int entrySize = 9;
    int rlSize = 9;
    int data[] = {2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5};

    int *encoded = (int *) aligned_alloc(64, 500 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 500 * sizeof(int));
    int numPair = encode_rle(data, encoded, 168, entrySize, rlSize);

    SimdRLEScanner *scanner = new SimdRLEScanner(entrySize, rlSize, false);

    Predicate p(opr_less, 5, 0);

    scanner->scan(encoded, numPair, output, &p);

    int expected[] = {2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3};

    for (int i = 0; i < numPair; i++) {
        int bitoff = i * (entrySize + rlSize) + entrySize - 1;
        int intidx = bitoff / 32;
        int intoff = bitoff % 32;
        if (expected[i * 2] < 5) {
            EXPECT_FALSE(output[intidx] & (1 << intoff)) << "Compare " << i;
        } else {
            EXPECT_TRUE(output[intidx] & (1 << intoff)) << "Compare " << i;
        }
        int extractedRl = extract_entry(output, (bitoff + 1) / 32, (bitoff + 1) % 32, rlSize);
        EXPECT_EQ(expected[i * 2 + 1], extractedRl) << "Rl " << i;
    }

    free(encoded);
    free(output);
}


TEST(SimdRLEScanner, TestEqAligned) {
    int entrySize = 9;
    int rlSize = 4;
    int data[] = {2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5};

    int *encoded = (int *) aligned_alloc(64, 400 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 400 * sizeof(int));
    int numPair = encode_rle(data, encoded, 168, entrySize, rlSize);

    SimdRLEScanner *scanner = new SimdRLEScanner(entrySize, rlSize, true);

    Predicate p(opr_eq, 5, 0);

    scanner->scan(encoded, numPair, output, &p);

    int expected[] = {2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3};

    for (int i = 0; i < numPair; i++) {
        int bitoff = i * (entrySize + rlSize) + entrySize - 1;
        int intidx = bitoff / 32;
        int intoff = bitoff % 32;
        if (expected[i * 2] == 5) {
            EXPECT_FALSE(output[intidx] & (1 << intoff)) << "Compare " << i;
        } else {
            EXPECT_TRUE(output[intidx] & (1 << intoff)) << "Compare " << i;
        }
        int extractedRl = extract_entry(output, (bitoff + 1) / 32, (bitoff + 1) % 32, rlSize);
        EXPECT_EQ(expected[i * 2 + 1], extractedRl) << "Rl " << i;
    }

    free(encoded);
    free(output);
}

TEST(SimdRLEScanner, TestEqUnalignedNormal) {
    int entrySize = 28;
    int rlSize = 5;
    int data[] = {2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5};

    int *encoded = (int *) aligned_alloc(64, 400 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 400 * sizeof(int));
    int numPair = encode_rle(data, encoded, 168, entrySize, rlSize);

    SimdRLEScanner *scanner = new SimdRLEScanner(entrySize, rlSize, false);

    Predicate p(opr_eq, 5, 0);

    scanner->scan(encoded, numPair, output, &p);

    int expected[] = {2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3};

    for (int i = 0; i < numPair; i++) {
        int bitoff = i * (entrySize + rlSize) + entrySize - 1;
        int intidx = bitoff / 32;
        int intoff = bitoff % 32;
        if (expected[i * 2] == 5) {
            EXPECT_FALSE(output[intidx] & (1 << intoff)) << "Compare " << i;
        } else {
            EXPECT_TRUE(output[intidx] & (1 << intoff)) << "Compare " << i;
        }
        int extractedRl = extract_entry(output, (bitoff + 1) / 32, (bitoff + 1) % 32, rlSize);
        EXPECT_EQ(expected[i * 2 + 1], extractedRl) << "Rl " << i;
    }

    free(encoded);
    free(output);
}

TEST(SimdRLEScanner, TestEqUnalignedFast) {
    int entrySize = 24;
    int rlSize = 20;
    int data[] = {2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5,
                  2, 2, 3, 7, 7, 8, 9, 1, 2, 2, 4, 5, 29, 11,
                  6, 8, 22, 12, 12, 21, 21, 21, 4, 4, 4, 5, 5, 5};

    int *encoded = (int *) aligned_alloc(64, 400 * sizeof(int));
    int *output = (int *) aligned_alloc(64, 400 * sizeof(int));
    int numPair = encode_rle(data, encoded, 168, entrySize, rlSize);

    SimdRLEScanner *scanner = new SimdRLEScanner(entrySize, rlSize, false);

    Predicate p(opr_eq, 5, 0);

    scanner->scan(encoded, numPair, output, &p);

    int expected[] = {2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3,
                      2, 2, 3, 1, 7, 2, 8, 1, 9, 1, 1, 1, 2, 2, 4, 1, 5, 1,
                      29, 1, 11, 1, 6, 1, 8, 1, 22, 1, 12, 2, 21, 3, 4, 3, 5, 3};

    for (int i = 0; i < numPair; i++) {
        int bitoff = i * (entrySize + rlSize) + entrySize - 1;
        int intidx = bitoff / 32;
        int intoff = bitoff % 32;
        if (expected[i * 2] == 5) {
            EXPECT_FALSE(output[intidx] & (1 << intoff)) << "Compare " << i;
        } else {
            EXPECT_TRUE(output[intidx] & (1 << intoff)) << "Compare " << i;
        }
        int extractedRl = extract_entry(output, (bitoff + 1) / 32, (bitoff + 1) % 32, rlSize);
        EXPECT_EQ(expected[i * 2 + 1], extractedRl) << "Rl " << i;
    }

    free(encoded);
    free(output);
}