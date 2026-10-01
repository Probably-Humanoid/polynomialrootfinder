#include "bignumber.h"

#include <vector>
#include <bitset>
#include <cmath>


using std::bitset;
using std::vector;

//constexpr int bitsetSize {64};
//constexpr int finalBit {bitsetSize-1};

using singleUnit = bitset<bitsetSize>;

//constexpr singleUnit emptyBitset {0b0000000000000000000000000000000000000000000000000000000000000000};
constexpr singleUnit bitsetVal1 {0b00000000000000000000000000000000000000000000000000000000000000001};
//const vector<singleUnit> emptyNumPart {vector<singleUnit>{emptyBitset}};
const double doubleNaN {0.0/0.0};


static bool shiftAndCarry(const bool& oldCarry,singleUnit& setToShift)
{
    const bool carryNext {setToShift.test(finalBit)};

    setToShift <<= 1;
    if (oldCarry) setToShift.set(0);


    return carryNext;
}

static void addNums(singleUnit& x,const singleUnit& y,bool& returnCarry)
{



    // first operation
    singleUnit carry {x & y};
    x ^= y;


    if (returnCarry)
        {
            returnCarry = (carry.test(finalBit));
            carry <<= 1;
            carry.set(0);
        }
        


    returnCarry = (carry.test(finalBit));
    carry <<= 1;

    
    
    singleUnit tempCarry;
    if (carry.any())
    {
        while (carry.any())
        {
            tempCarry = {carry & x};
            x ^= carry;
            if (carry.test(finalBit))
                returnCarry = true;
            carry = tempCarry << 1;

        }
    }
}

void BigNumber::shrinkAll()
    {
        const int wholeSize = m_wholePart.size();
        const int decimalSize = m_decimalPart.size();
        for (int i=decimalSize-1; i>=0; i--)
        {
            if (m_decimalPart[i].none())
                m_decimalPart.pop_back();
            else
                break;
        }

        for (int i=wholeSize-1; i>=0; i--)
        {
            if (m_wholePart[i].none())
                m_wholePart.pop_back();
            else
                break;
        }
    }

void BigNumber::expandWhole(const int& newSize) // no checks to see if bigger than current size, leave that up to whoever's calling the function
    {/*
        const int repeatTimes = {newSize - (m_wholePart.size())};
        for (int i=0;i<repeatTimes;i++)
        {
            m_wholePart.push_back(emptyBitset);
        }*/
    }

void BigNumber::expandDecimal(const int& newSize)
    {
        /*
        const int repeatTimes {newSize - m_decimalPart.size()};
        for (int i=0;i<repeatTimes;i++)
        {
            m_decimalPart.push_back(emptyBitset);
        }*/
    }

//BigNumber::BigNumber() = default;
BigNumber::BigNumber(vector<singleUnit> wholePart, vector<singleUnit> decimalPart, bool sign, bool isNaN, bool isInf)
: m_wholePart {wholePart}
, m_decimalPart {decimalPart}
, m_extraInfo {0b00000000}
{
    if (sign) m_extraInfo.set(0);
    if (isNaN) m_extraInfo.set(1);
    if (isInf) m_extraInfo.set(2);
}

void BigNumber::doubleNum()
{
        const int wholeSize = m_wholePart.size();
        const int decimalSize = m_decimalPart.size();
        bool doCarry {false};

        for (int i=decimalSize-1; i>=0; i--)
        {
            doCarry = shiftAndCarry(doCarry,m_decimalPart[i]);
        }

        for (int i=0; i<wholeSize; i++)
        {
            doCarry = shiftAndCarry(doCarry,m_wholePart[i]);
        }
        if (doCarry) m_wholePart.push_back(bitsetVal1);

    }

void BigNumber::addNum(BigNumber& x)
    {
        // 3 cases - new decimal > old decimal -- create extra parts of old decimal, then add normally from the first part
        // equal -- add normally
        // smaller -- add normally after the first one
        bool carry;
        const int wholeSize = m_wholePart.size();
        const int decimalSize = m_decimalPart.size();
        const int wholeSizeNew = x.m_wholePart.size();
        const int decimalSizeNew = x.m_decimalPart.size();
        

        if (decimalSizeNew > decimalSize) // case 1
        {
            for (int i = decimalSize;i < decimalSizeNew;i++)
            {
                m_decimalPart.push_back(x.m_decimalPart[i]);
            }
        }

        const int& timesToLoopDecimalAdd {&decimalSizeNew > &decimalSize ? decimalSize : decimalSizeNew};

        
        for (int i=timesToLoopDecimalAdd-1; i>=0; i--)
        {
            addNums(m_decimalPart[i],x.m_decimalPart[i],carry);
        }

        ///////////////
        if (wholeSizeNew > wholeSize) // case 1
        {
            for (int i = wholeSize;i < wholeSizeNew;i++)
            {
                m_wholePart.push_back(x.m_wholePart[i]);
            }
        }

        const int& timesToLoopWholeAdd {&wholeSizeNew > &wholeSize ? wholeSize : wholeSizeNew};

        
        for (int i=0; i<timesToLoopWholeAdd; i++)
        {
            addNums(m_wholePart[i],x.m_wholePart[i],carry);
        }
        if (carry) m_wholePart.push_back(bitsetVal1);
        shrinkAll();
    }


constexpr double biggestDecimalDouble = 4503599627370496;
constexpr double smallestDecimalDouble = -4503599627370496;


//should return bignum but not coded yet
void doubleToBigNum(const double& input)
{
    /*
    double intPart;
    double decimalPart;

    if (input >= biggestDecimalDouble || input <= smallestDecimalDouble)
    {
        intPart = input;
    }
    else
    {
        intPart = static_cast<int64_t> (input);
        decimalPart = input - intPart;
    }


    double intPart = static_cast<int64_t> (input);
    double decimalPart = input - intPart;
    */

}