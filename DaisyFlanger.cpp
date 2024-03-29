#include "DaisyFlanger.h"

void DaisyFlanger::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    mixKnob.Init(hw, mixKnobChannel, mixLevel);
    rateKnob.Init(hw, rateKnobChannel, rate);
    widthKnob.Init(hw, widthKnobChannel, width);
    feedbackKnob.Init(hw, feedbackKnobChannel, feedback);

    // Initialize the Flanger
    float sample_rate = hw->AudioSampleRate();
    flanger.Init(sample_rate);
    flanger.SetLfoFreq(rate);
    flanger.SetLfoDepth(width);
    flanger.SetFeedback(feedback);
}

float DaisyFlanger::Process(float in)
{
    float wet, dry;

    // Adjust the input signal by the boost
    dry = in;

    // Process the flanger
    wet = flanger.Process(dry);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void DaisyFlanger::Cleanup()
{
}

void DaisyFlanger::ConfigureKnobPositions(int mixChannel, int rateChannel, int widthChannel, int feedbackChannel)
{
    mixKnobChannel = mixChannel;
    rateKnobChannel = rateChannel;
    widthKnobChannel = widthChannel;
    feedbackKnobChannel = feedbackChannel;
}

void DaisyFlanger::Loop(bool allowEffectControl)
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
            flanger.SetLfoFreq(rate);

            debugPrintlnF(hw, "Updated the rate to: %f", rate);
            updateEditModeKnobValue(display, 1, rate);
        }

        // Knob 3 controls the LFO width
        if (widthKnobChannel != KNOB_NO_CHN && widthKnob.SetNewValue(width))
        {
            flanger.SetLfoDepth(width);

            debugPrintlnF(hw, "Updated the width to: %f", width);
            updateEditModeKnobValue(display, 2, width);
        }

        // Knob 4 controls the feedback
        if (feedbackKnobChannel != KNOB_NO_CHN && feedbackKnob.SetNewValue(feedback))
        {
            flanger.SetFeedback(feedback);

            debugPrintlnF(hw, "Updated the feedback to to: %f", feedback);
            updateEditModeKnobValue(display, 3, feedback);
        }
    }
}

char *DaisyFlanger::GetEffectName()
{
    return (char *)"FLANGER";
}

char **DaisyFlanger::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DaisyFlanger::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = mixLevel;
    effectSettings.knobSettings[1] = rate;
    effectSettings.knobSettings[2] = width;
    effectSettings.knobSettings[3] = feedback;

    // Return the settings
    return effectSettings;
}

void DaisyFlanger::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels with effect settings
    mixLevel = effectSettings.knobSettings[0];
    rate = effectSettings.knobSettings[1];
    flanger.SetLfoFreq(rate);
    width = effectSettings.knobSettings[2];
    flanger.SetLfoDepth(width);
    feedback = effectSettings.knobSettings[3];
    flanger.SetFeedback(feedback);
}