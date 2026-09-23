#include "mathfuncs.h"
#include <vector>
#include <iostream>
#include <span>
#include <bitset>
#include <cmath>

const double nanval = (0.0/0.0);
constexpr double upperSearchLimit {1e100};
constexpr double lowerPrecisionValue {1e-10};
constexpr double sameNumberLimit {1e-6};

double getSign(double numberToCheck)
{
    return ((numberToCheck >= 0.0) ? 1.0 : -1.0);
}

bool sameNumber(double num1, double num2)
{
    const double difference {num1-num2};
    if (difference<sameNumberLimit && difference>-sameNumberLimit)
        return true;
    else
        return false;
}

double power(double base,int exponent)
{
    double returnNum = base;
    for(int i=1; i<exponent; i++) {
        returnNum *= base;
    }
    return (exponent > 0 ? returnNum : 1);
}

std::vector<double> takeDerivative(std::span<double> polynomial)
{
    const int outputPolySize = polynomial.size() - 1;
    std::vector<double> returnPoly (outputPolySize);
    for (int i=0; i < outputPolySize; i++)
    {
        returnPoly[i] = (i+1)*polynomial[i+1];
    }

    return returnPoly;

}

double evalFunction(std::span<double> function,double inputVal) {
    double returnValue = 0;
    for (int i=0;i<function.size();i++) {
        returnValue += (function[i] * power(inputVal,i));
    }
    return returnValue;
}

double findFlipPoint(double startPoint,int goRight, std::span<double> function)
{
    constexpr double firstStepSize = (1) / 2.0;
    constexpr double stepSize = 2.0; // multiply by step val
    const double startingPointEval = evalFunction(function,startPoint);
    const double startingSign = getSign(startingPointEval);

    double stepVal = firstStepSize * goRight;
    double currentPointEval = startingPointEval;
    while (currentPointEval*startingSign>0)
    {
        stepVal*=stepSize;
        currentPointEval = evalFunction(function,startPoint+stepVal);
        if (stepVal > upperSearchLimit or stepVal < -upperSearchLimit)
            return nanval;
    }
    return startPoint+(stepVal);
}

double findRoot2TP(double turnP1,double turnP2, std::span<double> function)
{
    //ensure signs on p1 and p2 are diff before running func
    const double startingSign = getSign(evalFunction(function,turnP1));
    while (true)
    {
        const double halfwayPoint = (turnP1 + turnP2)/2;
        const double hpValue = evalFunction(function,halfwayPoint);
        if (hpValue < lowerPrecisionValue && hpValue > -lowerPrecisionValue)
            return halfwayPoint;
        if (hpValue*startingSign>0)
            turnP1 = halfwayPoint;
        else
            turnP2 = halfwayPoint;
        
    }
}

double findRootOfLinear(std::span<double> linearFunc)
{
    return (-1.0*linearFunc[0]/linearFunc[1]);
}

bool checkFinalDoubleValue(double val1, std::span<double> table)
{
    if (table.size()>0)
    {
        return sameNumber(val1,table.back());
    }
    else
        return false;
}

std::vector<double> findRootsFromTPs(std::span<double> turningPoints, std::span<double> function)
{
    std::vector<double> returnTPs;
    if (turningPoints.size()>0){
    double zerothRoot = findFlipPoint(turningPoints[0],-1,function);
    if (!std::isnan(zerothRoot))
    {
       returnTPs.push_back(findRoot2TP(zerothRoot,turningPoints[0],function));
    }
    if (turningPoints.size()>1){
    for (int i = 0; i<(turningPoints.size()-1);i++)
    {
        if (getSign(evalFunction(function,turningPoints[i])) != getSign(evalFunction(function,turningPoints[i+1])))
        {
            const double foundRoot {findRoot2TP(turningPoints[i],turningPoints[i+1],function)};
            if (!checkFinalDoubleValue(foundRoot,returnTPs))
                returnTPs.push_back(foundRoot);
        }
    }}
    const int tpFinal {static_cast<int>(turningPoints.size())-1};
    double finalthRoot = findFlipPoint(turningPoints[tpFinal],1,function);
    if (!std::isnan(finalthRoot))
    {
        const double foundRoot {findRoot2TP(finalthRoot,turningPoints[tpFinal],function)};
            if (!checkFinalDoubleValue(foundRoot,returnTPs))
                returnTPs.push_back(foundRoot);
    }}
    else
    {
        constexpr double randomPoint {1000000000.0}; 
        double potentialTP1 = findFlipPoint(randomPoint,-1,function);
        double potentialTP2 = findFlipPoint(randomPoint,1,function);
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

std::vector<double> getRootsFromDerivList(std::span<std::vector<double>> polylist)
{
    std::vector<double> previousRoots = {findRootOfLinear(polylist[1])};
    //std::vector<double> previousRoots = linearRoot;
    if (polylist.size()>2)
    {
        
        for (int i=2;i<(polylist.size());i++)
        {
            previousRoots = findRootsFromTPs(previousRoots,polylist[i]);
        }
    }
    return previousRoots;
}