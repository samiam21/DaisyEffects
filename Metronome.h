#ifndef METRONOME_H
#define METRONOME_H

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
 * Knob 2 - Tempo
 * Knob 3 - Time Signature
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class Metronome : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"VOLUME", (char *)"TEMPO", (char *)"TM SIG", (char *)""};

    // Beat parameters
    const float bFrequency = 550.0f;
    const float dbFrequency = 1000.0f;
    const float dbDirtiness = 0.0f;
    const float dbFmEnvelopeAmount = 0.0f;
    const float dbFmEnvelopeDecay = 0.0f;
    const float dbDecay = 0.0f;
    const float dbTone = 1.0f;
    const float dbAccent = 0.0f;

    // Adjustable parameters
    const float volumeLevelMin = 0.0f;
    const float volumeLevelMax = 2.0f;
    const float tempoMin = 0.0f;
    const float tempoMax = 1.0f;
    const float timeSigMin = 0.0f;
    const float timeSigMax = 1.0f;

    float volume = 1.0f;
    float tempo = 1.0f;
    float timeSig = 1.0f;

    int count = 0;

    Knob volumeKnob;
    Knob tempoKnob;
    Knob timeSigKnob;

    // Drum kit
    Metro tick;
    SyntheticBassDrum beat;
};

#endif