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


constexpr singleUnit emptyBitset {0b0000000000000000000000000000000000000000000000000000000000000000};
constexpr singleUnit bitsetVal1 {0b00000000000000000000000000000000000000000000000000000000000000001};
const numStoreType emptyNumPart {numStoreType{emptyBitset}};

const double doubleNaN {0.0/0.0};




class BigNumber {
private:
    numStoreType m_wholePart{};
    numStoreType m_decimalPart{};
    std::bitset<8> m_extraInfo {};
    
    void shrinkAll();

    void expandWhole(const int& newSize);

    void expandDecimal(const int& newSize);

public:
    //BigNumber();
    BigNumber(numStoreType wholePart = emptyNumPart, numStoreType decimalPart = emptyNumPart, bool sign = false, bool isNaN = false, bool isInf = false);

    void doubleNum();

    void addNum(BigNumber& x);
};


void doubleToBigNum(const double& input);