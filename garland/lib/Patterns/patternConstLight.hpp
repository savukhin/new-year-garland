#pragma once

#include "patternBase.hpp"
#include <Arduino.h>

class PatternConstLight : public PatternBase {
public:
    PatternConstLight(): PatternBase() {
        state1 = 255;
        state2 = 255;
    }

    void loop() override {}
};
