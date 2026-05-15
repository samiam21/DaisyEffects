#ifndef WORSHIP_PAD_H
#define WORSHIP_PAD_H

#include "IEffect.h"

/**********************************************
 * WorshipPad Effect
 *
 * SPDT 1 - Octave (Low / Mid / High)
 *
 * Knob 1 - Guitar volume (dry signal)
 * Knob 2 - Pad level (wet signal)
 * Knob 3 - Key (all 12 chromatic notes)
 * Knob 4 -
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

private:
    void UpdateFrequencies();

    const char *knobNames[MAX_KNOBS] = {
        (char *)"GUITAR", (char *)"PAD", (char *)"KEY", (char *)""};

    Oscillator oscRoot_;
    Oscillator oscFifth_;

    NFNToggle octaveToggle_;

    Knob guitarKnob_;
    Knob padKnob_;
    Knob keyKnob_;

    float guitarLevel_ = 0.7f;
    float padLevel_    = 0.7f;
    float keyPosition_ = 0.0f; // 0.0–1.0, maps to 12 chromatic notes
    int   currentOctave_ = 1;  // 0 = C3, 1 = C4, 2 = C5

    // C3 root frequencies for all 12 chromatic notes (A4 = 440 Hz tuning)
    static const float kNoteFreqsC3[12];
};

#endif
