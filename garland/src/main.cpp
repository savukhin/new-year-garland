#include "target.hpp"
#include <Arduino.h>

#include "patterns.hpp"

unsigned char currentPattern = 0;
PatternBase* pattern = patterns[0];

PatternBase* togglePattern()  {
    currentPattern++;
    currentPattern = currentPattern % PATTERNS_COUNT;
    pattern = patterns[currentPattern];
    return pattern;
}

int lastButtonState = LOW;

void loopButton() {
    int button = digitalRead(TOGGLE_BUTTON_PIN);
    if (button == lastButtonState) {
        return;
    }
    if (button == HIGH) {
        togglePattern();
    }
    lastButtonState = button;
}

void loopLeds() {
    pattern->loop();
    auto state1 = patterns[currentPattern]->getState1();
    auto state2 = patterns[currentPattern]->getState2();
    analogWrite(LED1_PIN, state1);
    analogWrite(LED2_PIN, state2);
}

void setup()
{
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(TOGGLE_BUTTON_PIN, INPUT);
}

void loop()
{
    loopButton();
    loopLeds();
}
