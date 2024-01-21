#pragma once

#include "patternBase.hpp"
#include <Arduino.h>

class PatternRiseAndFall : public PatternBase {
private:
    enum class SignalState {
        Rise,
        HoldTop,
        Fall,
        HoldBottom
    };
    int millisecsRise, millisecsFall, millisecsHoldTop, millisecsHoldBottom;
    bool needsRise=true, needsFall=true, needsHoldTop=false, needsHoldBottom=false;
    uint8_t eachBlinkTimes;

    uint8_t currentLed = 0;
    uint8_t currentBlinkCount = 0;
    SignalState currentSignalState;
    SignalState initSignalState;
    uint32_t lastTimeMarkMillis = 0;
    uint32_t nextTimeMarkMillis = 0;

    float currentK;
    uint8_t currentY0;

    float calculateK(uint32_t transitionTime, SignalState state) const {
        if (state == SignalState::HoldTop || state == SignalState::HoldBottom)
            return 0;

        float k = 255./(float)transitionTime;

        if (state == SignalState::Fall)
            return -k;
        return k;
    }

    uint8_t calculateY0(SignalState state) const {
        switch (state)
        {
        case SignalState::Rise:
            return 0;
        case SignalState::HoldTop:
            return 255;
        case SignalState::Fall:
            return 255;
        case SignalState::HoldBottom:
            return 0;
        }
        return 0;
    }

    uint8_t val(float k, uint32_t t, uint8_t y0) const {
        int f = y0 + k * t;
        return max(min(f, 255), 0); // in bounds [0, 255]
    }

    SignalState calculateInitialSignalState() const {
        if (needsRise) return SignalState::Rise;
        if (needsHoldTop) return SignalState::HoldTop;
        if (needsFall) return SignalState::Fall;
        return SignalState::HoldBottom;
    }

    SignalState calculateNextSignalState(SignalState state) const {
        switch (state)
        {
        case SignalState::Rise:
            if (needsHoldTop) return SignalState::HoldTop;
            if (needsFall) return SignalState::Fall;
            if (needsHoldBottom) return SignalState::HoldBottom;
            return SignalState::Rise;
        case SignalState::HoldTop:
            if (needsFall) return SignalState::Fall;
            if (needsHoldBottom) return SignalState::HoldBottom;
            if (needsRise) return SignalState::Rise;
            return SignalState::HoldTop;
        case SignalState::Fall:
            if (needsHoldBottom) return SignalState::HoldBottom;
            if (needsRise) return SignalState::Rise;
            if (needsHoldTop) return SignalState::HoldTop;
            return SignalState::Fall;
        case SignalState::HoldBottom:
            if (needsRise) return SignalState::Rise;
            if (needsHoldTop) return SignalState::HoldTop;
            if (needsFall) return SignalState::Fall;
            return SignalState::HoldBottom;
        }
        return initSignalState;
    }

    int getTransitionMillis(SignalState state) const {
        switch (state)
        {
        case SignalState::Rise:
            return millisecsRise;
        case SignalState::HoldTop:
            return millisecsHoldTop;
        case SignalState::Fall:
            return millisecsFall;
        case SignalState::HoldBottom:
            return millisecsHoldBottom;
        }
        return 0;
    }

    uint8_t getNextLed(uint8_t currentLed) const {
        return (currentLed + 1) % 2;
    }

    void tryToggleBlinkAndLed() {
        if (currentSignalState != initSignalState) 
            return;

        currentBlinkCount++;

        if (currentBlinkCount < eachBlinkTimes)
            return;
        
        currentLed = getNextLed(currentLed);
        currentBlinkCount = 0;
    }

    void toggleState(uint32_t currentMillis) {
        tryToggleBlinkAndLed();
        currentSignalState = calculateNextSignalState(currentSignalState);
        lastTimeMarkMillis = currentMillis;
        uint32_t transitionTime = getTransitionMillis(currentSignalState);
        nextTimeMarkMillis = lastTimeMarkMillis + transitionTime;
        currentK = calculateK(transitionTime, currentSignalState);
        currentY0 = calculateY0(currentSignalState);
    }

    void updateLedValue(uint8_t ledNum, uint8_t value) {
        if (ledNum == 0) {
            state1 = value;
        } else {
            state2 = value;
        }
    }

public:
    PatternRiseAndFall(float millisecsRise=3000, float millisecsFall=3000, float millisecsHoldTop=0, float millisecsHoldBottom=0, bool needsRise=true, bool needsFall=true, bool needsHoldTop=false, bool needsHoldBottom=false, unsigned char eachBlinkTimes=1): 
        millisecsRise(millisecsRise), millisecsFall(millisecsFall), millisecsHoldTop(millisecsHoldTop), millisecsHoldBottom(millisecsHoldBottom),
        needsRise(needsRise), needsFall(needsFall), needsHoldTop(needsHoldTop), needsHoldBottom(needsHoldBottom),
        eachBlinkTimes(eachBlinkTimes), lastTimeMarkMillis(millis()),
        PatternBase() {
            initSignalState = calculateInitialSignalState();
            currentSignalState = initSignalState;
        }

    void loop() override {
        uint32_t currentMillis = millis();
        if (currentMillis > nextTimeMarkMillis) {
            toggleState(currentMillis);
        }

        uint32_t t = currentMillis - lastTimeMarkMillis;
        uint8_t value = val(currentK, t, currentY0);
        updateLedValue(value, currentLed);
    }
};
