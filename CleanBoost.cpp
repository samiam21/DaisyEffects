#include "CleanBoost.h"

void CleanBoost::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    boostKnob.Init(hw, boostKnobChannel, boostLevel, boostLevelMin, boostLevelMax);
    onOffToggle.Init(hw->GetPin(onOffTogglePin));
}

float CleanBoost::Process(float in)
{
    if (audioOn)
        return in * boostLevel;
    else
        return 0;
}

void CleanBoost::Cleanup()
{
}

void CleanBoost::ConfigureKnobPositions(int boostChannel)
{
    boostKnobChannel = boostChannel;
}

void CleanBoost::ConfigureTogglePositions(int onOffPin)
{
    onOffTogglePin = onOffPin;
}

void CleanBoost::SetBoostMinMax(float boostMin, float boostMax)
{
    boostLevelMin = boostMin;
    boostLevelMax = boostMax;
}

void CleanBoost::Loop(bool allowEffectControl)
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

        // Toggle controls audio on or off
        audioOn = onOffToggle.ReadToggle();
    }
}

char *CleanBoost::GetEffectName()
{
    return (char *)"CLEAN BOOST";
}

char **CleanBoost::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings CleanBoost::GetEffectSettings()
{
    // Add boost level to the effect settings
    effectSettings.knobSettings[0] = boostLevel;

    // Return the settings
    return effectSettings;
}

void CleanBoost::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    boostLevel = effectSettings.knobSettings[0];
}