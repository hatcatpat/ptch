#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "AudioObject.h"

class Oscillator : public AudioObject
{
public:
    const int FREQ = 0, LO = 1, HI = 2;

    Oscillator() {}
    Oscillator(float freq, float lo, float hi, int n_outputs, bool has_audio_output = false) { setup(freq, lo, hi, n_outputs, has_audio_output); }

    void setup(float freq, float lo, float hi, int n_outputs, bool has_audio_output = false)
    {
        AudioObject::setup(3, n_outputs);

        setAudioOutput(has_audio_output);

        getParam(FREQ)->setup(freq);
        getParam(LO)->setup(lo);
        getParam(HI)->setup(hi);

        b_is_setup = true;
    }

    void audioUpdate() override
    {
        if (b_is_setup)
        {
            o = ofMap(sin(phase), -1, 1, getParam(LO)->get(), getParam(HI)->get());
            for (c = 0; c < getNumOutputs(); c++)
            {
                setOutputChannel(c, o);
            }
            phase += (float)getParam(FREQ)->get() * 2.0 * M_PI / SAMPLE_RATE;
            phase = fmod(phase, 2.0 * M_PI);
        }
    }

private:

    bool b_is_setup = false;

    int c = 0;
    float o = 0, phase = 0;
};

#endif