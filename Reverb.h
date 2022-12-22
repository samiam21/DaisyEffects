#ifndef REVERB_H
#define REVERB_H

#include "IEffect.h"

/**********************************************
 * Reverb Effect
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
 * Knob 2 - Decay
 * Knob 3 - Tone
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

// #define DSY_REVERBSC_MAX_SIZE 791488
static ReverbSc DSY_SDRAM_BSS verb;

class Reverb : public IEffect
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
    void ConfigureKnobPositions(int mixChannel, int decayChannel, int toneChannel);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"DECAY", (char *)"TONE", (char *)""};

    const float decayMin = 0.89f;
    const float decayMax = 0.99f;
    const float toneMin = 0.0f;

    float sample_rate;
    float decay = 1.0f;
    float tone = 0.5f;
    float mixLevel = 0.5f;

    int mixKnobChannel = KNOB_1_CHN;
    int decayKnobChannel = KNOB_2_CHN;
    int toneKnobChannel = KNOB_3_CHN;

    Knob decayKnob;
    Knob toneKnob;
    Knob mixKnob;
};

#endif