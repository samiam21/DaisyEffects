#ifndef SHIFTER_H
#define SHIFTER_H

#include <cmath>
#include "IEffect.h"

/**********************************************
 * Pitch Shifter Effect
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
 * Knob 2 - Transpose
 * Knob 3 - Delay
 * Knob 4 - Fun
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class Shifter : public IEffect
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
    void ConfigureKnobPositions(int mixChannel, int transposeChannel, int delayChannel, int funChannel);
    void SetMinMaxMix(float minMix, float maxMix);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"TRANS", (char *)"DEL", (char *)"FUN"};

    uint32_t GetDelay();
    float GetTranspose();

    float mixMin = 0.0f;
    float mixMax = 1.f;
    const float transMin = -12.0f;
    const float transMax = 24.0f;
    const float delayMin = 5760.f;
    const float delayMax = SHIFT_BUFFER_SIZE * 2;
    const float funMin = 0.0f;
    const float funMax = 10.0f;

    float mix = 1.0f;
    float transpose = 12.0f;
    float delay = 5760.f;
    float fun = 0.0f;

    uint8_t mixKnobChannel = KNOB_1_CHN;
    uint8_t transKnobChannel = KNOB_2_CHN;
    uint8_t delayKnobChannel = KNOB_3_CHN;
    uint8_t funKnobChannel = KNOB_3_CHN;

    Knob mixKnob;
    Knob transKnob;
    Knob delayKnob;
    Knob funKnob;

    PitchShifter shifter;
};

#endif