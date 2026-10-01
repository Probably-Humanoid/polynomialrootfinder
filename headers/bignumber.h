#pragma once

#include <vector>
#include <bitset>
#include <cmath>

using std::bitset;
using std::vector;

constexpr int bitsetSize {64};
constexpr int finalBit {bitsetSize-1};

using singleUnit = bitset<bitsetSize>;


constexpr singleUnit emptyBitset {0b0000000000000000000000000000000000000000000000000000000000000000};
const vector<singleUnit> emptyNumPart {vector<singleUnit>{emptyBitset}};




class BigNumber {
private:
    vector<singleUnit> m_wholePart{};
    vector<singleUnit> m_decimalPart{};
    bitset<8> m_extraInfo {};
    
    void shrinkAll();

    void expandWhole(const int& newSize);

    void expandDecimal(const int& newSize);

public:
    //BigNumber();
    BigNumber(vector<singleUnit> wholePart, vector<singleUnit> decimalPart = emptyNumPart, bool sign = false, bool isNaN = false, bool isInf = false);

    void doubleNum();

    void addNum(BigNumber& x);
};


void doubleToBigNum(const double& input);