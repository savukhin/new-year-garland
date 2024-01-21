#pragma once

class PatternBase {
protected:
    unsigned char state1;
    unsigned char state2;
public:
    unsigned char getState1() { return state1; }
    unsigned char getState2() { return state2; }
    virtual void loop() {}
};
