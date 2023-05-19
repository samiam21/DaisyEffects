#include "DaisyCompressor.h"

void DaisyCompressor::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    ratioKnob.Init(hw, ratioKnobChannel, ratio, ratioMin, ratioMax);
    thresholdKnob.Init(hw, thresholdKnobChannel, threshold, thresholdMin, thresholdMax);
    attackKnob.Init(hw, attackKnobChannel, attack, attackMin, attackMax);
    releaseKnob.Init(hw, releaseKnobChannel, release, releaseMin, releaseMax);

    // Initialize the compressor
    compressor.Init(hw->AudioSampleRate());
    compressor.AutoMakeup(true);
}

float DaisyCompressor::Process(float in)
{
    return compressor.Process(in);
}

void DaisyCompressor::Cleanup()
{
}

void DaisyCompressor::ConfigureKnobPositions(int ratioChannel, int thresholdChannel, int attackChannel, int releaseChannel)
{
    ratioKnobChannel = ratioChannel;
    thresholdKnobChannel = thresholdChannel;
    attackKnobChannel = attackChannel;
    releaseKnobChannel = releaseChannel;
}

void DaisyCompressor::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the ratio
        if (ratioKnobChannel != KNOB_NO_CHN && ratioKnob.SetNewValue(ratio))
        {
            compressor.SetRatio(ratio);
            debugPrintlnF(hw, "Updated the ratio to: %f", ratio);
            updateEditModeKnobValue(display, 0, ratio);
        }

        // Knob 2 controls the threshold
        if (thresholdKnobChannel != KNOB_NO_CHN && thresholdKnob.SetNewValue(threshold))
        {
            compressor.SetThreshold(threshold);
            debugPrintlnF(hw, "Updated the threshold to: %f", threshold);
            updateEditModeKnobValue(display, 1, threshold);
        }

        // Knob 3 controls the attack
        if (attackKnobChannel != KNOB_NO_CHN && attackKnob.SetNewValue(attack))
        {
            compressor.SetAttack(attack);
            debugPrintlnF(hw, "Updated the attack to: %f", attack);
            updateEditModeKnobValue(display, 2, attack);
        }

        // Knob 4 controls the release
        if (releaseKnobChannel != KNOB_NO_CHN && releaseKnob.SetNewValue(release))
        {
            compressor.SetRelease(release);
            debugPrintlnF(hw, "Updated the release to: %f", release);
            updateEditModeKnobValue(display, 3, release);
        }
    }
}

char *DaisyCompressor::GetEffectName()
{
    return (char *)"COMPRESSOR";
}

char **DaisyCompressor::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DaisyCompressor::GetEffectSettings()
{
    // Update the effect settings
    effectSettings.knobSettings[0] = ratio;
    effectSettings.knobSettings[1] = threshold;
    effectSettings.knobSettings[2] = attack;
    effectSettings.knobSettings[3] = release;

    // Return the settings
    return effectSettings;
}

void DaisyCompressor::SetEffectSettings(EffectSettings effectSettings)
{
    // Update the effect settings
    ratio = effectSettings.knobSettings[0];
    threshold = effectSettings.knobSettings[1];
    attack = effectSettings.knobSettings[2];
    release = effectSettings.knobSettings[3];

    compressor.SetRatio(ratio);
    compressor.SetThreshold(threshold);
    compressor.SetAttack(attack);
    compressor.SetRelease(release);
}