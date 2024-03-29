#include "Shifter.h"

void Shifter::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    mixKnob.Init(hw, mixKnobChannel, mix, mixMin, mixMax);
    transKnob.Init(hw, transKnobChannel, transpose, transMin, transMax);
    delayKnob.Init(hw, delayKnobChannel, delay, delayMin, delayMax);
    funKnob.Init(hw, funKnobChannel, fun, funMin, funMax);

    // Initialize the shifter
    shifter.Init(hw->AudioSampleRate());
    shifter.SetFun(fun);

    // Convert the delay to an int and update the shifter
    uint32_t del = (uint32_t)delay;
    shifter.SetDelSize(del);

    // Round the transpose to the nearest 0.5
    float trans = floor((transpose * 2) + 0.5) / 2;
    shifter.SetTransposition(trans);
}

float Shifter::Process(float in)
{
    float wet = shifter.Process(in);

    // Mix wet and dry and send to I/O
    return wet * mix + in * (1 - mix);
}

void Shifter::Cleanup()
{
}

void Shifter::ConfigureKnobPositions(int mixChannel, int transposeChannel, int delayChannel, int funChannel)
{
    mixKnobChannel = mixChannel;
    transKnobChannel = transposeChannel;
    delayKnobChannel = delayChannel;
    funKnobChannel = funChannel;
}

void Shifter::SetMinMaxMix(float minMix, float maxMix)
{
    mixMin = minMix;
    mixMax = maxMix;
}

void Shifter::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the mix
        if (mixKnobChannel != KNOB_NO_CHN && mixKnob.SetNewValue(mix))
        {
            debugPrintlnF(hw, "Updated the mix to: %f", mix);
            updateEditModeKnobValue(display, 0, mix);
        }

        // Knob 2 controls the transpose
        if (transKnobChannel != KNOB_NO_CHN && transKnob.SetNewValue(transpose))
        {
            // Round the transpose to the nearest 0.5
            float trans = floor((transpose * 2) + 0.5) / 2;
            shifter.SetTransposition(trans);

            debugPrintlnF(hw, "Updated the transpose to: %f", trans);
            updateEditModeKnobValue(display, 1, trans);
        }

        // Knob 3 controls the delay
        if (delayKnobChannel != KNOB_NO_CHN && delayKnob.SetNewValue(delay))
        {
            // Convert the delay to an int and update the shifter
            uint32_t del = (uint32_t)delay;
            shifter.SetDelSize(del);

            debugPrintlnF(hw, "Updated the delay to: %f", del);
            updateEditModeKnobValueI(display, 2, del);
        }

        // Knob 4 controls the fun
        if (funKnobChannel != KNOB_NO_CHN && funKnob.SetNewValue(fun))
        {
            shifter.SetFun(fun);

            debugPrintlnF(hw, "Updated the fun to: %f", fun);
            updateEditModeKnobValue(display, 3, fun);
        }
    }
}

char *Shifter::GetEffectName()
{
    return (char *)"SHIFTER";
}

char **Shifter::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings Shifter::GetEffectSettings()
{
    // Update the effect settings
    effectSettings.knobSettings[0] = mix;
    effectSettings.knobSettings[1] = transpose;
    effectSettings.knobSettings[2] = delay;
    effectSettings.knobSettings[3] = fun;

    // Return the settings
    return effectSettings;
}

void Shifter::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    mix = effectSettings.knobSettings[0];
    transpose = effectSettings.knobSettings[1];
    delay = effectSettings.knobSettings[2];
    fun = effectSettings.knobSettings[3];

    shifter.SetFun(fun);

    // Round the transpose to the nearest 0.5
    float trans = floor((transpose * 2) + 0.5) / 2;
    shifter.SetTransposition(trans);

    // Convert the delay to an int and update the shifter
    uint32_t del = (uint32_t)delay;
    shifter.SetDelSize(del);
}