#include "DrumMachine.h"

void DrumMachine::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    volumeKnob.Init(hw, KNOB_1_CHN, volume, volumeLevelMin, volumeLevelMax);
    speedKnob.Init(hw, KNOB_2_CHN, speed, speedMin, speedMax);
    timeSigKnob.Init(hw, KNOB_3_CHN, timeSig, timeSigMin, timeSigMax);

    // Initialize drums
    float sample_rate = hw->AudioSampleRate();
    tick.Init(1.f, sample_rate);
    kick.Init(sample_rate);
    snare.Init(sample_rate);

    // Setup kick
    kick.SetAttackFmAmount(kickAttackFmAmount);
    kick.SetFreq(kickFrequency);
    kick.SetAccent(kickAccent);
    kick.SetDecay(kickDecay);

    // Setup snare
    snare.SetSustain(false);
    snare.SetDecay(0.5f);
    snare.SetSnappy(0.5f);
    snare.SetTone(0.5f);
}

float DrumMachine::Process(float in)
{
    bool t = tick.Process();

    //float drums = (kick.Process(t) * kickBoost) + snare.Process(t);
    float drums = (kick.Process(t) * kickBoost);

    return in + (drums * volume);
}

void DrumMachine::Cleanup()
{
}

void DrumMachine::Loop(bool allowEffectControl)
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
        if (speedKnob.SetNewValue(speed))
        {
            debugPrintlnF(hw, "Updated the speed level to: %f", speed);
            updateEditModeKnobValue(display, 1, speed);

            
        }

        // Knob 3 controls the time signature
        if (timeSigKnob.SetNewValue(timeSig))
        {
            debugPrintlnF(hw, "Updated the timeSig level to: %f", timeSig);
            updateEditModeKnobValue(display, 2, timeSig);

            
        }
    }
}

char *DrumMachine::GetEffectName()
{
    return (char *)"DRUM MACHINE";
}

char **DrumMachine::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DrumMachine::GetEffectSettings()
{
    // Add boost level to the effect settings
    effectSettings.knobSettings[0] = volume;

    // Return the settings
    return effectSettings;
}

void DrumMachine::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    volume = effectSettings.knobSettings[0];
}