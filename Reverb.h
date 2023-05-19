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
    void SetMinMaxMix(float minMix, float maxMix);
    void SetMinMaxDecay(float minDecay, float maxDecay);
    void ShouldReverseTonePot(bool reverseTone);
    void LengthenTail(bool lengthen);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"DECAY", (char *)"TONE", (char *)""};

    const float lengthen_amount = 0.05f;

    float decayMin = 0.79f;
    float decayMax = 0.99f;
    float mixMin = 0.0f;
    float mixMax = 1.f;
    float toneMin = 0.0f;

    float sample_rate;
    float decay = 1.0f;
    float decay_lengthen = 0.0f;
    float tone = 5000.f;
    float mixLevel = 0.5f;

    bool reverseTonePot = false;
    bool lengthenTail = false;

    uint8_t mixKnobChannel = KNOB_1_CHN;
    uint8_t decayKnobChannel = KNOB_2_CHN;
    uint8_t toneKnobChannel = KNOB_3_CHN;

    Knob decayKnob;
    Knob toneKnob;
    Knob mixKnob;
};

#endif