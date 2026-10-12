#include "bignumber.h"

#include <vector>
#include <bitset>
#include <cmath>

// do subtraction and negative number


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
    else
    {
        returnCarry = (carry.test(finalBit));
        carry <<= 1;
    }


    

    
    
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
        for (int i=(m_decimalPart.size())-1; i>=0; i--)
        {
            if (m_decimalPart[i].none()) {
                m_decimalPart.pop_back();
        }
            else
                break;
        }

        for (int i=(m_wholePart.size())-1; i>=0; i--)
        {
            if (m_wholePart[i].none()) {
                m_wholePart.pop_back();
        }
            else
                break;
        }
}

void BigNumber::matchSizes(BigNumber& x) // also adds an extra segment to the front
{
    //idfk
    numStoreType* smallerDecimalPart;
    int decimalPartTargetSize;
    if (m_decimalPart.size() > x.m_decimalPart.size())
    {
        smallerDecimalPart = &x.m_decimalPart;
        decimalPartTargetSize = m_decimalPart.size();
    }
    else
    {
        smallerDecimalPart = &m_decimalPart;
        decimalPartTargetSize = x.m_decimalPart.size();
    }
    
    for (int i = (*smallerDecimalPart).size();i<decimalPartTargetSize;i++)
    {
        (*smallerDecimalPart).push_back(emptyBitset);
    }
    /////////////////////////
    numStoreType* smallerWholePart;
    int wholePartTargetSize;
    if (m_wholePart.size() > x.m_wholePart.size())
    {
        smallerWholePart = &x.m_wholePart;
        wholePartTargetSize = m_wholePart.size();
    }
    else
    {
        smallerWholePart = &m_wholePart;
        wholePartTargetSize = x.m_wholePart.size();
    }
    
    for (int i = (*smallerWholePart).size();i<wholePartTargetSize;i++)
    {
        (*smallerWholePart).push_back(emptyBitset);
    }
    m_wholePart.push_back(emptyBitset);
    x.m_wholePart.push_back(emptyBitset);
}




void BigNumber::twoCompliment() // check if 0
{
    for (int i=(m_decimalPart.size())-1; i>=0; i--)
            m_decimalPart[i] = ~m_decimalPart[i];

    for (int i=0; i<(m_wholePart.size()); i++)
        m_wholePart[i] = ~m_wholePart[i];

    
    increment();

    m_extraInfo.flip(0);


    
    
    //m_wholePart = ~m_wholePart;
    // now add 1

}

bool BigNumber::checkIfZero()
{
    for (int i = 0;i < m_decimalPart.size();i++)
    {
        if (m_decimalPart[i].any())
        {
            return false;
        }
    }
    for (int i = 0;i < m_wholePart.size();i++)
    {
        if (m_wholePart[i].any())
        {
            return false;
        }
    }

    return true;
}

    ///////////////////////////////////////////////////////////
//BigNumber::BigNumber() = default;
BigNumber::BigNumber(numStoreType wholePart, numStoreType decimalPart, bool sign, bool isNaN, bool isInf)
: m_wholePart {wholePart}
, m_decimalPart {decimalPart}
, m_extraInfo {0b00000000}
{
    if (sign) m_extraInfo.set(0);
    if (isNaN) m_extraInfo.set(1);
    if (isInf) m_extraInfo.set(2);
    shrinkAll();
}

void BigNumber::doubleNum()
{
        bool doCarry {false};

        for (int i=(m_decimalPart.size())-1; i>=0; i--)
        {
            doCarry = shiftAndCarry(doCarry,m_decimalPart[i]);
        }

        for (int i=0; i<(m_wholePart.size()); i++)
        {
            doCarry = shiftAndCarry(doCarry,m_wholePart[i]);
        }


        if (doCarry)
            m_wholePart.push_back(bitsetVal1);

        shrinkAll();

    }

void BigNumber::increment() {
    
    const int decimalSize {m_decimalPart.size()};
    const int wholeSize {m_wholePart.size()};

    if (decimalSize > 0)
    {
    for (int i=decimalSize-1; i>=0; i--)
            {
                if (m_decimalPart[i].all()) {
                    m_decimalPart[i] = emptyBitset;
                    continue;
                }

                for (int i1 = 0;i1<bitsetSize;i1++)
                {
                    if (m_decimalPart[i].test(i1) == false) {
                        m_decimalPart[i].set(i1);
                        goto addedOne;  }
                    else
                        m_decimalPart[i].reset(i1);
                    // add func idk
                }
            }
    }

    if (wholeSize > 0)
    {
    for (int i=0; i<wholeSize; i++)
            {
                if (m_wholePart[i].all()) {
                    m_wholePart[i] = emptyBitset;
                    continue;
                }

                for (int i1 = 0;i1<bitsetSize;i1++)
                {
                    if (m_wholePart[i].test(i1) == false) {
                        m_wholePart[i].set(i1);
                        goto addedOne;  }
                    else
                        m_wholePart[i].reset(i1);
                    // add func idk
                }
            }
    }
    

    // only runs if goto doesn't skip it
    m_wholePart.push_back(bitsetVal1);

    addedOne:
    //shrinkAll();
    //skps the rest
}

void BigNumber::addNum(BigNumber& x)
    {
        if (checkIfZero()) {
            m_decimalPart = x.m_decimalPart;
            m_wholePart = x.m_wholePart;
            m_extraInfo = x.m_extraInfo;
            return;
        }
        else if (x.checkIfZero())
        {
            return;
        }

        const bool isNegative {m_extraInfo.test(0)};
        const bool isNegativeX {x.m_extraInfo.test(0)};

        matchSizes(x);

        if (isNegative ^ isNegativeX) //if one is negative and the other isn't
        {
            
            (isNegative ? twoCompliment() : x.twoCompliment());
        }




        bool carry {false};
        const int wholeSize = m_wholePart.size();
        const int decimalSize = m_decimalPart.size();
        
        
        for (int i=decimalSize-1; i>=0; i--)
        {
            addNums(m_decimalPart[i],x.m_decimalPart[i],carry);
        }

        /////////////// 

        
        for (int i=0; i<wholeSize; i++)
        {
            addNums(m_wholePart[i],x.m_wholePart[i],carry);
        }

        // if carry and one was negative then it's now positive, if there's a carry and neither are neg
        if (carry)
        {
            m_extraInfo.reset(0);
        }
        else if (isNegative ^ isNegativeX)
        {
            twoCompliment();
        }
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