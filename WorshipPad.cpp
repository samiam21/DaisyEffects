#include "WorshipPad.h"

// Placed in SDRAM — the aux_ buffer inside ReverbSc is ~386 KB.
ReverbSc DSY_SDRAM_BSS wpad_verb;

// C3 root frequencies (Hz) for each chromatic note, A4 = 440 Hz tuning.
const float WorshipPad::kNoteFreqsC3[12] = {
    130.81f, // C
    138.59f, // C#/Db
    146.83f, // D
    155.56f, // D#/Eb
    164.81f, // E
    174.61f, // F
    185.00f, // F#/Gb
    196.00f, // G
    207.65f, // G#/Ab
    220.00f, // A
    233.08f, // A#/Bb
    246.94f, // B
};

void WorshipPad::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw      = hardware;
    display = daisyDisplay;

    float sr = hw->AudioSampleRate();

    oscRoot_.Init(sr);
    oscRoot_.SetWaveform(Oscillator::WAVE_SIN);
    oscRoot_.SetAmp(0.5f);

    oscFifth_.Init(sr);
    oscFifth_.SetWaveform(Oscillator::WAVE_SIN);
    oscFifth_.SetAmp(0.5f);

    // ReverbSc runs poorly above 48 kHz — halve the rate when at 96 kHz.
    wpad_verb.Init(sr > 48000.f ? sr / 2.0f : sr);
    wpad_verb.SetFeedback(0.90f);
    wpad_verb.SetLpFreq(6000.0f);

    octaveToggle_.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));
    currentOctave_ = octaveToggle_.ReadToggle();

    guitarKnob_.Init(hw, KNOB_1_CHN, guitarLevel_, 0.0f, 1.0f);
    padKnob_.Init(hw, KNOB_2_CHN, padLevel_, 0.0f, 1.0f);
    keyKnob_.Init(hw, KNOB_3_CHN, keyPosition_, 0.0f, 1.0f);

    UpdateFrequencies();
}

void WorshipPad::UpdateFrequencies()
{
    int noteIndex = static_cast<int>(keyPosition_ * 12.0f);
    if (noteIndex > 11)
        noteIndex = 11;

    // Octave multipliers relative to kNoteFreqsC3 base: 0 = C3, 1 = C4, 2 = C5
    float octMul = 1.0f;
    if (currentOctave_ == 1)
        octMul = 2.0f;
    else if (currentOctave_ == 2)
        octMul = 4.0f;

    float rootFreq = kNoteFreqsC3[noteIndex] * octMul;

    oscRoot_.SetFreq(rootFreq);
    oscFifth_.SetFreq(rootFreq * 1.49831f); // perfect fifth (2^(7/12))
}

float WorshipPad::Process(float in)
{
    float padMono = oscRoot_.Process() + oscFifth_.Process();

    float out1, out2;
    wpad_verb.Process(padMono, padMono, &out1, &out2);
    float wetPad = (out1 + out2) * 0.5f;

    return (in * guitarLevel_) + (wetPad * padLevel_);
}

void WorshipPad::Loop(bool allowEffectControl, bool isTapPressed)
{
    bool freqChanged = false;

    // Always track the toggle so octave responds without entering edit mode.
    int newOctave = octaveToggle_.ReadToggle();
    if (newOctave != currentOctave_)
    {
        currentOctave_                = newOctave;
        effectSettings.togglePosition = currentOctave_;
        freqChanged                   = true;
    }

    if (allowEffectControl)
    {
        if (guitarKnob_.SetNewValue(guitarLevel_))
        {
            debugPrintlnF(hw, "WorshipPad guitar level: %f", guitarLevel_);
            updateEditModeKnobValue(display, 0, guitarLevel_);
            effectSettings.knobSettings[0] = guitarLevel_;
        }

        if (padKnob_.SetNewValue(padLevel_))
        {
            debugPrintlnF(hw, "WorshipPad pad level: %f", padLevel_);
            updateEditModeKnobValue(display, 1, padLevel_);
            effectSettings.knobSettings[1] = padLevel_;
        }

        if (keyKnob_.SetNewValue(keyPosition_))
        {
            debugPrintlnF(hw, "WorshipPad key position: %f", keyPosition_);
            updateEditModeKnobValue(display, 2, keyPosition_);
            effectSettings.knobSettings[2] = keyPosition_;
            freqChanged                     = true;
        }
    }

    if (freqChanged)
        UpdateFrequencies();
}

char *WorshipPad::GetEffectName()
{
    return (char *)"WORSHIP PAD";
}

char **WorshipPad::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings WorshipPad::GetEffectSettings()
{
    effectSettings.knobSettings[0] = guitarLevel_;
    effectSettings.knobSettings[1] = padLevel_;
    effectSettings.knobSettings[2] = keyPosition_;
    effectSettings.togglePosition  = currentOctave_;
    return effectSettings;
}

void WorshipPad::SetEffectSettings(EffectSettings settings)
{
    guitarLevel_   = settings.knobSettings[0];
    padLevel_      = settings.knobSettings[1];
    keyPosition_   = settings.knobSettings[2];
    currentOctave_ = settings.togglePosition;
    UpdateFrequencies();
}
