#ifndef FOLD_H
#define FOLD_H

#include "IEffect.h"

/**********************************************
 * Fold Effect
 *
 * SPST 1 -
 * SPST 2 -
 * SPST 3 -
 * SPST 4 -
 *
 * SPDT 1 -
 * SPDT 2 -
 *
 * Knob 1 - Mix
 * Knob 2 - Increment
 * Knob 3 -
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class DaisyFold : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay = nullptr, int *newBpm = nullptr) override;
    void Cleanup() override;
    float Process(float in) override;
    void Loop(bool allowEffectControl, bool isTapPressed = false) override;
    char *GetEffectName() override;
    char **GetKnobNames() override;
    EffectSettings GetEffectSettings() override;
    void SetEffectSettings(EffectSettings effectSettings) override;

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"INC", (char *)"", (char *)""};

    const float incrementMin = 0.0f;
    const float incrementMax = 100.0f;

    float increment = 1.0f;
    float mix = 0.0f;

    Knob incrementKnob;
    Knob mixKnob;

    Fold fold;
};

#endif