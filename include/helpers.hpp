//
// Created by adi on 15.01.2026.
//

#ifndef NETSIM_HELPERS_HXX
#define NETSIM_HELPERS_HXX
#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //NETSIM_HELPERS_HXX
