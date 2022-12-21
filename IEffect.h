#ifndef IEFFECT_H
#define IEFFECT_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../include/PedalConfig.h"
#include "Hardware/Storage.h"
#include "Hardware/DaisyDisplay.h"
#include "../DaisyInputs/Knob.h"
#include "../DaisyInputs/Button.h"
#include "../DaisyInputs/NFNToggle.h"
#include "../DaisyInputs/SimpleToggle.h"

using namespace daisy;
using namespace daisysp;

class IEffect
{
public:
    IEffect() {}
    virtual ~IEffect() {}
    virtual void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay = nullptr, int *newBpm = nullptr) {}
    virtual void Cleanup() {}
    virtual float Process(float in) { return 0; }
    virtual void Loop(bool allowEffectControl, bool isTapPressed = false) {}
    virtual char *GetEffectName() { return (char *)"uh-oh"; }
    virtual char **GetKnobNames() { return (char **)""; }
    virtual EffectSettings GetEffectSettings() { return effectSettings; }
    virtual void SetEffectSettings(EffectSettings effectSettings) {}
    virtual void UpdateToggleDisplay() {}
    virtual void ConfigureKnobPositions(...) {}
    virtual void ConfigureTogglePositions(...) {}

protected:
    daisy::DaisySeed *hw;
    DaisyDisplay *display;
    EffectSettings effectSettings;
};

#endif