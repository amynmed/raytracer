#include "Interval.hpp"



const Interval Interval::empty    = Interval(+INFINITY, -INFINITY);
const Interval Interval::universe = Interval(-INFINITY, +INFINITY);