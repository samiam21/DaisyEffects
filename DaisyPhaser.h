#ifndef PHASER_H
#define PHASER_H

#include "IEffect.h"

/**********************************************
 * Phaser Effect
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
 * Knob 2 - Rate
 * Knob 3 - Width
 * Knob 4 - Feedback
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class DaisyPhaser : public IEffect
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
    void ConfigureKnobPositions(int mixChannel, int rateChannel, int widthChannel, int feedbackChannel);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"RATE", (char *)"WIDTH", (char *)"FEEDBK"};

    const float minRate = 0.f;
    const float maxRate = 1000.f;

    float mixLevel = 1.f;
    float rate = 500.f;
    float width = 0.5f;
    float feedback = 0.5f;

    uint8_t mixKnobChannel = KNOB_1_CHN;
    uint8_t rateKnobChannel = KNOB_2_CHN;
    uint8_t widthKnobChannel = KNOB_3_CHN;
    uint8_t feedbackKnobChannel = KNOB_4_CHN;

    Knob mixKnob;
    Knob rateKnob;
    Knob widthKnob;
    Knob feedbackKnob;

    Phaser phaser;
};

#endif