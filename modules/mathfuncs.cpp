#include "mathfuncs.h"
#include <vector>
#include <iostream>
#include <span>
#include <bitset>
#include <cmath>

const std::float128_t g_nanval = (0.0/0.0); //can't be constexpr for some reason
constexpr std::float128_t g_upperSearchLimit {1e100};
constexpr std::float128_t g_lowerPrecisionValue {1e-10};
constexpr std::float128_t g_sameNumberLimit {1e-6};

std::float128_t getSign(std::float128_t numberToCheck)
{
    return ((numberToCheck >= 0.0) ? 1.0 : -1.0);
}

bool sameNumber(std::float128_t num1, std::float128_t num2)
{
    const std::float128_t difference {num1-num2};
    if (difference<g_sameNumberLimit && difference>-g_sameNumberLimit)
        return true;
    else
        return false;
}

std::float128_t power(std::float128_t base,int exponent)
{
    std::float128_t returnNum = base;
    for(int i=1; i<exponent; i++) {
        returnNum *= base;
    }
    return (exponent > 0 ? returnNum : 1);
}

std::vector<std::float128_t> takeDerivative(std::span<std::float128_t> polynomial)
{
    const int outputPolySize = polynomial.size() - 1;
    std::vector<std::float128_t> returnPoly (outputPolySize);
    for (int i=0; i < outputPolySize; i++)
    {
        returnPoly[i] = (i+1)*polynomial[i+1];
    }

    return returnPoly;

}

std::float128_t evalFunction(std::span<std::float128_t> function,std::float128_t inputVal) {
    std::float128_t returnValue = 0;
    for (int i=0;i<function.size();i++) {
        returnValue += (function[i] * power(inputVal,i));
    }
    return returnValue;
}

std::float128_t findFlipPoint(std::float128_t startPoint,int goRight, std::span<std::float128_t> function)
{
    constexpr std::float128_t firstStepSize = (1) / 2.0;
    constexpr std::float128_t stepSize = 2.0; // multiply by step val
    const std::float128_t startingPointEval = evalFunction(function,startPoint);
    const std::float128_t startingSign = getSign(startingPointEval);

    std::float128_t stepVal = firstStepSize * goRight;
    std::float128_t currentPointEval = startingPointEval;
    while (currentPointEval*startingSign>0)
    {
        stepVal*=stepSize;
        currentPointEval = evalFunction(function,startPoint+stepVal);
        if (stepVal > g_upperSearchLimit or stepVal < -g_upperSearchLimit)
            return g_nanval;
    }
    return startPoint+(stepVal);
}

std::float128_t findRoot2TP(std::float128_t turnP1,std::float128_t turnP2, std::span<std::float128_t> function)
{
    //ensure signs on p1 and p2 are diff before running func
    const std::float128_t startingSign = getSign(evalFunction(function,turnP1));
    while (true)
    {
        const std::float128_t halfwayPoint = (turnP1 + turnP2)/2;
        const std::float128_t hpValue = evalFunction(function,halfwayPoint);
        if (hpValue < g_lowerPrecisionValue && hpValue > -g_lowerPrecisionValue)
            return halfwayPoint;
        if (hpValue*startingSign>0)
            turnP1 = halfwayPoint;
        else
            turnP2 = halfwayPoint;
        
    }
}

std::float128_t findRootOfLinear(std::span<std::float128_t> linearFunc)
{
    return (-1.0*linearFunc[0]/linearFunc[1]);
}

bool DoubleValue(std::float128_t val1, std::span<std::float128_t> table)
{
    if (table.size()>0)
    {
        return sameNumber(val1,table.back());
    }
    else
        return false;
}

std::vector<std::float128_t> findRootsFromTPs(std::span<std::float128_t> turningPoints, std::span<std::float128_t> function)
{
    std::vector<std::float128_t> returnTPs;
    if (turningPoints.size()>0){
    std::float128_t zerothRoot = findFlipPoint(turningPoints[0],-1,function);
    if (!std::isnan(zerothRoot))
    {
       returnTPs.push_back(findRoot2TP(zerothRoot,turningPoints[0],function));
    }
    if (turningPoints.size()>1){
    for (int i = 0; i<(turningPoints.size()-1);i++)
    {
        if (getSign(evalFunction(function,turningPoints[i])) != getSign(evalFunction(function,turningPoints[i+1])))
        {
            const std::float128_t foundRoot {findRoot2TP(turningPoints[i],turningPoints[i+1],function)};
            if (!DoubleValue(foundRoot,returnTPs))
                returnTPs.push_back(foundRoot);
        }
    }}
    const int tpFinal {static_cast<int>(turningPoints.size())-1};
    std::float128_t finalthRoot = findFlipPoint(turningPoints[tpFinal],1,function);
    if (!std::isnan(finalthRoot))
    {
        const std::float128_t foundRoot {findRoot2TP(finalthRoot,turningPoints[tpFinal],function)};
            if (!DoubleValue(foundRoot,returnTPs))
                returnTPs.push_back(foundRoot);
    }}
    else
    {
        constexpr std::float128_t randomPoint {1000000000.0}; 
        std::float128_t potentialTP1 = findFlipPoint(randomPoint,-1,function);
        std::float128_t potentialTP2 = findFlipPoint(randomPoint,1,function);
        if (!std::isnan(potentialTP1))
        {
           returnTPs.push_back(findRoot2TP(potentialTP1,randomPoint,function));
        }
        else if (!std::isnan(potentialTP2))
        {
           returnTPs.push_back(findRoot2TP(potentialTP2,randomPoint,function));
        }
    }
    return returnTPs;
    

    //add at back returnTPs.push_back()
}

std::vector<std::float128_t> getRootsFromDerivList(std::span<std::vector<std::float128_t>> polylist)
{
    std::vector<std::float128_t> previousRoots = {findRootOfLinear(polylist[1])};
    //std::vector<std::float128_t> previousRoots = linearRoot;
    if (polylist.size()>2)
    {
        
        for (int i=2;i<(polylist.size());i++)
        {
            previousRoots = findRootsFromTPs(previousRoots,polylist[i]);
        }
    }
    return previousRoots;
}