#include "DaisyChorus.h"

void DaisyChorus::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    mixKnob.Init(hw, mixKnobChannel, mixLevel);
    rateKnob.Init(hw, rateKnobChannel, rate);
    widthKnob.Init(hw, widthKnobChannel, width);
    delayKnob.Init(hw, delayKnobChannel, delay);

    // Initialize the Chorus
    float sample_rate = hw->AudioSampleRate();
    chorus.Init(sample_rate);
    chorus.SetLfoFreq(rate);
    chorus.SetLfoDepth(width);
    chorus.SetDelay(delay);
}

float DaisyChorus::Process(float in)
{
    float wet, dry;

    // Adjust the input signal by the boost
    dry = in;

    // Process the chorus
    wet = chorus.Process(dry);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void DaisyChorus::Cleanup()
{
}

void DaisyChorus::ConfigureKnobPositions(int mixChannel, int rateChannel, int widthChannel, int delayChannel)
{
    mixKnobChannel = mixChannel;
    rateKnobChannel = rateChannel;
    widthKnobChannel = widthChannel;
    delayKnobChannel = delayChannel;
}

void DaisyChorus::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the mix level
        if (mixKnobChannel != KNOB_NO_CHN && mixKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
            updateEditModeKnobValue(display, 0, mixLevel);
        }

        // Knob 2 controls the LFO rate
        if (rateKnobChannel != KNOB_NO_CHN && rateKnob.SetNewValue(rate))
        {
            chorus.SetLfoFreq(rate);

            debugPrintlnF(hw, "Updated the rate to: %f", rate);
            updateEditModeKnobValue(display, 1, rate);
        }

        // Knob 3 controls the LFO width
        if (widthKnobChannel != KNOB_NO_CHN && widthKnob.SetNewValue(width))
        {
            chorus.SetLfoDepth(width);

            debugPrintlnF(hw, "Updated the width to: %f", width);
            updateEditModeKnobValue(display, 2, width);
        }

        // Knob 4 controls the delay
        if (delayKnobChannel != KNOB_NO_CHN && delayKnob.SetNewValue(delay))
        {
            chorus.SetDelay(delay);

            debugPrintlnF(hw, "Updated the delay to: %f", delay);
            updateEditModeKnobValue(display, 3, delay);
        }
    }
}

char *DaisyChorus::GetEffectName()
{
    return (char *)"CHORUS";
}

char **DaisyChorus::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DaisyChorus::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = mixLevel;
    effectSettings.knobSettings[1] = rate;
    effectSettings.knobSettings[2] = width;
    effectSettings.knobSettings[3] = delay;

    // Return the settings
    return effectSettings;
}

void DaisyChorus::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels from effect settings
    mixLevel = effectSettings.knobSettings[0];
    rate = effectSettings.knobSettings[1];
    chorus.SetLfoFreq(rate);
    width = effectSettings.knobSettings[2];
    chorus.SetLfoDepth(width);
    delay = effectSettings.knobSettings[3];
    chorus.SetDelay(delay);
}