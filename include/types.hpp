#ifndef NETSIM_TYPES_HXX
#define NETSIM_TYPES_HXX

#include <functional>
using ElementID = int;
using TimeOffset = int;
using Time = int;
using ProbabilityGenerator = std::function<double()> ;
#endif
