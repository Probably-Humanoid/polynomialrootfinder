#include "mathfuncs.h"
#include <iostream>
#include <vector>
#include <chrono>


void printExecutionTime(std::chrono::_V2::system_clock::time_point start)
{
    const auto stop = std::chrono::high_resolution_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken: " << duration.count() << " microseconds\n";
}

void printPolynomial(std::vector<std::float128_t> polynomial)
{
    for (int i=polynomial.size()-1; i>=0; i--)
    {
        std::cout << std::format("{}", polynomial[i]) << "x^" << i;
        if (i != 0)
            std::cout << " + ";
        else
            std::cout << '\n';
    }
}

std::vector<std::vector<std::float128_t>> fillTable(std::vector<std::vector<std::float128_t>> tableToFill)
{
    const int highestOrder = tableToFill.size()-1;
    std::vector<std::float128_t> tempTable = tableToFill[highestOrder];
    for (int i=highestOrder-1; i>=0; i--)
    {
        tempTable = takeDerivative(tempTable);
        tableToFill[i] = tempTable;
    }
    return tableToFill;
}

int getPolySize()
{
    int polySize;
    std::cout << "enter the degree of the polynomial: ";
    std::cin >> polySize;
    return polySize+1;
}

std::vector<std::float128_t> getFirstPolynomial(int size)
{
    std::vector<std::float128_t> poly (size);

    for (int i=size-1;i>=0;i--)
    {
        std::cout << "x^" << i << " = ";
        double tempVar;
        std::cin >> tempVar;
        poly[i] = static_cast<std::float128_t>(tempVar);
    }
    return poly;
}

int main()
{

    const int polySize = getPolySize(); // highest degree + 1
    std::vector<std::vector<std::float128_t>> polysCollection (polySize);
    // polys collection is a table of tables, where an element at index 'i' is a polynomial of degree i
    // a table storing a polynomial stores it in reverse order, where a number 'n' at index 'i' translates to n*x^i

    const std::vector<std::float128_t> firstPoly = getFirstPolynomial(polySize);
    polysCollection[polySize-1] = firstPoly;
    
    

    printPolynomial(firstPoly);

    const auto start = std::chrono::high_resolution_clock::now();

    polysCollection = fillTable(polysCollection);

    std::vector<std::float128_t> rootsOfQuad = getRootsFromDerivList(polysCollection);

    printExecutionTime(start);

    std::cout << "roots are:\n";
    for (int i=0;i<rootsOfQuad.size();i++)
    {
        std::cout << std::format("{}", rootsOfQuad[i]) << '\n';
    }

    
    //std::cout << "first root is at: " << rootsOfQuad[0] << '\n';
    //std::cout << "second root is at: " << rootsOfQuad[1] << '\n';

    return 0;
}