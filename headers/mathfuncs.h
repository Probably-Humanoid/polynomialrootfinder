#pragma once
#include <vector>
#include <span>

double power(double base,int exponent);
std::vector<double> takeDerivative(std::span<double> polynomial);
double evalFunction(std::span<double> function,double inputVal);
double findFlipPoint(double startPoint,int goRight, std::span<double> function);
double findRoot2TP(double turnP1,double turnP2, std::span<double> function);
double findRootOfLinear(std::span<double> linearFunc);
std::vector<double> findRootsFromTPs(std::span<double> turningPoints, std::span<double> function);
std::vector<double> getRootsFromDerivList(std::span<std::vector<double>> polylist);