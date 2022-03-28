#include "StompBox.h"

void StompBox::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    levelKnob.Init(hw, KNOB_1_CHN, level, levelMin, levelMax);
}

float StompBox::Process(float in)
{
    return in * level;
}

void StompBox::Cleanup()
{
}

void StompBox::Loop(bool allowEffectControl, bool isTapPressed)
{
    if (isTapPressed)
    {
        debugPrintln(hw, "PRESSED");
    }

    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the boost level
        if (levelKnob.SetNewValue(level))
        {
            debugPrintlnF(hw, "Updated the boost level to: %f", level);
            updateEditModeKnobValue(display, 0, level);

            // Update the effect settings
            effectSettings.knobSettings[0] = level;
        }
    }
}

char *StompBox::GetEffectName()
{
    return (char *)"STOMP BOX";
}

char **StompBox::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings StompBox::GetEffectSettings()
{
    // Add boost level to the effect settings
    effectSettings.knobSettings[0] = level;

    // Return the settings
    return effectSettings;
}

void StompBox::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    level = effectSettings.knobSettings[0];
}