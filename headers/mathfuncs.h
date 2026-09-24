#pragma once
#include <vector>
#include <span>
#include <stdfloat>

std::float128_t power(std::float128_t base,int exponent);
std::vector<std::float128_t> takeDerivative(std::span<std::float128_t> polynomial);
std::float128_t evalFunction(std::span<std::float128_t> function,std::float128_t inputVal);
std::float128_t findFlipPoint(std::float128_t startPoint,int goRight, std::span<std::float128_t> function);
std::float128_t findRoot2TP(std::float128_t turnP1,std::float128_t turnP2, std::span<std::float128_t> function);
std::float128_t findRootOfLinear(std::span<std::float128_t> linearFunc);
std::vector<std::float128_t> findRootsFromTPs(std::span<std::float128_t> turningPoints, std::span<std::float128_t> function);
std::vector<std::float128_t> getRootsFromDerivList(std::span<std::vector<std::float128_t>> polylist);