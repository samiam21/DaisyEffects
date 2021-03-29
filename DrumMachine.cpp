#include "DrumMachine.h"

void DrumMachine::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    boostKnob.Init(hw, KNOB_1_CHN, boostLevel, boostLevelMin, boostLevelMax);
}

float DrumMachine::Process(float in)
{
    return in * boostLevel;
}

void DrumMachine::Cleanup()
{
}

void DrumMachine::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the boost level
        if (boostKnob.SetNewValue(boostLevel))
        {
            debugPrintlnF(hw, "Updated the boost level to: %f", boostLevel);
            updateEditModeKnobValue(display, 0, boostLevel);

            // Update the effect settings
            effectSettings.knobSettings[0] = boostLevel;
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
    effectSettings.knobSettings[0] = boostLevel;

    // Return the settings
    return effectSettings;
}

void DrumMachine::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    boostLevel = effectSettings.knobSettings[0];
}