#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "IEffect.h"

/**********************************************
 * Compressor Effect
 *
 * SPST 1 -
 * SPST 2 -
 * SPST 3 -
 * SPST 4 -
 *
 * SPDT 1 -
 * SPDT 2 -
 *
 * Knob 1 - Ratio
 * Knob 2 - Threshold
 * Knob 3 - Attack
 * Knob 4 - Release
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class DaisyCompressor : public IEffect
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
    void ConfigureKnobPositions(int ratioChannel, int thresholdChannel, int attackChannel, int releaseChannel);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"RATIO", (char *)"THRESH", (char *)"ATTACK", (char *)"RELEASE"};

    const float ratioMin = 1.0f;
    const float ratioMax = 40.0f;
    const float thresholdMin = 0.0f;
    const float thresholdMax = -80.0f;
    const float attackMin = 0.001f;
    const float attackMax = 10.0f;
    const float releaseMin = 0.001f;
    const float releaseMax = 10.0f;

    float ratio = 4.0f;
    float threshold = -6.0f;
    float attack = 0.02f;
    float release = 0.02f;

    uint8_t ratioKnobChannel = KNOB_1_CHN;
    uint8_t thresholdKnobChannel = KNOB_2_CHN;
    uint8_t attackKnobChannel = KNOB_3_CHN;
    uint8_t releaseKnobChannel = KNOB_3_CHN;

    Knob ratioKnob;
    Knob thresholdKnob;
    Knob attackKnob;
    Knob releaseKnob;

    Compressor compressor;
};

#endif