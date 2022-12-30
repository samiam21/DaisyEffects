#ifndef FLANGER_H
#define FLANGER_H

#include "IEffect.h"

/**********************************************
 * Flanger Effect
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
 * Knob 4 - Feedback
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class DaisyFlanger : public IEffect
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
    void ConfigureKnobPositions(int mixChannel, int rateChannel, int widthChannel, int feedbackChannel);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"RATE", (char *)"WIDTH", (char *)"FEEDBK"};

    float mixLevel = 0.5f;
    float rate = 0.5f;
    float width = 0.5f;
    float feedback = 0.88f;

    uint8_t mixKnobChannel = KNOB_1_CHN;
    uint8_t rateKnobChannel = KNOB_2_CHN;
    uint8_t widthKnobChannel = KNOB_3_CHN;
    uint8_t feedbackKnobChannel = KNOB_4_CHN;

    Knob mixKnob;
    Knob rateKnob;
    Knob widthKnob;
    Knob feedbackKnob;

    Flanger flanger;
};

#endif