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

    oscRoot.Init(sr);
    oscRoot.SetWaveform(Oscillator::WAVE_SIN);
    oscRoot.SetAmp(0.3f);

    oscFifth.Init(sr);
    oscFifth.SetWaveform(Oscillator::WAVE_SIN);
    oscFifth.SetAmp(0.3f);

    // ReverbSc runs poorly above 48 kHz — halve the rate when at 96 kHz.
    wpad_verb.Init(sr > 48000.f ? sr / 2.0f : sr);
    wpad_verb.SetFeedback(0.90f);
    wpad_verb.SetLpFreq(6000.0f);

    octaveToggle.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));
    currentOctave = octaveToggle.ReadToggle();

    padKnob.Init(hw, padKnobChannel, padLevel, padLevelMin, padLevelMax);

    UpdateFrequencies();
}

void WorshipPad::UpdateFrequencies()
{
    // Octave multipliers relative to kNoteFreqsC3 base: 0 = C3, 1 = C4, 2 = C5
    float octMul = 1.0f;
    if (currentOctave == 1)
        octMul = 2.0f;
    else if (currentOctave == 2)
        octMul = 4.0f;

    float rootFreq = kNoteFreqsC3[rootNoteIndex] * octMul;

    oscRoot.SetFreq(rootFreq);
    oscFifth.SetFreq(rootFreq * 1.49831f); // perfect fifth (2^(7/12))
}

void WorshipPad::ConfigureKnobPositions(int padChannel)
{
    padKnobChannel = padChannel;
}

void WorshipPad::NextRootNote()
{
    rootNoteIndex = (rootNoteIndex + 1) % 12;
    UpdateFrequencies();
}

void WorshipPad::PrevRootNote()
{
    rootNoteIndex = (rootNoteIndex + 11) % 12;
    UpdateFrequencies();
}

float WorshipPad::Process(float in)
{
    (void)in; // Pad-only effect — audio input is intentionally ignored.

    float padMono = oscRoot.Process() + oscFifth.Process();

    float out1, out2;
    wpad_verb.Process(padMono, padMono, &out1, &out2);
    float wetPad = (out1 + out2) * 0.5f;

    return wetPad * padLevel;
}

void WorshipPad::Loop(bool allowEffectControl, bool isTapPressed)
{
    bool freqChanged = false;

    // Always track the toggle so octave responds without entering edit mode.
    int newOctave = octaveToggle.ReadToggle();
    if (newOctave != currentOctave)
    {
        currentOctave                = newOctave;
        effectSettings.togglePosition = currentOctave;
        freqChanged                   = true;
    }

    if (allowEffectControl)
    {
        if (padKnob.SetNewValue(padLevel))
        {
            debugPrintlnF(hw, "WorshipPad pad level: %f", padLevel);
            updateEditModeKnobValue(display, 1, padLevel);
            effectSettings.knobSettings[1] = padLevel;
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
    effectSettings.knobSettings[1] = padLevel;
    effectSettings.togglePosition  = currentOctave;
    return effectSettings;
}

void WorshipPad::SetEffectSettings(EffectSettings settings)
{
    padLevel      = settings.knobSettings[1];
    currentOctave = settings.togglePosition;
    UpdateFrequencies();
}
