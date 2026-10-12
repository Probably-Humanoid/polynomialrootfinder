#pragma once

#include <vector>
#include <bitset>
#include <cmath>

//using std::bitset;
//using std::vector;

constexpr int bitsetSize {64};
constexpr int finalBit {bitsetSize-1};

using singleUnit = std::bitset<bitsetSize>;
using numStoreType = std::vector<singleUnit>;


constexpr singleUnit emptyBitset {0x0000000000000000};
constexpr singleUnit bitsetVal1 {0x0000000000000001};
constexpr singleUnit bitsetValmin1 {0x8000000000000000};
const numStoreType emptyNumPart {numStoreType(0)};

const double doubleNaN {0.0/0.0};




class BigNumber {
private:
    numStoreType m_wholePart{};
    numStoreType m_decimalPart{};
    std::bitset<8> m_extraInfo {};

    void shrinkAll();

    void matchSizes(BigNumber& x);

    void twoCompliment();

    bool checkIfZero();

public:
    //BigNumber();
    BigNumber(numStoreType wholePart = emptyNumPart, numStoreType decimalPart = emptyNumPart, bool sign = false, bool isNaN = false, bool isInf = false);

    void doubleNum();

    void increment();

    void addNum(BigNumber& x);

    
};


void doubleToBigNum(const double& input);