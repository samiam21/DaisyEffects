#ifndef CLEAN_BOOST_H
#define CLEAN_BOOST_H

#include "IEffect.h"

/**********************************************
 * Clean Boost Effect
 *
 * SPST 1 -
 * SPST 2 -
 * SPST 3 -
 * SPST 4 -
 *
 * SPDT 1 -
 * SPDT 2 -
 *
 * Knob 1 - Volume Boost
 * Knob 2 -
 * Knob 3 -
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class CleanBoost : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay = nullptr, int *newBpm = nullptr);
    void Cleanup();
    float Process(float in);
    void Loop(bool allowEffectControl);
    char *GetEffectName();
    char **GetKnobNames();
    EffectSettings GetEffectSettings();
    void SetEffectSettings(EffectSettings effectSettings);
    void ConfigureKnobPositions(int boostChannel);
    void ConfigureTogglePositions(int onOffPin);
    void SetBoostMinMax(float boostMin, float boostMax);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"BOOST", (char *)"", (char *)"", (char *)""};

    float boostLevelMin = 1.0f;
    float boostLevelMax = 30.0f;
    float boostLevel = 10.0f;
    bool audioOn = true;

    int boostKnobChannel = KNOB_1_CHN;
    int onOffTogglePin = effectTogglePin1;

    Knob boostKnob;
    SimpleToggle onOffToggle;
};

#endif