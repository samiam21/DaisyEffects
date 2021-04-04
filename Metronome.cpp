#include "Metronome.h"

void Metronome::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    volumeKnob.Init(hw, KNOB_1_CHN, volume, volumeLevelMin, volumeLevelMax);
    tempoKnob.Init(hw, KNOB_2_CHN, tempo, tempoMin, tempoMax);
    timeSigKnob.Init(hw, KNOB_3_CHN, timeSig, timeSigMin, timeSigMax);

    // Initialize drums
    float sample_rate = hw->AudioSampleRate();
    tick.Init(1.f, sample_rate);
    beat.Init(sample_rate);

    // Setup beat
    beat.SetFreq(bFrequency);
    beat.SetDirtiness(dbDirtiness);
    beat.SetFmEnvelopeAmount(dbFmEnvelopeAmount);
    beat.SetFmEnvelopeDecay(dbFmEnvelopeDecay);
    beat.SetDecay(dbDecay);
    beat.SetTone(dbTone);
    beat.SetAccent(dbAccent);
}

float Metronome::Process(float in)
{
    bool t = tick.Process();

    if (t)
    {
        // Play the down beat if we are on the 1
        if (count == 0)
        {
            beat.SetFreq(dbFrequency);
        }
        else
        {
            beat.SetFreq(bFrequency);
        }

        // Increment the count
        count++;
        if (count >= 4)
        {
            count = 0;
        }
    }

    return in + (beat.Process(t) * volume);
}

void Metronome::Cleanup()
{
}

void Metronome::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the volume level
        if (volumeKnob.SetNewValue(volume))
        {
            debugPrintlnF(hw, "Updated the volume level to: %f", volume);
            updateEditModeKnobValue(display, 0, volume);
        }

        // Knob 2 controls the speed
        if (tempoKnob.SetNewValue(tempo))
        {
            debugPrintlnF(hw, "Updated the speed level to: %f", tempo);
            updateEditModeKnobValue(display, 1, tempo);
        }

        // Knob 3 controls the time signature
        if (timeSigKnob.SetNewValue(timeSig))
        {
            debugPrintlnF(hw, "Updated the timeSig level to: %f", timeSig);
            updateEditModeKnobValue(display, 2, timeSig);
        }
    }
}

char *Metronome::GetEffectName()
{
    return (char *)"METRONOME";
}

char **Metronome::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings Metronome::GetEffectSettings()
{
    // Add boost level to the effect settings
    effectSettings.knobSettings[0] = volume;

    // Return the settings
    return effectSettings;
}

void Metronome::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    volume = effectSettings.knobSettings[0];
}