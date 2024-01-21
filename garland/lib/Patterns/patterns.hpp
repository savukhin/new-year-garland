#pragma once

#include "patternConstLight.hpp"
#include "patternRiseAndFall.hpp"

const unsigned char PATTERNS_COUNT = 3;
PatternBase *patterns[] = {
    new PatternConstLight(),
    new PatternRiseAndFall(),
    new PatternRiseAndFall(0, 0, 300, 300, false, false, true, true, 2),
};
