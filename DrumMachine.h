#ifndef DRUM_MACHINE_H
#define DRUM_MACHINE_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

using namespace daisy;
using namespace daisysp;

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
    const char *knobNames[MAX_KNOBS] = {(char *)"VOLUME", (char *)"SPEED", (char *)"TM SIG", (char *)""};

    // Drum parameters
    const float kickAttackFmAmount = 0.1f;
    const float kickFrequency = 1.0f;
    const float kickAccent = 0.5f;
    const float kickDecay = 0.75f;
    const float kickBoost = 10.0f;

    // Adjustable parameters
    const float volumeLevelMin = 0.2f;
    const float volumeLevelMax = 25.0f;  // DEBUG - lower this for release
    const float speedMin = 0.0f;
    const float speedMax = 1.0f;
    const float timeSigMin = 0.0f;
    const float timeSigMax = 1.0f;

    float volume = 1.0f;
    float speed = 1.0f;
    float timeSig = 1.0f;

    Knob volumeKnob;
    Knob speedKnob;
    Knob timeSigKnob;

    // Drum kit
    Metro tick;
    AnalogBassDrum kick;
    AnalogSnareDrum snare;
};

#endif