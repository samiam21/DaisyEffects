#ifndef CHORUS_H
#define CHORUS_H

#include "IEffect.h"

/**********************************************
 * Chorus Effect
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
 * Knob 2 - LFO Rate
 * Knob 3 - Width
 * Knob 4 - Delay
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class DaisyChorus : public IEffect
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
    void ConfigureKnobPositions(int mixChannel, int rateChannel, int widthChannel, int delayChannel);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"RATE", (char *)"WIDTH", (char *)"DELAY"};

    float mixLevel = 0.5f;
    float rate = 0.5f;
    float width = 0.5f;
    float delay = 0.15f;

    int mixKnobChannel = KNOB_1_CHN;
    int rateKnobChannel = KNOB_2_CHN;
    int widthKnobChannel = KNOB_3_CHN;
    int delayKnobChannel = KNOB_4_CHN;

    Knob mixKnob;
    Knob rateKnob;
    Knob widthKnob;
    Knob delayKnob;

    Chorus chorus;
};

#endif