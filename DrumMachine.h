#ifndef DRUM_MACHINE_H
#define DRUM_MACHINE_H

#include "daisy_seed.h"
#include "IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

/**********************************************
 * Drum Machine Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - 
 * SPDT 2 - 
 * 
 * Knob 1 - Volume
 * Knob 2 - 
 * Knob 3 - 
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class DrumMachine : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo);
    void Cleanup();
    float Process(float in);
    void Loop(bool allowEffectControl);
    char *GetEffectName();
    char **GetKnobNames();
    EffectSettings GetEffectSettings();
    void SetEffectSettings(EffectSettings effectSettings);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"VOLUME", (char *)"", (char *)"", (char *)""};

    const float boostLevelMin = 0.2f;
    const float boostLevelMax = 5.0f;
    float boostLevel = 1.0f;

    Knob boostKnob;
};

#endif