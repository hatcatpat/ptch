#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "ofMain.h"
#include "AudioObject.h"
#include "Oscillator.h"

class AudioManager
{
public:
    AudioManager() {}

    void setup(int n_inputs = 0, int n_outputs = 2, enum ofSoundDevice::Api api = ofSoundDevice::PULSE)
    {
        ofSoundStreamSettings settings;

        settings.setApi(api);
        settings.setOutListener(ofGetAppPtr());
        settings.numOutputChannels = n_outputs;
        settings.numInputChannels = n_inputs;
        ofSoundStreamSetup(settings);
    }
    ~AudioManager()
    {
        for (int i = 0; i < audio_objects.size(); i++)
        {
            delete audio_objects[i];
        }
        audio_objects.clear();
        audio_object_ids_with_output.clear();
    }

    int addAudioObject(AudioObject *ao)
    {
        ao->setId((int)audio_objects.size());
        audio_objects.push_back(ao);

        if (ao->hasAudioOutput())
            audio_object_ids_with_output.push_back(ao->getId());

        return audio_objects.size() - 1;
    }

    AudioObject *getAudioObject(int id)
    {
        for (auto ao : audio_objects)
        {
            if (ao->getId() == id)
                return ao;
        }
        ofLog() << "ERROR, component id not valid\n";
        return NULL;
    }

    void audioOut(float *output, int buffer_size, int n_chan)
    {
        for (i = 0; i < buffer_size * n_chan; i += n_chan)
        {
            for (c = 0; c < n_chan; c++)
            {
                output[i + c] = 0.0;

                for (auto ao : audio_object_ids_with_output)
                {
                    output[i + c] += audio_objects[ao]->getOutputChannel(c);
                }
            }

            for (auto ao : audio_objects)
            {
                ao->audioUpdate();
            }
        }
    }

    void audioIn(float *input, int buffer_size, int n_chan)
    {
    }

private:
    int i = 0, c = 0;
    vector<AudioObject *> audio_objects;
    vector<int> audio_object_ids_with_output;
};

#endif