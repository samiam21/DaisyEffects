#ifndef STOMPBOX_H
#define STOMPBOX_H

#include "daisy_seed.h"
#include "IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

/**********************************************
 * Stomp Box Effect
 *
 * SPST 1 -
 * SPST 2 -
 * SPST 3 -
 * SPST 4 -
 *
 * SPDT 1 -
 * SPDT 2 -
 *
 * Knob 1 - Level
 * Knob 2 -
 * Knob 3 -
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

class StompBox : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, int *newBpm);
    void Cleanup();
    float Process(float in);
    void Loop(bool allowEffectControl, bool isTapPressed = false);
    char *GetEffectName();
    char **GetKnobNames();
    EffectSettings GetEffectSettings();
    void SetEffectSettings(EffectSettings effectSettings);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"LEVEL", (char *)"", (char *)"", (char *)""};

    const float levelMin = 1.0f;
    const float levelMax = 30.0f;
    float level = 10.0f;

    Knob levelKnob;
};

#endif