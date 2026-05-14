#ifndef BYPASS_H
#define BYPASS_H

#include "IEffect.h"

/**********************************************
 * Bypass Effect
 *
 * SPST 1 -
 * SPST 2 -
 * SPST 3 -
 * SPST 4 -
 *
 * SPDT 1 -
 * SPDT 2 -
 *
 * Knob 1 - Volume
 * Knob 2 -
 * Knob 3 -
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class Bypass : public IEffect
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
    void ConfigureKnobPositions(int volChannel);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"VOL", (char *)"", (char *)"", (char *)""};

    const float volLevelMin = 10.0f;
    const float volLevelMax = 0.5f;
    float volLevel = 10.0f;

    int volKnobChannel = KNOB_1_CHN;

    Knob volKnob;
};

#endif