#ifndef TREMOLO_H
#define TREMOLO_H

#include "IEffect.h"

/**********************************************
 * Tremolo Effect
 *
 * SPST 1 -
 * SPST 2 -
 * SPST 3 -
 * SPST 4 -
 *
 * SPDT 1 - Waveform selector (SIN, SQUARE, RAMP)
 * SPDT 2 -
 *
 * Knob 1 - Mix
 * Knob 2 - Rate
 * Knob 3 - Width
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class DaisyTremolo : public IEffect
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
    void UpdateToggleDisplay();
    void ConfigureKnobPositions(int mixChannel, int rateChannel, int widthChannel);
    void ConfigureTogglePositions(int waveformToggle1, int waveformToggle2);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"RATE", (char *)"WIDTH", (char *)""};

    const float rateMin = 0.0f;
    const float rateMax = 20.0f;

    float mixLevel = 0.0f;
    float rate = 0.0f;
    float width = 0.0f;
    int waveform = Oscillator::WAVE_SIN;

    uint8_t mixKnobChannel = KNOB_1_CHN;
    uint8_t rateKnobChannel = KNOB_2_CHN;
    uint8_t widthKnobChannel = KNOB_3_CHN;
    int waveformTogglePin1 = effectSPDT1Pin1;
    int waveformTogglePin2 = effectSPDT1Pin2;

    Knob mixKnob;
    Knob rateKnob;
    Knob widthKnob;

    NFNToggle waveformSelector;

    Tremolo tremolo;
};

#endif