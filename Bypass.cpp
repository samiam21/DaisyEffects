#include "Bypass.h"

void Bypass::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    volKnob.Init(hw, volKnobChannel, volLevel, volLevelMin, volLevelMax);
}

float Bypass::Process(float in)
{
    return in * volLevel;
}

void Bypass::Cleanup()
{
}

void Bypass::ConfigureKnobPositions(int volChannel)
{
    volKnobChannel = volChannel;
}

void Bypass::Loop(bool allowEffectControl, bool isTapPressed)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the boost level
        if (volKnob.SetNewValue(volLevel))
        {
            debugPrintlnF(hw, "Updated the boost level to: %f", volLevel);
            updateEditModeKnobValue(display, 0, volLevel);

            // Update the effect settings
            effectSettings.knobSettings[0] = volLevel;
        }
    }
}

char *Bypass::GetEffectName()
{
    return (char *)"BYPASS";
}

char **Bypass::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings Bypass::GetEffectSettings()
{
    // Add volume level to the effect settings
    effectSettings.knobSettings[0] = volLevel;

    // Return the settings
    return effectSettings;
}

void Bypass::SetEffectSettings(EffectSettings effectSettings)
{
    // Update volume level from the effect settings
    volLevel = effectSettings.knobSettings[0];
}