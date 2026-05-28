#ifndef WORSHIP_PAD_H
#define WORSHIP_PAD_H

#include "IEffect.h"

/**********************************************
 * WorshipPad Effect
 *
 * SPDT 1 - Octave (C3 / C4 / C5)
 *
 * Knob 1 - Pad level (wet signal)
 * Knob 2 - 
 * Knob 3 -
 * Knob 4 -
 *
 * Pad-only effect: audio input is ignored. Root note is
 * fixed to C (rootNoteIndex); key switching to be added
 * later via buttons.
 **********************************************/

// ReverbSc internal buffer is ~386 KB — must live in SDRAM.
extern ReverbSc wpad_verb;

class WorshipPad : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay = nullptr, int *newBpm = nullptr) override;
    void Cleanup() override {}
    float Process(float in) override;
    void Loop(bool allowEffectControl, bool isTapPressed = false) override;
    char *GetEffectName() override;
    char **GetKnobNames() override;
    EffectSettings GetEffectSettings() override;
    void SetEffectSettings(EffectSettings effectSettings) override;
    void ConfigureKnobPositions(int padChannel);

    void NextRootNote();
    void PrevRootNote();

private:
    void UpdateFrequencies();

    const char *knobNames[MAX_KNOBS] = {
        (char *)"PAD", (char *)"", (char *)"", (char *)""};

    Oscillator oscRoot;
    Oscillator oscFifth;

    NFNToggle octaveToggle;

    int  padKnobChannel = KNOB_1_CHN;
    Knob padKnob;

    // Pad level acts as a boost (CleanBoost-style). 0.0 = silent, 1.0 ≈ unity,
    // higher values boost the wet signal — will clip at extreme settings.
    const float padLevelMin = 0.0f;
    const float padLevelMax = 1.0f;
    float       padLevel   = 1.0f;

    int currentOctave = 1; // 0 = C3, 1 = C4, 2 = C5
    int rootNoteIndex = 0; // 0 = C; future buttons will change this

    // C3 root frequencies for all 12 chromatic notes (A4 = 440 Hz tuning)
    static const float kNoteFreqsC3[12];
};

#endif
